#include "ReactorFlux.hh"
#include "TMath.h"
#include "TROOT.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <TGraph.h>
#include <TFile.h>
#include <TH1D.h>
#include <stdio.h>
#include <stdlib.h>
# include <TSystem.h>



using namespace std;


ReactorFlux* gReactorFlux;


ReactorFlux::ReactorFlux() {
  Initialize();
}

void ReactorFlux::Initialize(){
  fMethod = mHMbump;
  fFraction235U = 0.577;
  fFraction238U = 0.076;
  fFraction239Pu = 0.295;
  fFraction241Pu = 0.052;
  
  s_path_fcr = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_CommonInput/bumpcorrection.dat");

  s_path_logHM_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_235U.dat");
  s_path_logHM_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_238U.dat");
  s_path_logHM_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_239Pu.dat");
  s_path_logHM_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_241Pu.dat");

  s_path_logKI_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_235U.dat");
  s_path_logKI_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_238U.dat");
  s_path_logKI_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_239Pu.dat");
  s_path_logKI_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_241Pu.dat");


  s_path_logHKSS_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_235U.dat");
  s_path_logHKSS_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_238U.dat");
  s_path_logHKSS_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_239Pu.dat");
  s_path_logHKSS_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_241Pu.dat");

  s_path_logHKSSKI_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_235U.dat");
  s_path_logHKSSKI_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_238U.dat");
  s_path_logHKSSKI_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_239Pu.dat");
  s_path_logHKSSKI_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_241Pu.dat");

  s_path_logEF_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_235U.dat");
  s_path_logEF_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_238U.dat");
  s_path_logEF_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_239Pu.dat");
  s_path_logEF_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_241Pu.dat");

  s_path_logDYB_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_DYBunfolding/log_flux_235U.dat");
  s_path_logDYB_Pu = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_DYBunfolding/log_flux_Pu.dat");
  s_path_logDYB_tot = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_DYBunfolding/log_flux_total.dat");

  s_path_fcr = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_CommonInput/bumpcorrection.dat");

  s_path_logHM_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_235U.dat");
  s_path_logHM_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_238U.dat");
  s_path_logHM_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_239Pu.dat");
  s_path_logHM_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HM/log_flux_241Pu.dat");


  s_path_logKI_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_235U.dat");
  s_path_logKI_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_238U.dat");
  s_path_logKI_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_239Pu.dat");
  s_path_logKI_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_KI/log_flux_241Pu.dat");
  
  s_path_logHKSS_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_235U.dat");
  s_path_logHKSS_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_238U.dat");
  s_path_logHKSS_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_239Pu.dat");
  s_path_logHKSS_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS/log_flux_241Pu.dat");

  s_path_logHKSSKI_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_235U.dat");
  s_path_logHKSSKI_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_238U.dat");
  s_path_logHKSSKI_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_239Pu.dat");
  s_path_logHKSSKI_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_HKSS_KI/log_flux_241Pu.dat");

  s_path_logEF_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_235U.dat");
  s_path_logEF_238 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_238U.dat");
  s_path_logEF_239 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_239Pu.dat");
  s_path_logEF_241 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_EF/log_flux_241Pu.dat");


  s_path_logDYB_235 = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_DYBunfolding/log_flux_235U.dat");
  s_path_logDYB_Pu = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_DYBunfolding/log_flux_Pu.dat");
  s_path_logDYB_tot = get_absolute_path("/data/Generator/UnifiedGen/UnifiedInverseBeta/data/flux_DYBunfolding/log_flux_total.dat");

  // HM and HM + bump
  fspec235_HM =new TGraph(s_path_logHM_235.c_str());
  fspec238_HM =new TGraph(s_path_logHM_238.c_str());
  fspec239_HM =new TGraph(s_path_logHM_239.c_str());
  fspec241_HM =new TGraph(s_path_logHM_241.c_str());
  fcr =new TGraph(s_path_fcr.c_str());
  // KI
  fspec235_KI =new TGraph(s_path_logKI_235.c_str());
  fspec238_KI =new TGraph(s_path_logKI_238.c_str());
  fspec239_KI =new TGraph(s_path_logKI_239.c_str());
  fspec241_KI =new TGraph(s_path_logKI_241.c_str());
  // HKSS
  fspec235_HKSS =new TGraph(s_path_logHKSS_235.c_str());
  fspec238_HKSS =new TGraph(s_path_logHKSS_238.c_str());
  fspec239_HKSS =new TGraph(s_path_logHKSS_239.c_str());
  fspec241_HKSS =new TGraph(s_path_logHKSS_241.c_str());
  // HKSS-KI
  fspec235_HKSS_KI =new TGraph(s_path_logHKSSKI_235.c_str());
  fspec238_HKSS_KI =new TGraph(s_path_logHKSSKI_238.c_str());
  fspec239_HKSS_KI =new TGraph(s_path_logHKSSKI_239.c_str());
  fspec241_HKSS_KI =new TGraph(s_path_logHKSSKI_241.c_str());
  // EF
  fspec235_EF =new TGraph(s_path_logEF_235.c_str());
  fspec238_EF =new TGraph(s_path_logEF_238.c_str());
  fspec239_EF =new TGraph(s_path_logEF_239.c_str());
  fspec241_EF =new TGraph(s_path_logEF_241.c_str());
  // DYB unfolding 
  fspecPu_DYB = new TGraph(s_path_logDYB_Pu.c_str());
  fspectot_DYB = new TGraph(s_path_logDYB_tot.c_str());
  fspec235_DYB =new TGraph(s_path_logDYB_235.c_str());
}

void ReactorFlux::SetFissionFraction(double f235, double f238, double f239, double f241) {
  fFraction235U = f235;
  fFraction238U = f238;
  fFraction239Pu = f239;
  fFraction241Pu = f241;

}

Int_t ReactorFlux::GetMethod(){ 
  return fMethod; 
  }

void ReactorFlux::SetMethod(Int_t aMethod){
  fMethod = aMethod;
}



Double_t ReactorFlux::Flux(Double_t aEnu) {
  Double_t val;
  if (fMethod==mDYB){
    spec235 = TMath::Exp(fspec235_DYB->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_HM->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_HM->Eval(aEnu));
    specPu = TMath::Exp(fspecPu_DYB->Eval(aEnu));
    spectot = TMath::Exp(fspectot_DYB->Eval(aEnu));
    val = spectot + (fFraction235U-fFraction235U_DYB)*spec235 + (fFraction239Pu-fFraction239Pu_DYB)*specPu+(fFraction238U-fFraction238U_DYB)*spec238+(fFraction241Pu-fFraction241Pu_DYB-0.183*(fFraction239Pu-fFraction239Pu_DYB))*spec241;
  }
  else if (fMethod==mHMbump){
    spec235 = TMath::Exp(fspec235_HM->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_HM->Eval(aEnu));
    spec239 = TMath::Exp(fspec239_HM->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_HM->Eval(aEnu));
    val = (fFraction235U*spec235+fFraction238U*spec238+fFraction239Pu*spec239+fFraction241Pu*spec241)*fcr->Eval(aEnu);
  }
  else if (fMethod == mHM) {
    spec235 = TMath::Exp(fspec235_HM->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_HM->Eval(aEnu));
    spec239 = TMath::Exp(fspec239_HM->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_HM->Eval(aEnu));
    val = fFraction235U*spec235+fFraction238U*spec238+fFraction239Pu*spec239+fFraction241Pu*spec241;
  }
  else if (fMethod == mKI) {
    spec235 = TMath::Exp(fspec235_KI->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_KI->Eval(aEnu));
    spec239 = TMath::Exp(fspec239_KI->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_KI->Eval(aEnu));
    val = fFraction235U*spec235+fFraction238U*spec238+fFraction239Pu*spec239+fFraction241Pu*spec241;
  }
  else if (fMethod == mHKSS) {
    spec235 = TMath::Exp(fspec235_HKSS->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_HKSS->Eval(aEnu));
    spec239 = TMath::Exp(fspec239_HKSS->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_HKSS->Eval(aEnu));
    val = fFraction235U*spec235+fFraction238U*spec238+fFraction239Pu*spec239+fFraction241Pu*spec241;
  }
  else if (fMethod == mHKSS_KI) {
    spec235 = TMath::Exp(fspec235_HKSS_KI->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_HKSS_KI->Eval(aEnu));
    spec239 = TMath::Exp(fspec239_HKSS_KI->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_HKSS_KI->Eval(aEnu));
    val = fFraction235U*spec235+fFraction238U*spec238+fFraction239Pu*spec239+fFraction241Pu*spec241;
  }
  else if (fMethod == mEF) {
    spec235 = TMath::Exp(fspec235_EF->Eval(aEnu));
    spec238 = TMath::Exp(fspec238_EF->Eval(aEnu));
    spec239 = TMath::Exp(fspec239_EF->Eval(aEnu));
    spec241 = TMath::Exp(fspec241_EF->Eval(aEnu));
    val = fFraction235U*spec235+fFraction238U*spec238+fFraction239Pu*spec239+fFraction241Pu*spec241;
  }
  
  return val;
}

std::string ReactorFlux::get_absolute_path(const std::string& relative_path) {
    std::string absolute_path;

    // first, find it in JUNOTOP
    if (getenv("JUNOTOP")) {
        std::string p = getenv("JUNOTOP");
        p += relative_path;
        if (!gSystem -> AccessPathName(p.c_str())) {
            absolute_path = p;
        }
    }

    // then, find it in WORKTOP
    if (getenv("WORKTOP")) {
        std::string p = getenv("WORKTOP");
        p += relative_path;
        if (!gSystem -> AccessPathName(p.c_str())) {
            absolute_path = p;
        }
    }

    return absolute_path;
}
