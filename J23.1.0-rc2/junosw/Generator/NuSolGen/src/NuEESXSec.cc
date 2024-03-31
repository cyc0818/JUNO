/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: nu-e cross-section
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#include "NuEESXSec.h"
#include <math.h>
#include "TMath.h"

double NuEESXSec::NuE_E_ES(double q,double T) const {
  if(T>0 && T < q/(1+emass/2./q)) {
    //from Bahcall et al. PhysRevD. 51, 11 1995
    double gl=rhoNC*(1./2.-k_e(T)*sintw2)-1;
    double gr=-rhoNC*k_e(T)*sintw2;
    double z= T/q;
    double sigma;
    sigma=gl*gl*(1.+alpha/M_PI*fMinus(z,q))+gr*gr*(1.-z)*(1.-z)*(1.+alpha/M_PI*fPlus(z,q))
      -gr*gl*emass*z/q*(1+alpha/M_PI*fPlusMinus(z,q));
    return sigma0*sigma;
  } else 
    return 0;
}
double NuEESXSec::NuX_E_ES(double q,double T) const {
  if(T>0 && T < q/(1+emass/2./q)) {
    //from Bahcall et al. PhysRevD. 51, 11 1995
    double gl=rhoNC*(1./2.-k_mu(T)*sintw2);
    double gr=-rhoNC*k_mu(T)*sintw2;
    double z= T/q;
    double sigma;
    sigma=gl*gl*(1.+alpha/M_PI*fMinus(z,q))+gr*gr*(1.-z)*(1.-z)*(1.+alpha/M_PI*fPlus(z,q))
      -gr*gl*emass*z/q*(1+alpha/M_PI*fPlusMinus(z,q));
    return sigma0*sigma;
  } else 
    return 0;
}
double NuEESXSec::fMinus(double z, double q) const {
  double E=z*q+emass;
  double l=sqrt(E*E-emass*emass);
  double val= (E/l*log((E+l)/emass)-1.)*(2*log(1.-z-emass/(E+l))-log(1.-z)-1./2.*log(z)-5./12.)
    +1./2.*(-TMath::DiLog(z)+TMath::DiLog(l/E))
    -1./2.*log(1.-z)*log(1.-z)-(11./12.+z/2.)*log(1-z)+z*(log(z)+1/2.*log(2*q/emass))
    -(31./18.+1./12.*log(z))*l/E-11./12.*z+z*z/24.;
  if(std::isnan(val))
    return 0;

  if (val>0) val=0;
  return val;
}
double NuEESXSec::fPlus(double z, double q) const {
  double E=z*q+emass;
  double l=sqrt(E*E-emass*emass);
  double integral = -TMath::DiLog(1.-z);
  double val= 1./(1.-z)/(1.-z)*(
      (E/l*log((E+l)/emass)-1.) * ((1-z)*(1-z)*(2*log(1-z-emass/(E+l))-log(1-z)-log(z)/2.-2./3.)-(z*z*log(z)+1.-z)/2.)
      -(1-z)*(1-z)/2.*(log(1-z)*log(1-z)+l/E*(integral-log(z)*log(1-z)))
      +log(1-z)*(z*z/2.*log(z)+(1-z)/3.*(2*z-1./2.))-z*z/2.*integral-z*(1-2*z)/3.*log(z)-z*(1-z)/6.
      -l/E/12.*(log(z)+(1-z)*(115-109*z)/6.)
      );
  if(std::isnan(val))
    return 0;

  if (val>0) val=0;
  return val;
}
double NuEESXSec::fPlusMinus(double z, double q) const {
  double E=z*q+emass;
  double l=sqrt(E*E-emass*emass);
  double val= (E/l*log((E+l)/emass)-1.)*2.*log(1.-z-emass/(E+l));
  if(std::isnan(val))
    return 0;
  if (val>0) val=0;
  return val;
}
double NuEESXSec::I_f(double E) const {
  double x = sqrt(1. + 2.* emass/E);
  return 1./6. * ( 1./3. + (3.-x*x) * (1./2.*x*log((x+1.)/(x-1.)) - 1.) );
}
double NuEESXSec::k_e(double E) const {
  return 0.9786 + 0.0097 * I_f(E); // A. Wright internal memo 2012
  //return 0.9791 + 0.0097 * I_f(E); // Bahcall's (1995)
}
double NuEESXSec::k_mu(double E) const {
  return 0.9965 - 0.00037 * I_f(E); // A. Wright internal memo 2012
  //return 0.9970 - 0.00037 * I_f(E); // Bahcall's (1995)
}
