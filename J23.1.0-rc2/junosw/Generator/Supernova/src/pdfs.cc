#include "pdfs.h"
#include "dataflux.h"
#include "detector.h"
#include <iostream>

static int PDGID [] = {12, -12, 14, -14};

extern TDetector *detector;
extern TDataFlux *flux;

static int NuID(double pe, double peBar, double px, double pxBar, TRandom *const rand){
  double sum = pe + peBar + px + pxBar;
  if(sum != 1.0){
    pe    /= sum;
    peBar /= sum;
    px    /= sum;
    pxBar /= sum;
  }
  double cul[4] = {pe, pe+peBar, pe+peBar+px, pe+peBar+px+pxBar};
  double r      = rand->Uniform();
  
  int c = 0;
  for(;r>cul[c];++c);

  //std::cout << "Thilo: " << c << std::endl;
  //std::cout << pe << "\t" << peBar << "\t" << px << "\t" << pxBar << std::endl;

  return PDGID[c];
}

namespace PDFs{

  //--IBD------------------------------------------
  double IBDRate(double *x, double *par __attribute__((unused))){
    return flux->GetLocalFlux(x[0], x[1], nuAE) * detector->GetIBD(x[1], x[2]);
  }

  double IBDEe(double eNu, double costheta){
    double mP = 938.272046;
    double mN = 939.565379;
    double mE = 0.510998928;

    double eps    = eNu/mP;
    double delta  = (pow(mN, 2) - pow(mP, 2) - pow(mE, 2)) / (2*mP);
    double k      = pow(1+eps, 2) - pow(eps*costheta, 2);

    double Ee = ( (eNu - delta) * (1 + eps) + eps * costheta * sqrt(pow(eNu-delta, 2) - pow(mE, 2)*k ))/k;
  
    return Ee;
  }

  double IBDRateFlavor(double *x, double *par)
  {
      int iflavor = par[0];
      if(iflavor == 1){ 
          return flux->GetLocalFlux(x[0], x[1], nuAE) * detector->GetIBD(x[1], x[2]);
      }
      return 0;
  }



  //--PES------------------------------------------
  double PESTotalRate(double *x, double *par __attribute__((unused))){
    return (flux->GetLocalFlux(x[0], x[1], nuAE) + flux->GetLocalFlux(x[0], x[1], nuAX))
            * detector->GetTotalProtonES(x[1], true)
            + (flux->GetLocalFlux(x[0], x[1], nuE) + flux->GetLocalFlux(x[0], x[1], nuX))
            * detector->GetTotalProtonES(x[1], false);
  }

  double PESRate(double *x, double *par __attribute__((unused))){
    double eP = PESEp(x[1], x[2]);
    return ((flux->GetLocalFlux(x[0], x[1], nuAE) + flux->GetLocalFlux(x[0], x[1], nuAX))
            * detector->GetProtonES(x[1], eP, true)
            + (flux->GetLocalFlux(x[0], x[1], nuE) + flux->GetLocalFlux(x[0], x[1], nuX))
            * detector->GetProtonES(x[1], eP, false))
            * dEpdCos(x[1], x[2]);
  }

  double PESEp(double eNu, double costheta){
    double mP   = 938.272046;
    double tmp1 = pow(eNu + mP, 2);
    double tmp2 = pow(eNu * costheta, 2);
    return mP * (tmp1 + tmp2) / (tmp1 - tmp2) - mP;
  }

  double dEpdCos(double eNu, double costheta){
    double mP = 938.272046;
    return 4 * costheta * pow(eNu * (eNu + mP), 2) * mP
            / pow(pow(eNu + mP, 2) - pow(eNu * costheta, 2), 2);
  }


  int PESNuID(const double t, const double eNu, const double costheta, TRandom *const rand){
    double pe     = PESNue(t, eNu, costheta);
    double peBar  = PESNueBar(t, eNu, costheta);
    double px     = PESNux(t, eNu, costheta);
    double pxBar  = PESNuxBar(t, eNu, costheta);

    return NuID(pe, peBar, px, pxBar, rand);
  }

  double PESNue(const double &t, const double &e, const double &costheta){
    double eP = PESEp(e, costheta);
    return flux->GetLocalFlux(t, e, nuE) * detector->GetProtonES(e, eP, false) * dEpdCos(e, costheta);
  }

  double PESNueBar(const double &t, const double &e, const double &costheta){
    double eP = PESEp(e, costheta);
    return flux->GetLocalFlux(t, e, nuAE) * detector->GetProtonES(e, eP, true) * dEpdCos(e, costheta);
  }

  double PESNux(const double &t, const double &e, const double &costheta){
    double eP = PESEp(e, costheta);
    return flux->GetLocalFlux(t, e, nuX) * detector->GetProtonES(e, eP, false) * dEpdCos(e, costheta);
  }

  double PESNuxBar(const double &t, const double &e, const double &costheta){
    double eP = PESEp(e, costheta);
    return flux->GetLocalFlux(t, e, nuAX) * detector->GetProtonES(e, eP, true) * dEpdCos(e, costheta);
  }

  double PESRateFlavor(double *x, double *par)
  {
      double eP = PDFs::PESEp(x[1], x[2]);
      int iflavor = par[0];
      if(iflavor == 1 || iflavor == 3){ 
          return flux->GetLocalFlux(x[0], x[1], NuType(iflavor))*detector->GetProtonES(x[1], eP, true)             *PDFs::dEpdCos(x[1], x[2]);
      }
      if(iflavor ==0 || iflavor == 2){
          return flux->GetLocalFlux(x[0], x[1], NuType(iflavor))*detector->GetProtonES(x[1], eP, false) 
              *PDFs::dEpdCos(x[1], x[2]);
      }
      return 0;
  }

  double PESTotalRateFlavor(double *x, double *par)
  {
      int iflavor = par[0];
      if(iflavor == 1 || iflavor == 3){
          return flux->GetLocalFlux(x[0], x[1], (NuType)iflavor)*detector->GetTotalProtonES(x[1], true); 
      }
      if(iflavor == 0 || iflavor == 2){
          return flux->GetLocalFlux(x[0], x[1], (NuType)iflavor)*detector->GetTotalProtonES(x[1], false); 
      }
      return 0;
  }

  
  //--EES------------------------------------------
  double EESTotalRate(double *x, double *par __attribute__((unused))){
    return flux->GetLocalFlux(x[0], x[1], nuAE) * detector->GetTotalElectronES(x[1], nuAE)
            + flux->GetLocalFlux(x[0], x[1], nuAX) * detector->GetTotalElectronES(x[1], nuAX)
            + flux->GetLocalFlux(x[0], x[1], nuE) * detector->GetTotalElectronES(x[1], nuE)
            + flux->GetLocalFlux(x[0], x[1], nuX) * detector->GetTotalElectronES(x[1], nuX);
  }

  double EESRate(double *x, double *par __attribute__((unused))){
    double eE = EESEe(x[1], x[2]);
    return (flux->GetLocalFlux(x[0], x[1], nuAE) * detector->GetElectronES(x[1], eE, nuAE)
            + flux->GetLocalFlux(x[0], x[1], nuAX) * detector->GetElectronES(x[1], eE, nuAX)
            + flux->GetLocalFlux(x[0], x[1], nuE) * detector->GetElectronES(x[1], eE, nuE)
            + flux->GetLocalFlux(x[0], x[1], nuX) * detector->GetElectronES(x[1], eE, nuX))
              * dEedCos(x[1], x[2]);
  }

  double EESEe(double eNu, double costheta){
    double mE   = 0.510998928;
    double tmp1 = pow(eNu + mE, 2);
    double tmp2 = pow(eNu * costheta, 2);
    return mE * (tmp1 + tmp2) / (tmp1 - tmp2);
  
  }

  double dEedCos(double eNu, double costheta){
    double mE   = 0.510998928;
    return 4 * costheta * pow(eNu * (eNu + mE), 2) * mE
            / pow(pow(eNu + mE, 2) - pow(eNu * costheta, 2), 2);
  }


  int EESNuID(const double t, const double eNu, const double costheta, TRandom *const rand){
    double pe     = EESNue(t, eNu, costheta);
    double peBar  = EESNueBar(t, eNu, costheta);
    double px     = EESNux(t, eNu, costheta);
    double pxBar  = EESNuxBar(t, eNu, costheta);

    return NuID(pe, peBar, px, pxBar, rand);
  }

  double EESNue(const double &t, const double &e, const double &costheta){
    double eE = EESEe(e, costheta);
    return flux->GetLocalFlux(t, e, nuE) * detector->GetElectronES(e, eE, nuE) * dEedCos(e, costheta);
  }

  double EESNueBar(const double &t, const double &e, const double &costheta){
    double eE = EESEe(e, costheta);
    return flux->GetLocalFlux(t, e, nuAE) * detector->GetElectronES(e, eE, nuAE) * dEedCos(e, costheta);
  }

  double EESNux(const double &t, const double &e, const double &costheta){
    double eE = EESEe(e, costheta);
    return flux->GetLocalFlux(t, e, nuX) * detector->GetElectronES(e, eE, nuX) * dEedCos(e, costheta);
  }

  double EESNuxBar(const double &t, const double &e, const double &costheta){
    double eE = EESEe(e, costheta);
    return flux->GetLocalFlux(t, e, nuAX) * detector->GetElectronES(e, eE, nuAX) * dEedCos(e, costheta);
  }

  double EESRateFlavor(double *x, double *par)
  {
      double eE = PDFs::EESEe(x[1], x[2]);
      int iflavor = par[0];
      return flux->GetLocalFlux(x[0], x[1], (NuType)iflavor) * detector->GetElectronES(x[1], eE, (NuType)iflavor)  
          * PDFs::dEedCos(x[1], x[2]);
  }

  double EESTotalRateFlavor(double *x, double *par)
  {
      int iflavor = par[0];
      return flux->GetLocalFlux(x[0], x[1], (NuType)iflavor) * detector->GetTotalElectronES(x[1], (NuType)iflavor);
  }


  //--N12------------------------------------------
  double N12Rate(double *x, double *par __attribute__((unused))){
    return flux->GetLocalFlux(x[0], x[1], nuE) * detector->GetN12(x[1]);
  }

  double N12DecayRate(double *x, double *par __attribute__((unused))){
    double e = sqrt(pow(x[0], 2) + pow(0.511, 2));
    return x[0] * e * pow(16.827 - e, 2) 
            * sqrt(1.451 + 0.066525 / (sqrt(pow(x[0], 2) / pow(0.511, 2) + 1) - 1));
  }

  double N12RateFlavor(double *x, double *par)
  {
      int iflavor = par[0];
      if(iflavor == 0){ 
          return flux->GetLocalFlux(x[0], x[1], nuE) * detector->GetN12(x[1]);
      }   
      return 0;
  }


  //--B12------------------------------------------
  double B12Rate(double *x, double *par __attribute__((unused))){
    return flux->GetLocalFlux(x[0], x[1], nuAE) * detector->GetB12(x[1]);
  }
  
  double B12DecayRate(double *x, double *par __attribute__((unused))){
    double e = sqrt(pow(x[0], 2) + pow(0.511, 2));
    return x[0] * e * pow(13.88 - e, 2) 
            * sqrt(1.305 + 0.0407416 / (sqrt(pow(x[0], 2) / pow(0.511, 2) + 1) - 1));
  }

  double B12RateFlavor(double *x, double *par)
  {
      int iflavor = par[0];
      if(iflavor == 1){ 
          return flux->GetLocalFlux(x[0], x[1], nuAE) * detector->GetB12(x[1]);
      }   

      return 0;
  }


  //--C12-------------------------------------------
  double C12Rate(double *x, double *par __attribute__((unused))){
    return (flux->GetLocalFlux(x[0], x[1], nuAE) + flux->GetLocalFlux(x[0], x[1], nuAX))
            * detector->GetC12(x[1], true)
            + (flux->GetLocalFlux(x[0], x[1], nuE) + flux->GetLocalFlux(x[0], x[1], nuX))
            * detector->GetC12(x[1], false);
  }


  int C12NuID(const double t, const double eNu, TRandom *const rand){
    double pe     = C12Nue(t, eNu);
    double peBar  = C12NueBar(t, eNu);
    double px     = C12Nux(t, eNu);
    double pxBar  = C12NuxBar(t, eNu);

    return NuID(pe, peBar, px, pxBar, rand);
  }

  double C12Nue(const double &t, const double &e){
    return flux->GetLocalFlux(t, e, nuE) * detector->GetC12(e, false);
  }

  double C12NueBar(const double &t, const double &e){
    return flux->GetLocalFlux(t, e, nuAE) * detector->GetC12(e, true);
  }

  double C12Nux(const double &t, const double &e){
    return flux->GetLocalFlux(t, e, nuX) * detector->GetC12(e, false);
  }

  double C12NuxBar(const double &t, const double &e){
    return flux->GetLocalFlux(t, e, nuAX) * detector->GetC12(e, true);
  }

  double C12RateFlavor(double *x, double *par)
  {
      int iflavor = par[0];
      if(iflavor == 1 || iflavor == 3){ 
          return flux->GetLocalFlux(x[0], x[1], (NuType)iflavor)*detector->GetC12(x[1], true);
      }   
      if(iflavor == 0 || iflavor == 2){ 
          return flux->GetLocalFlux(x[0], x[1], (NuType)iflavor)*detector->GetC12(x[1], false);
      }   
      return 0;
  }

}
