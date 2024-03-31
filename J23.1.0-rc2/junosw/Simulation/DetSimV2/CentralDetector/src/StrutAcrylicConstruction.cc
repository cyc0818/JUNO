
#include "StrutAcrylicConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(StrutAcrylicConstruction);

StrutAcrylicConstruction::StrutAcrylicConstruction(const std::string& name)
    : ToolBase(name)
{
    declProp("UseRealSurface", m_useRealSurface=true);
    declProp("UseStrutOpSurface", m_useStrutSurface=true);
    logicStrut = 0;
    initVariables();
}

StrutAcrylicConstruction::~StrutAcrylicConstruction() {

}

G4LogicalVolume* 
StrutAcrylicConstruction::getLV() {
    if (logicStrut) {
        return logicStrut;
    }
    initVariables();
    initMaterials();

    makeStrutLogical();
    if (m_useRealSurface and m_useStrutSurface) {
        makeStrutOpSurface();
    }

    return logicStrut;
}

bool 
StrutAcrylicConstruction::inject(std::string /* motherName */, IDetElement* other, IDetElementPos* pos) {
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
StrutAcrylicConstruction::initVariables() {

 /* m_radStrut_in  = (102./2.-12.) *mm;
  m_radStrut_out = 102./2. *mm;
  m_lengthStrut  = 1200/2.*mm;
  gap = 1 *mm;
  strut_pos = 17820. *mm + 165. *mm + m_lengthStrut;
  */

    LogInfo << "m_useRealSurface is " << m_useRealSurface << std::endl;

    m_radStrut_in  = 0*mm;
    m_radStrut_out = 85./2. *mm;
    m_lengthStrut  = 1807.6*mm;
   
    m_radLS = 17.7*m;
    m_radAcrylic = m_radLS + 124*mm;
    m_anchorHeight = 256.9 *mm;


    if (m_useRealSurface) {
        // reduce the length
        const double radInnerWaterRealSurface = 19.629*m;
        double m_lengthStrut_old = m_lengthStrut;
        // m_lengthStrut = radInnerWaterRealSurface - (17820. *mm + 256.9 *mm);
        m_lengthStrut = sqrt(radInnerWaterRealSurface*radInnerWaterRealSurface
                             - m_radStrut_out*m_radStrut_out)
                        - (m_radAcrylic + m_anchorHeight);

        if (m_lengthStrut>m_lengthStrut_old) {
            m_lengthStrut = m_lengthStrut_old;
        }

        LogInfo << "Option RealSurface is enabled in Central Detector. "
                << " Reduce m_lengthStrut from " << m_lengthStrut_old
                << " to: " << m_lengthStrut
                << std::endl;
    }


    gap = 1 *mm;
    strut_pos = m_radAcrylic + m_anchorHeight + m_lengthStrut/2;

    
    /*                   _______  ----------------------------------------------
                         |      |                              ^
                         |      |                              |
                         |      |                              | 
                         |      |
                         |      | 
                         |      |
                         |      |
                         |      | 
          strut_pos      | *    |
                         |      |                            m_lengthStrut
                         |      | 
                         |      |
                         |      |
                         |      |                               | 
                         |      |                               |
                         |      |                               |
                         |      |                               |
                         |______|                               v
	                   - - -             --------------------------
               _ _ _     /       \    _ _ _        |            ^
              |      |	/         \  |     |    m_rad=50*mm     |
              |      |	|    *    |  |     |   -----------      | 
              |      |	 \       /   |     |        m_anchorHeight = 256.9*mm
              |      |	   -----     |     |                    |
              |      -----------------     |                    |
              |                            |                    |
              |____________________________|                    v
      --------------------------------------------------------------------
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
StrutAcrylicConstruction::initMaterials() {
    Steel = G4Material::GetMaterial("StrutSteel");
}

void
StrutAcrylicConstruction::makeStrutLogical() {
        solidStrut = new G4Tubs(
                        "sStrut",
                        m_radStrut_in,
                        m_radStrut_out,  
                        m_lengthStrut/2,  
                        0*deg, 
                        360*deg);


        logicStrut = new G4LogicalVolume(
                        solidStrut, 
                        Steel, 
                        "lSteel",
                        0,
                        0,
                        0);
        G4cout<< "m_radStrut = "<<m_radStrut_out<<G4endl;
        G4VisAttributes* strut_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0));
        //strut_visatt -> SetForceWireframe(true);  
        //strut_visatt -> SetForceAuxEdgeVisible(true);
        strut_visatt -> SetForceSolid(true);
        // strut_visatt -> SetForceLineSegmentsPerCircle(4);
        logicStrut -> SetVisAttributes(strut_visatt);
}

void
StrutAcrylicConstruction::makeStrutOpSurface() {
    G4OpticalSurface *strut_optical_surface = new G4OpticalSurface("opStrutAcrylic");
    strut_optical_surface->SetMaterialPropertiesTable(Steel->GetMaterialPropertiesTable());
    strut_optical_surface->SetModel(unified);
    strut_optical_surface->SetType(dielectric_metal);
    strut_optical_surface->SetFinish(ground);
    strut_optical_surface->SetSigmaAlpha(0.2);

    new G4LogicalSkinSurface("StrutAcrylicOpSurface", logicStrut, strut_optical_surface);
}



double StrutAcrylicConstruction::geom_info(const std::string& param) {
    if (param == "PosR") {
        return strut_pos;
    }
    return 0.0;
}
