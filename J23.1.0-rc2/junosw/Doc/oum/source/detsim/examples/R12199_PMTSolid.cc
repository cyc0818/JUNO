#include "R12199_PMTSolid.hh"

#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Polycone.hh"
#include "G4UnionSolid.hh"

#include <cmath>

R12199_PMTSolid::R12199_PMTSolid() {
    P1r = 0.0; P1z = 22.5*mm;
    P2r = 40.0*mm; P2z = 0.0*mm;
    P3r = 28.25*mm; P3z = -55.5*mm;
    P4r = 28.25*mm; P4z = -87.5*mm;
}

G4VSolid*
R12199_PMTSolid::GetSolid(G4String solidname, double thickness) {

    double _p1r = P1r;           double _p1z = P1z + thickness;
    double _p2r = P2r+thickness; double _p2z = P2z;
    double _p3r = P3r+thickness; double _p3z = P3z+thickness;
    double _p4r = P4r+thickness; double _p4z = P4z-thickness;

    double r_bottom = 51.9*mm/2;
    double r_botthin = r_bottom+thickness;

    std::cout << "R12199_PMTSolid::GetSolid " << __LINE__ << std::endl;
    // PART: ellipsoid
    G4Ellipsoid* pmttube_solid_sphere = new G4Ellipsoid(
                                            solidname+"_1_Ellipsoid",
                                            _p2r, // pxSemiAxis
                                            _p2r, // pySemiAxis
                                            _p1z // pzSemiAxis
                                            );
    std::cout << "R12199_PMTSolid::GetSolid " << __LINE__ << std::endl;
    // return pmttube_solid_sphere;

    // PART: polycone
    double zplane[] = {_p4z, _p3z, _p3z, _p2z};
    double rinner[] = {0, 0, 0, 0};
    double router[] = {_p4r, _p3r, r_botthin, r_botthin};

    G4VSolid* pmttube_solid_tube = new G4Polycone(
                                solidname+"_2_polycone",
                                0,
                                360*deg,
                                4,
                                zplane,
                                rinner,
                                router
                                );
    G4UnionSolid* pmttube_solid = new G4UnionSolid
        (solidname,
         pmttube_solid_sphere ,
         pmttube_solid_tube ,
         0,
         G4ThreeVector(0,0,0)
    ) ;

    return pmttube_solid;
}
