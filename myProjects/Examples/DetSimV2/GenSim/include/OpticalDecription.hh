
#ifndef OpticalDescription_hh
#define OpticalDescription_hh

#include <string>
#include <vector>

#include "globals.hh"

namespace Generator {
  namespace Utils {

    struct OpticalPhotonInfo {
      std::string name;
      G4double px;
      G4double py;
      G4double pz;

      G4ThreeVector photonPol;
      G4double eng;

      G4double dt;
    };
    typedef std::vector< OpticalPhotonInfo > OpticalPhotonContainer;
  }

}

#endif
