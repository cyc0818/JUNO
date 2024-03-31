#include "Tub3inchPMTSolid.hh"

#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include <cmath>
using namespace CLHEP;
Tub3inchPMTSolid::Tub3inchPMTSolid(
        double R,  double H)
    : m_R(R), m_H(H) {

}

G4VSolid*
Tub3inchPMTSolid::GetSolid(G4String solidname, double thickness) {
    G4Tubs* pmt_solid = new G4Tubs(
                                    solidname+"_Tube",
                                    0*mm,  /* inner */ 
                                    m_R + thickness, /* pmt_r */ 
                                    m_H + thickness, /* part 2 h */ 
                                    0*deg, 
                                    360*deg);
    return pmt_solid;
}

