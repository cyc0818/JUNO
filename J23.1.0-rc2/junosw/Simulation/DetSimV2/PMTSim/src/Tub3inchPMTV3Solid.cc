#include "Tub3inchPMTV3Solid.hh"

#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Ellipsoid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include <cmath>
using namespace CLHEP;
Tub3inchPMTV3Solid::Tub3inchPMTV3Solid(
        double R, double H, double Rc, double Zc1, double Zc2)
    : m_R(R), m_H(H), m_Rc(Rc), m_Zc1(Zc1), m_Zc2(Zc2) {

}

G4VSolid*
Tub3inchPMTV3Solid::GetContainerSolid(G4String solidname, double dr) {

    G4double zPlane[] = {m_Zc1, m_Zc2-dr};
    G4double rInner[] = {0.,      0.};
    G4double rOuter[] = {m_Rc+dr,  m_Rc+dr};

    G4VSolid* cntr_solid = new G4Polycone(
                                solidname,
                                0,
                                360*deg,
                                2,
                                zPlane,
                                rInner,
                                rOuter
                                );

    return cntr_solid;
}

G4VSolid*
Tub3inchPMTV3Solid::GetEllipsoidSolid(G4String solidname, double Z_cut1, double Z_cut2, double dr) {
    G4VSolid* ell_solid = new G4Ellipsoid(solidname+"_ell_helper", 
                                          m_R+dr,    // half x
                                          m_R+dr,    // half y
                                          m_H+dr,    // half z
                                          Z_cut2,    // botoom cut
                                          Z_cut1);   // top cut

    return ell_solid;
}

G4VSolid*
Tub3inchPMTV3Solid::GetEllipsoidSolid(G4String solidname, double dr) {

    return GetEllipsoidSolid(solidname+"_ell", m_H, m_Zc1, dr);
}

G4VSolid*
Tub3inchPMTV3Solid::GetUnionSolid(G4String solidname, double dr) {
  G4VSolid* cyl_solid = GetContainerSolid(solidname+"_cyl", dr);
//  G4VSolid* ell_solid = GetEllipsoidSolid(solidname+"_ell", m_H+dr, m_Zc1, dr); // Too slow initialization
  G4VSolid* sph_solid = new G4Sphere(solidname+"_sph", 0., m_R+dr, 0., 360.*deg, 0., 180.*deg);
  G4VSolid* uni_solid = new G4UnionSolid(solidname, cyl_solid, sph_solid, NULL, G4ThreeVector(0., 0., 0.));

  return uni_solid;
}

