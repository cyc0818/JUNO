#include "R12860OnlyFront_PMTSolid.hh"

#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Polyhedra.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include <cmath>
using namespace CLHEP;
double R12860OnlyFront_PMTSolid::delta_torlerance = 1E-2*mm;

R12860OnlyFront_PMTSolid::R12860OnlyFront_PMTSolid(
        double R1, double zfront, double zback)
    : m_R1(R1), m_zfront(zfront), m_zback(zback) {
    G4cout << __FILE__ << ":" << __LINE__ << std::endl;
}

G4VSolid*
R12860OnlyFront_PMTSolid::GetSolid(G4String solidname, double thickness) {
    // Calculate Parameter first
    double r1t = m_R1 + thickness;
    double zft = m_zfront + thickness; // front
    double zbt = m_zback - thickness;  // back. Because m_zback<0

    // Show variables
    G4cout << "r1t: " << r1t/mm << " mm" << G4endl;
    G4cout << "zft: " << zft/mm << " mm" << G4endl;
    G4cout << "zbt: " << zbt/mm << " mm" << G4endl;
    // Construct the PMT Solid
    // * PART 1
    // G4Sphere* pmttube_solid_sphere = new G4Sphere(
    //                                         solidname+"_1_Sphere",
    //                                         0*mm, // R min
    //                                         r1t, // R max
    //                                         0*deg, // Start Phi
    //                                         360*deg, // Delta Phi
    //                                         0*deg, // Start Theta
    //                                         180*deg  // Delta Theta
    //                                         );
    G4Ellipsoid* pmttube_solid_sphere = new G4Ellipsoid(
                                            solidname+"_1_Front_Ellipsoid",
                                            r1t, // pxSemiAxis
                                            r1t, // pySemiAxis
                                            zft, // pzSemiAxis
                                            zbt, // cut at bottom
                                            zft  // cut at top
                                            );
    // * PART 2
    G4double zPlane[] = {
        zbt, // back
        0    // equator 
    };
    G4double rInner[] = {0.,
                         0.};
    G4double rOuter[] = {r1t,
                         r1t};

    G4Polycone* pmttube_solid_back = new G4Polycone(
                                                    solidname+"_2_Back_Tube",
                                                    0*deg,
                                                    360*deg,
                                                    2,
                                                    zPlane,
                                                    rInner,
                                                    rOuter);

    // Join

    // * PART 1 + 2
    G4UnionSolid* pmttube_solid_1_2 = new G4UnionSolid(
                                            solidname+"_1_2",
                                            pmttube_solid_sphere,
                                            pmttube_solid_back,
                                            0,
                                            G4ThreeVector(0, 0, 0)
                                            );
    return pmttube_solid_1_2;
}

