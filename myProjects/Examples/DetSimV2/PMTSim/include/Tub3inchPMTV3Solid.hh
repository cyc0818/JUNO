#ifndef Tub3inchPMTV3Solid_hh
#define Tub3inchPMTV3Solid_hh

#include "globals.hh"

class G4VSolid;

class Tub3inchPMTV3Solid {

public:
    // R -- bulb radius
    // H -- bulb equator to top size
    // Rc -- container radius
    // Zc1 -- container top z-position
    // Zc2 -- container bottom z-position
    Tub3inchPMTV3Solid(double R, double H, double Rc, double Zc1, double Zc2);

    G4VSolid* GetContainerSolid(G4String solidname, double dr);
    G4VSolid* GetEllipsoidSolid(G4String solidname, double dr);
    G4VSolid* GetEllipsoidSolid(G4String solidname, double Z_cut1, double Z_cut2, double dr);
    G4VSolid* GetUnionSolid(G4String solidname, double dr);

private:
    double m_R;
    double m_H;
    double m_Rc;
    double m_Zc1;
    double m_Zc2;

};

#endif
