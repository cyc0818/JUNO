//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Sep 12 15:58:48 2015 by ROOT version 5.34/11
// from TTree nperelation/nperelation
// found on file: relation-npe.root
//////////////////////////////////////////////////////////

#ifndef NPERelation_h
#define NPERelation_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TAxis.h>
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class NPERelation {
public:
    enum Mode {
        kUnknown = 0,
        kR3 = 1, kR = 2,
        kCosTheta = 3, kTheta = 4
    };


public:
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         r;
   Float_t         theta;
   Int_t           nPE;

   // List of branches
   TBranch        *b_r;   //!
   TBranch        *b_theta;   //!
   TBranch        *b_nPE;   //!

   TString output;

   TAxis* bins_r3;       // R3
   TAxis* bins_r;        // R
   TAxis* bins_theta;    // T
   TAxis* bins_costh;    // CT

   int flag_r; // r or r3
   int flag_t; // theta or cos(theta)

   NPERelation(TTree *tree=0);
   virtual ~NPERelation();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef NPERelation_cxx
NPERelation::NPERelation(TTree *tree) : fChain(0) 
, flag_r(kR), flag_t(kTheta)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("relation.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("relation.root");
      }
      f->GetObject("nperelation",tree);

   }
   Init(tree);
}

NPERelation::~NPERelation()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NPERelation::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NPERelation::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NPERelation::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("r", &r, &b_r);
   fChain->SetBranchAddress("theta", &theta, &b_theta);
   fChain->SetBranchAddress("nPE", &nPE, &b_nPE);
   Notify();
}

Bool_t NPERelation::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NPERelation::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NPERelation::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef NPERelation_cxx
