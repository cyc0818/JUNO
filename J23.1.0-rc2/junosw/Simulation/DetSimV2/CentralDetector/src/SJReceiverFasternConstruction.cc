#include "SJReceiverFasternConstruction.hh"
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

DECLARE_TOOL(SJReceiverFasternConstruction);


SJReceiverFasternConstruction::SJReceiverFasternConstruction(const std::string& name)
    : ToolBase(name)
{
    logicSJReceiverFastern = 0;
    initVariables();

}

SJReceiverFasternConstruction::~SJReceiverFasternConstruction() {

}

G4LogicalVolume*
SJReceiverFasternConstruction::getLV() {
    if (logicSJReceiverFastern){ return logicSJReceiverFastern;}
    initMaterials();

    makeSJReceiverFasternLogical();

    return logicSJReceiverFastern;
}


bool 
SJReceiverFasternConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lSJReceiverFastern")
    {
        mothervol = logicSJReceiverFastern;
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
    G4cout<< " SJReceiverFastern_Number = "<< copyno <<G4endl;

    return true;
}

void SJReceiverFasternConstruction::initVariables(){
     G4double m_radLS = 17.7*m ;
     m_sjreceiverfastern_pos = m_radLS - 10 *mm - 13./2*mm -0.102*mm ;
}

void SJReceiverFasternConstruction::initMaterials(){
    PE_PA = G4Material::GetMaterial("PE_PA");
}

void SJReceiverFasternConstruction::makeSJReceiverFasternLogical(){

// fixture part
    solidSJReceiverFastern_down1 = new G4Tubs("solidSJReceiverFastern_down1", 25.*mm, 45.*mm, 13./2*mm, 0.*deg, 360.*deg);
    solidSJReceiverFastern_down2 = new G4Box("solidSJReceiverFastern_down2", 10.*mm, 11.5*mm, 13/2.*mm);
    solidSJReceiverFastern_down_uni1 = new G4UnionSolid("solidSJReceiverFastern_down_uni1", solidSJReceiverFastern_down1, solidSJReceiverFastern_down2, 0, G4ThreeVector(52.*mm, 0.*mm, 0.*mm));
    solidSJReceiverFastern_down_uni2 = new G4UnionSolid("solidSJReceiverFastern_down_uni2", solidSJReceiverFastern_down_uni1, solidSJReceiverFastern_down2, 0, G4ThreeVector(-52.*mm, 0.*mm, 0.*mm));
    solidSJReceiverFastern_down3 = new G4Box("solidSJReceiverFastern_down3", 15.*mm, 15.*mm, 13/2.*mm);
    solidSJReceiverFastern_down_uni3 = new G4UnionSolid("solidSJReceiverFastern_down_uni3", solidSJReceiverFastern_down_uni2, solidSJReceiverFastern_down3, 0, G4ThreeVector(0.*mm, 50.*mm, 0.*mm));
    solidSJReceiverFastern_down_uni4 = new G4UnionSolid("solidSJReceiverFastern_down_uni4", solidSJReceiverFastern_down_uni3, solidSJReceiverFastern_down3, 0, G4ThreeVector(0.*mm, -50.*mm, 0.*mm));

// cover part
    solidSJReceiverFastern_up1 = new G4Box("solidSJReceiverFastern_up1", 15.*mm, 40.*mm, 17/2.*mm);    
    solidSJReceiverFastern_up2 = new G4Box("solidSJReceiverFastern_up2", 15.*mm, 65*mm, 5.*mm);
    solidSJReceiverFastern_up_uni = new G4UnionSolid("solidSJReceiverFastern_up_uni", solidSJReceiverFastern_up2, solidSJReceiverFastern_up1, 0, G4ThreeVector(0.*mm, 0.*mm, 13.5*mm));

// union 
    solidSJReceiverFastern = new G4UnionSolid("solidSJReceiverFastern", solidSJReceiverFastern_down_uni4, solidSJReceiverFastern_up_uni, 0, G4ThreeVector(0.*mm, 0.*mm, 11.5*mm));

    logicSJReceiverFastern = new G4LogicalVolume(
		solidSJReceiverFastern,
		//Acrylic,
		PE_PA,
        "lSJReceiverFastern",
		0,
		0,
		0);

  G4VisAttributes* SJReceiverFastern_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  SJReceiverFastern_visatt -> SetForceWireframe(true);  
  SJReceiverFastern_visatt -> SetForceSolid(true);
  SJReceiverFastern_visatt -> SetForceAuxEdgeVisible(true);
  logicSJReceiverFastern -> SetVisAttributes(SJReceiverFastern_visatt);
}
double SJReceiverFasternConstruction::geom_info(const std::string& param) {
    if (param == "PosR") {
        return m_sjreceiverfastern_pos;
    }
    return 0.0;
}   
