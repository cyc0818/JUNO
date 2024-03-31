#include "PrototypeConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "RoundBottomFlaskSolidMaker.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(PrototypeConstruction);

PrototypeConstruction::PrototypeConstruction(const std::string& name)
    : ToolBase(name)
{
    initVariables();
    declProp("BufferMaterial", m_buffer_material="LAB");
    declProp("UseChimney", m_use_chimney = false);
    declProp("UseEquatorCircle", m_use_equator_circle = false);
}

PrototypeConstruction::~PrototypeConstruction() {

}

G4LogicalVolume*
PrototypeConstruction::getLV() {
    initMaterials();

    makeSteelTubeLogical();

    makeBufferLogical();
    makeBufferPhysical();

    if (m_use_chimney) {
        // +++ Acrylic Ball or nylon ball
        makeAcrylicWithChimneyLogical();
        // ++++ LS
        makeLSWithChimneyLogical();
    } else {
        // +++ Acrylic Ball or nylon ball
        makeAcrylicLogical();
        // ++++ LS
        makeLSLogical();
    }
    makeAcrylicPhysical();
    makeLSPhysical();

    return logicSteelTube;
}

bool
PrototypeConstruction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    G4LogicalVolume* mothervol = 0;
    if ( motherName == "lBuffer" ) {
        mothervol = logicBuffer;
    }
    if (not mothervol) {
        // don't find the volume.
        return false;
    }

    // retrieve the daughter's LV
    G4LogicalVolume* daughtervol = other->getLV();

    if (not daughtervol) {
        return false;
    }

    static int copyno = 0;
    while (pos->hasNext()) {
        new G4PVPlacement(
            pos->next(),
            daughtervol,
            daughtervol->GetName()+"_phys",
            mothervol,
            false,
            copyno++
                );
    }
    return true;
}

void
PrototypeConstruction::initVariables() {
    m_steel_tube_height = 2*m;
    m_steel_tube_radius = 1*m;
    m_steel_tube_thickness = 3*mm;

    m_buffer_tube_height = m_steel_tube_height - 2*m_steel_tube_thickness;
    m_buffer_tube_radius = m_steel_tube_radius - m_steel_tube_thickness;

    m_target_radius = 0.25*m;
    m_target_chimney_radius = 50.*mm/2;

    m_acrylic_ball_thickness = 1.2*cm;
    m_acrylic_ball_radius = m_target_radius + m_acrylic_ball_thickness;
    m_acrylic_chimney_radius = 74.*mm/2;

    m_acrylic_circle_radius = 570.*mm/2;
    m_acrylic_circle_height = 40.*mm/2;

    // FIXME: need to update this value
    // assume the ball is at center.
    m_ChimneyTopToCenter = m_buffer_tube_height/2;
}

void 
PrototypeConstruction::initMaterials() {
    LS = G4Material::GetMaterial("LS");
    LAB = G4Material::GetMaterial("LAB");
    Water = G4Material::GetMaterial("Water");
    Steel = G4Material::GetMaterial("Steel");
    Acrylic = G4Material::GetMaterial("Acrylic");
}

void
PrototypeConstruction::makeSteelTubeLogical() {

    solidSteelTube = new G4Tubs("sSteelTube",
                                0*m,
                                m_steel_tube_radius,
                                m_steel_tube_height/2,
                                0.*deg,
                                360*deg);
    logicSteelTube = new G4LogicalVolume(solidSteelTube,
                                         Steel,
                                         "lSteelTube",
                                         0,
                                         0,
                                         0);
}

// ++ LAB
void 
PrototypeConstruction::makeBufferLogical() {
    solidBuffer = new G4Tubs("sBuffer",
                                0*m,
                                m_buffer_tube_radius,
                                m_buffer_tube_height/2,
                                0.*deg,
                                360*deg);
    G4Material* buffermat = LAB;
    if (m_buffer_material == "Water") {
        buffermat = Water;
    }
    logicBuffer = new G4LogicalVolume(solidBuffer,
                                         buffermat,
                                         "lBuffer",
                                         0,
                                         0,
                                         0);

}
void 
PrototypeConstruction::makeBufferPhysical() {
    physiBuffer = new G4PVPlacement(0,              // no rotation
                                     G4ThreeVector(0,0,0), // at (x,y,z)
                                     logicBuffer,    // its logical volume 
                                     "pBuffer",       // its name
                                     logicSteelTube,  // its mother  volume
                                     false,           // no boolean operations
                                     0);              // no particular field

}

// +++ Acrylic Ball or nylon ball
void 
PrototypeConstruction::makeAcrylicLogical() {
    G4Sphere* solidBall= new G4Sphere("sAcrylicBall",
                                0*m,
                                m_acrylic_ball_radius,
                                0.*deg, 
                                360.*deg, 
                                0.*deg, 
                                180.*deg);
    if (m_use_equator_circle) {
        // construct the circle 
        G4Tubs* solidAcrylicCircle = new G4Tubs("sAcrylicCircle",
                                                0*m,
                                                m_acrylic_circle_radius,
                                                m_acrylic_circle_height/2,
                                                0.*deg,
                                                360.*deg);
        solidAcrylic = new G4UnionSolid("sAcrylic",
                                        solidBall,
                                        solidAcrylicCircle,
                                        0,
                                        G4ThreeVector());
    } else {
        solidAcrylic = solidBall;
    }
    logicAcrylic = new G4LogicalVolume(solidAcrylic, 
                                       Acrylic, 
                                       "lAcrylic",
                                       0,
                                       0,
                                       0);
    G4VisAttributes* acrylic_visatt = new G4VisAttributes(G4Colour(0, 0, 1.0));
    // acrylic_visatt -> SetForceWireframe(true);  
    // acrylic_visatt -> SetForceAuxEdgeVisible(true);
    acrylic_visatt -> SetForceSolid(true);
    // acrylic_visatt -> SetForceLineSegmentsPerCircle(8);
    logicAcrylic -> SetVisAttributes(acrylic_visatt);

}
void 
PrototypeConstruction::makeAcrylicWithChimneyLogical() {
    RoundBottomFlaskSolidMaker rbfs("sAcrylicBall",
                                    m_acrylic_ball_radius,
                                    m_acrylic_chimney_radius,
                                    m_ChimneyTopToCenter,
                                    TOPTOEQUATORH);
    G4VSolid* solidBall= rbfs.getSolid();
    if (m_use_equator_circle) {
        // construct the circle 
        G4Tubs* solidAcrylicCircle = new G4Tubs("sAcrylicCircle",
                                                0*m,
                                                m_acrylic_circle_radius,
                                                m_acrylic_circle_height/2,
                                                0.*deg,
                                                360.*deg);
        solidAcrylic = new G4UnionSolid("sAcrylic",
                                        solidBall,
                                        solidAcrylicCircle,
                                        0,
                                        G4ThreeVector());
    } else {
        solidAcrylic = solidBall;
    }
    logicAcrylic = new G4LogicalVolume(solidAcrylic, 
                                       Acrylic, 
                                       "lAcrylic",
                                       0,
                                       0,
                                       0);
    G4VisAttributes* acrylic_visatt = new G4VisAttributes(G4Colour(0, 0, 1.0));
    // acrylic_visatt -> SetForceWireframe(true);  
    // acrylic_visatt -> SetForceAuxEdgeVisible(true);
    acrylic_visatt -> SetForceSolid(true);
    // acrylic_visatt -> SetForceLineSegmentsPerCircle(8);
    logicAcrylic -> SetVisAttributes(acrylic_visatt);

}

void 
PrototypeConstruction::makeAcrylicPhysical() {
    physiAcrylic = new G4PVPlacement(0,              // no rotation
                                     G4ThreeVector(0,0,0), // at (x,y,z)
                                     logicAcrylic,    // its logical volume 
                                     "pAcrylic",       // its name
                                     logicBuffer,  // its mother  volume
                                     false,           // no boolean operations
                                     0);              // no particular field

}

// ++++ LS
void 
PrototypeConstruction::makeLSLogical()
{
    solidTarget = new G4Sphere("sTarget", 
                               0*m, 
                               m_target_radius, 
                               0.*deg, 
                               360.*deg, 
                               0.*deg, 
                               180.*deg);
    logicTarget = new G4LogicalVolume(solidTarget, 
                                      LS, 
                                      "lTarget",
                                      0,
                                      0,
                                      0);

}
void 
PrototypeConstruction::makeLSWithChimneyLogical()
{
    RoundBottomFlaskSolidMaker rbfs("sTarget",
                                    m_target_radius,
                                    m_target_chimney_radius,
                                    m_ChimneyTopToCenter,
                                    TOPTOEQUATORH);
    solidTarget= rbfs.getSolid();
    logicTarget = new G4LogicalVolume(solidTarget, 
                                      LS, 
                                      "lTarget",
                                      0,
                                      0,
                                      0);

}
void 
PrototypeConstruction::makeLSPhysical() {
    physiTarget = new G4PVPlacement(0,              // no rotation
                                    G4ThreeVector(0,0,0), // at (x,y,z)
                                    logicTarget,    // its logical volume 
                                    "pTarget",       // its name
                                    logicAcrylic,  // its mother  volume
                                    false,           // no boolean operations
                                    0);              // no particular field

}
