
#include "ExplosionProofSolid.hh"

#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Torus.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4RotationMatrix.hh"


#include <cmath>

using namespace CLHEP;

double ExplosionProofSolid::delta_torlerance = 1E-2*mm;

ExplosionProofSolid::ExplosionProofSolid(
    double R1, double R2, double R3, double H)
: m_R1(R1), m_R2(R2), m_R3(R3), m_H(H) {

  m_theta = asin((m_R2+m_R3)/(m_R1+m_R2));
  m_H_1_2 = (m_R1+m_R2) * cos(m_theta);
  m_H3 = m_H - m_R1 - m_H_1_2;
}

G4VSolid*
ExplosionProofSolid::GetSolidTop(G4String solidname,double thickness) {
  double r1t = m_R1 + thickness;
  G4Sphere* top_sphere = new G4Sphere(
      solidname+"top_Sphere",
      0*mm, // R min
      r1t, // R max
      0*deg, // Start Phi
      360*deg, // Delta Phi
      0*deg, // Start Theta
      90*deg  // Delta Theta
      );
  G4Tubs* top_tube = new G4Tubs(
      solidname+"_top_tube",
      0 *mm,  
      r1t *mm,  
      50. *mm,  
      0*deg, 
      360*deg);
  G4UnionSolid* top_solid = new G4UnionSolid
    (solidname+"_top_solid",
     top_sphere,
     top_tube,
     0,
     G4ThreeVector(0,0,-50.)) ;
  return top_solid;
}
  
G4VSolid*
ExplosionProofSolid::GetSolidBottom(G4String solidname,double thickness) {
  double r0 = m_R1 + thickness;
  double theta1 = asin(100/r0);
  double r1t = r0/cos(theta1)-1;
  double r4t = r1t * sin(m_theta);
  double r2t = m_R2 -thickness;
  double r3t = m_R3 + thickness;
  double h1t = r1t * cos(m_theta);
  double h2t = r2t * cos(m_theta);
  double h3t = m_H3 + thickness;

  G4Sphere* bottom_sphere = new G4Sphere(
          solidname+"_bottom_Sphere",
          0*mm, 
          r1t, 
          0*deg,
          360*deg, 
          90*deg, 
          180*deg  
          );
  G4Tubs* sub_tube = new G4Tubs(
          solidname+"_sub_tube",
          0 *mm,  
          r1t+delta_torlerance*10,  
          100.001 *mm,  
          0*deg, 
          360*deg);

  G4Tubs* bottom_tube = new G4Tubs(
          solidname+"_2_Tube",
          0*mm,   
          r4t+delta_torlerance,  
          h2t/2+delta_torlerance,  
          0*deg, 
          360*deg);

  G4Torus* bottom_torus = new G4Torus(
          solidname+"_2_Torus",
          0*mm,  // R min
          r2t+delta_torlerance, // R max
          (r2t+r3t), // Swept Radius
          -0.01*deg,
          360.01*deg);

  G4SubtractionSolid* bottom_part2 = new G4SubtractionSolid(
          solidname+"_part2",
          bottom_tube,
          bottom_torus,
          0,
          G4ThreeVector(0,0,-h2t/2)
          );

  G4Tubs* bottom_end_tube = new G4Tubs(
          solidname+"_3_EndTube",
          0*mm,   
          r3t+delta_torlerance, 
          h3t/2+delta_torlerance, 
          0*deg, 
          360*deg);

  G4UnionSolid* bottom_1_2 = new G4UnionSolid(
          solidname+"_1_2",
          bottom_sphere,
          bottom_part2,
          0,
          G4ThreeVector(0, 0, -(h1t+h2t/2))
          );

  G4UnionSolid* bottom_1_2_3 = new G4UnionSolid(
          solidname,
          bottom_1_2,
          bottom_end_tube,
          0,
          G4ThreeVector(0,0, 
          -(m_H_1_2+h3t*0.50))
          );
  G4SubtractionSolid* bottom = new G4SubtractionSolid
    (solidname+"_bottom",
     bottom_1_2_3,
     sub_tube ,
     0,
     G4ThreeVector(0,0,-1)) ;


  return bottom;
}
G4VSolid*
ExplosionProofSolid::GetSolid(G4String solidname, double radiu,double height) {

  G4Sphere*  Top= new G4Sphere(
      solidname+"_Sphere",
      0*mm, 
      radiu, 
      0*deg,
      360*deg, 
      0*deg,
      90*deg 
      );
  G4Tubs* Bottom = new G4Tubs(
      solidname+"_Tube",
      0*mm,   
      radiu,  
      height/2,  
      0*deg, 
      360*deg);
  G4UnionSolid* ExplosionSoild = new G4UnionSolid
    (solidname+"_pmttube_solid_sphere_bottom_1",
     Top ,
     Bottom ,
     0,
     G4ThreeVector(0,0,-height/2)    ) ;
    return ExplosionSoild; 
}

