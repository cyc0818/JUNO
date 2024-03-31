// Author: Chris Jillings 10/18/2004

/********************************************************************
 * Class to calucltae the anti-nu flux from a nuclear reactor
 *
 ********************************************************************/

#ifndef __KRLREACTORFLUX_HH__
#define __KRLREACTORFLUX_HH__

#include "TObject.h"
#include "TMath.h"

const int gkVogelFit = 1;

class KRLReactorFlux : public TObject {
protected:
  Int_t fMethod;  //method for calculating spectrum

  Double_t fMethod99Constant;
  Double_t fVogelCoeff235U[3];
  Double_t fVogelCoeff239Pu[3];
  Double_t fVogelCoeff238U[3];
  Double_t fVogelCoeff241Pu[3];
  Double_t fFraction235U;
  Double_t fFraction239Pu;
  Double_t fFraction238U;
  Double_t fFraction241Pu;

public:
  KRLReactorFlux();
  ~KRLReactorFlux() {;}
  
  Int_t GetMethod() { return fMethod; }
  void SetMethod(Int_t aMethod) { fMethod = aMethod; }
  
  Double_t GetMethod99Constant() { return fMethod99Constant; }
  void SetMethod99Constant(Double_t aX) { fMethod99Constant = aX; }

  Double_t Flux( Double_t aEnuMeVno); // returns flux in units ne_bar/GWatt/year
  
private:
  Double_t fluxMethod99(Double_t aEnuMeV);  // silly method: uses one exponential
  Double_t fluxVogelFit(Double_t aEnuMeV); // from Petr's fits
  
};

R__EXTERN KRLReactorFlux* gReactorFlux;

#endif
