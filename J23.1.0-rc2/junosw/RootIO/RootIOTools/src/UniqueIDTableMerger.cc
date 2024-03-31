#include "UniqueIDTableMerger.h"
#include "EDMUtil/UniqueIDTable.h"
#include "EDMUtil/RootFileInter.h"
#include "TFile.h"

UniqueIDTableMerger::UniqueIDTableMerger()
    : IMerger()
{
}

UniqueIDTableMerger::~UniqueIDTableMerger()
{
}

void UniqueIDTableMerger::merge(TObject*& obj, std::string& path, std::string& name)
{
    JM::UniqueIDTable* oTable = new JM::UniqueIDTable;
    IMerger::StringVector::iterator it, end = m_inputFiles.end();
    for (it = m_inputFiles.begin(); it != end; ++it) {
        TFile* file = TFile::Open(it->c_str(), "read");
        JM::UniqueIDTable* iTable = RootFileInter::GetUniqueIDTable(file);
        oTable->MergeTable(*iTable);
        file->Close();
        delete file;
        delete iTable;
    }
    obj = oTable;
    path = "Meta";
    name = "UniqueIDTable";
}
