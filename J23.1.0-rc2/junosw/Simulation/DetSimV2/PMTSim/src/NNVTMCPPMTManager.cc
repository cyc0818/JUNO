
#include <iostream>
#include <iomanip>

#include "CommonPMTManager.h"
#include "NNVTMCPPMTManager.hh"
#include "NNVT_MCPPMT_PMTSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh" // for boolean solids
#include "G4SubtractionSolid.hh" // for boolean solids
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh" // for G4VisAttributes::Invisible
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Polycone.hh"
#include "G4SDManager.hh"

#include "ZSolid.h"

#ifdef PMTSIM_STANDALONE

#define LogInfo  std::cout 
#define LogError std::cerr 
#include "junoPMTOpticalModel.hh"

#include "PMTSimParamSvc/PMTAccessor.h"
#include "C4IPMTAccessor.h"

#else

//#include <boost/python.hpp>
#include "junoPMTOpticalModel.hh"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"

#endif

using namespace CLHEP;


#ifdef PMTSIM_STANDALONE
#else
DECLARE_TOOL(NNVTMCPPMTManager);
#endif

G4LogicalVolume* NNVTMCPPMTManager::getLV() 
{
    if(!m_logical_pmt) init();
    return m_logical_pmt;
}

G4LogicalVolume* NNVTMCPPMTManager::getLV(const char* name)
{
    if(!m_logical_pmt) init();
    G4LogicalVolume* lv = nullptr ; 
    if(StartsWithPrefix(name, "LogicalPMT")) lv = m_logical_pmt ;
    if(StartsWithPrefix(name, "BodyLog"))    lv = body_log ;
    if(StartsWithPrefix(name, "Inner1Log"))  lv = inner1_log ;
    if(StartsWithPrefix(name, "Inner2Log"))  lv = inner2_log ;

    if(StartsWithPrefix(name, "DynodeLog"))  lv = dynode_log ;
    if(StartsWithPrefix(name, "LogicalCover")) lv = m_logical_cover ;

    if(lv == nullptr)
    {
        std::cout << "NNVTMCPPMTManager::getLV failed for name [" << name << "]" << std::endl ;  
    }
    return lv ;
}

G4PVPlacement* NNVTMCPPMTManager::getPV(const char* name)
{
    if(!m_logical_pmt) init();
    G4PVPlacement* pv = nullptr ;
    if(StartsWithPrefix(name, "BodyPhys"))   pv = body_phys ;
    if(StartsWithPrefix(name, "Inner1Phys")) pv = inner1_phys ;
    if(StartsWithPrefix(name, "Inner2Phys")) pv = inner2_phys ;
    if(StartsWithPrefix(name, "DynodePhys")) pv = dynode_phys ;

    if(pv == nullptr)
    {
        std::cout << "NNVTMCPPMTManager::getPV failed for name [" << name << "]" << std::endl ;  
    }
    return pv ;
}

G4double NNVTMCPPMTManager::GetPMTRadius() {
    if (!getLV()) {
        LogError << "Can't initialize PMT." << std::endl;;
    }
    return m_pmt_r;
}

G4double NNVTMCPPMTManager::GetPMTHeight() {
    if (!getLV()) {
        LogError << "Can't initialize PMT." << std::endl;;
    }
    return m_pmt_h;
}

G4double NNVTMCPPMTManager::GetZEquator() {
    if (!getLV()) {
        LogError << "Can't initialize PMT." << std::endl;;
    }
    return m_z_equator;
}

G4ThreeVector NNVTMCPPMTManager::GetPosInPMT() {
    G4ThreeVector rndm_pos;
    return rndm_pos;
}

NNVTMCPPMTManager::NNVTMCPPMTManager(const G4String& plabel)
    : 
#ifdef PMTSIM_STANDALONE
      IGeomManager(plabel), 
#else
      ToolBase(plabel), 
#endif
      m_label(plabel),
      m_pmtsolid_maker(0),
      pmtOpticalModel(nullptr),
      pmt_solid(nullptr), 
      body_solid(nullptr), 
      inner_solid(nullptr),
      inner1_solid(nullptr), 
      inner2_solid(nullptr), 
      uncut_pmt_solid(nullptr),
      uncut_body_solid(nullptr),
      uncut_inner_solid(nullptr),
      uncut_inner2_solid(nullptr),
      dynode_solid(nullptr),
      pmt_log(nullptr),
      body_log(nullptr),
      inner_log(nullptr),
      inner1_log(nullptr),
      inner2_log(nullptr),
      dynode_log(nullptr),
      body_phys(nullptr),
      inner_phys(nullptr),
      inner1_phys(nullptr),
      inner2_phys(nullptr), 
      dynode_phys(nullptr),
#ifdef PMTSIM_STANDALONE
#else
      m_pmt_param_svc(nullptr),
      m_pmt_sim_param_svc(nullptr),
#endif
      m_logical_pmt(nullptr),
      m_mirror_opsurf(nullptr),
      Photocathode_opsurf(nullptr),
      GlassMat(nullptr),
      PMT_Vacuum(nullptr),
      DynodeMat(nullptr),
      MaskMat(nullptr),
      m_detector(nullptr),
      m_logical_cover(nullptr), 
      m_cover_mat(nullptr),
      m_profligate_tail_cut(getenv("JUNO_PMT20INCH_PROFLIGATE_TAIL_CUT") == nullptr ? false : true ),
      m_pmt_equator_to_bottom(0.)
{
    declProp("FastCover", m_fast_cover=false);
    declProp("FastCoverMaterial", m_cover_mat_str="Water");
    declProp("UsePMTOpticalModel", m_UsePMTOpticalModel=true);
    declProp("UsePMTNaturalGeometry", m_UsePMTNaturalGeometry=true);
    declProp("UseRealSurface", m_useRealSurface=true);
}

std::string NNVTMCPPMTManager::desc() const 
{
    std::stringstream ss ; 
    ss 
         << std::setw(30) << "NNVTMCPPMTManager"
         << " m_UsePMTNaturalGeometry " << ( m_UsePMTNaturalGeometry ? "Y" : "N" )
         << " m_UsePMTOpticalModel " << ( m_UsePMTOpticalModel ? "Y" : "N" )
         << " m_useRealSurface " << ( m_useRealSurface ? "Y" : "N" )
         << " m_profligate_tail_cut " << ( m_profligate_tail_cut ? "Y" : "N" )
         ;
    std::string s = ss.str(); 
    return s ; 
}


NNVTMCPPMTManager::~NNVTMCPPMTManager() 
{
    if (m_pmtsolid_maker) {
        delete m_pmtsolid_maker;
    }
}
    

void NNVTMCPPMTManager::init() 
{
#ifdef PMTSIM_STANDALONE
#else
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    m_detector = SDman->FindSensitiveDetector("PMTSDMgr");
    assert(m_detector);
#endif
    init_material();
    init_variables();
    init_mirror_surface();
    init_pmt();

    if (m_fast_cover) {
        init_fast_cover();
    }
}

void NNVTMCPPMTManager::init_material() 
{
     GlassMat = G4Material::GetMaterial("Pyrex");
     PMT_Vacuum = G4Material::GetMaterial("Vacuum"); 
     DynodeMat = G4Material::GetMaterial("Steel");

     Photocathode_opsurf =  new G4OpticalSurface(GetName()+"_Photocathode_opsurf");
     Photocathode_opsurf->SetType(dielectric_metal); // ignored if RINDEX defined
     //Photocathode_opsurf->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode")->GetMaterialPropertiesTable() );
     Photocathode_opsurf->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode_MCP20inch")->GetMaterialPropertiesTable() );

     if (m_fast_cover) {
         m_cover_mat = G4Material::GetMaterial(m_cover_mat_str);
         assert(m_cover_mat);
     }
}

void NNVTMCPPMTManager::init_variables() 
{
    m_pmt_r = 254.*mm;
    m_pmt_h = 570.*mm;
    m_z_equator = 184.*mm; // From top to equator

    // Reduce the height of PMT
    // Tao Lin, 09 Aug 2021
    if (m_useRealSurface) {
        const double radInnerWaterRealSurface = 19.629*m;
        const double r = m_pmt_r + 1.*cm; // 1cm is the mask
        double pmt_eq_to_bottom = sqrt(radInnerWaterRealSurface*radInnerWaterRealSurface
                                       -r*r) - 19.434*m; // at z equator
        // then, subtract the thickness of mask
        pmt_eq_to_bottom -= 10.*mm;

        // avoid the overlap between PMT tail and innerWater
        const double safety_distance = 1.*cm;
        pmt_eq_to_bottom -= safety_distance;



        m_pmt_equator_to_bottom = pmt_eq_to_bottom ;

        double pmt_h = pmt_eq_to_bottom + m_z_equator ;
        LogInfo << "Option RealSurface is enabled in Central Detector. "
                << " Reduce the m_pmt_h from "
                << m_pmt_h << " to " << pmt_h
                << std::endl;
        m_pmt_h = pmt_h;
    }


    LogInfo 
#ifdef PMTSIM_STANDALONE
#else
         << " scope:tag/objName " << scope() << tag() << "/" << objName() 
#endif
         << " GetName() " << GetName()
         <<  std::endl
         ;

    m_pmtsolid_maker = new NNVT_MCPPMT_PMTSolid();

}

void NNVTMCPPMTManager::init_mirror_surface() 
{
    if ( m_mirror_opsurf == NULL ) {
        // construct a static mirror surface with idealized properties

        G4String name ; 
        name += CommonPMTManager::GetMirrorOpticalSurfacePrefix(m_UsePMTNaturalGeometry, m_UsePMTOpticalModel );
        name += GetName() ; 
        name += "_Mirror_opsurf" ; 

        m_mirror_opsurf =  new G4OpticalSurface(name);
        m_mirror_opsurf->SetFinish(polishedfrontpainted); // needed for mirror
        m_mirror_opsurf->SetModel(glisur); 
        m_mirror_opsurf->SetType(dielectric_metal); 
        m_mirror_opsurf->SetPolish(0.999);              // a guess -- FIXME
        G4MaterialPropertiesTable* propMirror= NULL;
        G4Material *matMirror = G4Material::GetMaterial("PMT_Mirror");
        if (matMirror) {
            propMirror= matMirror->GetMaterialPropertiesTable();
        }
        if ( propMirror == NULL ) {
            G4cout << "Warning: setting PMT mirror reflectivity to 0.9999 "
                   << "because no PMT_Mirror material properties defined" << G4endl;
            propMirror= new G4MaterialPropertiesTable();
            propMirror->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
            //propMirror->AddEntry("REFLECTIVITY", twopi*hbarc/(800.0e-9*m), 0.9999);
            //propMirror->AddEntry("REFLECTIVITY", twopi*hbarc/(200.0e-9*m), 0.9999);
            propMirror->AddEntry("REFLECTIVITY", 1.55*eV, 0.92);
            propMirror->AddEntry("REFLECTIVITY", 15.5*eV, 0.92);
        }
        m_mirror_opsurf->SetMaterialPropertiesTable( propMirror );

    }

}

void NNVTMCPPMTManager::init_pmt()
{
    helper_make_solid();  
    helper_make_logical_volume();
    helper_make_physical_volume();

    if(m_UsePMTOpticalModel )
    {
        helper_make_mcp_volume();
    }
    helper_make_optical_surface();

    if(m_UsePMTOpticalModel == true && m_UsePMTNaturalGeometry == false )
    {
        helper_fast_sim();
    }
    helper_vis_attr();
}

/**
NNVTMCPPMTManager::helper_make_solid
-------------------------------------

* Initial implementation "Reduce the size when real surface is enabled." done by Tao Lin, 09 Aug 2021
* Adoption of more efficienct actual CSG tree cutting done by Simon Blyth, Nov 2021
* Adoption of natural geometry, Simon Blyth, Feb 2023


          m_UsePMTNaturalGeometry:false                                      m_UsePMTNaturalGeometry:true


     +---------------pmt-Pyrex----------------+                 +---------------pmt-Pyrex----------------+
     |                                        |                 |                                        |
     |                                        |                 |                                        |
     |     +----------body-Pyrex--------+     |                 |                                        |
     |     | +------------------------+ |     |                 |       +~inner~Vacuum~~~~~~~~~~~+       |
     |     | |                        | |     |                 |       !                        !       |
     |     | |                        | |     |                 |       !                        !       |
     |     | |        inner1-Vacuum   |-|     |                 |       !                        !       |
     |     | |                        |1e-3   |                 |       !                        !       |
     |     | |                        | |     |                 |       !                        !       |
     |     | +~~coincident~face~~~~~~~+ |     |                 |       +                        +       |
     |     | |                        | |     |                 |       |                        |       |
     |     | |                        | |     |                 |       |                        |       |
     |     | |        inner2-Vacuum   | |     |                 |       |                        |       |
     |     | |                        | |     |                 |       |                        |       |
     |     | |                        | |     |                 |       |                        |       |
     |     | +------------------------+ |     |                 |       +------------------------+       |
     |     +----------------------------+     |                 |                                        |
     |                                        |                 |                                        |
     |                                        |                 |                                        |
     +----------------------------------------+                 +----------------------------------------+


**/

void NNVTMCPPMTManager::helper_make_solid() 
{
    double pmt_delta = 1E-3*mm ;
    double inner_delta = -5*mm ; 

    double body_delta = m_UsePMTOpticalModel == false ? 0. : inner_delta+1E-3*mm ; 
    // TODO: find out why body_delta depends on m_UsePMTOpticalModel and add comment about that 

    double zcut = m_pmt_equator_to_bottom ;

    NNVT_MCPPMT_PMTSolid* maker = m_pmtsolid_maker ; 


    if(m_UsePMTNaturalGeometry == false)
    {
        pmt_solid    = maker->GetSolid(GetName() + "_pmt_solid",    pmt_delta  , ' ');
        body_solid   = maker->GetSolid(GetName() + "_body_solid",   body_delta , ' ');
        inner_solid  = maker->GetSolid(GetName() + "_inner_solid",  inner_delta, ' ');
        inner1_solid = maker->GetSolid(GetName() + "_inner1_solid", inner_delta, 'H'); // head
        inner2_solid = maker->GetSolid(GetName() + "_inner2_solid", inner_delta, 'T'); // tail

        uncut_pmt_solid = pmt_solid ; 
        uncut_body_solid = body_solid ; 
        uncut_inner_solid = inner_solid ;
        uncut_inner2_solid = inner2_solid ; 
    }
    else
    {
        pmt_solid    = maker->GetSolid(GetName() + "_pmt_solid",    pmt_delta  , ' ');
        inner_solid  = maker->GetSolid(GetName() + "_inner_solid",  inner_delta, ' ');

        uncut_pmt_solid = pmt_solid ;
        uncut_inner_solid = inner_solid ;
    }



    if (m_useRealSurface && m_profligate_tail_cut == false) 
    {
        std::cout 
            << "[ ZSolid::ApplyZCutTree"
            << " zcut " << std::setw(10) << std::fixed << std::setprecision(3) << zcut 
            << " pmt_delta " << std::setw(10) << std::fixed << std::setprecision(3) << pmt_delta 
            << " body_delta " << std::setw(10) << std::fixed << std::setprecision(3) << body_delta 
            << " inner_delta " << std::setw(10) << std::fixed << std::setprecision(3) << inner_delta 
            << " zcut+pmt_delta " << std::setw(10) << std::fixed << std::setprecision(3) << zcut + pmt_delta 
            << " zcut+body_delta " << std::setw(10) << std::fixed << std::setprecision(3) << zcut + body_delta 
            << " zcut+inner_delta " << std::setw(10) << std::fixed << std::setprecision(3) << zcut + inner_delta 
            << std::endl ; 

        if(pmt_solid)    pmt_solid    = ZSolid::ApplyZCutTree( pmt_solid   , -(zcut + pmt_delta)   );
        if(body_solid)   body_solid   = ZSolid::ApplyZCutTree( body_solid  , -(zcut + body_delta)  );
        if(inner_solid)  inner_solid  = ZSolid::ApplyZCutTree( inner_solid  , -(zcut + inner_delta)  );
        if(inner2_solid) inner2_solid = ZSolid::ApplyZCutTree( inner2_solid, -(zcut + inner_delta) );

        std::cout << "] ZSolid::ApplyZCutTree zcut " << zcut << std::endl ; 
    }
    else if (m_useRealSurface && m_profligate_tail_cut == true)
    {
        helper_make_solid_profligate_tail_cut() ; 
    } 
}




void NNVTMCPPMTManager::helper_make_solid_profligate_tail_cut()
{
    std::cout << "NNVTMCPPMTManager::helper_make_solid_profligate_tail_cut" << std::endl ; 
    // inner2 
    G4double helper_sep_tube_r = m_pmt_r;
    const double tail_height = m_pmt_h - m_z_equator;
    const double tail_half_height = tail_height / 2;
    const G4ThreeVector cut_tail_displacement(0., 0., -tail_half_height);
    G4VSolid* cut_tail_solid = new G4Tubs("CutTail_NNVTMCPPMT_Solid",
                                          0.,
                                          helper_sep_tube_r+1E-9*mm,
                                          tail_half_height,
                                          0., 360.*degree);
    inner2_solid = new G4IntersectionSolid( GetName() + "_inner2_tail_solid",
                                            inner2_solid,
                                            cut_tail_solid,
                                            NULL,
                                            cut_tail_displacement);

    // pmt solid
    const double pmt_height = m_pmt_h;
    const double pmt_half_height = pmt_height / 2;
    const G4ThreeVector cut_pmt_displacement(0., 0., m_z_equator-pmt_half_height);
    G4VSolid* cut_pmt_solid = new G4Tubs("CutPMT_NNVTMCPPMT_Solid",
                                          0.,
                                          helper_sep_tube_r+1E-9*mm,
                                          pmt_half_height,
                                          0., 360.*degree);
    pmt_solid = new G4IntersectionSolid( GetName() + "_pmt_cut_solid",
                                            pmt_solid,
                                            cut_pmt_solid,
                                            NULL,
                                            cut_pmt_displacement);

    // body solid
    const double body_height = m_pmt_h;
    const double body_half_height = body_height / 2;
    const G4ThreeVector cut_body_displacement(0., 0., m_z_equator-pmt_half_height);
    G4VSolid* cut_body_solid = new G4Tubs("CutBody_NNVTMCPPMT_Solid",
                                          0.,
                                          helper_sep_tube_r+1E-9*mm,
                                          body_half_height,
                                          0., 360.*degree);
    body_solid = new G4IntersectionSolid( GetName() + "_pmt_cut_solid",
                                            body_solid,
                                            cut_body_solid,
                                            NULL,
                                            cut_body_displacement);
}
 






bool NNVTMCPPMTManager::StartsWithPrefix(const char* name, const char* prefix)  // static
{
    return strlen(name) >= strlen(prefix) && strncmp( name, prefix, strlen(prefix)) == 0 ;    
}

G4VSolid* NNVTMCPPMTManager::getSolid(const char* name)
{
    if(!m_logical_pmt) 
    {
        std::cout << "[ NNVTMCPPMTManager::getSolid init " << name << std::endl;
        init();
        std::cout << "] NNVTMCPPMTManager::getSolid init " << name << std::endl;
    }

    G4VSolid* so = nullptr ;
    if(StartsWithPrefix(name, "PMTSolid"))    so = pmt_solid ;
    if(StartsWithPrefix(name, "BodySolid"))   so = body_solid ;
    if(StartsWithPrefix(name, "InnerSolid"))  so = inner_solid ;
    if(StartsWithPrefix(name, "Inner1Solid")) so = inner1_solid ;
    if(StartsWithPrefix(name, "Inner2Solid")) so = inner2_solid ;
    if(StartsWithPrefix(name, "DynodeSolid")) so = dynode_solid ;

    if(StartsWithPrefix(name, "UncutPMTSolid"))    so = uncut_pmt_solid ;
    if(StartsWithPrefix(name, "UncutBodySolid"))   so = uncut_body_solid ;
    if(StartsWithPrefix(name, "UncutInnerSolid"))  so = uncut_inner_solid ;
    if(StartsWithPrefix(name, "UncutInner2Solid")) so = uncut_inner2_solid ;

    return so ;
}

junoPMTOpticalModel* NNVTMCPPMTManager::getPMTOpticalModel(const char* name)
{
    std::cout
        << "NNVTMCPPMTManager::getPMTOpticalModel"
        << " name " << ( name ? name : "-" )
        << " pmtOpticalModel " << ( pmtOpticalModel ? "Y" : "N" )
        << std::endl
        ;
    return pmtOpticalModel ;
}


void NNVTMCPPMTManager::obsolete_inner_cut()
{
    G4double helper_sep_tube_r = m_pmt_r;
    G4double helper_sep_tube_h = m_z_equator;
    G4double helper_sep_tube_hh = helper_sep_tube_h/2;

    G4VSolid * pInnerSep = new G4Tubs("Inner_Separator",
            0.,
            helper_sep_tube_r+1E-9*mm,
            helper_sep_tube_hh+1E-9*mm,
            0.,360.*degree);
    G4ThreeVector innerSepDispl(0.,0.,helper_sep_tube_hh-1E-9*mm);
    inner1_solid = new G4IntersectionSolid( GetName()
            + "_inner1_solid", inner_solid, pInnerSep, NULL, innerSepDispl);
    inner2_solid = new G4SubtractionSolid( GetName()
            + "_inner2_solid", inner_solid, pInnerSep, NULL, innerSepDispl);
}




void NNVTMCPPMTManager::helper_make_logical_volume()
{
    if(m_UsePMTNaturalGeometry == false)
    {
        pmt_log = new G4LogicalVolume
            ( pmt_solid,
              GlassMat,
              GetName()+"_log" );

        body_log= new G4LogicalVolume
            ( body_solid,
              GlassMat,
              GetName()+"_body_log" );

        body_log->SetSensitiveDetector(m_detector);

        inner1_log= new G4LogicalVolume
            ( inner1_solid,
              PMT_Vacuum,
              GetName()+"_inner1_log" );
        inner1_log->SetSensitiveDetector(m_detector);

        inner2_log= new G4LogicalVolume
            ( inner2_solid,
              PMT_Vacuum,
              GetName()+"_inner2_log" );

        m_logical_pmt = pmt_log ;  
    }
    else
    {
        pmt_log = new G4LogicalVolume
            ( pmt_solid,
              GlassMat,
              GetName()+"_log" );

        pmt_log->SetSensitiveDetector(m_detector);
        // G4SteppingManager::Stepping only calls Hit/ProcessHits 
        // when the prestep point volume is sensitive so for 
        // sensitive boundary need to set two volumes as sensitive 

        inner_log= new G4LogicalVolume
            ( inner_solid,
              PMT_Vacuum,
              GetName()+"_inner_log" );

        inner_log->SetSensitiveDetector(m_detector);

        m_logical_pmt = pmt_log ;  
    }
}

void
NNVTMCPPMTManager::helper_make_physical_volume()
{
    
    G4ThreeVector equatorTranslation(0.,0.,m_z_equator);
    G4ThreeVector noTranslation(0.,0.,0.);

    if(m_UsePMTNaturalGeometry == false)
    {
        // place outer solids in envelope
        body_phys= new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,  // puts body equator in right place
              body_log,            // the logical volume
              GetName()+"_body_phys", // a name for this physical volume
              m_logical_pmt,                // the mother volume
              false,               // no boolean ops
              0 );                 // copy number

        // place inner solids in outer solid (vacuum)
        inner1_phys= new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,       // puts face equator in right place
              GetName()+"_inner1_phys",         // a name for this physical volume
              inner1_log,                    // the logical volume
              body_phys,           // the mother volume
              false,               // no boolean ops
              0 );                 // copy number
        inner2_phys= new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,       // puts face equator in right place
              GetName()+"_inner2_phys",         // a name for this physical volume
              inner2_log,                    // the logical volume
              body_phys,           // the mother volume
              false,               // no boolean ops
              0 );                 // copy number
    }
    else
    {
        inner_phys= new G4PVPlacement
            ( 0,                   // no rotation
              noTranslation,  // puts body equator in right place
              inner_log,            // the logical volume
              GetName()+"_inner_phys", // a name for this physical volume
              m_logical_pmt,                // the mother volume
              false,               // no boolean ops
              0 );                 // copy number

    }

}

void 
NNVTMCPPMTManager::helper_make_mcp_volume()
{
    G4LogicalVolume* parent_log = m_UsePMTNaturalGeometry ? inner_log : inner2_log ;
    G4PVPlacement*   parent_phys = m_UsePMTNaturalGeometry ? inner_phys : inner2_phys ;


    G4double thickness  = 1.*mm;
    G4double edge_hz    = 10.*mm;

    G4double mcp_r      = 20.0*mm;
    G4double mcp_hz     = 1.*mm;

    G4double plate_r    = 47.9*mm;
    G4double plate_hz   = 5.*mm;

    G4double tube_r     = plate_r-5.*mm;
    G4double tube_hz    = 60.0*mm;

    G4double shield_r   = 45.0*mm;
    G4double shield_hz  = 1.*mm;
    G4double shield_d   = 213.5*mm;

    G4double dist       = 116.*mm;

    G4VisAttributes *visAtt;
    G4bool fCheckOverlaps = false;
    /* MCP STRUCTURE */
    // PART I
    G4VSolid *edge_solid = new G4Tubs
        ( GetName() + "_edge_solid",
          plate_r - thickness,
          plate_r,
          edge_hz,
          0.,
          2.*M_PI );

    G4LogicalVolume *edge_log = new G4LogicalVolume
        ( edge_solid,
          DynodeMat,
          GetName() + "_edge_log" );

    visAtt = new G4VisAttributes(G4Color::Red());
    edge_log->SetVisAttributes(visAtt);

    G4PVPlacement *edge_phy = new G4PVPlacement
        ( 0,
          -G4ThreeVector(0., 0., dist-edge_hz),
          edge_log,
          GetName() + "_edge_phy",
          parent_log,
          false,
          0,
          fCheckOverlaps );

    // PART II
    G4VSolid *plate_solid = new G4Tubs
        ( GetName() + "_plate_solid",
          mcp_r,
          plate_r,
          plate_hz,
          0.,
          2.*M_PI );

    G4LogicalVolume *plate_log = new G4LogicalVolume
        ( plate_solid,
          DynodeMat,
          GetName() + "_plate_log" );

    plate_log->SetVisAttributes(visAtt);

    G4PVPlacement *plate_phy = new G4PVPlacement
        ( 0,
          -G4ThreeVector(0., 0., dist+plate_hz),
          plate_log,
          GetName() + "_plate_phy",
          parent_log,
          false,
          0,
          fCheckOverlaps );

    // PART III
    //    
    // Note:
    //     The backend of tube need to be cut when the PMT back is cut
    // 27th Oct 2021, Tao Lin
    //

    if (m_useRealSurface) {
        // the 5mm is the thickness of PMT
        double new_tube_hz = (m_pmt_h-m_z_equator-5.*mm-(dist+2*plate_hz))/2;
        LogInfo << "Option RealSurface is enabed. Reduce the height of tube_hz from "
                << tube_hz << " to " << new_tube_hz << std::endl;
        tube_hz = new_tube_hz; // reduce from 120mm to ~60mm. so the z will be at ~180mm.
    }
   
    G4VSolid *tube_solid = new G4Tubs
        ( GetName() + "_tube_solid",
          tube_r-thickness,
          tube_r,
          tube_hz,
          0.,
          2.*M_PI );

    G4LogicalVolume *tube_log = new G4LogicalVolume
        ( tube_solid,
          DynodeMat,
          GetName() + "_tube_log" );

    visAtt = new G4VisAttributes(G4Color::Yellow());
    tube_log->SetVisAttributes(visAtt);

    G4PVPlacement *tube_phy = new G4PVPlacement
        ( 0,
          -G4ThreeVector(0., 0., dist+plate_hz*2+tube_hz),
          tube_log,
          GetName() + "_tube_phy",
          parent_log,
          false,
          0,
          fCheckOverlaps );

    // PART VI

    G4VSolid *mcp_solid = new G4Tubs
        ( GetName() + "_mcp_solid",
          0.,
          mcp_r,
          mcp_hz,
          0.,
          2.*M_PI );

    G4LogicalVolume *mcp_log = new G4LogicalVolume
        ( mcp_solid,
          DynodeMat,
          GetName() + "_mcp_log" );

    visAtt = new G4VisAttributes(G4Color::Green());
    mcp_log->SetVisAttributes(visAtt);

    G4PVPlacement *mcp_phy = new G4PVPlacement
        ( 0,
          -G4ThreeVector(0., 0., dist+plate_hz*2+mcp_hz),
          mcp_log,
          GetName() + "_mcp_phy",
          parent_log,
          false,
          0,
          fCheckOverlaps );

    // PART V
    //
    // Note: when the PMT back is cut, just disable this part.
    // 27th Oct 2021, Tao Lin
    G4PVPlacement *shield_phy = nullptr;
    if (!m_useRealSurface) {
        G4VSolid *shield_solid = new G4Tubs
            ( GetName() + "_shield_solid",
              tube_r,
              shield_r,
              shield_hz,
              0.,
              2.*M_PI );

        G4LogicalVolume *shield_log = new G4LogicalVolume
            ( shield_solid,
              DynodeMat,
              GetName() + "_shield_log" );

        visAtt = new G4VisAttributes(G4Color::Blue());
        shield_log->SetVisAttributes(visAtt);

        shield_phy = new G4PVPlacement
            ( 0,
              -G4ThreeVector(0., 0., shield_d+shield_hz),
              shield_log,
              GetName() + "_shield_phy",
              parent_log,
              false,
              0,
              fCheckOverlaps );
    }

    /* OPTICAL SURFACE */
    // PLATE SURFACE
    G4MaterialPropertiesTable *plateSurfaceMPT = new G4MaterialPropertiesTable();
    plateSurfaceMPT->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
    plateSurfaceMPT->AddEntry("REFLECTIVITY", 1.55*eV, 0.65);
    plateSurfaceMPT->AddEntry("REFLECTIVITY", 15.5*eV, 0.65);

    G4OpticalSurface *plateOpSurface = new G4OpticalSurface("plateOpSurface");
    plateOpSurface->SetType(dielectric_metal);
    plateOpSurface->SetModel(glisur);
    plateOpSurface->SetPolish(0.999);
    plateOpSurface->SetMaterialPropertiesTable(plateSurfaceMPT);

    new G4LogicalBorderSurface(GetName()+"_mcp_plate_opsurface", parent_phys, plate_phy, plateOpSurface);

    // EDGE SURFACE
    G4MaterialPropertiesTable *edgeSurfaceMPT = new G4MaterialPropertiesTable();
    edgeSurfaceMPT->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
    edgeSurfaceMPT->AddEntry("REFLECTIVITY", 1.55*eV, 0.65);
    edgeSurfaceMPT->AddEntry("REFLECTIVITY", 15.5*eV, 0.65);
    
    G4OpticalSurface *edgeOpSurface = new G4OpticalSurface("edgeOpSurface");
    edgeOpSurface->SetType(dielectric_metal);
    edgeOpSurface->SetModel(glisur);
    edgeOpSurface->SetPolish(0.999);
    edgeOpSurface->SetMaterialPropertiesTable(edgeSurfaceMPT);

    new G4LogicalBorderSurface(GetName()+"_mcp_edge_opsurface", parent_phys, edge_phy, edgeOpSurface);

    // TUBE SURFACE
    G4MaterialPropertiesTable *tubeSurfaceMPT = new G4MaterialPropertiesTable();
    tubeSurfaceMPT->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
    tubeSurfaceMPT->AddEntry("REFLECTIVITY", 1.55*eV, 0.65);
    tubeSurfaceMPT->AddEntry("REFLECTIVITY", 15.5*eV, 0.65);
    
    G4OpticalSurface *tubeOpSurface = new G4OpticalSurface("tubeOpSurface");
    tubeOpSurface->SetType(dielectric_metal);
    tubeOpSurface->SetModel(glisur);
    tubeOpSurface->SetPolish(0.999);
    tubeOpSurface->SetMaterialPropertiesTable(tubeSurfaceMPT);

    new G4LogicalBorderSurface(GetName()+"_mcp_tube_opsurface", parent_phys, tube_phy, tubeOpSurface);

    // MCP SURFACE
    G4MaterialPropertiesTable *mcpSurfaceMPT = new G4MaterialPropertiesTable();
    mcpSurfaceMPT->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
    mcpSurfaceMPT->AddEntry("REFLECTIVITY", 1.55*eV, 0.);
    mcpSurfaceMPT->AddEntry("REFLECTIVITY", 15.5*eV, 0.);
    
    G4OpticalSurface *mcpOpSurface = new G4OpticalSurface("mcpOpSurface");
    mcpOpSurface->SetType(dielectric_metal);
    mcpOpSurface->SetModel(glisur);
    mcpOpSurface->SetPolish(0.999);
    mcpOpSurface->SetMaterialPropertiesTable(mcpSurfaceMPT);

    new G4LogicalBorderSurface(GetName()+"_mcp_opsurface", parent_phys, mcp_phy, mcpOpSurface);
    if (shield_phy) { // only if shield_phy exists, enable the OP surface
        new G4LogicalBorderSurface(GetName()+"_shield_opsurface", parent_phys, shield_phy, mcpOpSurface);
    }
}



/**
NNVTMCPPMTManager::helper_make_optical_surface
---------------------------------------------------


          m_UsePMTNaturalGeometry:false                                      m_UsePMTNaturalGeometry:true


     +---------------pmt-Pyrex----------------+                 +---------------pmt-Pyrex----------------+
     |                                        |                 |                                        |
     |                                        |                 |                                        |
     |     +----------body-Pyrex--------+     |                 |                                        |
     |     | +------------------------+ |     |                 |       +~inner~Vacuum~~~~~~~~~~~+       |
     |     | |                        | |     |                 |       !                        !       |
     |     | |                        | |     |                 |       !                        !       |
     |     | |        inner1-Vacuum   |-|     |                 |       !                        !       |
     |     | |                        |1e-3   |                 |       !                        !       |
     |     | |                        | |     |                 |       !                        !       |
     |     | +~~coincident~face~~~~~~~+ |     |                 |       +                        +       |
     |     | |                        | |     |                 |       |                        |       |
     |     | |                        | |     |                 |       |                        |       |
     |     | |        inner2-Vacuum   | |     |                 |       |                        |       |
     |     | |                        | |     |                 |       |                        |       |
     |     | |                        | |     |                 |       |                        |       |
     |     | +------------------------+ |     |                 |       +------------------------+       |
     |     +----------------------------+     |                 |                                        |
     |                                        |                 |                                        |
     |                                        |                 |                                        |
     +----------------------------------------+                 +----------------------------------------+


HMM: notice that the optical surface is not with the pmt-Pyrex but the body-Pyrex

* photocathode_logsurf1,2 : inner1_phys<->body_phys
* mirror_logsurf1,2       : inner2_phys<->body_phys


m_UsePMTNaturalGeometry:false
   Note that the G4LogicalBorderSurface properties are the same in both directions
   Ordinarily would use a SkinSurface when that is the case. 

m_UsePMTNaturalGeometry:true

   As the interface provides an unplaced LV (not a PV)
   it is not convenient to use G4LogicalBorderSurface
   when using very simple geometry without fakes as
   do not easily have a "pmt_phys".  
   But using unplaced G4LogicalSkinSurface does exactly the same thing more cheaply. 

   Notice that there is no prefix on the G4LogicalSurface that this method creates.
   Control of the CustomG4OpBoundaryProcess is done via prefixes on the 
   names of the G4OpticalSurface:m_mirror_opsurf NOT the G4LogicalSurface that contains 
   the optical surface.

   Implementation is a lot simpler that with unnatural geometry 
   because G4LogicalSkinSurface is used and because the different behavior for 
   the upper and lower hemispheres is provided by the CustomG4OpBoundaryProcess
 

Q: For m_UsePMTOpticalModel:true is photocathode_logsurf doing anything ?
A: NO, because it is within the FastSim region and the contorted ModelTrigger 
   is arranged to be true for intersections onto the photocathode surface 
   (body_phys<->inner1_phys) so FastSim is in control and the boundary process
   does not run when reaching that boundary. 

**/

void
NNVTMCPPMTManager::helper_make_optical_surface()
{
    if(m_UsePMTNaturalGeometry == false )
    {
        new G4LogicalBorderSurface(GetName()+"_photocathode_logsurf1",
                inner1_phys, body_phys,
                Photocathode_opsurf);
        new G4LogicalBorderSurface(GetName()+"_photocathode_logsurf2",
                body_phys, inner1_phys,
                Photocathode_opsurf);
        new G4LogicalBorderSurface(GetName()+"_mirror_logsurf1",
                inner2_phys, body_phys,
                m_mirror_opsurf);
        new G4LogicalBorderSurface(GetName()+"_mirror_logsurf2",
                body_phys, inner2_phys,
                m_mirror_opsurf);
    }
    else
    {
        new G4LogicalSkinSurface(GetName()+"_photocathode_mirror_logsurf", inner_log, m_mirror_opsurf); 
    }

}

void
NNVTMCPPMTManager::helper_fast_sim()
{
    assert( m_UsePMTOpticalModel == true && m_UsePMTNaturalGeometry == false );

    G4Region* body_region = new G4Region(this->GetName()+"_body_region");
    body_log->SetRegion(body_region);
    body_region->AddRootLogicalVolume(body_log);

    pmtOpticalModel = new junoPMTOpticalModel(GetName()+"_optical_model",
                                              body_phys, body_region);

#ifdef PMTSIM_STANDALONE
    const C4IPMTAccessor* accessor = PMTAccessor::Load("$PMTSimParamData_BASE/PMTSimParamData") ;
    pmtOpticalModel->setPMTAccessor(accessor); 
#else

    m_pmt_param_svc = 0;
    LogInfo << "Retrieving PMTParamSvc." << std::endl;
    SniperPtr<IPMTParamSvc> parsvc(*getParent(), "PMTParamSvc");
    if(parsvc.invalid()){
        LogError << "Can't get PMTParamSvc. We can't initialize PMT." << std::endl;
        assert(0);
        exit(EXIT_FAILURE);
    }else{
        LogInfo << "Retrieve PMTParamSvc successfully." << std::endl;
        m_pmt_param_svc = parsvc.data();
    }
    pmtOpticalModel->setPMTParamSvc(m_pmt_param_svc);

    m_pmt_sim_param_svc = 0;
    LogInfo << "Retrieving PMTParamSvc." << std::endl;
    SniperPtr<IPMTSimParamSvc> simsvc(*getParent(), "PMTSimParamSvc");
    if(simsvc.invalid()){
        LogError << "Can't get PMTSimParamSvc. We can't initialize PMT." << std::endl;
        assert(0);
        exit(EXIT_FAILURE);
    }else{
        LogInfo <<"Retrieve PMTSimParamSvc successfully." << std::endl;
        m_pmt_sim_param_svc = simsvc.data();
    }
    pmtOpticalModel->setPMTSimParamSvc(m_pmt_sim_param_svc);

    // We don't support the original PMT optical model in this new class.
    // new dywPMTOpticalModel( GetName()+"_optical_model", 
    //        body_phys, body_region);

#endif

}
void
NNVTMCPPMTManager::helper_vis_attr()
{  
    G4VisAttributes * visAtt;
    m_logical_pmt -> SetVisAttributes (G4VisAttributes::Invisible);
    // PMT glass
    // visAtt= new G4VisAttributes(G4Color(0.0,1.0,1.0,0.05));
    // body_log->SetVisAttributes( visAtt );
    if(body_log)
    body_log->SetVisAttributes( G4VisAttributes::Invisible );
    // dynode is medium gray
    visAtt= new G4VisAttributes(G4Color(0.5,0.5,0.5,1.0));
    //dynode_log->SetVisAttributes( visAtt );
    // (surface of) interior vacuum is clear orangish gray on top (PC),
    // silvery blue on bottom (mirror)
    // visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3,0.27));
    visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3, 1.0));
    visAtt -> SetForceSolid(true);

    if(inner1_log)
    inner1_log->SetVisAttributes (visAtt);
    // visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,0.67));
    visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,1.0));
    visAtt -> SetForceSolid(true);

    if(inner2_log)
    inner2_log->SetVisAttributes (visAtt);

}

void
NNVTMCPPMTManager::init_fast_cover() {
    // solid
    double thickness = 1E-3*mm;
    G4double zPlane[] = {
                        (m_z_equator-m_pmt_h)+thickness,
                        m_z_equator+thickness 
                        };
    G4double rInner[] = {0.,
                         0.};
    G4double rOuter[] = {m_pmt_r + thickness,
                         m_pmt_r + thickness};

    G4VSolid* cover_solid = new G4Polycone(
                                GetName()+"Tubs",
                                0,
                                360*deg,
                                2,
                                zPlane,
                                rInner,
                                rOuter
                                );
    // volume
    G4LogicalVolume* cover_lv = new G4LogicalVolume
        ( cover_solid,
          m_cover_mat,
          GetName()+"_cover");
    // place pmt into the cover
    G4ThreeVector noTranslation(0.,0.,0.);
    new G4PVPlacement
        (0,
         noTranslation,
         m_logical_pmt,
         GetName()+"_cover_phys",
         cover_lv,
         false,
         0
        );

    m_logical_pmt = cover_lv;
}

