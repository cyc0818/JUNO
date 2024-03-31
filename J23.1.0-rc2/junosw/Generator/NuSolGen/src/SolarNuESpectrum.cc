/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: solar neutrino energy spectrum
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#include "SolarNuESpectrum.h"
#include <cmath>
double SolarNuESpectrum::dNdq(enum SolarNu::SolarNuType type,double q) const {
  const int nuIndex = static_cast<int>(type);
  const double dq = getDq(type);
  const double bin_d = q/dq;
  const size_t bin = static_cast<size_t>(floor(bin_d));
  return (rawData(nuIndex,bin+1)-rawData(nuIndex,bin))*(bin_d-bin)+rawData(nuIndex,bin);
}

double SolarNuESpectrum::rawData(int nuIndex,int q) const {
  return (q<=0||q>=static_cast<int>(data[nuIndex].size()+1))?0:data[nuIndex].at(q-1);
}

#include <fstream>
#include <stdexcept>
#include <iostream>

void SolarNuESpectrum::load(const std::string &prefix) {
  // follow the order in SolarNu.h
  const std::string fname[] = {
    "ppspectrum.dat",
    "", // pep, skip
    "hepspectrum.dat",
    "", // be7, skip
    "b8spectrum_winter.dat",
    "n13spectrum.dat",
    "o15spectrum.dat",
    "f17spectrum.dat"
  };
  for(int nuIndex = 0; nuIndex < SolarNu::NSolarNuType; ++nuIndex) {
    if(nuIndex==1) { // pep
      qMaxs[1] = 1.44; // MeV
      data[1].push_back(1);
      continue;
    } else if(nuIndex==3) { // be7
      qMaxs[3] = 0.86184; // MeV
      data[3].push_back(1);
      continue;
    } else {
      const std::string fullname(prefix+"/data/"+fname[nuIndex]);
      std::ifstream f;
      f.open(fullname);
      if(!f.is_open()) { 
        std::cerr<<"file ["<<fullname<<"] cannot be openned."<<std::endl;
        throw std::runtime_error("file not open");
      }
      double psum(0),qMax,pLast;
      std::vector<double> qs,pdf;
      while(true) {
        f>>qMax>>pLast;
        if(f.fail()) break;
        qs.push_back(qMax);
        pdf.push_back(pLast);
        psum += pLast;
      }
      f.close();
      auto format_check_fail = [](int line) { 
        std::cerr<<"Break: line ["<<line<<"]"<<std::endl;
        std::cerr<<"Assume neutrino spectrum has energy mesh of dq*i, i=1,2,...N, and last point has 0 magnitude"<<std::endl;
        std::cerr<<"Assumption check failed"<<std::endl;
        throw std::runtime_error("format check fail");
      };
      if(fabs(pLast)>1e-13) format_check_fail(__LINE__);
      qMaxs[nuIndex] = qMax;
      const double dq = qMax / qs.size();
      for(size_t i = 0;i<pdf.size();++i) {
        //std::cout<<fname[nuIndex]<<" "<<i<<" "<<qMax<<" "<<qs.size()<<" "<<dq<<" "<<qs.at(i)<<" "<<qs.at(i)/dq-(i+1)<<std::endl;
        if((qs.at(i)/dq-(i+1))/(i+1)>1e-2) format_check_fail(__LINE__);
        pdf[i] /= psum*dq;
      }

      data[nuIndex] = pdf;
    }
  }
}
