#ifndef R12860_TorusPMTSolid_hh
#define R12860_TorusPMTSolid_hh

#include "globals.hh"
#include "G4SystemOfUnits.hh"
class G4VSolid;

class R12860_TorusPMTSolid {

public:
    R12860_TorusPMTSolid(double R1, double R1z, double R2, double R3, double H_1_2, double H3, double R4, double R4z, double R5, double R6, double H6, double H);

    G4VSolid* GetSolid(G4String solidname, double thickness=0.0);

    G4VSolid* GetCoverTop(G4String solidname, 
                            G4double c_h1,
                            G4double c_r1,
                            G4double c_h2,
                            G4double c_r2,
                            G4double thickness=1*mm);
    G4VSolid* GetCoverTopCurve(G4String solidname,
                            G4double c_h1,
                            G4double c_r1,
                            G4double c_h2,
                            G4double c_r2,
                            G4double center2equator,
                            G4double ball_r);
    G4VSolid* GetCoverBottom(G4String solidname, 
                            G4double c_h2,
                            G4double c_r2,
                            G4double c_h3,
                            G4double c_r3,
                            G4double thick_out=11*mm,
                            G4double thick_in=1*mm);

    G4VSolid* GetCoverSolid(G4String solidname, 
                            G4double c_h1,
                            G4double c_r1,
                            G4double c_h2,
                            G4double c_r2,
                            G4double c_h3,
                            G4double c_r3,
                            G4double thickness=11.01*mm);
    G4VSolid* GetCoverSolidWithTopCurve(G4String solidname, 
                            G4double c_h0,
                            G4double c_r0,
                            G4double c_h1,
                            G4double c_r1,
                            G4double c_h2,
                            G4double c_r2,
                            G4double c_h3,
                            G4double c_r3,
                            G4double center2equator,
                            G4double ball_r,
                            G4double thickness=11.01*mm);

    void SetCoverEdge(G4int n) {
        numSide = n;
    }

private:
    double m_R1;
    double m_R1z;
    double m_R1p;
    double m_R2;
    double m_R3;
    double m_theta1;
    double m_H;
    double m_H_1_2;

    double m_H3;

    double m_R4;
    double m_R4z;
    double m_R4p;
    double m_R5;
    double m_R6;
    double m_theta2;
    double m_H_3_4;

    double m_H6;

    static double delta_torlerance;
    G4int numSide;

};

#endif
