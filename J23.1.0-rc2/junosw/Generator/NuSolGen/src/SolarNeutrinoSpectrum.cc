// --------------------------------------------------------------------------//
/** 
 * Solar neutrino generator imported from Borexino
 * AUTHOR: D. Franco
 * Revised by A. Caminata and S. Marcocci, Sept. 2014
 * Revised by X.F. Ding July 2016
 * X F Ding
 * 	18 Aug 2016 Modify the endpoint. use bahcall's b8 spectrum
 * 	1 Sep 2016 fix thetaw bug. it should be thetaw2. switch back to new sigma
 */
// --------------------------------------------------------------------------//
#include "TF1.h"
#include "SolarNeutrinoSpectrum.hh"
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
//#include "G4Electron.hh"
// in G4SystemOfUnits you can find using CLHEP::MeV
#include "G4SystemOfUnits.hh"
#include "TMath.h"
using std::cout;
using std::endl;
using std::cerr;
using std::ifstream;
//---------------------------------------------------------------------------//

SolarNeutrinoSpectrum::SolarNeutrinoSpectrum() {
  prefix = std::getenv("NUSOLGENROOT");
  if(prefix=="") {
    std::cerr<<"Please set $NUSOLGENROOT to the location of the NuSolarGen package root."<<std::endl;
    throw std::runtime_error("cannot find data folder");
  }
}

//---------------------------------------------------------------------------//

bool SolarNeutrinoSpectrum::SetNeutrinoType(const char *neutype) {
  std::string newValue(neutype);
  if(newValue == "pp") {
    m_type = SolarNu::SolarNuType::pp;
  } else if(newValue == "Be7") {
    m_type = SolarNu::SolarNuType::be7;
  } else if(newValue == "B8") {
    m_type = SolarNu::SolarNuType::b8;
  } else if(newValue == "N13") {
    m_type = SolarNu::SolarNuType::n13;
  } else if(newValue == "O15") {
    m_type = SolarNu::SolarNuType::o15;
  } else if(newValue == "F17") {
    m_type = SolarNu::SolarNuType::f17;
  } else if(newValue == "pep") {
    m_type = SolarNu::SolarNuType::pep;
  } else if(newValue == "hep") {
    m_type = SolarNu::SolarNuType::hep;
  } else {
    std::cerr<<"Un-supported neutrino type: ["<<neutype<<"]"<<std::endl;
    throw std::runtime_error("unknown solar neutrino type");
    return false;
  }
  m_dNdt.initialize(prefix,m_type);
  return true;
}

bool SolarNeutrinoSpectrum::GeneratePrimaries(double &electron_kinetic_energy) {
  electron_kinetic_energy = m_dNdt.getCdfInv(m_type,UniformRand());
  return true;
}
