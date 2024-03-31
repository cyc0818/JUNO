#include "XJfixtureConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;

DECLARE_TOOL(XJfixtureConstruction);


XJfixtureConstruction::XJfixtureConstruction(const std::string& name)
    : ToolBase(name)
{
    logicXJfixture = 0;
    initVariables();

}

XJfixtureConstruction::~XJfixtureConstruction() {

}

G4LogicalVolume*
XJfixtureConstruction::getLV() {
    if (logicXJfixture){ return logicXJfixture;}
    initMaterials();

    makeXJfixtureLogical();

    return logicXJfixture;
}


bool 
XJfixtureConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lXJfixture")
    {
        mothervol = logicXJfixture;
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
    G4cout<< " XJfixture_Number = "<< copyno <<G4endl;

    return true;
}

void XJfixtureConstruction::initVariables(){;}

void XJfixtureConstruction::initMaterials(){
    PE_PA = G4Material::GetMaterial("PE_PA");
}

void XJfixtureConstruction::makeXJfixtureLogical(){

// fixture part
    solidXJfixture_down1 = new G4Tubs("solidXJfixture_down1", 25.*mm, 45.*mm, 13./2*mm, 0.*deg, 360.*deg);
    solidXJfixture_down2 = new G4Box("solidXJfixture_down2", 10.*mm, 11.5*mm, 13/2.*mm);
    solidXJfixture_down_uni1 = new G4UnionSolid("solidXJfixture_down_uni1", solidXJfixture_down1, solidXJfixture_down2, 0, G4ThreeVector(52.*mm, 0.*mm, 0.*mm));
    solidXJfixture_down_uni2 = new G4UnionSolid("solidXJfixture_down_uni2", solidXJfixture_down_uni1, solidXJfixture_down2, 0, G4ThreeVector(-52.*mm, 0.*mm, 0.*mm));
    solidXJfixture_down3 = new G4Box("solidXJfixture_down3", 15.*mm, 15.*mm, 13/2.*mm);
    solidXJfixture_down_uni3 = new G4UnionSolid("solidXJfixture_down_uni3", solidXJfixture_down_uni2, solidXJfixture_down3, 0, G4ThreeVector(0.*mm, 50.*mm, 0.*mm));
    solidXJfixture_down_uni4 = new G4UnionSolid("solidXJfixture_down_uni4", solidXJfixture_down_uni3, solidXJfixture_down3, 0, G4ThreeVector(0.*mm, -50.*mm, 0.*mm));

// cover part
    solidXJfixture_up1 = new G4Box("solidXJfixture_up1", 15.*mm, 40.*mm, 17/2.*mm);    
    solidXJfixture_up2 = new G4Box("solidXJfixture_up2", 15.*mm, 65*mm, 5.*mm);
    solidXJfixture_up_uni = new G4UnionSolid("solidXJfixture_up_uni", solidXJfixture_up1, solidXJfixture_up2, 0, G4ThreeVector(0.*mm, 0.*mm, 13.5*mm));

// union 
    solidXJfixture = new G4UnionSolid("solidXJfixture", solidXJfixture_down_uni4, solidXJfixture_up_uni, 0, G4ThreeVector(0.*mm, 0.*mm, -25.*mm));

    logicXJfixture = new G4LogicalVolume(
		solidXJfixture,
		//Acrylic,
		PE_PA,
        "lXJfixture",
		0,
		0,
		0);

  G4VisAttributes* XJfixture_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  XJfixture_visatt -> SetForceWireframe(true);  
  XJfixture_visatt -> SetForceSolid(true);
  XJfixture_visatt -> SetForceAuxEdgeVisible(true);
  logicXJfixture -> SetVisAttributes(XJfixture_visatt);



}

