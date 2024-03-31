#ifndef Tub3inchPMTV3Manager_hh
#define Tub3inchPMTV3Manager_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IPMTElement.h"

class G4OpticalSurface;
class G4Material;
class G4VSensitiveDetector;
class G4PVPlacement;
class G4VSolid;
class G4Tubs;

class Tub3inchPMTV3Solid;

class Tub3inchPMTV3Manager: public IPMTElement,
                            public ToolBase {
public:
    // Interface
    G4LogicalVolume* getLV();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();
public:
    // Constructor
    Tub3inchPMTV3Manager(const G4String& plabel);
    
    ~Tub3inchPMTV3Manager();
private:
    void init();
    void init_material();
    void init_variables();
    void init_mirror_surface();
    void init_absorb_surface();
    void init_pmt();

    G4String GetName() { return m_label;}

    void ConstructPMT_UsingTorusStack();
private:
    G4String m_label;
    /* solid maker */
    Tub3inchPMTV3Solid* m_pmtsolid_maker;
    /* solid related */
    void helper_make_solid();
    // * pmt solid 
    //   * body solid
    //     * inner1
    //   * container
    G4VSolid* pmt_solid; 
    G4VSolid* body_solid;
    G4VSolid* inner_solid;
    G4VSolid* inner1_solid;
    G4VSolid* inner2_solid;
    G4VSolid* cntr_solid;
    /* logical volumes */
    void helper_make_logical_volume();
    G4LogicalVolume* body_log;
    G4LogicalVolume* inner1_log;
    G4LogicalVolume* inner2_log;
    G4LogicalVolume* cntr_log;
    /* physical volumes */
    void helper_make_physical_volume();
    G4PVPlacement* body_phys;
    G4PVPlacement* inner1_phys;
    G4PVPlacement* inner2_phys;
    G4PVPlacement* cntr_phys;
    /* optical surface */
    void helper_make_optical_surface();
    /* fast simulation */
    void helper_fast_sim();
    /* visual attribute */
    void helper_vis_attr();
private:
    G4LogicalVolume* m_logical_pmt;

    G4OpticalSurface* m_mirror_opsurf;
    G4OpticalSurface* m_absorb_opsurf;

    G4Material* ExteriorMat;
    G4Material* GlassMat;
    G4OpticalSurface* Photocathode_opsurf;
    G4Material* PMT_Vacuum;
    G4Material* DynodeMat;
    G4Material* MaskMat;
    G4VSensitiveDetector *m_detector;

private:
    // Cover Related
    G4LogicalVolume* m_logical_cover;

private:
    G4double m_z_equator;
    G4double m_pmt_R;
    G4double m_pmt_H;  // top-equator
    G4double m_cntr_R;
    G4double m_cntr_H;
    G4double m_cntr_Z1; // container top z-position
    G4double m_cntr_Z2; // container bottom z-position
    G4double m_glass_thickness;
    G4double m_photocathode_R;
    G4double m_photocathode_Z;

};

#endif
