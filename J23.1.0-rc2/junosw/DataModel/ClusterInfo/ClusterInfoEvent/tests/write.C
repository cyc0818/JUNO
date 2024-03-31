#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TClass.h>
#include "Event/ClusterInfoHeader.h"
#include "Event/ClusterInfoEvt.h"
#include "Event/ClusterHitIdx.h"
#include "Event/ClusterHitCol.h"

#include <iostream>
#include <utility>

void testit(TTree* mytree) {
    std::cout << "Test Header+Event RootFileWriter No Data'" << std::endl;
    //JM::ClusterInfoHeader* cih = 0;
    //JM::ClusterInfoEvt* cie = 0;
    void* cih = 0;
    void* cie = 0;
    mytree->Branch("hdr", "JM::ClusterInfoHeader", &cih);
    mytree->Branch("evt", "JM::ClusterInfoEvt", &cie);
    for (Int_t i = 0; i < 100; ++i) {
        std::cout << "Handle Event " << i << std::endl;
        JM::ClusterInfoHeader* tmpcih = new JM::ClusterInfoHeader;
        // JM::SimEvent* se = new JM::SimEvent(0);
        JM::ClusterInfoEvt* tmpcie = new JM::ClusterInfoEvt;
        //tmpcie->Clear();
        tmpcih->setRunID(i % 1000);
        tmpcih->setEventID(i);
        for (UInt_t clidx = 0;clidx<3;clidx++){
            for (UInt_t chidx = 0; chidx<10000;chidx++){
               tmpcie->AddHitToCluster(chidx,0,clidx);
               tmpcie->AddHitToCluster(chidx,1,clidx);
               tmpcie->AddHitToCluster(chidx,2,clidx,2.);
            }
        }
        // sh->setEvent(se);
        //
        //tmpcih->setEvent(tmpcie);
        cih = tmpcih;
        cie = tmpcie;

        
        mytree->Fill();

        delete tmpcih;
        delete tmpcie;

    }

}



int main() {
    TFile* f = new TFile("clustertest.root", "RECREATE");
    TTree* mytree = new TTree("hdrevt5", "Cluster Info Event");

    testit(mytree);

    f->Write();
    f->Close();
}
