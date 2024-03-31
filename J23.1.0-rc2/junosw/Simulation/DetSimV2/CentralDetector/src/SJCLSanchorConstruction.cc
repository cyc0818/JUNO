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
    m_simplify_calib_anchor = getenv("JUNO_SIMPLIFY_CALIB_ANCHOR") == nullptr ? false : true ;    
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
{
    m_radLS = 17.7*m ;
    
    if(m_simplify_calib_anchor){
      m_sjclsanchor_pos = m_radLS - 5*mm - 0.102*mm ;
    }   
    else{
      m_sjclsanchor_pos = m_radLS - 0.062*mm; 
    }

}

void SJCLSanchorConstruction::initMaterials(){
    Acrylic = G4Material::GetMaterial("Acrylic", false);
}


void SJCLSanchorConstruction::makeSJCLSanchorLogical() {

    G4UnionSolid* solidSJCLSanchor = nullptr;

    if(m_simplify_calib_anchor){
    
      
    solidSJCLSanchor_up   = new G4Box("solidSJCLSanchor_up", 25.*mm, 20.*mm, 13/2.*mm);
    solidSJCLSanchor_down = new G4Cons("solidSJCLSanchor_down", 0.*mm, 60.*mm, 0.*mm, 47.*mm, 5.*mm, 0.*deg, 360.*deg);   // to subtract the ball
       
    solidSJCLSanchor = new G4UnionSolid("solidSJCLSanchor",solidSJCLSanchor_down, solidSJCLSanchor_up, 0, G4ThreeVector(0.*mm, 0*mm,11.5*mm));

    }
    else{

    solidSJCLSanchor_up   = new G4Box("solidSJCLSanchor_up", 25.*mm, 20.*mm, 13/2.*mm);
    solidSJCLSanchor_down = new G4Cons("solidSJCLSanchor_down", 0.*mm, 73.*mm, 0.*mm, 47.*mm, 10.*mm, 0.*deg, 360.*deg);   // to subtract the ball
    solidSJCLSanchor_box = new G4Box("solidSJCLSanchor_box", 17780.*mm, 17780.*mm, 17780.*mm);
    solidSJCLSanchor_ball = new G4Sphere("solidSJCLSanchor_ball", 0.*mm, m_radLS, 0.*deg, 360.*deg, 0.*deg, 180.*deg); 
    G4SubtractionSolid* solidSphere_sub = new G4SubtractionSolid("solidSphere_sub", solidSJCLSanchor_box, solidSJCLSanchor_ball);
    G4SubtractionSolid* solidSJCLSanchor_sub = new G4SubtractionSolid("solidSJCLSanchor_sub",solidSJCLSanchor_down, solidSphere_sub, 0, G4ThreeVector(0.*mm, 0*mm,  m_radLS - 0.062*mm ));
    solidSJCLSanchor = new G4UnionSolid("solidSJCLSanchor",solidSJCLSanchor_sub, solidSJCLSanchor_up, 0, G4ThreeVector(0.*mm, 0*mm,16.5*mm));
    
    }

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

double SJCLSanchorConstruction::geom_info(const std::string& param) {
    if (param == "PosR") {
        return m_sjclsanchor_pos;
    }
    return 0.0;
}






