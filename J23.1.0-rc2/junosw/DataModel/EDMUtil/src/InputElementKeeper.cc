#include "EDMUtil/InputElementKeeper.h"
#include "InputFileManager.h"
#include "SmartRefTable.h"
#include "EDMUtil/RootFileInter.h"
#include "EDMUtil/FileMetaData.h"
#include "EDMUtil/UniqueIDTable.h"
#include "PassiveStream.h"

#include "TTree.h"
#include "TFile.h"
#include "TProcessID.h"
#include <boost/foreach.hpp>
#include <mutex>

InputElementKeeper* InputElementKeeper::m_keeper = new InputElementKeeper;
std::recursive_mutex keeper_lock;

InputElementKeeper::TreeInfo::TreeInfo(PassiveStream* ps, int index, int fileid, const std::vector<Long64_t>& bp)
  : stream(ps), streamIndex(index), fileID(fileid), breakPoints(bp)
{
}

InputElementKeeper::InputElementKeeper()
            : m_table(new SmartRefTable)
            , m_fileMgr(new InputFileManager)
{
}

InputElementKeeper::~InputElementKeeper()
{
    delete m_table;
    delete m_fileMgr;
    if ( this == m_keeper ) {
        m_keeper = 0;
    }
}

void InputElementKeeper::ClearTable(int fileid)
{
    m_table->DeleteTable(fileid);
}

int InputElementKeeper::RegisterFile(const std::string& filename, const JM::FileMetaData* metadata)
{
    std::lock_guard<std::recursive_mutex> guard(keeper_lock);
    int fileid = m_fileMgr->FindFile(filename);
    if (fileid != -1) {
        // File already registered, do nothing.
        return fileid;
    }
    // Register file
    fileid = m_fileMgr->AddFile(filename);

    // Register trees
    const std::vector<JM::TreeMetaData*>& trees = metadata->GetTreeMetaData();
    BOOST_FOREACH(const JM::TreeMetaData* tmd, trees) {
        // Check if need to create a passive stream
        const std::string& treeName = tmd->GetTreeName();
        PassiveStream* ps = 0;
        PSMap::iterator pos = m_psMap.find(treeName);
        if (pos == m_psMap.end()) {
            ps = new PassiveStream;
            m_psMap.insert(std::make_pair(treeName, ps));
        }
        else { 
            ps = pos->second;
        }
        // Add this tree to the certain passive stream
        int treeid = m_treeInfoList.size();
        int treeIndex = ps->AddTree(fileid);
        // Create a treeInfo
        std::vector<Long64_t> bp;
        if ( metadata->GetBreakPoints().find(treeName) != metadata->GetBreakPoints().end())   bp = metadata->GetBreakPoints().find(treeName)->second;
        m_treeInfoList.push_back(new TreeInfo(ps, treeIndex, fileid, bp));
        // Tell fileMgr about the info of this tree.
        // So that when a file need to be re-opened, we can update passive streams with new tree pointer
        m_fileMgr->AddTreeInfo(fileid, treeid, treeName);
        // Add this file to the path2fileid map
        // Only need dir path, burn tree name
        std::string path = treeName.substr(0, treeName.rfind('/'));
        if (m_path2FileList.find(path) == m_path2FileList.end()) {
            m_path2FileList.insert(std::make_pair(path, std::vector<int>()));
        }
        // TODO redupiclated fileid?
        m_path2FileList[path].push_back(fileid);
    }
    // Add this file to the uuid2fileid map
    const JM::FileMetaData::StringVector& uuidList = metadata->GetUUIDList();
    BOOST_FOREACH(const std::string& uuid, uuidList) {
        if (m_uuid2FileList.find(uuid) == m_uuid2FileList.end()) {
            m_uuid2FileList.insert(std::make_pair(uuid, std::vector<int>()));
        }
        m_uuid2FileList[uuid].push_back(fileid);
    }
    // Record the NavPath
    m_fileID2NavPath[fileid] = metadata->GetNavPath();
    return fileid;
}

void InputElementKeeper::AddObjRef(Int_t uid, const TProcessID* pid)
{
    int treeid = m_table->GetTreeID(uid, pid);
    // Unregistered object
    if (-1 == treeid) {
        // Try to load meta data from other files and search again
        String2FileIDs::iterator pos = m_uuid2FileList.find(pid->GetTitle());
        if (pos != m_uuid2FileList.end()) {
            std::vector<int>::iterator it, end = pos->second.end();
            // Open rest file that holds same TProcessID uuid
            for (it = pos->second.begin(); it != end; ++it) {
                // If meta data of this file is not in memory, load it
                this->LoadUniqueID(*it);
            }
            // Search tree id again
            treeid = m_table->GetTreeID(uid, pid);
        }
        // Sorry, the object is not in the input file list
        if (-1 == treeid) {
            return;
        }
    }
    int fileid = m_treeInfoList[treeid]->fileID;
    m_fileMgr->AddObjRef(fileid);
}

void InputElementKeeper::DecObjRef(Int_t uid, const TProcessID* pid)
{
    int treeid = m_table->GetTreeID(uid, pid);
    // Unregistered object
    if (-1 == treeid) {
        return;
    }
    int fileid = m_treeInfoList[treeid]->fileID;
    m_fileMgr->DecObjRef(fileid);
}


void InputElementKeeper::AddTreeRef(int fileid)
{
    m_fileMgr->AddTreeRef(fileid);
}

void InputElementKeeper::DecTreeRef(int fileid)
{
    m_fileMgr->DecTreeRef(fileid);
}

InputElementKeeper* InputElementKeeper::GetInputElementKeeper()
{
    return m_keeper;
}

bool InputElementKeeper::CheckFileStatus(int fileid) const
{
    return m_fileMgr->CheckFileStatus(fileid);
}

bool InputElementKeeper::GetNavTree(int fileid, TTree*& tree)
{
    std::lock_guard<std::recursive_mutex> guard(keeper_lock);
    if (!CheckFileStatus(fileid)) {
        OpenFile(fileid); // TODO rubust check: fail to open file
    }
    TFile* file = m_fileMgr->GetFile(fileid);
    tree = RootFileInter::GetTree(file, "Meta/navigator");
    return true;
}

std::vector<std::string>& InputElementKeeper::GetNavPath(int fileid)
{
    return m_fileID2NavPath[fileid];
}

const std::string& InputElementKeeper::GetFileName(int fileid)
{
    return m_fileMgr->GetFileName(fileid);
}

TFile* InputElementKeeper::GetFile(int fileid)
{
    return m_fileMgr->GetFile(fileid);
}

void InputElementKeeper::LoadUniqueID(int fileid)
{
    if (m_table->InspectFileID(fileid)) {
        // Meta data already in-momory, do nothing
        return;
    }

    m_table->StartNewTable(fileid);
    JM::UniqueIDTable* uidTable = 0;
    bool preStatus = CheckFileStatus(fileid);
    // TODO robust check: fail to get uidTable
    if (preStatus) {
        uidTable = RootFileInter::GetUniqueIDTable(GetFile(fileid));
    }
    else {
        TFile* file = RootFileInter::OpenFile(GetFileName(fileid));
        uidTable = RootFileInter::GetUniqueIDTable(file);
        // Only open file when start to read object
        file->Close();
    }
    // Map of tree id to tree name.
    std::map<int,std::string> treeInfo = m_fileMgr->GetTreeInfo(fileid);
    // TableMap : std::map<std::string, TablePerTree*>
    JM::UniqueIDTable::TableMap tables = uidTable->GetTable();
    std::map<int,std::string>::iterator it, end = treeInfo.end();
    for (it = treeInfo.begin(); it != end; ++it) {
        JM::UniqueIDTable::TableMap::iterator tpos = tables.find(it->second);
        if (tpos != tables.end()) {
            const std::vector<Long64_t>& breakPoints = m_treeInfoList[it->first]->breakPoints;
            m_table->ReadMetaData(/*TablePerTree*/tpos->second, /*treeID*/it->first, breakPoints);
        }
    }
    delete uidTable;
}

void InputElementKeeper::OpenFile(int fileid)
{
    const std::string& filename = m_fileMgr->GetFileName(fileid);
    const std::map<int,std::string>& treeInfo = m_fileMgr->GetTreeInfo(fileid);

    TFile* file = RootFileInter::OpenFile(filename);
    std::map<int,std::string>::const_iterator treeInfoIt;
    for (treeInfoIt = treeInfo.begin(); treeInfoIt != treeInfo.end(); ++treeInfoIt) {
        // Update PassiveStream
        TTree* tree = RootFileInter::GetTree(file, treeInfoIt->second);
        int index =  m_treeInfoList[treeInfoIt->first]->streamIndex;
        PassiveStream* stream = m_treeInfoList[treeInfoIt->first]->stream;
        stream->UpdateTree(index, tree);
    }
    // Update InputFileHandle
    m_fileMgr->UpdateFile(fileid, file);
    // Load unique id data if nesessary
    this->LoadUniqueID(fileid);
}

void InputElementKeeper::ReadObject(Int_t uid, const TProcessID* pid, Long64_t entry)
{
    int treeid = m_table->GetTreeID(uid, pid);
    if (-1 == treeid) {
        return;
    }
    // Tree id is found, now try to load object
    // Check status of the file holding the tree
    int fileID = m_treeInfoList[treeid]->fileID;
    int streamIndex = m_treeInfoList[treeid]->streamIndex;
    bool status = this->CheckFileStatus(fileID);
    if (!status) {
        // File is closed, need to open
        this->OpenFile(fileID);
    }
    Long64_t offset = 0;
    int offsetIndex = m_table->GetOffset(uid, pid);
    if (-1 != offsetIndex) {
        offset = m_treeInfoList[treeid]->breakPoints[offsetIndex];
    }
    m_treeInfoList[treeid]->stream->ReadObject(streamIndex, entry + offset);
}

void InputElementKeeper::Notify(int option, Int_t uid, const TProcessID* pid, Long64_t entry)
{
    std::lock_guard<std::recursive_mutex> guard(keeper_lock);
    switch (option) {
        case Read:
            ReadObject(uid, pid, entry);
            break;
        case New:
            AddObjRef(uid, pid);
            break;
        case Delete:
            DecObjRef(uid, pid);
            break;
    }
}

bool InputElementKeeper::GetObj(TObject*& obj, const std::string& fullName)
{
    std::lock_guard<std::recursive_mutex> guard(keeper_lock);
    std::string path, name;
    size_t pos = fullName.find("::");
    if (pos == std::string::npos) {
      name = fullName;
    }
    else {
      name = fullName.substr(pos + 2);
      path = fullName.substr(0, pos);
    }
    int fileid;
    if (path.size() == 0) {
        fileid = m_path2FileList.begin()->second[0];
    }
    else if (m_path2FileList.find(path) == m_path2FileList.end()) {
        return false;
    }
    else {
        fileid = m_path2FileList.find(path)->second[0];
    }

    if (CheckFileStatus(fileid)) {
        TFile* file = GetFile(fileid);
        obj = RootFileInter::ReadObject(file, name);
    }
    else {
        TFile* file = RootFileInter::OpenFile(GetFileName(fileid));
        if (!file) return false;
        obj = RootFileInter::ReadObject(file, name);
    }

    return obj != 0;
}
