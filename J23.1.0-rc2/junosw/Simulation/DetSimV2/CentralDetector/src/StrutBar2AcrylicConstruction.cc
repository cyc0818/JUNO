
#include "StrutBar2AcrylicConstruction.hh"
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
DECLARE_TOOL(StrutBar2AcrylicConstruction);

StrutBar2AcrylicConstruction::StrutBar2AcrylicConstruction(const std::string& name)
    : ToolBase(name)
{
    declProp("UseRealSurface", m_useRealSurface=true);
    declProp("UseStrutOpSurface", m_useStrutSurface=true);
    logicStrut = 0;
    initVariables();
}

StrutBar2AcrylicConstruction::~StrutBar2AcrylicConstruction() {

}

G4LogicalVolume* 
StrutBar2AcrylicConstruction::getLV() {
    if (logicStrut) {
        return logicStrut;
    }
    initVariables();
    initMaterials();

    makeStrutLogical();
    if (m_useStrutSurface) {
        makeStrutOpSurface();
    }

    return logicStrut;
}

bool 
StrutBar2AcrylicConstruction::inject(std::string /* motherName */, IDetElement* other, IDetElementPos* pos) {
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
StrutBar2AcrylicConstruction::initVariables() {

 /* m_radStrut_in  = (102./2.-12.) *mm;
  m_radStrut_out = 102./2. *mm;
  m_lengthStrut  = 1200/2.*mm;
  gap = 1 *mm;
  strut_pos = 17820. *mm + 165. *mm + m_lengthStrut;
  */
    m_radStrut_in  = 0*mm;
    m_radStrut_out = 60./2. *mm;
    m_lengthStrut  = 1913.6 *mm;
  
    m_radLS = 17.7*m;
    m_radAcrylic = m_radLS + 124*mm;
    m_anchorHeight = 256.9 *mm;


    if (m_useRealSurface) {
        // reduce the length
        const double radInnerWaterRealSurface = 19.629*m;
        double m_lengthStrut_old = m_lengthStrut;
        // avoid the overlapping, the length should consider the edge of the bar. 
        // m_lengthStrut = radInnerWaterRealSurface - (17820. *mm + 256.9 *mm);
        m_lengthStrut = sqrt(radInnerWaterRealSurface*radInnerWaterRealSurface
                             - m_radStrut_out*m_radStrut_out)
                        - (m_radAcrylic + m_anchorHeight);

        LogInfo << "Option RealSurface is enabled in Central Detector. "
                << " Reduce m_lengthStrut from " << m_lengthStrut_old
                << " to: " << m_lengthStrut
                << std::endl;
    }

    gap = 1 *mm;
    strut_pos = m_radAcrylic + m_anchorHeight + m_lengthStrut/2; // at center of strut


 
}

void 
StrutBar2AcrylicConstruction::initMaterials() {
    Steel = G4Material::GetMaterial("StrutSteel");
}

void
StrutBar2AcrylicConstruction::makeStrutLogical() {
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
                        "lSteel2",
                        0,
                        0,
                        0);
        G4cout<< "m_radStrut = "<<m_radStrut_out<<G4endl;
        G4VisAttributes* strut_visatt = new G4VisAttributes(G4Colour(0, 1.0, 1.0));
        //strut_visatt -> SetForceWireframe(true);  
        //strut_visatt -> SetForceAuxEdgeVisible(true);
        strut_visatt -> SetForceSolid(true);
        // strut_visatt -> SetForceLineSegmentsPerCircle(4);
        logicStrut -> SetVisAttributes(strut_visatt);
}

void 
StrutBar2AcrylicConstruction::makeStrutOpSurface() {
    G4OpticalSurface *strut2_optical_surface = new G4OpticalSurface("opStrut2Acrylic");
    strut2_optical_surface->SetMaterialPropertiesTable(Steel->GetMaterialPropertiesTable());
    strut2_optical_surface->SetModel(unified);
    strut2_optical_surface->SetType(dielectric_metal);
    strut2_optical_surface->SetFinish(ground);
    strut2_optical_surface->SetSigmaAlpha(0.2);

    new G4LogicalSkinSurface("Strut2AcrylicOpSurface", logicStrut, strut2_optical_surface);
}

double StrutBar2AcrylicConstruction::geom_info(const std::string& param) {
    if (param == "PosR") {
        return strut_pos;
    }
    return 0.0;
}
