#include "SJCLSanchorConstruction.hh"
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

DECLARE_TOOL(SJCLSanchorConstruction);

SJCLSanchorConstruction::SJCLSanchorConstruction(const std::string& name)
    : ToolBase(name)
{
    logicSJCLSanchor = 0;
    initVariables();

}

SJCLSanchorConstruction::~SJCLSanchorConstruction() {

}

G4LogicalVolume*
SJCLSanchorConstruction::getLV() {
    if (logicSJCLSanchor){ return logicSJCLSanchor;}
    initMaterials();

    makeSJCLSanchorLogical();

    return logicSJCLSanchor;
}


bool 
SJCLSanchorConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lSJCLSanchor")
    {
        mothervol = logicSJCLSanchor;
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
    G4cout<< " SJCLSanchor_Number = "<< copyno <<G4endl;

    return true;
}
void SJCLSanchorConstruction::initVariables()
{;}

void SJCLSanchorConstruction::initMaterials(){
    Acrylic = G4Material::GetMaterial("Acrylic", false);
}


void SJCLSanchorConstruction::makeSJCLSanchorLogical() {

    //solidSJCLSanchor_up   = new G4Cons("solidSJCLSanchor_up", 0.*mm, 31.7*mm, 0.*mm, 25*mm, 13./2*mm, 0.*deg, 360.0*deg);
    solidSJCLSanchor_up   = new G4Box("solidSJCLSanchor_up", 25.*mm, 20.*mm, 13/2.*mm);
    solidSJCLSanchor_down = new G4Cons("solidSJCLSanchor_down", 0.*mm, 73.*mm, 0.*mm, 47.*mm, 10.*mm, 0.*deg, 360.*deg);   // to subtract the ball
    //solidSJCLSanchor_down = new G4Cons("solidSJCLSanchor_down", 0.*mm, 47.*mm, 0.*mm, 60.*mm, 5.*mm, 0.*deg, 360.*deg); // original size
    solidSJCLSanchor_box = new G4Box("solidSJCLSanchor_box", 17780.*mm, 17780.*mm, 17780.*mm);
    solidSJCLSanchor_ball = new G4Sphere("solidSJCLSanchor_ball", 0.*mm, 17700.*mm, 0.*deg, 360.*deg, 0.*deg, 180.*deg); 
    G4SubtractionSolid* solidSphere_sub = new G4SubtractionSolid("solidSphere_sub", solidSJCLSanchor_box, solidSJCLSanchor_ball);
    G4SubtractionSolid* solidSJCLSanchor_sub = new G4SubtractionSolid("solidSJCLSanchor_sub",solidSJCLSanchor_down, solidSphere_sub, 0, G4ThreeVector(0.*mm, 0*mm,  17699.938*mm));
    G4UnionSolid* solidSJCLSanchor = new G4UnionSolid("solidSJCLSanchor",solidSJCLSanchor_sub, solidSJCLSanchor_up, 0, G4ThreeVector(0.*mm, 0*mm,16.5*mm));
    logicSJCLSanchor = new G4LogicalVolume(
		solidSJCLSanchor,
		Acrylic,
		"lSJCLSanchor",
  		0,	
		0,
		0);
  G4VisAttributes* SJCLSanchor_visatt = new G4VisAttributes(G4Colour(0.0, 0, 1.0));
  SJCLSanchor_visatt -> SetForceWireframe(true);  
  SJCLSanchor_visatt -> SetForceAuxEdgeVisible(true);
  SJCLSanchor_visatt -> SetForceSolid(true);
  logicSJCLSanchor -> SetVisAttributes(SJCLSanchor_visatt);

}








