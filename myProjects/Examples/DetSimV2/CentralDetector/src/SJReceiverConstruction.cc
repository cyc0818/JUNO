#include "SJReceiverConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;

DECLARE_TOOL(SJReceiverConstruction);

SJReceiverConstruction::SJReceiverConstruction(const std::string& name)
    : ToolBase(name)
{
    logicSJReceiver = 0;
    initVariables();

}

SJReceiverConstruction::~SJReceiverConstruction() {

}

G4LogicalVolume*
SJReceiverConstruction::getLV() {
    if (logicSJReceiver){ return logicSJReceiver;}
    initMaterials();

    makeSJReceiverLogical();

    return logicSJReceiver;
}


bool 
SJReceiverConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lSJReceiver")
    {
        mothervol = logicSJReceiver;
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
    G4cout<< " SJReceiver_Number = "<< copyno <<G4endl;

    return true;
}
void SJReceiverConstruction::initVariables()
{;}

void SJReceiverConstruction::initMaterials(){
    Acrylic = G4Material::GetMaterial("Acrylic", false);
}


void SJReceiverConstruction::makeSJReceiverLogical() {

    //solidSJReceiver_up   = new G4Cons("solidSJReceiver_up", 0.*mm, 31.7*mm, 0.*mm, 25*mm, 13./2*mm, 0.*deg, 360.0*deg);
    solidSJReceiver_up = new G4Tubs("solidXJanchor_up", 0.*mm, 25.*mm, 13./2*mm, 0.*deg, 360.*deg);
    solidSJReceiver_down = new G4Cons("solidSJReceiver_down", 0.*mm, 73.*mm, 0.*mm, 47.*mm, 10.*mm, 0.*deg, 360.*deg);   // to subtract the ball
    //solidSJReceiver_down = new G4Cons("solidSJReceiver_down", 0.*mm, 47.*mm, 0.*mm, 60.*mm, 5.*mm, 0.*deg, 360.*deg); // original size
    solidSJReceiver_box = new G4Box("solidSJReceiver_box", 17780.*mm, 17780.*mm, 17780.*mm);
    solidSJReceiver_ball = new G4Sphere("solidSJReceiver_ball", 0.*mm, 17700.*mm, 0.*deg, 360.*deg, 0.*deg, 180.*deg); 
    G4SubtractionSolid* solidSphere_sub = new G4SubtractionSolid("solidSphere_sub", solidSJReceiver_box, solidSJReceiver_ball);
    G4SubtractionSolid* solidSJReceiver_sub = new G4SubtractionSolid("solidSJReceiver_sub",solidSJReceiver_down, solidSJReceiver_ball, 0, G4ThreeVector(0.*mm, 0*mm,  17699.938*mm));
    G4UnionSolid* solidSJReceiver = new G4UnionSolid("solidSJReceiver",solidSJReceiver_sub, solidSJReceiver_up, 0, G4ThreeVector(0.*mm, 0*mm,16.5*mm));
    logicSJReceiver = new G4LogicalVolume(
		solidSJReceiver,
		Acrylic,
		"lSJReceiver",
  		0,	
		0,
		0);
  G4VisAttributes* SJReceiver_visatt = new G4VisAttributes(G4Colour(0.0, 0, 1.0));
  SJReceiver_visatt -> SetForceWireframe(true);  
  SJReceiver_visatt -> SetForceAuxEdgeVisible(true);
  SJReceiver_visatt -> SetForceSolid(true);
  logicSJReceiver -> SetVisAttributes(SJReceiver_visatt);

}



