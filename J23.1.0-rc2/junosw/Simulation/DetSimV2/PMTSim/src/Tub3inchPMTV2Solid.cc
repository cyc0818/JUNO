#include "Tub3inchPMTV2Solid.hh"

#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4Polycone.hh"

#include <cmath>
using namespace CLHEP;
Tub3inchPMTV2Solid::Tub3inchPMTV2Solid(
        double R, double h, double H)
    : m_R(R), m_h(h), m_H(H) {

}

G4VSolid*
Tub3inchPMTV2Solid::GetSolid(G4String solidname, double thickness) {
    G4double zPlane[] = {m_h-m_H-thickness, m_h+thickness};
    G4double rInner[] = {0.,                0.};
    G4double rOuter[] = {m_R + thickness,   m_R + thickness};

    G4VSolid* pmt_solid = new G4Polycone(
                                solidname+"Tubs",
                                0,
                                360*deg,
                                2,
                                zPlane,
                                rInner,
                                rOuter
                                );
    return pmt_solid;
}

