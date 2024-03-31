// Author: Zhao Xin 03/07/2022

/********************************************************************
 * Class to calucltae the anti-nu flux from a nuclear reactor
 * The model flux is provided as follows
 ** Huber-Mueller model: Phys. Rev. C83, 054615 (2011) & P. Huber, Phys. Rev. C84, 024617 (2011)
 ** Kurchatov Institute model: arXiv:2103.01684
 ** HKSS model: Phys.Rev. C100, 054323 (2019)
 ** Estienne-Fallot model: Phys. Rev. Lett. 123, 022502 (2019)
 ** The unit of flux is [/fission/MeV]
 ********************************************************************/

#ifndef __REACTORFLUX_HH__
#define __REACTORFLUX_HH__

#include "TROOT.h"
#include "TMath.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <TGraph.h>
#include <TH1D.h>
#include <TFile.h>
#include <stdio.h>
#include <stdlib.h>
# include <TSystem.h>

// using namespace std;


class ReactorFlux : public TObject {
protected:
  Int_t fMethod;  //method for calculating spectrum
  Double_t fFraction235U;
  Double_t fFraction239Pu;
  Double_t fFraction238U;
  Double_t fFraction241Pu;
  Double_t fFraction235U_DYB = 0.564;
  Double_t fFraction238U_DYB = 0.076;
  Double_t fFraction239Pu_DYB = 0.304;
  Double_t fFraction241Pu_DYB = 0.056;

  Double_t spec235; 
  Double_t spec238; 
  Double_t spec239; 
  Double_t spec241;
  Double_t specPu;
  Double_t spectot;

public:
  ReactorFlux();
  ~ReactorFlux() {;}
  void Initialize();
  void SetFissionFraction(double f235, double f238, double f239, double f241);
  Int_t GetMethod() ;
  // void SetMethod(Int_t aMethod) { fMethod = aMethod; }
  Double_t Flux( Double_t aEnu); // returns flux in units ne_bar/GWatt/year
  void SetMethod(Int_t aMethod);

  const int mHMbump = 0;
  const int mHM = 1;
  const int mKI = 2;
  const int mHKSS = 3;
  const int mHKSS_KI = 4;
  const int mEF = 5;
  const int mDYB = 6;
  const int NumFiss = 4;

    std::string get_absolute_path(const std::string& relative_path);
  
private:
  // HM and HM + bump
  TGraph * fspec235_HM ;
  TGraph * fspec238_HM ;
  TGraph * fspec239_HM ;
  TGraph * fspec241_HM ;
  TGraph * fcr;
  std:: string s_path_logHM_235;
  std:: string s_path_logHM_238;
  std:: string s_path_logHM_239;
  std:: string s_path_logHM_241;
  std:: string s_path_fcr;
  // KI
  TGraph * fspec235_KI ;
  TGraph * fspec238_KI ;
  TGraph * fspec239_KI ;
  TGraph * fspec241_KI ;
  std:: string s_path_logKI_235;
  std:: string s_path_logKI_238;
  std:: string s_path_logKI_239;
  std:: string s_path_logKI_241;
  // HKSS
  TGraph * fspec235_HKSS ;
  TGraph * fspec238_HKSS ;
  TGraph * fspec239_HKSS ;
  TGraph * fspec241_HKSS ;
  std:: string s_path_logHKSS_235;
  std:: string s_path_logHKSS_238;
  std:: string s_path_logHKSS_239;
  std:: string s_path_logHKSS_241;
  // HKSS-KI
  TGraph * fspec235_HKSS_KI ;
  TGraph * fspec238_HKSS_KI ;
  TGraph * fspec239_HKSS_KI ;
  TGraph * fspec241_HKSS_KI ;
  std:: string s_path_logHKSSKI_235;
  std:: string s_path_logHKSSKI_238;
  std:: string s_path_logHKSSKI_239;
  std:: string s_path_logHKSSKI_241;
  // EF
  TGraph * fspec235_EF ;
  TGraph * fspec238_EF ;
  TGraph * fspec239_EF ;
  TGraph * fspec241_EF ;
  std:: string s_path_logEF_235;
  std:: string s_path_logEF_238;
  std:: string s_path_logEF_239;
  std:: string s_path_logEF_241;
  // DYB unfolding 
  TGraph * fspecPu_DYB ;
  TGraph * fspectot_DYB ;
  TGraph * fspec235_DYB ;
  std:: string s_path_logDYB_Pu;
  std:: string s_path_logDYB_tot;
  std:: string s_path_logDYB_235;
};

R__EXTERN ReactorFlux* gReactorFlux;

#endif
