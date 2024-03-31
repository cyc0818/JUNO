/*****************************************************************************/
// Author: Xuefeng Ding <dingxf@ihep.ac.cn> @ IHEP-CAS
//
// Date: 2023 Febrary 10
// Version: v1.0
// Description: recoil electron spectrum for solar nu on electron
//
// All rights reserved. 2023 copyrighted.
/*****************************************************************************/
#include "SolarNuEES.h"
void SolarNuEES::initialize(const std::string &prefix,enum SolarNu::SolarNuType type) {
  m_Enu.load(prefix);
  calculate(type);
}
double SolarNuEES::getTMax(enum SolarNu::SolarNuType type) const {
  return m_xsec.Tmax(m_Enu.getQMax(type));
}
#include <cmath>
double SolarNuEES::getXsecAvg(enum SolarNu::SolarNuType type,double t) const {
  const int nuIndex = static_cast<int>(type);
  const auto &pdf (m_xsecAvg[nuIndex]);
  const int bin_d = t/getTMax(type)*nBins;
  const int bin = static_cast<int>(floor(bin_d));
  auto eval = [&pdf](int i) { return (i>=0&&i<static_cast<int>(pdf.size()))?pdf.at(i):0; };
  return (eval(bin+1)-eval(bin))*(bin_d-bin)+eval(bin);
}
double SolarNuEES::getCdfInv(enum SolarNu::SolarNuType type,double pcum) const {
  const int nuIndex = static_cast<int>(type);
  const auto &cdfInv(m_cdfInv[nuIndex]);
  const double bin_d = pcum*nBins;
  const int bin = static_cast<int>(floor(bin_d));
  auto eval = [&cdfInv,this,type](int i) { 
    if(i<=0) 
      return 0.;
    else if(i<static_cast<int>(cdfInv.size()))
      return cdfInv.at(i);
    else
      return this->getTMax(type);
  };
  return (eval(bin+1)-eval(bin))*(bin_d-bin)+eval(bin);
}

void SolarNuEES::calculate(enum SolarNu::SolarNuType type) {
  const int nuIndex = static_cast<int>(type);
  m_pee.setNuType(type);
  std::vector<double> xsecAvg;
  if(type == SolarNu::SolarNuType::be7) {
    std::vector<double> xsecAvg_862,xsecAvg_384;
    const double qMax_862 = 0.86184;
    const double qMax_384 = 0.38428;
    calculate(xsecAvg_862,qMax_862,m_xsec.Tmax(qMax_862));
    calculate(xsecAvg_384,qMax_384,m_xsec.Tmax(qMax_862));
    //from http://www.nucleide.org/DDEP_WG/Nuclides/Be-7_tables.xsecAvg
    const double br_862 = 0.8956;
    const double br_384 = 1-br_862;
    for(int i = 0;i<nBins;++i) {
      xsecAvg.push_back(xsecAvg_862[i]*br_862+xsecAvg_384[i]*br_384);
    }
  } else if(type == SolarNu::SolarNuType::pep) {
    const double qMax = 1.44; // in MeV
    calculate(xsecAvg,qMax,m_xsec.Tmax(qMax));
  } else {
    std::vector<double> xsec;
    xsecAvg.resize(nBins,0);
    const double tMax = getTMax(type); // in MeV
    const double dq = m_Enu.getQMax(type)/nBins; // in MeV
    for(int i = 0;i<nBins;++i) {
      const double q = dq*(i+0.5);
      const double p = m_Enu.dNdq(type,q)*dq;
      calculate(xsec,q,tMax);
      for(int j = 0;j<nBins;++j) {
        xsecAvg[j] += xsec[j]*p;
      }
    }
  }
  m_xsecAvg[nuIndex] = xsecAvg;
  double sum = 0;
  for(int i = 0;i<nBins;++i) {
    sum += xsecAvg[i];
  }
  auto prob = [&xsecAvg,sum](int i) { return xsecAvg.at(i)/sum; };

  double pcum(0);
  int i_t(0);
  const double dt = getTMax(type)/nBins; // in MeV
  const double dpcum = 1./nBins; // in MeV
  for(int i = 0;i<nBins;++i) {
    const double target_pcum = 1./nBins*i;
    while(pcum+prob(i_t)/sum<target_pcum) {
      pcum += prob(i_t);
      ++i_t;
    }
    m_cdfInv[nuIndex].push_back(((target_pcum-pcum)/prob(i_t)+i_t)*dt);
  }
}
void SolarNuEES::calculate(std::vector<double> &xsecAvg,double q,double tMax) {
  xsecAvg.clear();
  const double pee = m_pee.Pee(q);
  const double tMax_fromQ = m_xsec.Tmax(q);
  for(int i = 0;i<nBins;++i) {
    const double t = tMax/nBins*i;
    const double p = t<tMax_fromQ?pee*m_xsec.NuE_E_ES(q,t)+(1-pee)*m_xsec.NuX_E_ES(q,t):0;
    xsecAvg.push_back(p);
  }
}
