#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TString.h"
#include "TObjString.h"
#include "TChain.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/TMVAGui.h"
#include "TCut.h"

using namespace TMVA;
void tmva_train() {

  TString sigfname = "/scratchfs/dyw/chengjie/PSDrec/DSNB/FV1+FV2/dsnb_input_fvboth_p1train.root";
  TString bkgfname = "/scratchfs/dyw/chengjie/PSDrec/atmNC/FV1+FV2/atm_input_fvboth_p1train.root";

  TFile *Output1 = new TFile(bkgfname);
  TFile *Output2 = new TFile(sigfname);
  TTree *tSig = (TTree *)Output2->Get("sigtree_new");
  TTree *tBkg = (TTree *)Output1->Get("bkgtree_new");

  TString outfileName = "/scratchfs/dyw/chengjie/PSDtmvaoutput/tmva_output_trainfv1fv2.root";
  TFile *outputFile = TFile::Open(outfileName, "RECREATE");
  std::string fWeightOutput = "train1.root";
  double SigWeight = 1.0;
  double BkgWeight = 1.0;

    TMVA::DataLoader *dataloader = new TMVA::DataLoader("AtmNCRejection_FV1FV2");
  TMVA::Factory *factory = new TMVA::Factory("Induction_train", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
  dataloader->AddBackgroundTree(tBkg, BkgWeight);
  dataloader->AddSignalTree(tSig, SigWeight);
  dataloader->AddVariable("w1", 'F');
  dataloader->AddVariable("w2", 'F');
  dataloader->AddVariable("tau1", 'F');
  dataloader->AddVariable("tau2", 'F');
  dataloader->AddVariable("eta1", 'F');
  dataloader->AddVariable("ndark", 'F');
  dataloader->AddVariable("rpsdpeak_ham", 'F');
  dataloader->AddVariable("rpsdtail_ham", 'F');
  dataloader->AddVariable("r3", 'F');
  dataloader->AddSpectator("Eqe", "Eqe", "units", 'F');
  dataloader->AddSpectator("id_tag", "id_tag", "units", 'F');
  dataloader->AddSpectator("isoz", "isoz", "units", 'I');
  dataloader->AddSpectator("ison", "ison", "units", 'I');
  
  TCut mycuts = "";
  TCut mycutb = "";
  int ntrain = 7e5;
  int ntest = 5e4;
  dataloader->PrepareTrainingAndTestTree("", ntrain, ntrain, ntest, ntest, "SplitMode=Random:NormMode=NumEvents:!V");
  factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDTG_FV1FV2", "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2");
  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();
  Output1->Close();
  Output2->Close();
  outputFile->Close();
  std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
  std::cout << "==> TMVAClassification is done!" << std::endl;
  delete factory;
  delete dataloader;
  if (!gROOT->IsBatch()) TMVA::TMVAGui(outfileName);
  return 0;
}
