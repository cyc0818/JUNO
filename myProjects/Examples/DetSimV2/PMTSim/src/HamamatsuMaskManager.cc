
#include "HamamatsuMaskManager.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"
#include "G4Polycone.hh"
#include "G4VisAttributes.hh"
#include "G4RegionStore.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"

using namespace CLHEP;

DECLARE_TOOL(HamamatsuMaskManager);

HamamatsuMaskManager::HamamatsuMaskManager(const std::string& name)
    : ToolBase(name)
{
    declProp("BufferMaterial", m_buffer_material = "Water");
    declProp("TopThickness", htop_thickness=8.*mm);
    declProp("TopGap", htop_gap=2.*mm); // gap between PMT and Mask at top

    declProp("EquatorThickness", requator_thickness=8.*mm); // thickness at equator
    declProp("EquatorGap", requator_gap=2.*mm); // gap between PMT and Mask at equator

    logicMaskVirtual = 0;

    MAGIC_virtual_thickness = 0.05*mm;
}

HamamatsuMaskManager::~HamamatsuMaskManager() {

}

G4LogicalVolume* 
HamamatsuMaskManager::getLV() {
    if (logicMaskVirtual) {
        return logicMaskVirtual;
    }
    initVariables();
    initMaterials();

    makeMaskOutLogical();

    makeMaskLogical();
    makeMaskPhysical();
    // logicMaskVirtual = logicMask;
    
    return logicMaskVirtual;
}

G4double
HamamatsuMaskManager::GetPMTRadius() {
    return mask_radiu_virtual;
}

G4double
HamamatsuMaskManager::GetPMTHeight() {
    return height_virtual;
}

G4double
HamamatsuMaskManager::GetZEquator() {
    return mask_radiu_virtual;
}
bool
HamamatsuMaskManager::inject(std::string motherName, IDetElement* other, IDetElementPos* /*pos*/) {
    // initialize self.
    getLV();
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
HamamatsuMaskManager::initVariables() {
/* 
 * NOTE: The virtual mask outside is to speedup detector construction

out --_
MASK    htop_thickness
 in --_
    **_ htop_gap            gap 
PMT ^                        ^ thickness
    |                    PMT | MASK
   H = 184                  * \  \
    |                        * \  \
---- EQUATOR -- R = 254.mm -->* |  | BELOW is Tubs
    |                        *^ |  | ^
    |                       * | |  | |
H = 523-184               **  | |  | |
    |                   **    | |  | |
......................................
......................................
PMT |     |               H_in  |  | H_out
    v_____|                   | |  | |
gap                           | |  | |
    __________________________v_|  | |
    _______________________________| v
*/     



    mask_radiu_in = 254.*mm + requator_gap;
    mask_radiu_out = mask_radiu_in + requator_thickness;
    mask_radiu_virtual = mask_radiu_out + MAGIC_virtual_thickness;
    htop_in = 190.*mm + htop_gap;
    htop_out = htop_in + htop_thickness;

    height_in = (640.-190.)*mm + 5.*mm;
    height_out = height_in + 10*mm;
    height_virtual = height_out + MAGIC_virtual_thickness;
    
    gap = 0.1 *mm;
}

void 
HamamatsuMaskManager::initMaterials() {
    LAB = G4Material::GetMaterial("LAB");
    Water = G4Material::GetMaterial("Water");
    Acrylic = G4Material::GetMaterial("Acrylic");
}

void
HamamatsuMaskManager::makeMaskOutLogical() {
    
    G4double zPlane[] = {
                        -height_virtual,
                        htop_out + MAGIC_virtual_thickness
                        };
    G4double rInner[] = {0.,
                         0.};
    G4double rOuter[] = {mask_radiu_virtual,
                         mask_radiu_virtual};


    G4VSolid* SolidMaskVirtual = new G4Polycone(
				objName()+"sMask_virtual",
                                0,
                                360*deg,
                                2,
                                zPlane,
                                rInner,
                                rOuter
                                );

    G4Material* BufferMaterials = Water;
    if (m_buffer_material=="LAB") {
        BufferMaterials = LAB;
    }
 
    logicMaskVirtual = new G4LogicalVolume(
            SolidMaskVirtual, 
            BufferMaterials, 
            objName()+"lMaskVirtual",
            0,
            0,
            0);
    G4cout<< " BufferMaterial = "<< BufferMaterials << G4endl; 
    G4VisAttributes* maskout_visatt = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    maskout_visatt -> SetForceWireframe(true);  
    maskout_visatt -> SetForceAuxEdgeVisible(true);
    //mask_visatt -> SetForceSolid(true);
    //mask_visatt -> SetForceLineSegmentsPerCircle(4);
    maskout_visatt -> SetVisibility(false);
    logicMaskVirtual -> SetVisAttributes(maskout_visatt);
}

void
HamamatsuMaskManager::makeMaskLogical() {
    
    /* 
    G4Sphere*  Top_out = new G4Sphere(
            "Top_Sphere",
            0*mm, 
            mask_radiu_out, 
            0*deg,
            360*deg, 
            0*deg,
            90*deg 
            );
    */
    G4Ellipsoid* Top_out = new G4Ellipsoid(
            objName()+"Top_Sphere",
            mask_radiu_out, // pxSemiAxis
            mask_radiu_out, // pySemiAxis
            htop_out  // pzSemiAxis
            // XXX, // pzBottomCut
            // XXX  // pzTopCut
            );

    G4Tubs* Bottom_out = new G4Tubs(
            objName()+"Bottom_Tube",
            0*mm,   
            mask_radiu_out,  
            height_out/2,  
            0*deg, 
            360*deg);

    G4UnionSolid* Mask_out = new G4UnionSolid
        (objName()+"sMask_out",
         Top_out ,
         Bottom_out ,
         0,
         G4ThreeVector(0,0,-height_out/2 + gap)    ) ;

    /* 
    G4Sphere*  Top_in = new G4Sphere(
            "Top_Sphere_in",
            0*mm, 
            mask_radiu_in, 
            0*deg,
            360*deg, 
            0*deg,
            90*deg 
            );
    */
    G4Ellipsoid* Top_in = new G4Ellipsoid(
            objName()+"Top_Sphere_in",
            mask_radiu_in, // pxSemiAxis
            mask_radiu_in, // pySemiAxis
            htop_in  // pzSemiAxis
            // XXX, // pzBottomCut
            // XXX  // pzTopCut
            );

    G4Tubs* Bottom_in = new G4Tubs(
            objName()+"Bottom_Tube_in",
            0*mm,   
            mask_radiu_in,  
            height_in/2,  
            0*deg, 
            360*deg);

    G4UnionSolid* Mask_in = new G4UnionSolid
        (objName()+"sMask_in",
         Top_in ,
         Bottom_in ,
         0,
         G4ThreeVector(0,0,-height_in/2 + gap)    ) ;

    G4SubtractionSolid* solidMask = new G4SubtractionSolid(
            objName()+"sMask",
            Mask_out,
            Mask_in,
            0,
            G4ThreeVector(0,0,0)
            );
    
    logicMask = new G4LogicalVolume(
            solidMask, 
            Acrylic, 
            objName()+"lMask",
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
HamamatsuMaskManager::makeMaskPhysical() {
    physiMask = new G4PVPlacement(0,              // no rotation
            G4ThreeVector(0,0,0), // at (x,y,z)
            logicMask,    // its logical volume 
            objName()+"pMask",       // its name
            logicMaskVirtual,  // its mother  volume
            false,           // no boolean operations
            0);              // no particular field
}

G4ThreeVector
HamamatsuMaskManager::GetPosInPMT() {
    G4ThreeVector pos;
    if (inner_pmt) {
        pos = inner_pmt->GetPosInPMT();
    }
    return pos;
}

