#include <TFile.h>
#include <TTree.h>

#include <Context/TimeStamp.h>

void write() {
    TFile* f = new TFile("test.root", "recreate");
    TTree* tree = new TTree("context", "context");

    TimeStamp* ts = new TimeStamp;

    tree->Branch("timestamp", &ts);

    for (Int_t i = 0; i < 100; ++i) {

        *ts = TimeStamp(20140326,140000,i); 
        tree->Fill();
    }
    tree->Write();
    f->Close();
}

int main() {
    write();
}
