#ifndef R12199_PMTSolid_hh
#define R12199_PMTSolid_hh

#include "globals.hh"

class G4VSolid;

class R12199_PMTSolid {
public:
    R12199_PMTSolid();

    G4VSolid* GetSolid(G4String solidname, double thickness=0.0);

private:
    double P1r, P1z;
    double P2r, P2z;
    double P3r, P3z;
    double P4r, P4z;
};

#endif
