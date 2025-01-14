#include "RootIOUtil/RootFileWriter.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/EvtNavigator.h"
#include "RootIOUtil/RootOutputFileManager.h"
#include "EDMUtil/EDMManager.h"
#include "Event/HeaderObject.h"
#include "EDMUtil/FileMetaData.h"
#include <algorithm>

#include "TTree.h"
#include "TFile.h"
#include "TProcessID.h"

OutputTreeHandle::OutputTreeHandle(const std::string& path, const std::string& objName, TDirectory* dir)
    : m_path(path)
    , m_objName(objName)
    , m_tree(0)
    , m_addr(0)
    , m_dir(dir)
    , m_metaData(0)
    , m_entries(0)
{
    if (m_path[m_path.length() - 1] == '/') {
        m_fullTreeName = path + objName.substr(objName.rfind("::")+2);
    }
    else {
        m_fullTreeName = path + '/' + objName.substr(objName.rfind("::")+2);
    }
}

OutputTreeHandle::~OutputTreeHandle()
{
    // Yes, we don't delete TTree*
    // And TreeMetaData will be handled by RootOutputFileManager
}

void OutputTreeHandle::initialize()
{
    std::string title = "Tree at " + m_path + " holding " + m_objName;
    std::string treeName = m_objName.substr(m_objName.rfind("::")+2);
    std::string branchName = treeName;
    m_tree = new TTree(treeName.c_str(), title.c_str());
    //Limit the TTree size to 1Tb
    m_tree->SetMaxTreeSize(1024*Long64_t(1024*1024*1024));
    m_tree->SetDirectory(m_dir);
    m_tree->Branch(branchName.c_str(), m_objName.c_str(), &m_addr, 32000, 1);
    m_metaData = new JM::TreeMetaData();
    m_metaData->SetTreeName(m_fullTreeName);
    m_metaData->SetObjName(m_objName);
}

bool OutputTreeHandle::fill(int& nbytes)
{
    if (!m_addr) {
        return false;
    }
    if (!m_tree) {
        // Initialize tree first
        this->initialize();
    }
    nbytes = m_tree->Fill();
    ++m_entries;
    this->fillUID();
    return true;
}

void OutputTreeHandle::write()
{
    if (m_tree) {
        m_tree->Write(NULL,TObject::kOverwrite);
    }
}

void OutputTreeHandle::writeUID(RootOutputFileHandle* file)
{
    file->addUniqueIDTable(m_fullTreeName, m_guid, m_uid, m_bid);
    file->addUUID(m_guid);
}

void OutputTreeHandle::fillUID(int bid)
{
    TObject* obj = static_cast<TObject*>(m_addr);
    UInt_t uid = obj->GetUniqueID();
    TProcessID* pid = TProcessID::GetProcessWithUID(uid,obj);
    const char* guid = pid->GetTitle();
    int iid;
    StringVector::const_iterator posPID = find( m_guid.begin(), m_guid.end(), guid);
    if (posPID == m_guid.end()) {
        m_guid.push_back(guid);
        m_uid.push_back(std::vector<Int_t>());
        // When bid is -1, branch id won't be saved
        if (-1 != bid) {
            m_bid.push_back(std::vector<Short_t>());
        }
        iid = m_guid.size() - 1;
    }
    else {
        iid = posPID - m_guid.begin();
    }
    uid = uid & 0xffffff;
    m_uid[iid].push_back(uid);
    if (-1 != bid) {
        m_bid[iid].push_back(bid);
    }
}

//---------------------- RootFileWriter ------------------------

RootFileWriter::RootFileWriter(const std::string& path, const std::string& headerName, RootOutputFileManager* fileMgr)
    : m_file(0)
    , m_headerTree(0)
    , m_navTree(0)
    , m_dir(0)
    , m_path(path)
    , m_headerName(headerName)
    , m_initialized(false)
    , m_fileEntries(0)
    , m_navAddr(0)
    , m_fileMgr(fileMgr)
{
}

RootFileWriter::~RootFileWriter()
{
}

RootOutputFileHandle* RootFileWriter::getFile()
{
    return m_file;
}

bool RootFileWriter::write(JM::EvtNavigator* nav)
{
    // Check rationality
    if (!m_file) {
        LogError << "No output file started, can not write"
                 << std::endl;
        return false;
    }

    // Try to initialize if nessassry
    if (!m_initialized) {
        this->initialize();
    }

    // Check whether should the EvtNavigator be written together with this event.
    // For the output file, if all rest paths in the nav are already written, then write EvtNavigator.
    const std::vector<std::string>& unwritten_paths = m_file->getUnwrittenPaths();
    const std::vector<std::string>& all_paths_nav = nav->getPath();;

    int count = 0;
    for (auto unwritten_path : unwritten_paths) {
        if (nav->writePath(unwritten_path)) {
            if (std::find(all_paths_nav.begin(),all_paths_nav.end(),unwritten_path) != all_paths_nav.end()) {
                ++count;
            }
        }
    }
    if (count == 1) {
        // This is the last path to be written in the file. Write the EvtNavigator together.
        m_navTree = m_file->getNavTree();
        if (!m_navTree) {
            LogError << "No tree for EvtNavigator, can not write"
                     << std::endl;
            return false;
        }
        // Tell FileMetaData what paths EvtNavigator privides.
        const std::vector<std::string>& adjustedPath = m_file->setNavPath(all_paths_nav);
        // Make sure all EvtNavigators in this stream hold same paths.
        nav->adjustPath(adjustedPath); 
        m_file->resetUnwrittenPath();
    }
    else {
        m_file->addWrittenPath(m_path);
    }
   
    // Set Address
    this->setAddress(nav);

    bool write = m_navAddr->writePath(m_path);
    if (!m_initialized || !write) {
        // Currently unknown stream or skipped entry, just idling
        ++m_fileEntries;
        if (m_navTree) {
            this->writeNav();
        }
        return true;  
    }

    m_file->getFile()->cd(m_path.c_str());

    bool ok = this->writeEvent();
    if (!ok) {
        LogError << "Fail to write event data for " << m_path
                 << std::endl;
        return false;
    }

    ok = this->writeHeader();
    if (!ok) {
        LogError << "Fail to write header data for " << m_path
                 << std::endl;
        return false;
    }

    if (m_navTree) ok = this->writeNav();
    if (!ok) {
        LogError << "Fail to write EvtNavigator with " << m_path
                 << std::endl;
        return false;
    }

    ++m_fileEntries;
    this->resetAddress();    
    m_navTree = 0;

    return true;
}

bool RootFileWriter::writeHeader()
{
    // Set entry number
    m_navAddr->setHeaderEntry(m_path, m_headerTree->entries());

    int nbytes = 0;
    // Header will always be written
    m_headerTree->fill(nbytes);
    LogDebug <<  "Wrote " << nbytes
             << " byte(s) to entry " << m_headerTree->entries()
             << " of header of " << m_path
             << std::endl;
    return nbytes > 0;
}

bool RootFileWriter::writeEvent()
{
    String2TreeHandle::iterator it, end = m_eventTrees.end();
    for (it = m_eventTrees.begin(); it != end; ++it) {
        int nbytes = 0;
        bool write = it->second->fill(nbytes);
        if (!write) continue;

        // Set entry number of this event
        Long64_t nentry = it->second->entries() - 1;
        m_navAddr->getHeader(m_path)->setEventEntry(it->first, nentry);

        LogDebug << "Wrote " << nbytes
                 << " byte(s) to entry " << it->second->entries()
                 << " of " << it->second->objectName()
                 << " of path " << m_path
                 << std::endl;
        if (nbytes <= 0) {
            return false;
        }
    }
    return true;
}

bool RootFileWriter::writeNav()
{
    m_file->setNavAddr(m_navAddr);
    int nbytes = m_navTree->Fill();
    LogDebug <<  "Wrote " << nbytes
             << " bytes to entry " << m_fileEntries + 1
             << " of tree for EvtNavigator "
             << std::endl;
    return nbytes > 0;
}

bool RootFileWriter::close()
{
    if (!m_initialized) return true;
    // Reset current directory
    m_dir->cd();
    // Write trees and lazy-loading data
    m_headerTree->write();
    m_headerTree->writeUID(m_file);
    String2TreeHandle::iterator it, end = m_eventTrees.end();
    for (it = m_eventTrees.begin(); it != end; ++it) {
        it->second->write();
        it->second->writeUID(m_file);
        JM::TreeMetaData* md = it->second->getTreeMetaData();
        if (md) {
            m_treeMetaDatas.push_back(md);
        }
        delete it->second;
    }

    // Tree for EvtNavigator will be written by file handle
    //if (m_navTree) m_navTree->Write(NULL,TObject::kOverwrite);

    // Set TreeMetaData
    TMDVector::iterator tit, tend = m_treeMetaDatas.end();
    for (tit = m_treeMetaDatas.begin(); tit != tend; ++tit) {
        m_file->addTreeMetaData(*tit);
    }

    // Dec file reference, close when it hits 0
    m_fileMgr->close_file(m_file->getName());

    // Reset pointers and clean gabage
    m_file = 0;
    delete m_headerTree;
    m_headerTree = 0;
    m_eventTrees.clear();
    m_navTree = 0;
    m_dir = 0;
    // Reset file entry
    m_fileEntries = 0;
    m_initialized = false;
    return true;
}

void RootFileWriter::newFile(RootOutputFileHandle* file)
{
    m_file = file;
}

void RootFileWriter::initialize()
{
    if ("unknown" == m_headerName) {
        // Still unknown stream, can not initialize
        return;
    }
    // Make the directories up to the path
    TFile* rootFile =  m_file->getFile();
    m_dir = rootFile;
    std::string tempPath = m_path[m_path.length() - 1] == '/' ? m_path : m_path + '/';
    std::string::size_type last = 0, slash = tempPath.find('/');
    for (; slash != std::string::npos; slash = tempPath.find('/',last)) {
        if (!slash) {
            last = slash + 1;
            continue;   // skip initial '/'
        }
        std::string subdir = tempPath.substr(last,slash-last);
        TDirectory* dir = m_dir->GetDirectory(subdir.c_str());
        if (dir) m_dir = dir;
        else {
          m_dir = m_dir->mkdir(subdir.c_str());
        }
        last = slash + 1;
    }
    rootFile->cd(m_path.c_str());

    // Create the OutputTreeHandle for header
    m_headerTree = new OutputTreeHandle(m_path, m_headerName, m_dir);
    // Create the OutputTreeHandle for event
    const std::vector<std::string>& eventNames = EDMManager::get()->getEventNameWithHeader(m_headerName);
    for (std::vector<std::string>::const_iterator it = eventNames.begin(); it != eventNames.end(); ++it) {
        m_eventTrees.insert(std::make_pair(*it, new OutputTreeHandle(m_path, *it, m_dir)));
    }

    JM::TreeMetaData* htmd = new JM::TreeMetaData();
    htmd->SetTreeName(tempPath + m_headerName.substr(m_headerName.rfind("::") + 2));
    htmd->SetObjName(m_headerName);
    m_treeMetaDatas.push_back(htmd);

    // Add file reference count only when the writer is initialized
    m_file->addRef();
    m_initialized = true;
}

void RootFileWriter::setAddress(JM::EvtNavigator* nav)
{
    m_navAddr = nav;
    JM::HeaderObject* header = nav->getHeader(m_path);
    m_headerTree->setAddress(header);
    String2TreeHandle::iterator it, end = m_eventTrees.end();
    for (it = m_eventTrees.begin(); it != end; ++it) {
        it->second->setAddress(header->event(it->first));
    }
}

void RootFileWriter::setHeaderName(const std::string& name)
{
    m_headerName = name;
}

int RootFileWriter::fileEntries()
{
    return m_fileEntries;
}

/*
void RootFileWriter::checkFilePath()
{
    // Make the TTree holding EvtNavigator
    // Save EvtNavigator together with the last path in the file
    if (m_file->isLastPath(m_path)) {
        m_withNav = true;
        m_navTree = m_file->getNavTree();
    }
}
*/

void RootFileWriter::resetAddress()
{
    m_navAddr = 0; 
    m_headerTree->resetAddress();
    String2TreeHandle::iterator it, end = m_eventTrees.end();
    for (it = m_eventTrees.begin(); it != end; ++it) {
        it->second->resetAddress();
    }
}
