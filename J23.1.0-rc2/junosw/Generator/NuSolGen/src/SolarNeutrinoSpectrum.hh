// --------------------------------------------------------------------------//
/** 
 * Solar neutrino generator imported from Borexino
 * AUTHOR: D. Franco
 * Revised by A. Caminata and S. Marcocci, Sept. 2014
 * Revised by X.F. Ding July 2016
*/
// --------------------------------------------------------------------------//

#ifndef _GENERATORSolarNeutrino2_HH
#define _GENERATORSolarNeutrino2_HH
#include "CLHEP/Random/RandFlat.h"
#include "SolarNuEES.h"

//---------------------------------------------------------------------------//
/**Solar Neutrino generator: generates solar neutrino spectra, including distortions due to oscillations; stores information only about recoiled electron and interacting neutrino (slower than GeneratorSolarNeutrino)
*/
class SolarNeutrinoSpectrum {
public:

  ///default constructor
  SolarNeutrinoSpectrum();

  ///destructor

  ///public interface
  bool GeneratePrimaries(double &electron_kinetic_energy);
  bool SetNeutrinoType(const char *neutype);
private:
  double UniformRand() { return CLHEP::RandFlat::shoot(0.0, 1.0); };
  SolarNuEES m_dNdt;
  SolarNu::SolarNuType m_type;
  std::string prefix;
};
#endif
