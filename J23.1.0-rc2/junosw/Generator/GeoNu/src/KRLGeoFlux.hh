// Author: Chris Jillings 10/18/2004

/********************************************************************
 * Class to calucltae the anti-nu flux from the Earth *
 ********************************************************************/

#ifndef __KRLGEOFLUX_HH__
#define __KRLGEOFLUX_HH__

#include "TObject.h"
#include "TMath.h"
#include <TApplication.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TF2.h>
#include <TH1F.h>
#include <TRandom.h>
#include <TFile.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


const int gkVogelFit = 1;

class KRLGeoFlux : public TObject {
protected:
  Int_t fMethod;  //method for calculating spectrum

  Double_t fEnergy;
  Double_t fNe;
  Double_t fphiU;
  Double_t fphiTh;
  Double_t fphiEU;
  Double_t fphiETh;
  TFile *ReactorFile;
  TH1F *hisTh;
  TH1F *hisU;
  TH1F *hisUTh;
  TH1F *hisReac;
  TAxis *xaxisTh;
  TAxis *xaxisU;
  TAxis *xaxisUTh;
  TAxis *xaxisWR; // world reactor
  Int_t Inline;
  Int_t binxTh;
  Int_t binxU;
  Int_t binxUTh;
  std::string line;

public:
  KRLGeoFlux(const std::string& fluxfile, const std::string& reactorfile);
  

  ~KRLGeoFlux() {;}
  
  Int_t GetMethod() { return fMethod; }
  void SetMethod(Int_t aMethod) { fMethod = aMethod; }
  
//  Double_t GetMethod99Constant() { return fMethod99Constant; }
//  void SetMethod99Constant(Double_t aX) { fMethod99Constant = aX; }
  void SetiGeocase(Int_t val);
//  void UpdateParameters();
 
  Double_t FluxGeo( Double_t aEnuMeVno); // returns flux in units ne_bar/GWatt/year

private:
  Int_t iGeo;
 
};

R__EXTERN KRLGeoFlux* gGeoFlux;

#endif
