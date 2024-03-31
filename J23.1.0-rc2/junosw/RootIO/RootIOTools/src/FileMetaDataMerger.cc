#include "FileMetaDataMerger.h"
#include "EDMUtil/RootFileInter.h"
#include "EDMUtil/FileMetaData.h"
#include "TFile.h"
#include <algorithm>
FileMetaDataMerger::FileMetaDataMerger(std::map<std::string, std::vector<Long64_t> >* breakPoints)
       : IMerger(), m_breakPoints(breakPoints)
{
}

FileMetaDataMerger::~FileMetaDataMerger()
{
}

void FileMetaDataMerger::merge(TObject*& obj, std::string& path, std::string& name)
{
    JM::FileMetaData* ofmd = 0;
    IMerger::StringVector::iterator it, end = m_inputFiles.end();
    std::vector<std::string> ouidlist;
    for (it = m_inputFiles.begin(); it != end; ++it) {
        TFile* file = TFile::Open(it->c_str(), "read");
        JM::FileMetaData* ifmd = RootFileInter::GetFileMetaData(file);
        if (!ofmd) {
            ofmd = new JM::FileMetaData(*ifmd);
            ofmd->SetBreakPoints(*m_breakPoints);
        }
        std::vector<std::string>& iuidlist = ifmd->GetUUIDList();
        std::vector<std::string>::iterator uit, uend = iuidlist.end();
        for (uit = iuidlist.begin(); uit != uend; ++uit) {
            if (std::find(ouidlist.begin(), ouidlist.end(), *it) == ouidlist.end()) ouidlist.push_back(*uit);
        }
        delete ifmd;
        file->Close();
        delete file;
    }
    ofmd->SetUUIDList(ouidlist);
    obj = ofmd;
    path = "Meta";
    name = "FileMetaData";
}
