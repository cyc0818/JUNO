/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: Pee for solar neutrinos
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#ifndef PEESUN_H
#define PEESUN_H
#include "SolarNu.h"
class PeeSun {
  public:
    void setNuType(enum SolarNu::SolarNuType type) { nuIndex = static_cast<int>(type); }
    double Pee(double q) const;
  private:
    double Pee2_ad(double q) const;
    int nuIndex = -100; // remember to initialize it
    const double sin2_theta12 = 0.307; // PDG 2022
    const double sin2_theta13 = 0.0220; // PDG 2022
    const double delta_m212 = 7.53e-5; // PDG 2022, in eV^2
    // See table 1 of P.C. de Holanda, Wei Liao and A. Yu. Smirnov (hep-ph/0404042)
    // order: pp, pep, hep, be7, b8, n13, o15, f17
    const double Vavg[SolarNu::NSolarNuType] = {4.68,5.13,3.96,6.16,6.81,6.22,6.69,6.74}; // in 1e-12 eV
    const double Vcorr[SolarNu::NSolarNuType] = {0.109,0.076,0.165,0.029,0.010,0.054,0.013,0.012};  // unit-less
};
#endif
