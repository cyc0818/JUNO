#include "CalibTube_flatwindow_Construction.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(CalibTube_flatwindow_Construction);

CalibTube_flatwindow_Construction::CalibTube_flatwindow_Construction(const std::string& name)
    : ToolBase(name)
      , solidCT(0), logicCT(0)
      , solidTube1(0), logicTube1(0), physiTube1(0)
      , vacTube1(0), logicvacTube1(0), physivacTube1(0)
      , solidTube2(0), logicTube2(0), physiTube2(0)
      , vacTube2(0), logicvacTube2(0), physivacTube2(0)
      , solidWindow(0), logicWindow(0), physiWindow(0)
      , LSWindow(0), logicLSWindow(0), physiLSWindow(0)
      , SS(0), Vacuum(0), Mylar(0)
{

}

CalibTube_flatwindow_Construction::~CalibTube_flatwindow_Construction() {

}

G4LogicalVolume*
CalibTube_flatwindow_Construction::getLV() {
    if (logicCT) {
        return logicCT;
    }

    initMaterials();
    initVariables();

    makeCTLogical();

    // set vis attribute

    G4VisAttributes* ct_visatt = new G4VisAttributes(G4Colour(0, 0.5, 1.0));
    ct_visatt -> SetForceWireframe(true);  
    ct_visatt -> SetForceAuxEdgeVisible(true);
    //acrylic_visatt -> SetForceSolid(true);
    //acrylic_visatt -> SetForceLineSegmentsPerCircle(4);
    logicCT -> SetVisAttributes(ct_visatt);
    return logicCT;
}

bool
CalibTube_flatwindow_Construction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    return true;
}

void
CalibTube_flatwindow_Construction::initVariables() {
    // Xin Qian added Nov 2nd 2014
    m_calibTubeLength1 = 17.3*m; // SS section
    m_calibTubeLength2 = 0.3*m; // acrylic section

    // m_calibTubeLength1 = 0.05*m; // SS section
    // m_calibTubeLength2 = 0.05*m; // acrylic section

    m_calibTubeOuterRadius = 2.54*cm; 
    m_calibTubeThickness = 0.15*cm;
    
    m_calibWindowDistortionHeight = 2.54*(2-1.732)*cm;
    m_calibWindowThickness = 0.003*2.54*cm;
    //

}

void 
CalibTube_flatwindow_Construction::initMaterials() {
    // Xin Qian added Nov 2nd 2014 
    
    LS = G4Material::GetMaterial("LS");
    SS = G4Material::GetMaterial("StainlessSteel");
    Acrylic = G4Material::GetMaterial("Acrylic");
    Mylar = G4Material::GetMaterial("Mylar");
    //Mylar = G4Material::GetMaterial("LS");
    //Acrylic = G4Material::GetMaterial("LS");
    Vacuum = G4Material::GetMaterial("VacuumT");

}

void
CalibTube_flatwindow_Construction::makeCTLogical() {
  // main CT tube volume
  solidCT = new G4Tubs("sCT",
               0*m,
               m_calibTubeOuterRadius,
               (m_calibTubeLength1+m_calibTubeLength2)/2.,
               0.*deg,
               360*deg
               );
  
  logicCT = new G4LogicalVolume(solidCT,
                LS,
                "lCT",
                0,
                0,
                0);

  // first SS tube volume
  solidTube1 = new G4Tubs("sTube1",
               0*m,
               m_calibTubeOuterRadius,
               (m_calibTubeLength1)/2.,
               0.*deg,
               360*deg
               );
  
  logicTube1 = new G4LogicalVolume(solidTube1,
                SS,
                "lTube1",
                0,
                0,
                0);
  
  physiTube1 = new G4PVPlacement(0,              // no rotation
                 G4ThreeVector(0,0,m_calibTubeLength2/2.), // at (x,y,z)
                 logicTube1,    // its logical volume 
                 "pTube1",       // its name
                 logicCT,  // its mother  volume
                 false,           // no boolean operations
                 0);              // no particular field


  vacTube1 = new G4Tubs("vTube1",
               0*m,
               m_calibTubeOuterRadius-m_calibTubeThickness,
               (m_calibTubeLength1)/2.,
               0.*deg,
               360*deg
               );
  
  logicvacTube1 = new G4LogicalVolume(vacTube1,
                Vacuum,
                "lvTube1",
                0,
                0,
                0);

  physivacTube1 = new G4PVPlacement(0,              // no rotation
                 G4ThreeVector(0,0,0), // at (x,y,z)
                 logicvacTube1,    // its logical volume 
                 "pvTube1",       // its name
                 logicTube1,  // its mother  volume
                 false,           // no boolean operations
                 0);              // no particular field
  
  // second Acrylic tube volume
  solidTube2 = new G4Tubs("sTube2",
               0*m,
               m_calibTubeOuterRadius,
               (m_calibTubeLength2)/2.,
               0.*deg,
               360*deg
               );
  
  logicTube2 = new G4LogicalVolume(solidTube2,
                Acrylic,
                "lTube2",
                0,
                0,
                0);
  
  physiTube1 = new G4PVPlacement(0,              // no rotation
                 G4ThreeVector(0,0,-m_calibTubeLength1/2.), // at (x,y,z)
                 logicTube2,    // its logical volume 
                 "pTube2",       // its name
                 logicCT,  // its mother  volume
                 false,           // no boolean operations
                 0);              // no particular field


  vacTube2 = new G4Tubs("vTube2",
               0*m,
               m_calibTubeOuterRadius-m_calibTubeThickness,
               (m_calibTubeLength2)/2.,
               0.*deg,
               360*deg
               );
  
  logicvacTube2 = new G4LogicalVolume(vacTube2,
                Vacuum,
                "lvTube2",
                0,
                0,
                0);

  physivacTube2 = new G4PVPlacement(0,              // no rotation
                 G4ThreeVector(0,0,0), // at (x,y,z)
                 logicvacTube2,    // its logical volume 
                 "pvTube2",       // its name
                 logicTube2,  // its mother  volume
                 false,           // no boolean operations
                 0);              // no particular field
  
 



   // flat window
  solidWindow = new G4Tubs("sWindow",
  		       0*m,
  		       m_calibTubeOuterRadius-m_calibTubeThickness,
  		       m_calibWindowThickness/2.,
  		       0.*deg,
  		       360*deg
  		       );

  
  logicWindow = new G4LogicalVolume(solidWindow,
  				    Mylar,
  				"lWindow",
  				0,
  				0,
  				0);

  physiWindow = new G4PVPlacement(0,              // no rotation
  				  G4ThreeVector(0,0,-(m_calibTubeLength2)/2.+m_calibWindowThickness/2.), // at (x,y,z)
  				 logicWindow,    // its logical volume 
  				 "pWindow",       // its name
  				 logicvacTube2,  // its mother  volume
  				 false,           // no boolean operations
  				 0);              // no particular field
}
