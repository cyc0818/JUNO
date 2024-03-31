/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: Pee for solar neutrinos
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#include "PeeSun.h"
#include <cmath>
double PeeSun::Pee(double q) const {
  const double cos2_theta13 = 1-sin2_theta13;
  return cos2_theta13*cos2_theta13*Pee2_ad(q)+sin2_theta13*sin2_theta13;
}

double PeeSun::Pee2_ad(double q) const {
  const double epsilon12X = 2*Vavg[nuIndex]*1e-12*q*1e6/delta_m212; // all in eV
  const double cos_2theta12 = 1-2*sin2_theta12;
  const double cos2_theta13 = 1-sin2_theta13;
  const double cos_2theta12mX = (cos_2theta12-cos2_theta13*epsilon12X)
    /sqrt(1-2*cos_2theta12*cos2_theta13*epsilon12X+pow(cos2_theta13*epsilon12X,2));
  const double sin2_2theta12 = 1-cos_2theta12*cos_2theta12;
  const double deltaX = 1.5*epsilon12X*epsilon12X*sin2_2theta12*Vcorr[nuIndex]
    /pow(1-2*cos_2theta12*epsilon12X+epsilon12X*epsilon12X,2);
  return 0.5+0.5*(1-deltaX)*cos_2theta12mX*cos_2theta12;
}
