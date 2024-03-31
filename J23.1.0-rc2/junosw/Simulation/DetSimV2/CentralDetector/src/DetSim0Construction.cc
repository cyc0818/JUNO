
#include "DetSim0Construction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(DetSim0Construction);

DetSim0Construction::DetSim0Construction(const std::string& name)
    : ToolBase(name) 
{
    logicSteelBall = 0;
}

DetSim0Construction::~DetSim0Construction() {

}

G4LogicalVolume* 
DetSim0Construction::getLV() {
    if (logicSteelBall) {
        return logicSteelBall;
    }
    initMaterials();
    initVariables();
    makeSteelBallLogical();

    makeLSLogical();
    makeLSPhysical();

    return logicSteelBall;
}

bool 
DetSim0Construction::inject(std::string motherName, IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if ( motherName == "lTarget" ) {
        mothervol = logicTarget;
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

    int copyno = 0;
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
DetSim0Construction::initVariables() {

    m_radLS = 19*m;
    m_steelBallThickness = 10*cm;
    m_steelBallRad = m_radLS + m_steelBallThickness;
}

void 
DetSim0Construction::initMaterials() {
    Steel = G4Material::GetMaterial("Steel");
    LS = G4Material::GetMaterial("LS");
}

// Steel Ball
//
void
DetSim0Construction::makeSteelBallLogical()
{
  solidSteelBall = new G4Sphere("sSteelBall", 
                                0*m, 
                                m_steelBallRad,
                                0.*deg,
                                360.*deg, 
                                0.*deg,
                                180.*deg);
  logicSteelBall = new G4LogicalVolume(solidSteelBall, 
                                       Steel, 
                                       "lSteelBall",
                                       0,
                                       0,
                                       0);
}

// LS
void
DetSim0Construction::makeLSLogical()
{
  solidTarget = new G4Sphere("sTarget", 
                             0*m, 
                             m_radLS, 
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
DetSim0Construction::makeLSPhysical()
{
  physiTarget = new G4PVPlacement(0,              // no rotation
                                  G4ThreeVector(0,0,0), // at (x,y,z)
                                  logicTarget,    // its logical volume 
                                  "pTarget",       // its name
                                  logicSteelBall,  // its mother  volume
                                  false,           // no boolean operations
                                  0);              // no particular field
}
