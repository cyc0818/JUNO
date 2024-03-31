#ifndef DetectionContructionUtils_hh
#define DetectionContructionUtils_hh

#include "globals.hh"

namespace DYB2 {
    namespace Ball {
        G4int GetMaxiumNinCircle(G4double r_tube,
                                 G4double r_pmt,
                                 G4double gap);
        G4int GetMaxiumNinHalfHeight(G4double h_tube,
                                     G4double r_pmt,
                                     G4double gap);
    }
}

#endif
