
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4StateManager.hh"

#include "OpticalPhotonGun.hh"
#include "globals.hh"

#include <string>
#include <sstream>
#include <cassert>

namespace Generator {
  namespace Utils {
    OpticalPhotonGun::OpticalPhotonGun(std::string name,G4int number,G4double wave)
      :m_name(name),m_number(number),m_wave(wave)
    {

    }

    OpticalPhotonInfo 
      OpticalPhotonGun::GetOpticalInfo() 
      {
        OpticalPhotonInfo opt_info;

        G4double pwl = m_wave/nm;
        G4double pEng = 1240/pwl*eV;
        G4double cost = 1. - 2.*G4UniformRand();
        G4double sint = sqrt((1.-cost)*(1.+cost));

        G4double phi = twopi*G4UniformRand();
        G4double sinp = sin(phi);
        G4double cosp = cos(phi);

        G4double mpx = sint*cosp;
        G4double mpy = sint*sinp;
        G4double mpz = cost;

        G4ThreeVector photonMomentum(mpx, mpy, mpz);

        G4double m_px = pEng*mpx;
        G4double m_py = pEng*mpy;
        G4double m_pz = pEng*mpz;  

        G4double sx = cost*cosp;
        G4double sy = cost*sinp; 
        G4double sz = -sint;

        G4ThreeVector photonPolarization(sx, sy, sz);

        G4ThreeVector perp = photonMomentum.cross(photonPolarization);

        phi = twopi*G4UniformRand();
        sinp = sin(phi);
        cosp = cos(phi);

        photonPolarization = cosp * photonPolarization + sinp * perp;

        photonPolarization = photonPolarization.unit();
        
        opt_info.name = m_name;
        opt_info.photonPol = photonPolarization;
        opt_info.eng = pEng * eV;
        opt_info.px = m_px;
        opt_info.py = m_py;
        opt_info.pz = m_pz;
        opt_info.dt = 0.0;
        opt_info.photonPol = photonPolarization;

        return opt_info;

      }
    OpticalPhotonContainer
      OpticalPhotonGun::next() 
      {
        OpticalPhotonContainer new_opt;
        for (G4int i = 0; i <m_number; ++i ) {
          new_opt . push_back(GetOpticalInfo());
        }
        return new_opt;

      }

  }
}
