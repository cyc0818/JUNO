#if defined(__CINT__) && ! defined(__ACLIC__) 
#  error "Plase run in compiled mode: root -l read_detsim.C+"
#endif


#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>

#include <iostream>
#include <vector>

#include <Event/SimHeader.h>

void read7(TFile* f) {
    TTree* tree_hdr = (TTree*)f->Get("Event/Sim/SimHeader");
    TTree* tree_evt = (TTree*)f->Get("Event/Sim/SimEvt");
    JM::SimHeader* sh7 = 0;
    JM::SimEvt* se7 = 0;
    tree_hdr->SetBranchAddress("SimHeader", &sh7);
    tree_evt->SetBranchAddress("SimEvt", &se7);
    tree_hdr->GetBranch("SimHeader")->SetAutoDelete(true);
    tree_evt->GetBranch("SimEvt")->SetAutoDelete(true);

    for (int i = 0; i < tree_evt->GetEntries(); ++i) {
        tree_evt->GetEntry(i);

        Int_t totalpe = 0;
        Int_t totalpe_tt = 0;
        const std::vector<JM::SimPMTHit*> hits = se7->getCDHitsVec();
        const std::vector<JM::SimTTHit*>& hits_tt = se7->getTTHitsVec();
        for (std::vector<JM::SimPMTHit*>::const_iterator it = hits.begin();
                it != hits.end(); ++it) {

            JM::SimPMTHit* hit = *it;
            totalpe += hit->getNPE();
        }

        for (std::vector<JM::SimTTHit*>::const_iterator it_tt = hits_tt.begin();
                it_tt != hits_tt.end(); ++it_tt) {

            JM::SimTTHit* hit = *it_tt;
            totalpe_tt += hit->getPE();
        }
        

        std::cout << "Event " << i << " : " << totalpe;

        std::cout << std::endl;

    }

}

int read_detsim(const TString& fn="sample_detsim.root") {
    // gROOT->ProcessLine(".L $SIMEVENTV2ROOT/$CMTCONFIG/libSimEvent.so");
    TFile* f = TFile::Open(fn);
    if (!f) {
        return -1;
    }

    // Load the hdrevt5
    read7(f);
    return 0;
}
