#include <TFile.h>
#include <TTree.h>

#include <Context/TimeStamp.h>
#include <iostream>

void read() {
    TFile* f = new TFile("test.root");
    TTree* tree = (TTree*) f->Get("context");

    TimeStamp* ts = new TimeStamp;

    tree->SetBranchAddress("timestamp", &ts);

    for (Int_t i = 0; i < 100; ++i) {
        tree->GetEntry(i);
        std::cout << (*ts) << std::endl;
    }
    f->Close();
}

int main() {
    read();
}
