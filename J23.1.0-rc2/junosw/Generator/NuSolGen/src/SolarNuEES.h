/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: recoil electron spectrum for solar nu on electron
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#ifndef SOLARNUEES_H
#define SOLARNUEES_H
#include "SolarNuESpectrum.h"
#include "NuEESXSec.h"
#include "PeeSun.h"
class SolarNuEES {
  public:
    void initialize(const std::string &prefix,enum SolarNu::SolarNuType type);
    double getTMax(enum SolarNu::SolarNuType type) const;
    double getXsecAvg(enum SolarNu::SolarNuType type,double t) const; // in sigma0 = 1.722e-44 cm^2/MeV
    double getCdfInv(enum SolarNu::SolarNuType type,double pcum) const;
  private:
    void calculate(enum SolarNu::SolarNuType type);
    void calculate(std::vector<double> &pdf,double q,double Tmax);
    SolarNuESpectrum m_Enu;
    PeeSun m_pee; /// survival probability of solar neutrinos
    NuEESXSec m_xsec; /// nu-e cross-section
    const int nBins = 2000;
    std::vector<double> m_xsecAvg[SolarNu::NSolarNuType]; // cumulative probability density
    std::vector<double> m_cdfInv[SolarNu::NSolarNuType]; // cumulative probability density
};
#endif
