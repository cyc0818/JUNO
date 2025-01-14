#include "RootIOUtil/RootOutputFileManager.h"

#include "TTree.h"
#include "TFile.h"
#include "TGeoManager.h"
#include "EDMUtil/JobInfo.h"
#include "EDMUtil/FileMetaData.h"
#include "EDMUtil/UniqueIDTable.h"

#include <iostream>
#include <fstream>
#include <algorithm>

bool RootOutputFileHandle::s_isReproducible = false;

void RootOutputFileHandle::setReproducible(bool v) {
    s_isReproducible = v;
}

std::string
RootOutputFileHandle::getURLWithOption(const std::string& p) {
    std::string path = p;
    if (s_isReproducible) {
        path += "?reproducible";
    }
    return path;
}

RootOutputFileHandle::RootOutputFileHandle(const std::string& filename, 
                                           const std::map<std::string, int>& paths)
    : m_file(TFile::Open(getURLWithOption(filename).c_str(),"recreate","RootOutputStream file"))
    , m_navTree(new TTree("navigator", "Tree for EvtNavigator"))
    , m_fileMetaData(new JM::FileMetaData())
    , m_IDTable(new JM::UniqueIDTable())
    , m_jobInfo(0)
    , m_navAddr(0)
    , m_refCount(0)
    , m_filename(filename)
{
    std::map<std::string, int>::const_iterator it, end = paths.end();
    int pri = 0;
    for (it = paths.begin(); it != end; ++it) {
        m_paths.push_back(it->first);
        if (it->second > pri) pri = it->second;
    }
    m_unwrittenPath = m_paths;
    m_fileMetaData->SetNavPriority(pri);
    m_navTree->Branch("EvtNavigator", "JM::EvtNavigator", &m_navAddr);
}

RootOutputFileHandle::~RootOutputFileHandle()
{
    delete m_file;
    delete m_fileMetaData;
    delete m_IDTable;
}

std::string RootOutputFileHandle::getName()
{
    return m_filename;
}

TFile* RootOutputFileHandle::getFile()
{
    return m_file;
}

void RootOutputFileHandle::addRef()
{
    ++m_refCount;
}

void RootOutputFileHandle::addGeoManager(TGeoManager* geo)
{
    if (std::find(m_geos.begin(), m_geos.end(), geo) == m_geos.end()) {
        m_geos.push_back(geo);
    }
}

void RootOutputFileHandle::setJobInfo(JobInfo* jobInfo)
{
    if (!m_jobInfo) {
        m_jobInfo = jobInfo;
        jobInfo->addRef();
    }
}

void RootOutputFileHandle::addUniqueIDTable(const std::string& treename,
                                            const std::vector<std::string>& guid,
                                            const std::vector<std::vector<Int_t> >& uid,
                                            const std::vector<std::vector<Short_t> >& bid)
{
    m_IDTable->AddTable(treename, guid, uid, bid);
}

void RootOutputFileHandle::addUUID(const std::vector<std::string>& uuids)
{
    // Insert new uuids
    m_UUIDList.insert(m_UUIDList.end(), uuids.begin(), uuids.end());
    // Erase reduplicated uuids
    std::sort(m_UUIDList.begin(), m_UUIDList.end());
    m_UUIDList.erase(std::unique(m_UUIDList.begin(), m_UUIDList.end()), m_UUIDList.end());
}

int RootOutputFileHandle::decRef()
{
    return --m_refCount;
}

void RootOutputFileHandle::close()
{
    m_fileMetaData->SetUUIDList(m_UUIDList);

    m_file->mkdir("Meta");
    m_file->cd("Meta");

    // Write out job info
    if (!m_jobInfo) {
        m_jobInfo = new JobInfo;
    }
    m_jobInfo->Write();
    m_jobInfo->decRef();
    m_fileMetaData->SetNavPath(m_navPath);

    // Write out TTree holding EvtNavigator
    m_navTree->Write(NULL,TObject::kOverwrite);
    // Write out file meta data
    m_fileMetaData->Write("FileMetaData");
    // Write out the UniqueIDTable
    m_IDTable->Write("UniqueIDTable");

    // Write out geometry infomation
    if (m_geos.size()) {
        // Shield noizy log info for the moment
        std::streambuf *backup;
        std::ofstream fout;
        fout.open("/dev/null");
        backup = std::cout.rdbuf();
        std::cout.rdbuf(fout.rdbuf());
        GeoVector::iterator it, end = m_geos.end();
        m_file->cd();
        for (it = m_geos.begin(); it != end; ++it) {
            if (strlen((*it)->GetName()) == 0) (*it)->SetName("JunoGeo");
            (*it)->Write();
        }
        std::cout.rdbuf(backup);
    }    
    m_file->Close(); 
}

const std::vector<std::string>& RootOutputFileHandle::setNavPath(const StringVector& paths)
{
    if (!m_navPath.size()) {
        m_navPath = paths;
        return m_navPath;
    }
    StringVector::const_iterator it, end = paths.end();
    for (it = paths.begin(); it != end; ++it) {
        if (std::find(m_navPath.begin(), m_navPath.end(), *it) == m_navPath.end()) {
            m_navPath.push_back(*it);
        }
    }
    return m_navPath;
}

void RootOutputFileHandle::resetUnwrittenPath() {
    m_unwrittenPath = m_paths;
}

const std::vector<std::string>& RootOutputFileHandle::getUnwrittenPaths()
{
    return m_unwrittenPath;
}

void RootOutputFileHandle::addWrittenPath(const std::string& path) {
    auto it = std::find(m_unwrittenPath.begin(), m_unwrittenPath.end(), path);
    if (it != m_unwrittenPath.end()) m_unwrittenPath.erase(it);
}

void RootOutputFileHandle::addTreeMetaData(JM::TreeMetaData* treemetadata)
{
    m_fileMetaData->AddTreeMetaData(treemetadata);
}

TTree* RootOutputFileHandle::getNavTree()
{
    return m_navTree;
}

void RootOutputFileHandle::setNavAddr(void* navAddr)
{
    m_navAddr = navAddr;
}

bool RootOutputFileHandle::hasPath(const std::string& path)
{
    if (std::find(m_paths.begin(),m_paths.end(),path) != m_paths.end()) return true;
    return false; 
}

void RootOutputFileHandle::revise(int priority)
{
    // Reset the priority if it's larger
    if (priority > m_fileMetaData->GetNavPriority()) {
        m_fileMetaData->SetNavPriority(priority);
    }
}

/*   RootOutputFileManager   */


RootOutputFileManager::RootOutputFileManager()
{
}

RootOutputFileManager::~RootOutputFileManager()
{
}

void RootOutputFileManager::new_file(const std::string& filename, 
                                     const std::map<std::string, int>& pathMap)
{
    FileMap::iterator it = m_filemap.find(filename);
    if (it == m_filemap.end()) {
        // Generate a new RootOutputFile with the paths and coresponding data priotity
        RootOutputFileHandle* filehandle = new RootOutputFileHandle(filename, pathMap);
        m_filemap[filename] = filehandle;
    }
    return;
}

RootOutputFileHandle* RootOutputFileManager::get_file_with_name(const std::string& filename) 
{
    FileMap::iterator it = m_filemap.find(filename);
    if (it == m_filemap.end()) return 0;
    return it->second;
}

RootOutputFileHandle* RootOutputFileManager::get_file_with_path(const std::string& path)
{
    FileMap::iterator it, end = m_filemap.end();
    for (it = m_filemap.begin(); it != end; ++it) {
        if (it->second->hasPath(path)) {
            //it->second->addRef();
            return it->second;
        }
    }
    return 0;
}

void RootOutputFileManager::close_file(const std::string& filename)
{
    FileMap::iterator it = m_filemap.find(filename);
    if (it == m_filemap.end()) {
        // Can not find the file, may be already closed.
        return;
    }

    int refCount = it->second->decRef();
    if (refCount) return;  // still more references

    it->second->close();
    // Delete the file when it's closed
    delete it->second;
    m_filemap.erase(it); 
}

void RootOutputFileManager::reviseOutputFile(const std::string& filename, int priority)
{
    FileMap::iterator it = m_filemap.find(filename);
    if (it == m_filemap.end()) {
        return;
    }

    it->second->revise(priority);
}

void RootOutputFileManager::reset()
{
    for (auto it = m_filemap.begin(); it != m_filemap.end(); ++it) {
        delete it->second;
    }
    m_filemap.clear();
}
