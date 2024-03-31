#include "XJanchorConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;

DECLARE_TOOL(XJanchorConstruction);

XJanchorConstruction::XJanchorConstruction(const std::string& name)
    : ToolBase(name)
{
    logicXJanchor = 0;
    initVariables();

}

XJanchorConstruction::~XJanchorConstruction() {

}

G4LogicalVolume*
XJanchorConstruction::getLV() {
    if (logicXJanchor){ return logicXJanchor;}
    initMaterials();

    makeXJanchorLogical();

    return logicXJanchor;
}


bool 
XJanchorConstruction::inject(std::string  motherName , IDetElement* other, IDetElementPos* pos) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if(motherName == "lXJanchor")
    {
        mothervol = logicXJanchor;
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
    G4cout<< " XJanchor_Number = "<< copyno <<G4endl;

    return true;
}
void XJanchorConstruction::initVariables()
{;}

void XJanchorConstruction::initMaterials(){
    Acrylic = G4Material::GetMaterial("Acrylic", false);
}


void XJanchorConstruction::makeXJanchorLogical() {

    //solidXJanchor_up   = new G4Cons("solidXJanchor_up", 0.*mm, 31.7*mm, 0.*mm, 25*mm, 13./2*mm, 0.*deg, 360.0*deg);
    solidXJanchor_up   = new G4Tubs("solidXJanchor_up", 0.*mm, 25.*mm, 13./2*mm, 0.*deg, 360.*deg);
    solidXJanchor_down = new G4Cons("solidXJanchor_down", 0.*mm, 47.*mm, 0.*mm, 73.*mm, 10.*mm, 0.*deg, 360.*deg);   // to subtract the ball
    //solidXJanchor_down = new G4Cons("solidXJanchor_down", 0.*mm, 47.*mm, 0.*mm, 60.*mm, 5.*mm, 0.*deg, 360.*deg); // original size
    solidXJanchor_ball = new G4Sphere("solidXJanchor_ball", 0.*mm, 17820.*mm, 0.*deg, 360.*deg, 0.*deg, 180.*deg); 
    G4SubtractionSolid* solidXJanchor_sub = new G4SubtractionSolid("solidXJanchor_sub",solidXJanchor_down, solidXJanchor_ball, 0, G4ThreeVector(0.*mm, 0*mm,  17820.*mm));
    G4UnionSolid* solidXJanchor = new G4UnionSolid("solidXJanchor",solidXJanchor_sub, solidXJanchor_up, 0, G4ThreeVector(0.*mm, 0*mm,-16.5*mm));
    logicXJanchor = new G4LogicalVolume(
		solidXJanchor,
		Acrylic,
		"lXJanchor",
  		0,	
		0,
		0);
  G4VisAttributes* XJanchor_visatt = new G4VisAttributes(G4Colour(0.0, 0, 1.0));
  XJanchor_visatt -> SetForceWireframe(true);  
  XJanchor_visatt -> SetForceAuxEdgeVisible(true);
  XJanchor_visatt -> SetForceSolid(true);
  logicXJanchor -> SetVisAttributes(XJanchor_visatt);

}



