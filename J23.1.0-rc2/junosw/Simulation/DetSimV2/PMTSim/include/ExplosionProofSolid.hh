
#ifndef ExplosionProofSolid_hh
#define ExplosionProofSolid_hh

#include "globals.hh"

class G4VSolid;

class ExplosionProofSolid {

public:
    ExplosionProofSolid(double R1, double R2, double R3, double H);

    G4VSolid* GetSolid(G4String solidname,double thickness);
    G4VSolid* GetSolid(G4String solidname, double radiu,double height) ;
    G4VSolid* GetSolid_New(G4String solidname,double thickness);
    G4VSolid* GetSolidTop_In(G4String solidname,double thickness);
    G4VSolid* GetSolidTop(G4String solidname,double thickness);
    G4VSolid* GetConnectionBox(G4String solidname);
    G4VSolid* GetSolidTop_Out(G4String solidname,double thickness);
    G4VSolid* GetSolidBottom(G4String solidname,double thickness);
    G4VSolid* GetSolidBottom_Out(G4String solidname,double thickness);
    G4VSolid* GetSolidBottom_In(G4String solidname,double thickness);

private:
    double m_R1;
    double m_R2;
    double m_R3;
    double m_theta;
    double m_H;
    double m_thickness;
    double m_H_1_2;
    double m_H3;
    static double delta_torlerance;

};

#endif
