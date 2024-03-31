#include "CalibSourceConstruction.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
using namespace CLHEP;
DECLARE_TOOL(CalibSourceConstruction);

CalibSourceConstruction::CalibSourceConstruction(const std::string& name)
    : ToolBase(name)
      , solidWorld(0), logicWorld(0)
//      , solidCT(0), logicCT(0)
//      , solidSource(0), logicSource(0), physiSource(0)
//      , vacSource(0), logicvacSource(0), physivacSource(0)
//      , solidTube2(0), logicTube2(0), physiTube2(0)
//      , vacTube2(0), logicvacTube2(0), physivacTube2(0)
//      , solidWindow(0), logicWindow(0), physiWindow(0)
//      , LSWindow(0), logicLSWindow(0), physiLSWindow(0)
//      , SS(0), Vacuum(0), Mylar(0)
{

}

CalibSourceConstruction::~CalibSourceConstruction() {

}

G4LogicalVolume*
CalibSourceConstruction::getLV() {
    if (logicWorld) {
        return logicWorld;
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
    logicWorld -> SetVisAttributes(ct_visatt);
    return logicWorld;
}

bool
CalibSourceConstruction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    return true;
}

void
CalibSourceConstruction::initVariables() {
	m_SteelWeight_rad = 1.26*cm;
	m_SteelWeight_height = 2.5*cm;
	m_SteelWeight_distance = 20*cm;
	m_Acrylic_rad = 10*mm;
	m_AcrylicCylinder_height = 30.*mm;

	m_SteelPipe_height = 30.*mm;
	m_SteelPipe_rad = 8.*mm ;
	m_SteelPipe_thickness = 1.65*mm;
	m_Al_rad = 2*mm;
	m_Air_rad = 1*mm ;
}

void 
CalibSourceConstruction::initMaterials() {
    // Xin Qian added Nov 2nd 2014 
    Acrylic = G4Material::GetMaterial("Acrylic");
    LS = G4Material::GetMaterial("LS");
    SS = G4Material::GetMaterial("StainlessSteel");
    Mylar = G4Material::GetMaterial("Mylar");
    Vacuum = G4Material::GetMaterial("VacuumT");
    Steel = G4Material::GetMaterial("Steel");
    Air = G4Material::GetMaterial("Air");
    Al = new G4Material("Al", 13, 26.98153863*g/mole, 2.70*g/cm3);
}

void
CalibSourceConstruction::makeCTLogical() {

    G4VisAttributes* acrylic_visatt = new G4VisAttributes(G4Colour(0, 0, 1.0));
    acrylic_visatt -> SetForceWireframe(true);
    acrylic_visatt -> SetForceAuxEdgeVisible(true);

	solidWorld = new G4Sphere("sWorld",
				0*m,
				1*m,
				0*deg,
				360*deg,
				0*deg,
				180*deg);
	logicWorld = new G4LogicalVolume(solidWorld,
				LS,
				"lWorld",
				0,
				0,
				0);


    G4Tubs* solidWeight1 = new G4Tubs("sWeight1",
                                    0*m,
                                    m_SteelWeight_rad,
                                    m_SteelWeight_height/2,
                                    0.*deg,
                                    360.*deg);
    G4LogicalVolume* logicWeight1 = new G4LogicalVolume(solidWeight1,
                                    Steel,
                                    "lWeight1",
                                    0,0,0);
    G4PVPlacement* physiWeight1 = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,m_SteelWeight_distance),
                                    logicWeight1,
                                    "pWeight1",
                                    logicWorld,
                                    false,0);

    G4Tubs* solidWeight2 = new G4Tubs("sWeight2",
                                    0*m,
                                    m_SteelWeight_rad,
                                    m_SteelWeight_height/2,
                                    0.*deg,
                                    360.*deg);
    G4LogicalVolume* logicWeight2 = new G4LogicalVolume(solidWeight2,
                                    Steel,
                                    "lWeight2",
                                    0,0,0);
    G4PVPlacement* physiWeight2 = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,-m_SteelWeight_distance),
                                    logicWeight2,
                                    "pWeight2",
                                    logicWorld,
                                    false,0);

G4OpticalSurface *OpticalWeightMirror = new G4OpticalSurface("OpticalWeightMirror");
OpticalWeightMirror->SetModel(unified);
OpticalWeightMirror->SetType(dielectric_metal);
OpticalWeightMirror->SetFinish(polished);

const G4int NUM = 2;
G4double vessel_PP[NUM]   = { 1.5*eV, 7.50*eV };

G4double ICEREFLECTIVITY[NUM] = {0.45,0.45};
G4MaterialPropertiesTable *SteelMirrorMPT = new G4MaterialPropertiesTable();
SteelMirrorMPT->AddProperty("REFLECTIVITY", vessel_PP, ICEREFLECTIVITY,NUM);
OpticalWeightMirror->SetMaterialPropertiesTable(SteelMirrorMPT);

new G4LogicalSkinSurface("Weight Surface",logicWeight1,OpticalWeightMirror);
new G4LogicalSkinSurface("Weight Surface",logicWeight2,OpticalWeightMirror);



G4Sphere* solidHemiSphere1 = new G4Sphere("sHemiSphere1",
    0*mm,
    m_Acrylic_rad,
    0.*deg,
    360.*deg,
    0.*deg,
    90.*deg);
G4LogicalVolume* logicHemiSphere1 = new G4LogicalVolume(solidHemiSphere1,
    Acrylic,
    "lHemiSphere1",
    0,
    0,
    0);

G4PVPlacement* physiHemiSphere1 = new G4PVPlacement(0,
    G4ThreeVector(0,0, m_AcrylicCylinder_height/2),
    logicHemiSphere1,
    "pHemiSphere1",
    logicWorld,
    false,
    0);
logicHemiSphere1-> SetVisAttributes(acrylic_visatt);


G4Sphere* solidHemiSphere2 = new G4Sphere("sHemiSphere2",
    0*mm,
    m_Acrylic_rad ,
    0.*deg,
    360.*deg,
    90.*deg,
    180.*deg);
 G4LogicalVolume* logicHemiSphere2 = new G4LogicalVolume(solidHemiSphere2,
    Acrylic,
    "lHemiSphere2",
    0,
    0,
    0);

G4PVPlacement* physiHemiSphere2 = new G4PVPlacement(0,
    G4ThreeVector(0,0,-m_AcrylicCylinder_height/2),
    logicHemiSphere2,
    "pHemiSphere2",
    logicWorld,
    false,
    0);

logicHemiSphere2-> SetVisAttributes(acrylic_visatt);

G4Tubs* solidTubs = new G4Tubs("sTubs",
    0*mm,
    m_Acrylic_rad,
    m_AcrylicCylinder_height/2,
    0.*deg,
    360.*deg);
G4LogicalVolume* logicTubs = new G4LogicalVolume(solidTubs,
      Acrylic,
      "lTubs",
      0,
      0,
      0);

G4PVPlacement* physiTubs = new G4PVPlacement(0,              // no rotation
      G4ThreeVector(0,0,0), // at (x,y,z)
      logicTubs,    // its logical volume 
      "pTubs",       // its name
      logicWorld,
      false,           // no boolean operations
      0);

logicTubs-> SetVisAttributes(acrylic_visatt);



G4Tubs* solidPipe = new G4Tubs("sPipe",
    m_SteelPipe_rad - m_SteelPipe_thickness,
    m_SteelPipe_rad,
    m_SteelPipe_height/2,
    0.*deg,
    360.*deg);


G4LogicalVolume* logicPipe = new G4LogicalVolume(solidPipe,
      Steel,
      "lPipe",
      0,
      0,
      0);
G4PVPlacement* physiPipe = new G4PVPlacement(0,              // no rotation
      G4ThreeVector(0,0,0), // at (x,y,z)
      logicPipe,    // its logical volume 
      "pPipe",       // its name
      logicTubs,
      false,           // no boolean operations
      0);              // no particular field
logicPipe-> SetVisAttributes(acrylic_visatt);



G4OpticalSurface *OpticalSteelMirror = new G4OpticalSurface("SteelMirrorSurface");
OpticalSteelMirror->SetModel(unified);
OpticalSteelMirror->SetType(dielectric_metal);
OpticalSteelMirror->SetFinish(polished);

//const G4int NUM = 2;
//G4double vessel_PP[NUM]   = { 1.5*eV, 7.50*eV };

//G4double REFLECTIVITY[NUM] = {1,1};
G4double REFLECTIVITY[NUM] = {0.45,0.45};
G4MaterialPropertiesTable *PipeMirrorMPT = new G4MaterialPropertiesTable();
PipeMirrorMPT->AddProperty("REFLECTIVITY", vessel_PP, REFLECTIVITY,NUM);
OpticalSteelMirror->SetMaterialPropertiesTable(PipeMirrorMPT);

new G4LogicalBorderSurface("Air/Mirror Surface",physiTubs,physiPipe,OpticalSteelMirror);


G4Tubs* solidAl = new G4Tubs("sAl",
    0*mm,
    m_Al_rad,
    m_Al_rad,
    0.*deg,
    360.*deg);
G4LogicalVolume* logicAl = new G4LogicalVolume(solidAl,
    Al,
    "lAl",
    0,
    0,
    0);
G4PVPlacement* physiAl = new G4PVPlacement(0,
    G4ThreeVector(0,0,0),
    logicAl,
    "pAl",
    logicTubs,
    false,
    0);


G4Tubs* solidAir = new G4Tubs("sAir",
    0*mm,
    m_Air_rad,
    m_Air_rad,
    0.*deg,
    360.*deg);



G4LogicalVolume* logicAir = new G4LogicalVolume(solidAir,
    Air,
    "lAir",
    0,0,0);
G4PVPlacement* physiAir = new G4PVPlacement(0,
    G4ThreeVector(0,0,0),
    logicAir,
    "pAir",
//	logicWorld,
    logicAl,
    false,
    0);

logicAir-> SetVisAttributes(acrylic_visatt);

//  solidSource = new G4Tubs("sSource",
//               0*m,
//               10*cm,
//               10*cm,
//               0.*deg,
//               360*deg
//               );
//  
//  logicSource = new G4LogicalVolume(solidSource,
//                SS,
//                "lSource",
//                0,
//                0,
//                0);
//  
//  physiSource = new G4PVPlacement(0,              // no rotation
//                 G4ThreeVector(0,0,m_calibTubeLength2/2.), // at (x,y,z)
//                 logicSource,    // its logical volume 
//                 "pSource",       // its name
//                 logicWorld,  // its mother  volume
//                 false,           // no boolean operations
//                 0);              // no particular field
//
//
//  vacSource = new G4Tubs("vSource",
//               0*m,
//               m_calibTubeOuterRadius-m_calibTubeThickness,
//               (m_calibTubeLength1)/2.,
//               0.*deg,
//               360*deg
//               );
//  
//  logicvacSource = new G4LogicalVolume(vacSource,
//                Vacuum,
//                "lvSource",
//                0,
//                0,
//                0);
//
//  physivacSource = new G4PVPlacement(0,              // no rotation
//                 G4ThreeVector(0,0,0), // at (x,y,z)
//                 logicvacSource,    // its logical volume 
//                 "pvSource",       // its name
//                 logicSource,  // its mother  volume
//                 false,           // no boolean operations
//                 0);              // no particular field
//  
//  // second Acrylic tube volume
//  solidTube2 = new G4Tubs("sTube2",
//               0*m,
//               m_calibTubeOuterRadius,
//               (m_calibTubeLength2)/2.,
//               0.*deg,
//               360*deg
//               );
//  
//  logicTube2 = new G4LogicalVolume(solidTube2,
//                Acrylic,
//                "lTube2",
//                0,
//                0,
//                0);
//  
//  physiSource = new G4PVPlacement(0,              // no rotation
//                 G4ThreeVector(0,0,-m_calibTubeLength1/2.), // at (x,y,z)
//                 logicTube2,    // its logical volume 
//                 "pTube2",       // its name
//                 logicCT,  // its mother  volume
//                 false,           // no boolean operations
//                 0);              // no particular field
//
//
//  vacTube2 = new G4Tubs("vTube2",
//               0*m,
//               m_calibTubeOuterRadius-m_calibTubeThickness,
//               (m_calibTubeLength2)/2.,
//               0.*deg,
//               360*deg
//               );
//  
//  logicvacTube2 = new G4LogicalVolume(vacTube2,
//                Vacuum,
//                "lvTube2",
//                0,
//                0,
//                0);
//
//  physivacTube2 = new G4PVPlacement(0,              // no rotation
//                 G4ThreeVector(0,0,0), // at (x,y,z)
//                 logicvacTube2,    // its logical volume 
//                 "pvTube2",       // its name
//                 logicTube2,  // its mother  volume
//                 false,           // no boolean operations
//                 0);              // no particular field
//  
//  // window volume 
//  solidWindow = new G4Ellipsoid("sWindow",
//                m_calibTubeOuterRadius-m_calibTubeThickness,
//                m_calibTubeOuterRadius-m_calibTubeThickness,
//                m_calibWindowDistortionHeight,
//                0,
//                m_calibWindowDistortionHeight
//                );
//  
//  logicWindow = new G4LogicalVolume(solidWindow,
//                    //LS,
//                    Mylar,
//                "lWindow",
//                0,
//                0,
//                0);
//
//  physiWindow = new G4PVPlacement(0,              // no rotation
//                  G4ThreeVector(0,0,-(m_calibTubeLength2)/2.), // at (x,y,z)
//                 logicWindow,    // its logical volume 
//                 "pWindow",       // its name
//                 logicvacTube2,  // its mother  volume
//                 false,           // no boolean operations
//                 0);              // no particular field
//  
//  LSWindow = new G4Ellipsoid("lsWindow",
//                m_calibTubeOuterRadius-m_calibTubeThickness-m_calibWindowThickness,
//                m_calibTubeOuterRadius-m_calibTubeThickness-m_calibWindowThickness,
//                m_calibWindowDistortionHeight - m_calibWindowThickness,
//                0,
//                m_calibWindowDistortionHeight - m_calibWindowThickness
//                );
//
//  
//  logicLSWindow = new G4LogicalVolume(LSWindow,
//                LS,
//                "lLSWindow",
//                0,
//                0,
//                0);
//
//  physiLSWindow = new G4PVPlacement(0,              // no rotation
//                    G4ThreeVector(0,0,0), // at (x,y,z)
//                    logicLSWindow,    // its logical volume 
//                    "pLSWindow",       // its name
//                    logicWindow,  // its mother  volume
//                    false,           // no boolean operations
//                    0);              // no particular field



  //  // flat window
  // solidWindow = new G4Tubs("sWindow",
  // 		       0*m,
  // 		       m_calibTubeOuterRadius-m_calibTubeThickness,
  // 		       m_calibWindowThickness/2.,
  // 		       0.*deg,
  // 		       360*deg
  // 		       );

  
  // logicWindow = new G4LogicalVolume(solidWindow,
  // 				    Mylar,
  // 				"lWindow",
  // 				0,
  // 				0,
  // 				0);

  // physiWindow = new G4PVPlacement(0,              // no rotation
  // 				  G4ThreeVector(0,0,-(m_calibTubeLength2)/2.+m_calibWindowThickness/2.), // at (x,y,z)
  // 				 logicWindow,    // its logical volume 
  // 				 "pWindow",       // its name
  // 				 logicvacTube2,  // its mother  volume
  // 				 false,           // no boolean operations
  // 				 0);              // no particular field
}
