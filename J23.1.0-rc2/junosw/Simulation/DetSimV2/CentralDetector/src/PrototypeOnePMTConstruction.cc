#include "PrototypeOnePMTConstruction.hh"
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
DECLARE_TOOL(PrototypeOnePMTConstruction);

PrototypeOnePMTConstruction::PrototypeOnePMTConstruction(const std::string& name)
    : ToolBase(name)
{
    initVariables();
    declProp("BufferMaterial", m_buffer_material="Water");
}

PrototypeOnePMTConstruction::~PrototypeOnePMTConstruction() {

}

G4LogicalVolume*
PrototypeOnePMTConstruction::getLV() {
    initMaterials();

    makeSteelTubeLogical();

    makeBufferLogical();
    makeBufferPhysical();

    return logicSteelTube;
}

bool
PrototypeOnePMTConstruction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
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
PrototypeOnePMTConstruction::initVariables() {
    m_steel_tube_height = 2*m;
    m_steel_tube_radius = 1*m;
    m_steel_tube_thickness = 3*mm;

    m_buffer_tube_height = m_steel_tube_height - 2*m_steel_tube_thickness;
    m_buffer_tube_radius = m_steel_tube_radius - m_steel_tube_thickness;

}

void 
PrototypeOnePMTConstruction::initMaterials() {
    LS = G4Material::GetMaterial("LS");
    LAB = G4Material::GetMaterial("LAB");
    Water = G4Material::GetMaterial("Water");
    Steel = G4Material::GetMaterial("Steel");
    Acrylic = G4Material::GetMaterial("Acrylic");
    Air = G4Material::GetMaterial("Air");
    Oil = G4Material::GetMaterial("MineralOil");
}

void
PrototypeOnePMTConstruction::makeSteelTubeLogical() {

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
PrototypeOnePMTConstruction::makeBufferLogical() {
    solidBuffer = new G4Tubs("sBuffer",
                                0*m,
                                m_buffer_tube_radius,
                                m_buffer_tube_height/2,
                                0.*deg,
                                360*deg);
    G4Material* buffermat = 0;
    if (m_buffer_material == "Water") {
        buffermat = Water;
    } else if (m_buffer_material == "Air") {
        buffermat = Air;
    } else if (m_buffer_material == "Oil") {
        buffermat = Oil;
    } else if (m_buffer_material == "LAB") {
        buffermat = LAB;
    } else {
        G4cout << "Unknown material " << m_buffer_material << G4endl;
    }

    assert(buffermat);
    logicBuffer = new G4LogicalVolume(solidBuffer,
                                         buffermat,
                                         "lBuffer",
                                         0,
                                         0,
                                         0);

}
void 
PrototypeOnePMTConstruction::makeBufferPhysical() {
    physiBuffer = new G4PVPlacement(0,              // no rotation
                                     G4ThreeVector(0,0,0), // at (x,y,z)
                                     logicBuffer,    // its logical volume 
                                     "pBuffer",       // its name
                                     logicSteelTube,  // its mother  volume
                                     false,           // no boolean operations
                                     0);              // no particular field

}

