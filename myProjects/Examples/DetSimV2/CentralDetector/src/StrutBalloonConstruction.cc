
#include "StrutBalloonConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(StrutBalloonConstruction);

StrutBalloonConstruction::StrutBalloonConstruction(const std::string& name)
    : ToolBase(name)
{
    logicStrut = 0;
    initVariables();

}

StrutBalloonConstruction::~StrutBalloonConstruction() {

}

G4LogicalVolume* 
StrutBalloonConstruction::getLV() {
    if (logicStrut) {
        return logicStrut;
    }
    initMaterials();

    makeStrutLogical();

    return logicStrut;
}

bool 
StrutBalloonConstruction::inject(std::string /* motherName */, IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
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

    G4cout<< " Strut_Strut_Number = "<< copyno <<G4endl;

    return true;
}

   

void
StrutBalloonConstruction::initVariables() {

  m_radStrut_in  = 0 *mm;
  m_radStrut_out = 30. *mm;
  m_lengthStrut  = 1400./2.*mm;

  gap = 1 *mm;
  strut_pos = 17820. *mm + 165. *mm + m_lengthStrut;

}

void 
StrutBalloonConstruction::initMaterials() {
    Acrylic = G4Material::GetMaterial("Acrylic");
}

void
StrutBalloonConstruction::makeStrutLogical() {
        solidStrut = new G4Tubs(
                        "sStrut",
                        0,
                        m_radStrut_out,  
                        m_lengthStrut,  
                        0*deg, 
                        360*deg);


        logicStrut = new G4LogicalVolume(
                        solidStrut, 
                        Acrylic, 
                        "lStrut",
                        0,
                        0,
                        0);
        G4cout<< "m_radStrut = "<<m_radStrut_out<<G4endl;
        G4VisAttributes* strut_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0));
        //strut_visatt -> SetForceWireframe(true);  
        //strut_visatt -> SetForceAuxEdgeVisible(true);
        strut_visatt -> SetForceSolid(true);
        strut_visatt -> SetForceLineSegmentsPerCircle(4);
        logicStrut -> SetVisAttributes(strut_visatt);
}

