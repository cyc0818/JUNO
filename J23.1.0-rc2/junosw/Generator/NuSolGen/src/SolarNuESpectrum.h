/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: energy spectrum of solar neutrinos
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#ifndef SOLARNUESPECTRUM_H
#define SOLARNUESPECTRUM_H
#include "SolarNu.h"
#include <vector>
#include <string>
class SolarNuESpectrum {
  public:
    void load(const std::string &prefix);
    double dNdq(enum SolarNu::SolarNuType type,double q) const; // in MeV^{-1}
    double getDq(enum SolarNu::SolarNuType type) const { return qMaxs[static_cast<int>(type)]/data[static_cast<int>(type)].size(); }
    double getQMax(enum SolarNu::SolarNuType type) const { return qMaxs[static_cast<int>(type)]; }
  private:
    double rawData(int nuIndex,int q) const; // pdf at q MeV
    std::vector<double> data[SolarNu::NSolarNuType]; // sum normalized to 1
    double qMaxs[SolarNu::NSolarNuType] = {};
};
#endif
