#ifndef NNVTMCPPMTManager_hh
#define NNVTMCPPMTManager_hh
/**
NNVTMCPPMTManager.hh
=======================

* Original implementation -- Tao Lin, 2017/05/29
* Updated implementation -- Simon C Blyth, 2023/02

To follow this implementation see notes in CommonPMTManager.h 

**/


#ifdef PMTSIM_STANDALONE
#include "IGeomManager.h"
#else
#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IPMTElement.h"
#include "Geometry/IPMTParamSvc.h"
#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#endif

#include "G4ThreeVector.hh"
#include "globals.hh"

class G4OpticalSurface;
class G4Material;
class G4VSensitiveDetector;
class G4PVPlacement;
class G4VSolid;
class G4Tubs;
class G4LogicalVolume ; 

class junoPMTOpticalModel ; 

class NNVT_MCPPMT_PMTSolid;

#ifdef PMTSIM_STANDALONE
#include "PMTSIM_API_EXPORT.hh"
class PMTSIM_API NNVTMCPPMTManager : public IGeomManager {
#else

class NNVTMCPPMTManager: public IPMTElement,
                         public ToolBase {
#endif
public:
    // Interface
    G4LogicalVolume* getLV();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();

public:
    NNVTMCPPMTManager(const G4String& plabel );
    ~NNVTMCPPMTManager();
    std::string desc() const ; 
public:
    // debug interface
    G4LogicalVolume* getLV(const char* name);
    G4PVPlacement*   getPV(const char* name);
    G4VSolid*        getSolid(const char* name);
    junoPMTOpticalModel* getPMTOpticalModel(const char* name) ;

    static bool StartsWithPrefix(const char* name, const char* prefix); 

private:
    void init();
    void init_material();
    void init_variables();
    void init_mirror_surface();
    void init_pmt();

    void obsolete_inner_cut();
    void obsolete_tail_cut();

    G4String GetName() { return m_label;}

private:
    G4String m_label;

    NNVT_MCPPMT_PMTSolid* m_pmtsolid_maker;
    junoPMTOpticalModel*  pmtOpticalModel ;

    void helper_make_solid();
    void helper_make_solid_profligate_tail_cut(); 

    // * pmt solid (a little bigger than body solid)
    //   * body solid
    //     + inner1
    //     + inner2
    G4VSolid* pmt_solid; 
    G4VSolid* body_solid;
    G4VSolid* inner_solid;
    G4VSolid* inner1_solid;
    G4VSolid* inner2_solid;

    G4VSolid* uncut_pmt_solid;
    G4VSolid* uncut_body_solid;
    G4VSolid* uncut_inner_solid;
    G4VSolid* uncut_inner2_solid;

    G4Tubs* dynode_solid;
    G4double hh_dynode;
    G4double z_dynode;

    void helper_make_logical_volume();

    G4LogicalVolume* pmt_log;
    G4LogicalVolume* body_log;
    G4LogicalVolume* inner_log;
    G4LogicalVolume* inner1_log;
    G4LogicalVolume* inner2_log;
    G4LogicalVolume* dynode_log;

    void helper_make_physical_volume();

    G4PVPlacement* body_phys;
    G4PVPlacement* inner_phys;
    G4PVPlacement* inner1_phys;
    G4PVPlacement* inner2_phys;
    G4PVPlacement* dynode_phys;

#ifdef PMTSIM_STANDALONE
#else
    IPMTParamSvc* m_pmt_param_svc;
    IPMTSimParamSvc* m_pmt_sim_param_svc;
#endif

    void helper_make_mcp_volume();
    void helper_make_optical_surface();
    void helper_fast_sim();
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
    G4Material* m_cover_mat;

    bool m_fast_cover;
    std::string m_cover_mat_str;
    bool m_UsePMTOpticalModel;
    bool m_UsePMTNaturalGeometry; 

    void init_fast_cover();

private:
    G4double m_z_equator;
    G4double m_pmt_r;
    G4double m_pmt_h;

private:
    /**
    If m_useRealSurface is true, the PMT should insert into the surface.
    In order to optimize the speed, just reduce the size of the PMT.
    
              |    -
              |  /   \
          ------- P M T
          ------- 19.434m
              |  \   /
              |    -
            Tyvek  |
           19.629m

    **/
    bool m_useRealSurface;
    bool m_profligate_tail_cut ; 
    G4double m_pmt_equator_to_bottom ;
};

#endif
