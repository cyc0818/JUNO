#include "TreeLooper.h"

#include "EDMUtil/RootFileInter.h"
#include "EDMUtil/FileMetaData.h"
#include "TTree.h"
#include "TFile.h"

#include <iostream>
#include <cassert>

// #define LT_DEBUG std::cout << __FILE__ << " " << __func__ << " " << __LINE__ << std::endl;gDirectory->pwd();gDirectory->ls()
#define LT_DEBUG

//------------------- TreeMerger ---------------------

TreeMerger::TreeMerger(const std::string& path, const std::string& objName)
       : m_oTree(0), m_iTree(0), m_addr(0), m_idx(0), m_entries(0)
{
    std::string title = "Tree at " + path + " holding " + objName;
    std::string treeName = objName.substr(objName.rfind("::")+2);
    std::string branchName = treeName;
    LT_DEBUG;
    m_oTree = new TTree(treeName.c_str(), title.c_str());
    m_oTree->Branch(branchName.c_str(), objName.c_str(), &m_addr);
    // m_oTree->SetDirectory(0);
    LT_DEBUG;
}

TreeMerger::~TreeMerger()
{
    //Yes, we do NOT delete TTree*
}

bool TreeMerger::next()
{
    if (!m_iTree || m_idx >= m_entries) {
        return false;
    }
    m_iTree->GetEntry(m_idx++);
    m_oTree->Fill();
    delete static_cast<TObject*>(m_addr);
    m_addr = 0;
    return true;
}

void TreeMerger::newTree(TTree* tree, const std::vector<Long64_t>& iBreakPoints)
{
    LT_DEBUG;
    if (m_oTree->GetEntries()) {
        m_breakPoints.push_back(m_oTree->GetEntries());
    }
    if (m_breakPoints.size()) {
        // Link the break points
        std::vector<Long64_t>::const_iterator it, end = iBreakPoints.end();
        for (it = iBreakPoints.begin(); it != end; ++it) {
            m_breakPoints.push_back(*it + m_oTree->GetEntries());
        }
    }
    else {
        // First file, replace the break points directly
        m_breakPoints = iBreakPoints;
    }
    LT_DEBUG;
    m_idx = 0;
    m_iTree = tree;
    static_cast<TBranch*>(m_iTree->GetListOfBranches()->At(0))->SetAddress(&m_addr);
    m_entries = m_iTree->GetEntries();
    LT_DEBUG;
}

void TreeMerger::writeTree()
{
    LT_DEBUG;
    m_oTree->Write(NULL,TObject::kOverwrite);
    LT_DEBUG;
}


//------------------- TreeLooper ---------------------

TreeLooper::TreeLooper(const PathMap& dataPathMap, TFile* file)
      : m_outputFile(file), m_inputFile(0), m_iNavTree(0), m_oNavTree(0), m_addr(0), m_idx(0), m_entries(0)
{
    // Build nav tree
    m_oNavTree = new TTree("navigator", "Tree for EvtNavigator");
    m_oNavTree->Branch("EvtNavigator", "JM::EvtNavigator", &m_addr);
    // Build data tree map
    PathMap::const_iterator it, end = dataPathMap.end();
    for (it = dataPathMap.begin(); it != end; ++it) {
        std::cout << "dataPathMap: " << it->first << " -> " << it->second << std::endl;
        // create the directory before create tree object
        std::string path = (it->first[0] == '/' ? it->first.substr(1) : it->first);
        std::string dir = path.substr(0,path.rfind('/'));
        std::cout << "directory: " << dir << std::endl;
        // always use the absolute path, so use m_outputFile.
        if (!m_outputFile->GetDirectory(dir.c_str())) {
            std::cout << "create directory: " << dir << std::endl;
            TDirectory* ret = m_outputFile->mkdir(dir.c_str());
            assert(ret);
        }
        m_outputFile->cd(dir.c_str());

        m_treeMap.insert(std::make_pair(it->first, new TreeMerger(it->first, it->second)));
    }
}

TreeLooper::~TreeLooper()
{
    TreeMap::iterator it, end = m_treeMap.end();
    for (it = m_treeMap.begin(); it != end; ++it) {
        delete it->second;
    }
}

bool TreeLooper::next()
{
    bool notYetDone = false;
    // Fill nav tree
    if (m_iNavTree && m_idx < m_entries) {
        notYetDone = true;
        m_iNavTree->GetEntry(m_idx++);
        m_oNavTree->Fill();
        delete static_cast<TObject*>(m_addr);
        m_addr = 0;
    }
    // Fill data trees
    TreeMap::iterator it, end = m_treeMap.end();
    for (it = m_treeMap.begin(); it != end; ++it) {
        // std::cout << "treemap: " << it->first << " -> " << "" << std::endl;
        notYetDone = it->second->next();
    }
    return notYetDone;
}

void TreeLooper::finalize()
{
    // Write nav tree
    LT_DEBUG;
    m_outputFile->cd();
    LT_DEBUG;
    if (!m_outputFile->GetDirectory("Meta")) {
        m_outputFile->mkdir("Meta");
    }
    m_outputFile->cd("Meta");
    LT_DEBUG;
    m_oNavTree->Write(NULL,TObject::kOverwrite);
    LT_DEBUG;
    // Write data trees
    TreeMap::iterator it, end = m_treeMap.end();
    for (it = m_treeMap.begin(); it != end; ++it) {
        LT_DEBUG;
        m_outputFile->cd();
        LT_DEBUG;
        // std::cout << "LT: it->first: " << it->first << std::endl;
        std::string fullPath = (it->first[0] == '/' ? it->first.substr(1) : it->first);
        // std::cout << "LT: fullpath: " << fullPath << std::endl;
        std::string dir = fullPath.substr(0,fullPath.rfind('/'));
        // std::cout << "LT: dir: " << dir << std::endl;
        if (!m_outputFile->GetDirectory(dir.c_str())) {
            m_outputFile->mkdir(dir.c_str());
        }
        m_outputFile->cd(dir.c_str());
        it->second->writeTree();
        m_breakPoints[it->first] = it->second->getBreakPoints();
    }
}

void TreeLooper::newInputFile(const std::string& value)
{
    //Read trees
    if (m_inputFile) {
        m_inputFile->Close();
        delete m_inputFile;
        m_inputFile = 0;
    }
    m_inputFile = TFile::Open(value.c_str(), "read");
    // Get nav tree
    m_iNavTree = RootFileInter::GetTree(m_inputFile, "/Meta/navigator");
    static_cast<TBranch*>(m_iNavTree->GetListOfBranches()->At(0))->SetAddress(&m_addr);
    m_entries = m_iNavTree->GetEntries();
    m_idx = 0;
    // Get break points from FileMetaData
    JM::FileMetaData* fmd = RootFileInter::GetFileMetaData(m_inputFile);
    BreakPointsMap& breakPoints = fmd->GetBreakPoints();
    // Get data trees
    TreeMap::iterator it, end = m_treeMap.end();
    for (it = m_treeMap.begin(); it != end; ++it) {
        TTree* dataTree = RootFileInter::GetTree(m_inputFile, it->first);
        it->second->newTree(dataTree, breakPoints[it->first]);
    }
}
