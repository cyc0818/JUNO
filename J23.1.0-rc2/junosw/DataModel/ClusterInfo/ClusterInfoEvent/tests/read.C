

#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TClass.h>
#include "Event/ClusterInfoHeader.h"
#include "Event/ClusterInfoEvt.h"
#include "../Event/ClusterHitIdx.h"
#include "Event/ClusterHitCol.h"
#include <iostream>

void read(TTree* mytree) {
    JM::ClusterInfoHeader* cih = 0;
    JM::ClusterInfoEvt* cie = 0;
    mytree->SetBranchAddress("hdr", &cih);
    mytree->SetBranchAddress("evt", &cie);
    mytree->GetBranch("hdr")->SetAutoDelete(true);
    mytree->GetBranch("evt")->SetAutoDelete(true);
    for (int i = 0; i < mytree->GetEntries(); ++i) {
        mytree->GetEntry(i);

        std::cout << "+ Event: " << i << std::endl;
        std::cout << "Number of clusters: " << cie->getNCluster() << std::endl;
        // get hits
        for(UInt_t j = 0; j<cie->getNCluster() ;++j){
            //for(UInt_t k = 0;k<cie->getCluster(j)->getSize();++k){
                //std::cout << "cluster " << j << "\t channel " << cie->getCluster(j)->getChannel(k) << "\t hit " << cie->getCluster(j)->getHit(k) << '\n';   <-TODO 
            //}
            //std::vector<JM::ClusterHitIdx*> myCluster = cie->getCluster(j);
            //std::vector<double*> myCluster &= std::vector<double*>(200);
            //for(std::vector<JM::ClusterHitIdx*>::const_iterator it = myCluster.begin();it!=myCluster.end();++it){
            //    std::cout << "cluster " << j << "\t channel " << (*it)->channel << "\t hit " << (*it)->hit << '\n';
            //}
        }
    }
}


int main() {
    TFile* f = TFile::Open("clustertest.root");
    if (not f) {
        return -1;
    }
    // Load the hdrevt5
    TTree* mytree = (TTree*)(f->Get("hdrevt5"));
    read(mytree);

}

