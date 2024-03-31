#ifndef R12860OnlyFront_PMTSolid_hh
#define R12860OnlyFront_PMTSolid_hh

#include "globals.hh"
#include "G4SystemOfUnits.hh"
class G4VSolid;

class R12860OnlyFront_PMTSolid {

public:
    R12860OnlyFront_PMTSolid(double R1, double zfront, double zback);

    G4VSolid* GetSolid(G4String solidname, double thickness=0.0);

private:
    double m_R1;
    double m_zfront;
    double m_zback;

    static double delta_torlerance;

};

#endif
