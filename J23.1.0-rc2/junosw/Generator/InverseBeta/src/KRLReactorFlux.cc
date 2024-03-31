#include "KRLReactorFlux.hh"

KRLReactorFlux* gReactorFlux;

KRLReactorFlux::KRLReactorFlux() {
  fMethod = gkVogelFit;
  fMethod99Constant = 3;

  fVogelCoeff235U[0] = 0.870;
  fVogelCoeff235U[1] = -0.160;
  fVogelCoeff235U[2] = -0.0910;

  fVogelCoeff239Pu[0] = 0.896;
  fVogelCoeff239Pu[1] = -0.239;
  fVogelCoeff239Pu[2] = -0.0981;

  fVogelCoeff238U[0] = 0.976;
  fVogelCoeff238U[1] = -0.162;
  fVogelCoeff238U[2] = -0.079;

  fVogelCoeff241Pu[0] = 0.793;
  fVogelCoeff241Pu[1] = -0.080;
  fVogelCoeff241Pu[2] = -0.1085;
  fFraction235U = 0.6;
  fFraction239Pu = 0.3;
  fFraction238U = fFraction241Pu = 0.05;


}



Double_t KRLReactorFlux::Flux(Double_t aEnu) {
  Double_t answer;
  switch (fMethod) {
  case -99:
    answer = fluxMethod99(aEnu);
    break;
  case (gkVogelFit):
    answer = fluxVogelFit(aEnu);

    break;
  default:
    answer = 0;

  }
  return answer;
}

Double_t KRLReactorFlux::fluxMethod99(Double_t aEnuMeV) {
  Double_t answer = TMath::Exp(-aEnuMeV/fMethod99Constant) / fMethod99Constant;
  return answer;
}

// answer in units of 10^26/GWatt/year
Double_t KRLReactorFlux::fluxVogelFit(Double_t aEnu) {

  // Petr's flux's are normalized per fission. 
  Double_t u235 = fVogelCoeff235U[0] + fVogelCoeff235U[1]* aEnu + fVogelCoeff235U[2]*aEnu*aEnu;
  u235 = TMath::Exp(u235);
  Double_t pu239 = fVogelCoeff239Pu[0] + fVogelCoeff239Pu[1]* aEnu + fVogelCoeff239Pu[2]*aEnu*aEnu;
  pu239 = TMath::Exp(pu239);

  Double_t u238 = fVogelCoeff238U[0] + fVogelCoeff238U[1]* aEnu + fVogelCoeff238U[2]*aEnu*aEnu;
  u238 = TMath::Exp(u238);
  Double_t pu241 = fVogelCoeff241Pu[0] + fVogelCoeff241Pu[1]* aEnu + fVogelCoeff241Pu[2]*aEnu*aEnu;
  pu241 = TMath::Exp(pu241);

  // add the weighted sum of the terms.
  Double_t answer;
  answer = u235*fFraction235U + pu239*fFraction239Pu + u238*fFraction238U + pu241*fFraction241Pu;


  // there are 3.125E19 fissions/second/GWatt
  // see, e.g., http://www.nuc.berkeley.edu/dept/Courses/NE-150/Criticality.pdf
  answer *= 3.125;
  // there are about 3.156E7 seconds in a year
  answer *= 3.156;
  
  return answer;
}
