
#include "PMTMaskConstruction.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4RegionStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(PMTMaskConstruction);

PMTMaskConstruction::PMTMaskConstruction(const std::string& name)
    : ToolBase(name)
{
    initVariables();
    declProp("BufferMaterial", m_buffer_material = "Water");

    logicMaskVirtual = 0;
}

PMTMaskConstruction::~PMTMaskConstruction() {

}

G4LogicalVolume* 
PMTMaskConstruction::getLV() {
    if (logicMaskVirtual) {
        return logicMaskVirtual;
    }
    initMaterials();

    makeMaskOutLogical();

    makeMaskLogical();
    makeMaskPhysical();
    
    return logicMaskVirtual;
}

G4double
PMTMaskConstruction::GetPMTRadius() {
    return mask_radiu_virtual;
}

G4double
PMTMaskConstruction::GetPMTHeight() {
    return height_virtual;
}

G4double
PMTMaskConstruction::GetZEquator() {
    return mask_radiu_virtual;
}
bool
PMTMaskConstruction::inject(std::string motherName, IDetElement* other, IDetElementPos* /*pos*/) {
    // Get the mother volume in current DetElem.
    G4LogicalVolume* mothervol = 0;
    if ( motherName == "lMaskVirtual" ) {
        mothervol = logicMaskVirtual;
    }
    if (not mothervol) {
        // don't find the volume.
        LogError << "Can't find the mother volume " << motherName << std::endl;
        return false;
    }

    // retrieve the daughter's LV
    inner_pmt = dynamic_cast<IPMTElement*>(other);
    G4LogicalVolume* daughtervol = other->getLV();

    if (not daughtervol) {
        LogError << "Can't find the daughter volume " << std::endl;
        return false;
    }

    int copyno = 0;
    new G4PVPlacement(0,
            G4ThreeVector(0,0,0) ,
            daughtervol,
            daughtervol->GetName()+"_phys",
            mothervol,
            false,
            copyno++
            );

    G4cout<<" Mask = "<<copyno<<G4endl;
    
    return true;
}

void
PMTMaskConstruction::initVariables() {
    mask_radiu_in = 254.*mm + 5.*mm;
    mask_radiu_out = mask_radiu_in + 10.*mm;
    mask_radiu_virtual = mask_radiu_out + 0.5*mm;
    height_in = 700*mm- 254*mm + 5*mm;
    height_out = height_in + 10*mm;
    height_virtual = height_out + 0.5*mm;
    
    gap = 0.1 *mm;
}

void 
PMTMaskConstruction::initMaterials() {
    LAB = G4Material::GetMaterial("LAB");
    Water = G4Material::GetMaterial("Water");
    Acrylic = G4Material::GetMaterial("Acrylic");
}

void
PMTMaskConstruction::makeMaskOutLogical() {
    
    G4Sphere*  Top_Virtual = new G4Sphere(
            "Top_Virtual",
            0*mm, 
            mask_radiu_virtual, 
            0*deg,
            360*deg, 
            0*deg,
            90*deg 
            );

    G4Tubs* Bottom_Virtual = new G4Tubs(
            "Bottom_Virtual",
            0*mm,   
            mask_radiu_virtual,  
            height_virtual/2,  
            0*deg, 
            360*deg);

    G4UnionSolid* SolidMaskVirtual = new G4UnionSolid(
            "sMask_virtual",
            Top_Virtual ,
            Bottom_Virtual ,
            0,
            G4ThreeVector(0,0,-height_virtual/2 + gap)    ) ;

    G4Material* BufferMaterials = Water;
    if (m_buffer_material=="LAB") {
        BufferMaterials = LAB;
    }
 
    logicMaskVirtual = new G4LogicalVolume(
            SolidMaskVirtual, 
            BufferMaterials, 
            "lMaskVirtual",
            0,
            0,
            0);
    G4cout<< " BufferMaterial = "<< BufferMaterials << G4endl; 
    G4VisAttributes* maskout_visatt = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    maskout_visatt -> SetForceWireframe(true);  
    maskout_visatt -> SetForceAuxEdgeVisible(true);
    //mask_visatt -> SetForceSolid(true);
    //mask_visatt -> SetForceLineSegmentsPerCircle(4);
    logicMaskVirtual -> SetVisAttributes(maskout_visatt);
}

void
PMTMaskConstruction::makeMaskLogical() {
    
    G4Sphere*  Top_out = new G4Sphere(
            "Top_Sphere",
            0*mm, 
            mask_radiu_out, 
            0*deg,
            360*deg, 
            0*deg,
            90*deg 
            );

    G4Tubs* Bottom_out = new G4Tubs(
            "Bottom_Tube",
            0*mm,   
            mask_radiu_out,  
            height_out/2,  
            0*deg, 
            360*deg);

    G4UnionSolid* Mask_out = new G4UnionSolid
        ("sMask_out",
         Top_out ,
         Bottom_out ,
         0,
         G4ThreeVector(0,0,-height_out/2 + gap)    ) ;

    G4Sphere*  Top_in = new G4Sphere(
            "Top_Sphere_in",
            0*mm, 
            mask_radiu_in, 
            0*deg,
            360*deg, 
            0*deg,
            90*deg 
            );

    G4Tubs* Bottom_in = new G4Tubs(
            "Bottom_Tube_in",
            0*mm,   
            mask_radiu_in,  
            height_in/2,  
            0*deg, 
            360*deg);

    G4UnionSolid* Mask_in = new G4UnionSolid
        ("sMask_in",
         Top_in ,
         Bottom_in ,
         0,
         G4ThreeVector(0,0,-height_in/2 + gap)    ) ;

    G4SubtractionSolid* solidMask = new G4SubtractionSolid(
            "sMask",
            Mask_out,
            Mask_in,
            0,
            G4ThreeVector(0,0,0)
            );
    
    logicMask = new G4LogicalVolume(
            solidMask, 
            Acrylic, 
            "lMask",
            0,
            0,
            0);
    G4VisAttributes* mask_visatt = new G4VisAttributes(G4Colour(1.0, 0, 1.0));
    mask_visatt -> SetForceWireframe(true);  
    mask_visatt -> SetForceAuxEdgeVisible(true);
    //mask_visatt -> SetForceSolid(true);
    //mask_visatt -> SetForceLineSegmentsPerCircle(4);
    logicMask -> SetVisAttributes(mask_visatt);
}

void
PMTMaskConstruction::makeMaskPhysical() {
    physiMask = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0), // at (x,y,z)
            logicMask,    // its logical volume 
            "pMask",       // its name
            logicMaskVirtual,  // its mother  volume
            false,           // no boolean operations
            0);              // no particular field
}

G4ThreeVector
PMTMaskConstruction::GetPosInPMT() {
    G4ThreeVector pos;
    if (inner_pmt) {
        pos = inner_pmt->GetPosInPMT();
    }
    return pos;
}

