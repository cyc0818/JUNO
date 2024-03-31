#include "sneventsinput.h"
using namespace std;
TSNEventsInput::TSNEventsInput(const char *filename)
{
	evtfile = new TFile(filename, "READ");
	evttree = dynamic_cast <TTree *> (evtfile->Get("SNEvents"));
	evttree->SetBranchAddress("evt", &evt);
}

TSNEventsInput::~TSNEventsInput()
{
	evtfile->Close();
	delete evtfile;
}

int TSNEventsInput::getEventsNumber()
{
	return (int) evttree->GetEntries();
}

void TSNEventsInput::getEvent(int nEntry)
{
	evttree->GetEntry(nEntry);
}

