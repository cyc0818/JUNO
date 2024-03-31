#ifndef MCP8inchPMTManager_hh
#define MCP8inchPMTManager_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IPMTElement.h"

class G4OpticalSurface;
class G4Material;
class G4VSensitiveDetector;
class G4PVPlacement;
class G4VSolid;
class G4Tubs;

class R12860_PMTSolid;

class MCP8inchPMTManager: public IPMTElement,
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
    MCP8inchPMTManager
    (const G4String& plabel // label -- subvolume names are derived from this
    );
    ~MCP8inchPMTManager();
private:
    void init();

    void init_material();
    void init_variables();
    void init_mirror_surface();
    void init_pmt();
    void init_cover();

    G4String GetName() { return m_label;}

    void ConstructPMT_UsingTorusStack();
private:
    /* solid maker */
    R12860_PMTSolid* m_pmtsolid_maker;
    /* solid related */
    void helper_make_solid();
    // * pmt solid (a little big than body solid)
    //   * body solid
    //     + inner1
    //     + inner2
    G4VSolid* pmt_solid; 
    G4VSolid* body_solid;
    G4VSolid* inner_solid;
    G4VSolid* inner1_solid;
    G4VSolid* inner2_solid;
    G4Tubs* dynode_solid;
    G4double hh_dynode;
    G4double z_dynode;
    /* logical volumes */
    void helper_make_logical_volume();
    G4LogicalVolume* body_log;
    G4LogicalVolume* inner1_log;
    G4LogicalVolume* inner2_log;
    G4LogicalVolume* dynode_log;
    /* physical volumes */
    void helper_make_physical_volume();
    G4PVPlacement* body_phys;
    G4PVPlacement* inner1_phys;
    G4PVPlacement* inner2_phys;
    G4PVPlacement* dynode_phys;
    /* optical surface */
    void helper_make_optical_surface();
    /* fast simulation */
    void helper_fast_sim();
    /* visual attribute */
    void helper_vis_attr();
private:
    G4LogicalVolume* m_logical_pmt;

    G4OpticalSurface* m_mirror_opsurf;

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
    G4double m_pmt_r;
    G4double m_pmt_h;

    G4String m_label;

    // quick construction during simulation.
    // add a virtual cover to speedup sim
    bool m_fast_cover;
    std::string m_cover_mat_str;
    G4Material* m_cover_mat;
};
#endif
