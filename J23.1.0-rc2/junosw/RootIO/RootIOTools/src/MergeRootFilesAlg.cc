#include <boost/python.hpp>
#include "MergeRootFilesAlg.h"
#include "TreeLooper.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"
#include "FileMetaDataMerger.h"
#include "UniqueIDTableMerger.h"
#include "EDMUtil/FileMetaData.h"
#include "EDMUtil/RootFileInter.h"

#include "TObject.h"
#include "TFile.h"

// #define LT_DEBUG std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << std::endl;gDirectory->pwd();gDirectory->ls()
#define LT_DEBUG

DECLARE_ALGORITHM(MergeRootFilesAlg);

MergeRootFilesAlg::MergeRootFilesAlg(const std::string& name)
    : AlgBase(name)
    , m_index(0)
    , m_outputFile(0)
    , m_treeLooper(0)
{
    declProp("InputFiles", m_inputFileNames);
    declProp("OutputFile", m_outputFileName);
}

MergeRootFilesAlg::~MergeRootFilesAlg()
{
}

bool MergeRootFilesAlg::initialize()
{
    LT_DEBUG;
    // Reasonableness check
    PathMap dataPathMap;
    bool ok = this->rationalityCheck(dataPathMap);
    LT_DEBUG;
    if (!ok) {
        LogError << "Failed on rationality check, can not merge. Please check input files."
                 << std::endl;
        return false;
    }
    LT_DEBUG;

    // Create output file
    m_outputFile = TFile::Open(m_outputFileName.c_str(), "recreate");
    LT_DEBUG;

    // Create all the mergers
    IMerger* metaDataMerger = new FileMetaDataMerger(&m_mdBreakPoints);
    LT_DEBUG;
    IMerger* uidMerger = new UniqueIDTableMerger();
    LT_DEBUG;
    m_objMergers.push_back(metaDataMerger);
    m_objMergers.push_back(uidMerger);
    LT_DEBUG;
    m_treeLooper = new TreeLooper(dataPathMap, m_outputFile);
    LT_DEBUG;

    LogInfo << "Successfully initialized!" << std::endl;
    return true;
}

bool MergeRootFilesAlg::execute()
{
    // LogInfo << "EXECUTE ONE" << std::endl;
    bool ok = m_treeLooper->next();
    if (!ok) {
        if (m_index < m_inputFileNames.size()) {
            // Start a new file
            m_treeLooper->newInputFile(m_inputFileNames[m_index++]);
        }
        else {
            // Incident::fire(*getParent(), "StopRun");
            return dynamic_cast<Task*>(getRoot())->stop();
        }
    }
    return true;
}

bool MergeRootFilesAlg::finalize()
{
    LT_DEBUG;
    // if failed in initialize, m_treeLooper is not created.
    if (!m_treeLooper) {
        return true;
    }
    // Finalize tree looper
    m_treeLooper->finalize();
    LT_DEBUG;
    m_mdBreakPoints = m_treeLooper->getBreakPoints();
    LT_DEBUG;

    // Merge the additionally TObjects
    std::vector<IMerger*>::iterator it;
    TObject* obj = 0;
    for (it = m_objMergers.begin(); it != m_objMergers.end(); ++it) {
    LT_DEBUG;
        (*it)->setInputFile(m_inputFileNames);
    LT_DEBUG;
        (*it)->setOutputFile(m_outputFileName);
    LT_DEBUG;
        std::string path, name;
        (*it)->merge(obj, path, name);
    LT_DEBUG;
        LogInfo << "path/name: " << path << " / " << name << std::endl;
    LT_DEBUG;
        this->writeObj(obj, path, name);
    LT_DEBUG;
    }

    for (it = m_objMergers.begin(); it != m_objMergers.end(); ++it) {
        delete *it;
    }
    delete m_treeLooper;
    return true;
}

bool MergeRootFilesAlg::rationalityCheck(PathMap& dataPathMap)
{
    // Check the input file list to see if they were capable of being merged.
    if (m_inputFileNames.size() <= 1) {
        LogError << "No input file or just one input file specified, can not merge." << std::endl;
        return false;
    }
    std::vector<std::string>::iterator it, end = m_inputFileNames.end();
    JM::FileMetaData* firstFmd = 0;
    JM::FileMetaData* fmd = 0;
    for (it = m_inputFileNames.begin(); it != end; ++it) {
        TFile* file = TFile::Open(it->c_str(), "read");
        if (!file->IsOpen()) {
            LogError << "Failed to open file: " << *it << std::endl;
            delete file;
            return false;
        }
        fmd = RootFileInter::GetFileMetaData(file);
        file->Close();
        delete file;
        if (!fmd) {
            LogError << "Failed to read file meta data from file: " << *it << std::endl;
            return false;
        }
        if (!firstFmd) {
            // First file, made as reference
            firstFmd = fmd;
            continue;
        }
        if (!firstFmd->IsSameAs(fmd)) {
            LogError << "File: " << *it << " is not same, can not merge" << std::endl;
            delete fmd;
            return false;
        }
        delete fmd;
    } 
    const JM::FileMetaData::TMDVector& tmds = firstFmd->GetTreeMetaData();
    JM::FileMetaData::TMDVector::const_iterator tit, tend = tmds.end();
    for (tit = tmds.begin(); tit != tend; ++tit) {
        dataPathMap.insert(std::make_pair((*tit)->GetTreeName(),(*tit)->GetObjName()));
    }
    delete firstFmd;
    return true;
}

void MergeRootFilesAlg::writeObj(TObject* obj, const std::string& path, const std::string& name)
{
    LT_DEBUG;
    m_outputFile->cd();
    LT_DEBUG;
    if (!gDirectory->GetDirectory(path.c_str())) {
        gDirectory->mkdir(path.c_str());
    }
    gDirectory->cd(path.c_str());
    LT_DEBUG;
    obj->Write(name.c_str());
    LT_DEBUG;
    delete obj;
}
