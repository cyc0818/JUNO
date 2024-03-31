#ifndef Tub3inchPMTV2Solid_hh
#define Tub3inchPMTV2Solid_hh

#include "globals.hh"

class G4VSolid;

class Tub3inchPMTV2Solid {

public:
    // R is the radius
    // h is the height from front to equator
    // H is the Height of the PMT
    Tub3inchPMTV2Solid(double R, double h, double H);

    G4VSolid* GetSolid(G4String solidname, double thickness=0.0);

private:
    double m_R;
    double m_h;
    double m_H;


    static double delta_torlerance;

};

#endif
