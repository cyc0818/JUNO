
#include "StrutBallheadAcrylicConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4VisAttributes.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(StrutBallheadAcrylicConstruction);

StrutBallheadAcrylicConstruction::StrutBallheadAcrylicConstruction(const std::string& name)
    : ToolBase(name)
{
    logicStrut = 0;
    initVariables();

}

StrutBallheadAcrylicConstruction::~StrutBallheadAcrylicConstruction() {

}

G4LogicalVolume* 
StrutBallheadAcrylicConstruction::getLV() {
    if (logicStrut) {
        return logicStrut;
    }
    initMaterials();

    makeStrutLogical();

    return logicStrut;
}

bool 
StrutBallheadAcrylicConstruction::inject(std::string /* motherName */, IDetElement* other, IDetElementPos* pos) {
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

    G4cout<< " Strut_Acrylic_Number = "<< copyno <<G4endl;
    
    return true;
}

   

void
StrutBallheadAcrylicConstruction::initVariables() {

 /* m_radStrut_in  = (102./2.-12.) *mm;
  m_radStrut_out = 102./2. *mm;
  m_lengthStrut  = 1200/2.*mm;
  gap = 1 *mm;
  strut_pos = 17820. *mm + 165. *mm + m_lengthStrut;
  */
    m_rad = 50*mm;
    gap = 1 *mm;
    
    m_radLS = 17.7*m;
    m_radAcrylic = m_radLS + 124*mm;
    m_anchorHeight = 256.9 *mm;

    strut_pos = m_radAcrylic + m_anchorHeight - m_rad ;
    /* 
	                   - - -     --------------------------
        	         /       \      |             ^
                	/         \   m_rad=50*mm     |
                	|    *    |  -----------      | 
                 	 \       /                m_anchorHeight = 256.9*mm
                   	   -----                      |
                                                      |
                                                      |
                                                      v
      ----------------------------------------------------
                                                      ^
                                                      |
                                                      |
                                                   m_radAcrylic = 17.7*m + 124*mm
                                                      |
                                                      | 
                                                      v 
    */
}

void 
StrutBallheadAcrylicConstruction::initMaterials() {
    Steel = G4Material::GetMaterial("Steel");
}

void
StrutBallheadAcrylicConstruction::makeStrutLogical() {
        solidStrut  =new  G4Orb("sStrutBallhead",
                                m_rad);


        logicStrut = new G4LogicalVolume(
                        solidStrut, 
                        Steel, 
                        "lSteel",
                        0,
                        0,
                        0);
        G4cout<< "m_rad = "<<m_rad<<G4endl;
        G4VisAttributes* strut_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0));
        //strut_visatt -> SetForceWireframe(true);  
        //strut_visatt -> SetForceAuxEdgeVisible(true);
        strut_visatt -> SetForceSolid(true);
        // strut_visatt -> SetForceLineSegmentsPerCircle(4);
        logicStrut -> SetVisAttributes(strut_visatt);
}

double StrutBallheadAcrylicConstruction::geom_info(const std::string& param) {
    if (param == "PosR") {
        return strut_pos;
    }
    return 0.0;
}
