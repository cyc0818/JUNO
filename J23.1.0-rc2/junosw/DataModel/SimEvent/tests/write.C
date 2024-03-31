#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TClass.h>
#include <Event/SimHeader.h>
#include <Event/SimEvt.h>

#include <iostream>

void test1(TTree* tree) {
    std::cout << "Test Header" << std::endl;

    JM::SimHeader* sh = new JM::SimHeader;
    tree->Branch("sim_event", "JM::SimHeader", &sh);

    for (Int_t i = 0; i < 100; ++i) {
        sh->setRunID(i % 1000);
        sh->setEventID(i);
        sh->setEventType("IBD");

        tree->Fill();
    }
}

void test2(TTree* tree2) {
    std::cout << "Test Event" << std::endl;

    JM::SimEvt* se = new JM::SimEvt;
    tree2->Branch("sim_event", "JM::SimEvt", &se);
    for (Int_t i = 0; i < 100; ++i) {
        std::cout << "Begin Event " << i << std::endl;
        for (int hit = 0; hit < 1000; ++hit) {
            se->addCDHit();
        }
        tree2->Fill();
        std::cout << "End Event " << i << std::endl;
        se->Clear();
        std::cout << "Clear Tracks " << i << std::endl;
    }

}

void test3(TTree* tree3) {
    std::cout << "Test Event End" << std::endl;
    JM::SimHeader* sh3 = new JM::SimHeader;
    JM::SimEvt* se3 = new JM::SimEvt;
    tree3->Branch("hdr", "JM::SimHeader", &sh3);
    tree3->Branch("evt", "JM::SimEvt", &se3);
    for (Int_t i = 0; i < 100; ++i) {
        sh3->setRunID(i % 1000);
        sh3->setEventID(i);
        sh3->setEventType("IBD");
        for (int trk = 0; trk < 10; ++trk) {
            se3->addTrack();
        }
        for (int hit = 0; hit < 100000; ++hit) {
            se3->addCDHit();
        }
        // sh3->setEvent(se3);

        tree3->Fill();
        se3->Clear();
    }

}

void test4(TTree* tree4) {
    std::cout << "Test Heasder+Event RootFileWriter" << std::endl;
    // New try to use the code in RootFileWriter
    void* sh4 = 0;
    void* se4 = 0;
    tree4->Branch("hdr", "JM::SimHeader", &sh4);
    tree4->Branch("evt", "JM::SimEvt", &se4);
    for (Int_t i = 0; i < 100; ++i) {
        std::cout << "Handle Event " << i << std::endl;
        JM::SimHeader* sh = (JM::SimHeader*)(sh4);
        JM::SimEvt* se = (JM::SimEvt*)(se4);
        se->Clear();
        sh->setRunID(i % 1000);
        sh->setEventID(i);
        sh->setEventType("IBD");
        for (int trk = 0; trk < 10; ++trk) {
            se->addTrack();
        }
        for (int hit = 0; hit < 100000; ++hit) {
            se->addCDHit();
        }
        // sh->setEvent(se);

        tree4->Fill();

    }

}

void test5(TTree* tree5) {
    std::cout << "Test Heasder+Event old RootIO" << std::endl;
    for (Int_t i = 0; i < 0; ++i) {
        std::cout << "Old RootIO Handle Event " << i << std::endl;
        // JM::SimHeader* sh = new JM::SimHeader;
        // JM::SimEvt* se = new JM::SimEvt;
        JM::SimHeader* sh = 0;
        JM::SimEvt* se = 0;
        tree5->Branch("hdr", "JM::SimHeader", &sh);
        tree5->Branch("evt", "JM::SimEvt", &se);
        sh->setRunID(i % 1000);
        sh->setEventID(i);
        sh->setEventType("IBD");
        for (int trk = 0; trk < 10; ++trk) {
            se->addTrack();
        }
        for (int hit = 0; hit < 100000; ++hit) {
            se->addCDHit();
        }
        // sh3->setEvent(se3);
        //
        //gROOT->ProcessLine(".class JM::SimPMTHit");
        //if (TClass::GetClass("JM::SimPMTHit")) {
        //    TClass::GetClass("JM::SimPMTHit")->Dump();
        //} else {
        //    std::cout << "No TClass for MyClass!" << std::endl;
        //}



        // tree5->ResetBranchAddresses();
        tree5->Fill();

        delete sh;
        se->Clear();
        delete se;
    }

}

void test6(TTree* tree6) {
    std::cout << "Test Heasder+Event RootFileWriter'" << std::endl;
    void* sh6 = 0;
    void* se6 = 0;
    tree6->Branch("hdr", "JM::SimHeader", &sh6);
    tree6->Branch("evt", "JM::SimEvt", &se6);
    for (Int_t i = 0; i < 10000; ++i) {
        std::cout << "Handle Event " << i << std::endl;
        JM::SimHeader* sh = new JM::SimHeader;
        JM::SimEvt* se = new JM::SimEvt;
        se->Clear();
        sh->setRunID(i % 1000);
        sh->setEventID(i);
        sh->setEventType("IBD");
        for (int trk = 0; trk < 10; ++trk) {
            se->addTrack();
        }
        for (int hit = 0; hit < 100000; ++hit) {
            se->addCDHit();
        }
        // sh->setEvent(se);
        //
        sh6 = sh;
        se6 = se;

        tree6->Fill();

        delete sh;
        delete se;

    }


}

void test7(TTree* tree7) {
    std::cout << "Test Heasder+Event RootFileWriter No Data'" << std::endl;
    void* sh7 = 0;
    void* se7 = 0;
    tree7->Branch("hdr", "JM::SimHeader", &sh7);
    tree7->Branch("evt", "JM::SimEvt", &se7);
    for (Int_t i = 0; i < 10000; ++i) {
        std::cout << "Handle Event " << i << std::endl;
        JM::SimHeader* sh = new JM::SimHeader;
        // JM::SimEvt* se = new JM::SimEvt(0);
        JM::SimEvent* se = new JM::SimEvent;
        se->Clear();
        sh->setRunID(i % 1000);
        sh->setEventID(i);
        sh->setEventType("IBD");
        for (int trk = 0; trk < 10; ++trk) {
            JM::SimTrack* trkobj = se->addTrack();
            trkobj -> setTrackID(trk);
        }
        for (int hit = 0; hit < 10000; ++hit) {
            JM::SimPMTHit* hitobj = se->addCDHit();
            hitobj -> setPMTID(hit);
            hitobj -> setNPE(1);
            hitobj -> setHitTime(100.0);
            hitobj -> setTimeWindow(0.0);
        }
        // sh->setEvent(se);
        //
        sh7 = sh;
        se7 = se;

        tree7->Fill();

        delete sh;
        delete se;

    }

}

int main() {
    TFile* f = new TFile("sim.root", "RECREATE");
    TTree* tree7 = new TTree("hdrevt5", "Sim Event");


    test7(tree7);
    
    



    f->Write();
    f->Close();
}
