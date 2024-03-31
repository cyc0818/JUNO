#define NPERelation_cxx
#include "NPERelation.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH3F.h>
#include <TNtuple.h>
#include <TProfile2D.h>
#include <TMath.h>
#include <iostream>
#include <TAxis.h>

void NPERelation::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L NPERelation.C
//      Root > NPERelation t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TFile * f = TFile::Open(output, "recreate");

   const int Nr = 100; // radial direction
   const int Nt = 180; // angle

   // 100 bins of R3
   bins_r = new TAxis(Nr, 0, 17.7);
   bins_r3 = new TAxis(Nr, 0, 5600.);
   // 180 bins of theta [0, 180.01) . To make 180 is included
   bins_theta = new TAxis(Nt, 0, 180.01*TMath::Pi()/Nt);
   bins_costh = new TAxis(Nt, -1, 1.01);

   if (flag_r == kR3) {
       std::cout << "use R3 mode" << std::endl;
   } else if (flag_r == kR) {
       std::cout << "use R mode" << std::endl;
   }     

   if (flag_t == kCosTheta) {
       std::cout << "use CosTheta mode" << std::endl;
   } else if (flag_t == kTheta) {
       std::cout << "use Theta mode" << std::endl;
   }     

   TH1I* ths_npe[Nr][Nt];
   for (int i = 0; i < Nr; ++i) {
       for (int j = 0; j < Nt; ++j) {
           TString th_name = TString::Format("%d", i*Nt+j);
           ths_npe[i][j] = new TH1I(th_name, th_name, 50, 0, 50);
       }
   }

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      Int_t binx = -1;
      Int_t biny = -1;

      if (flag_r == kR3) {
          Double_t r3 = TMath::Power(r/1e3, 3);
          binx = bins_r3->FindBin(r3);
      } else if (flag_r == kR) {
          binx = bins_r->FindBin(r/1e3);
      }

      if (flag_t == kCosTheta) {
          Double_t ct = TMath::Cos(theta);
          biny = bins_costh->FindBin(ct);
      } else if (flag_t == kTheta) {
          biny = bins_theta->FindBin(theta);
      }

      if (1<=binx and binx<=Nr and 1<=biny and biny<=Nt) {
          ths_npe[binx-1][biny-1]->Fill(nPE);
      }
   }
   std::cout << nentries << std::endl;
   f->Write();
   f->Close();
}
