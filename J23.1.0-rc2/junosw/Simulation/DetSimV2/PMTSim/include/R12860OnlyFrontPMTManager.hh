#ifndef R12860OnlyFrontPMTManager_hh
#define R12860OnlyFrontPMTManager_hh

/*
 * This is a simplified PMT with front/photocathode only.
 * The use case is that in the outer water pool, all the PMTs are placed in holes.
 * To simplify the detector construction, we only construct the front part.
 *
 *  z_equator=184  - --  .
 *                 ^        .
 *                 |          .    -->  Front
 *                 |           .
 *                 v            .
 *  z = 0          - ------------ -->  Equator
 *                 ^            |
 *                 v            | -->  Back
 *  z = -5mm (?)   - ------------ 
 *
 *
 *                 |<---  R --->|
 *
 * It contains three logical volumes:
 *   + PMT (glass)
 *   + inner_1 (front) = (inner_solid intersection separator)
 *   + inner_2 (back)  = (inner_solid substraction separator)
 *
 * Two optical surfaces are placed between PMT and inner_1/2.
 * The front surface is the sensitive part.
 */

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IPMTElement.h"

class G4OpticalSurface;
class G4Material;
class G4VSensitiveDetector;
class G4PVPlacement;
class G4VSolid;
class G4Tubs;

class R12860OnlyFront_PMTSolid;

class R12860OnlyFrontPMTManager: public IPMTElement,
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
    R12860OnlyFrontPMTManager
    (const G4String& plabel // label -- subvolume names are derived from this
    );
    ~R12860OnlyFrontPMTManager();
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
    G4String m_label;

    /* solid maker */
    R12860OnlyFront_PMTSolid* m_pmtsolid_maker;
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

    G4OpticalSurface* Photocathode_opsurf;
    G4Material* GlassMat;
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

    // quick construction during simulation.
    // add a virtual cover to speedup sim
    bool m_fast_cover;
    std::string m_cover_mat_str;
    G4Material* m_cover_mat;
};
#endif
