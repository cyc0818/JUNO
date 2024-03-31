/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: nu-e cross-section
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#ifndef NUEESXEC_H
#define NUEESXEC_H
/// class for neutrino-electron elastic scattering cross section
class NuEESXSec {
  public:
    double NuE_E_ES(double e_nu,double T) const;
    double NuX_E_ES(double e_nu,double T) const;
    double Tmax(double qMax) const { return qMax/(1+emass/2/qMax); };
  private:
    double fMinus(double z,double q) const;
    double fPlus(double z,double q) const;
    double fPlusMinus(double z,double q) const;
    double I_f(double E) const;
    double k_e(double E) const;
    double k_mu(double E) const;
    const double emass = 0.51099895000; // PDG 2022
    const double sigma0 = 1;
    const double rhoNC = 1.0127; // From A. Re internal note
    const double sintw2 = 0.23121; // PDG 2022
    const double alpha = 7.2973525693e-3; // PDG 2022

};
#endif
