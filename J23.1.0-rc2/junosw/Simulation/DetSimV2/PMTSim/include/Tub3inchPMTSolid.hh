#ifndef Tub3inchPMTSolid_hh
#define Tub3inchPMTSolid_hh

#include "globals.hh"

class G4VSolid;

class Tub3inchPMTSolid {

public:
    Tub3inchPMTSolid(double R, double H);

    G4VSolid* GetSolid(G4String solidname, double thickness=0.0);

private:
    double m_R;
    double m_H;


    static double delta_torlerance;

};

#endif
