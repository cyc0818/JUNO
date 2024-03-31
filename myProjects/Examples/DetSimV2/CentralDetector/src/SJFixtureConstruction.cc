#include "SJFixtureConstruction.hh"
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

DECLARE_TOOL(SJFixtureConstruction);

SJFixtureConstruction::SJFixtureConstruction(const std::string& name)
    : ToolBase(name)
{
    logicSJFixture = 0;
    initVariables();

}

SJFixtureConstruction::~SJFixtureConstruction() {

}

G4LogicalVolume*
SJFixtureConstruction::getLV() {
    if (logicSJFixture){ return logicSJFixture;}
    initMaterials();

    makeSJFixtureLogical();

    return logicSJFixture;
}


bool 
SJFixtureConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lSJFixture")
    {
        mothervol = logicSJFixture;
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
    G4cout<< " SJFixture_Number = "<< copyno <<G4endl;

    return true;
}
void SJFixtureConstruction::initVariables()
{;}

void SJFixtureConstruction::initMaterials(){
    Acrylic = G4Material::GetMaterial("Acrylic", false);
}


void SJFixtureConstruction::makeSJFixtureLogical() {

    //solidSJFixture_up   = new G4Cons("solidSJFixture_up", 0.*mm, 31.7*mm, 0.*mm, 25*mm, 13./2*mm, 0.*deg, 360.0*deg);
    solidSJFixture_up   = new G4Box("solidSJFixture_up", 20.*mm, 7.*mm, 13/2.*mm);
    solidSJFixture_down = new G4Cons("solidSJFixture_down", 0.*mm, 30.*mm, 0.*mm, 20.*mm, 10.*mm, 0.*deg, 360.*deg);   // to subtract the ball
    //solidSJFixture_down = new G4Cons("solidSJFixture_down", 0.*mm, 47.*mm, 0.*mm, 60.*mm, 5.*mm, 0.*deg, 360.*deg); // original size
    solidSJFixture_box = new G4Box("solidSJFixture_box", 17780.*mm, 17780.*mm, 17780.*mm);
    solidSJFixture_ball = new G4Sphere("solidSJFixture_ball", 0.*mm, 17700.*mm, 0.*deg, 360.*deg, 0.*deg, 180.*deg); 
    G4SubtractionSolid* solidSphere_sub = new G4SubtractionSolid("solidSphere_sub", solidSJFixture_box, solidSJFixture_ball);
    G4SubtractionSolid* solidSJFixture_sub = new G4SubtractionSolid("solidSJFixture_sub",solidSJFixture_down, solidSJFixture_ball, 0, G4ThreeVector(0.*mm, 0*mm,  17699.975*mm));
    G4UnionSolid* solidSJFixture = new G4UnionSolid("solidSJFixture",solidSJFixture_sub, solidSJFixture_up, 0, G4ThreeVector(0.*mm, 0*mm,16.5*mm));
    logicSJFixture = new G4LogicalVolume(
		solidSJFixture,
		Acrylic,
		"lSJFixture",
  		0,	
		0,
		0);
  G4VisAttributes* SJFixture_visatt = new G4VisAttributes(G4Colour(0.0, 0, 1.0));
  SJFixture_visatt -> SetForceWireframe(true);  
  SJFixture_visatt -> SetForceAuxEdgeVisible(true);
  SJFixture_visatt -> SetForceSolid(true);
  logicSJFixture -> SetVisAttributes(SJFixture_visatt);

}



