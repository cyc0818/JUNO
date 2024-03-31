#include "sneventsoutput.h"
using namespace std;
TSNEventsOutput::TSNEventsOutput(const char *filename)
{
	evtfile = new TFile(filename, "RECREATE");
	evttree = new TTree("SNEvents", "All channels of events during an SN burst");
	evttree->Branch("evt", &evt, "nparticles/l:pdgid[2]/I:px[2]/D:py[2]/D:pz[2]/D:m[2]/D:t[2]/D:nuE/D:origPDGID/I");
}

TSNEventsOutput::~TSNEventsOutput()
{
	evtfile->Close();
	delete evtfile;
}

void TSNEventsOutput::pushEvents()
{
	evttree->Fill();
}

void TSNEventsOutput::writeFile()
{
	evtfile->Write();
}
