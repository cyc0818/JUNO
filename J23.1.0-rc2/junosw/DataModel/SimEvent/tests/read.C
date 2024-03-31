#include <TFile.h>
#include <TTree.h>
#include <Event/SimHeader.h>
#include <Event/SimEvt.h>

#include <iostream>

void read3(TTree* tree3) {
    JM::SimHeader* sh3 = 0;
    JM::SimEvt* se3 = 0;
    tree3->SetBranchAddress("hdr", &sh3);
    tree3->SetBranchAddress("evt", &se3);

    for (int i = 0; i < tree3->GetEntries(); ++i) {
        tree3->GetEntry(i);
        std::cout << "+ Event " << i << " Begin" << std::endl;
        std::cout << "++ Event Type: " << sh3->getEventType() << std::endl;
        std::cout << "++ Run ID    : " << sh3->RunID() << std::endl;
        std::cout << "++ Event ID  : " << sh3->EventID() << std::endl;

        std::cout << std::endl;
    }

}

void read6(TTree* tree6) {
    // Load the hdrevt4
    JM::SimHeader* sh6 = 0;
    JM::SimEvt* se6 = 0;
    tree6->SetBranchAddress("hdr", &sh6);
    tree6->SetBranchAddress("evt", &se6);
    for (int i = 0; i < tree6->GetEntries(); ++i) {
        tree6->GetEntry(i);

        std::cout << std::endl;

    }

}

void read7(TTree* tree7) {
    JM::SimHeader* sh7 = 0;
    JM::SimEvt* se7 = 0;
    tree7->SetBranchAddress("hdr", &sh7);
    tree7->SetBranchAddress("evt", &se7);
    tree7->GetBranch("hdr")->SetAutoDelete(true);
    tree7->GetBranch("evt")->SetAutoDelete(true);
    for (int i = 0; i < tree7->GetEntries(); ++i) {
        tree7->GetEntry(i);

        std::cout << "+ Event: " << i << std::endl;
        // get hits
        const std::vector<JM::SimPMTHit*> hits = se7->getCDHitsVec();
        for (std::vector<JM::SimPMTHit*>::const_iterator it = hits.begin();
                it != hits.end(); ++it) {

            JM::SimPMTHit* hit = *it;

            std::cout << " ++ hit: " 
                      << hit->getPMTID()
                      << " "
                      << hit->getNPE()
                      << std::endl;
        }
    }

}

int main() {
    TFile* f = TFile::Open("sim.root");
    if (not f) {
        return -1;
    }

    // Load the hdrevt5
    TTree* tree7 = (TTree*)(f->Get("hdrevt5"));
    read7(tree7);

}
