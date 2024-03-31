
#include "Ham8inchPMTManager.hh"
#include "dywPMTOpticalModel.hh"
#include "R12860_PMTSolid.hh"
// #include "RandomPositionGen.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh" // for boolean solids
#include "G4SubtractionSolid.hh" // for boolean solids
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh" // for G4VisAttributes::Invisible
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4SDManager.hh"
#include "G4Polycone.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"

using namespace CLHEP;

DECLARE_TOOL(Ham8inchPMTManager);

// Interface
G4LogicalVolume* 
Ham8inchPMTManager::getLV() {
    if (!m_logical_pmt) {
        init();
    }
    return m_logical_pmt;
}

G4double
Ham8inchPMTManager::GetPMTRadius() {
    return m_pmt_r;
}

G4double
Ham8inchPMTManager::GetPMTHeight() {
    return m_pmt_h;
}

G4double
Ham8inchPMTManager::GetZEquator() {
    return m_z_equator;
}

G4ThreeVector
Ham8inchPMTManager::GetPosInPMT() {
    return G4ThreeVector();
}

// Constructor
Ham8inchPMTManager::Ham8inchPMTManager
    (const G4String& plabel // label -- subvolume names are derived from this
    )
    : ToolBase(plabel), m_label(plabel),
      m_pmtsolid_maker(0),
      pmt_solid(NULL), body_solid(NULL), inner_solid(NULL),
      inner1_solid(NULL), inner2_solid(NULL), innerRe_solid(NULL), innerRe2_solid(NULL), dynode_solid(NULL),
      body_log(NULL), inner1_log(NULL), inner2_log(NULL), innerRe_log(NULL), innerRe2_log(NULL), dynode_log(NULL),
      body_phys(NULL), inner1_phys(NULL), inner2_phys(NULL), innerRe_phys(NULL), innerRe2_phys(NULL),
      dynode_phys(NULL), m_logical_pmt(NULL), m_mirror_opsurf(NULL), m_Remirror_opsurf(NULL),
      Photocathode_opsurf(NULL),
      GlassMat(NULL), PMT_Vacuum(NULL), DynodeMat(NULL),
      MaskMat(NULL), m_detector(NULL),
      m_logical_cover(NULL), m_cover_mat(NULL)
{
    declProp("FastCover", m_fast_cover=false);
    declProp("FastCoverMaterial", m_cover_mat_str="Water");
}

Ham8inchPMTManager::~Ham8inchPMTManager() {
    if (m_pmtsolid_maker) {
        delete m_pmtsolid_maker;
    }
}
    

// Helper Methods
void
Ham8inchPMTManager::init() {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    m_detector = SDman->FindSensitiveDetector("PMTSDMgr");
    assert(m_detector);
    // construct
    init_material();
    // * construct a mirror surface
    init_variables();
    init_mirror_surface();
    init_pmt();

    if (m_fast_cover) {
        init_cover();
    }
}

void
Ham8inchPMTManager::init_material() {

     GlassMat = G4Material::GetMaterial("Pyrex");
     PMT_Vacuum = G4Material::GetMaterial("Vacuum"); 
     DynodeMat = G4Material::GetMaterial("Steel");

     Photocathode_opsurf =  new G4OpticalSurface("Photocathode_opsurf");
     Photocathode_opsurf->SetType(dielectric_metal); // ignored if RINDEX defined
     Photocathode_opsurf->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode_Ham8inch")->GetMaterialPropertiesTable() );

     if (m_fast_cover) {
         m_cover_mat = G4Material::GetMaterial(m_cover_mat_str);
         assert(m_cover_mat);
     }
}

void
Ham8inchPMTManager::init_variables() {
    m_pmt_r = 101.*mm;
    m_pmt_h = 220.*mm;
    m_z_equator = 75.5*mm; // From top to equator

    G4cout << __FILE__ << ":" << __LINE__ << std::endl;
    m_pmtsolid_maker = new R12860_PMTSolid(
                            m_pmt_r,
                            m_z_equator,
                            23*mm,
                            42.5*mm,
                            m_pmt_h,
                            62.*mm
                            );
}

void
Ham8inchPMTManager::init_mirror_surface() {
    if ( m_mirror_opsurf == NULL ) {
        // construct a static mirror surface with idealized properties
        m_mirror_opsurf =  new G4OpticalSurface("Mirror_opsurf");
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
            propMirror->AddEntry("REFLECTIVITY", 1.55*eV, 0.9999);
            propMirror->AddEntry("REFLECTIVITY", 15.5*eV, 0.9999);
        }
        m_mirror_opsurf->SetMaterialPropertiesTable( propMirror );

    }
   if ( m_Remirror_opsurf == NULL ) {
        // construct a static mirror surface with idealized properties
        m_Remirror_opsurf =  new G4OpticalSurface("Mirror_opsurf");
        m_Remirror_opsurf->SetFinish(polishedfrontpainted); // needed for mirror
        m_Remirror_opsurf->SetModel(glisur);
        m_Remirror_opsurf->SetType(dielectric_metal);
        m_Remirror_opsurf->SetPolish(0.999);              // a guess -- FIXME
        G4MaterialPropertiesTable* propReMirror= NULL;
        G4Material *matReMirror = G4Material::GetMaterial("PMT_ReMirror");
        if (matReMirror) {
            propReMirror= matReMirror->GetMaterialPropertiesTable();
        }
        if ( propReMirror == NULL ) {
            G4cout << "Warning: setting PMT mirror reflectivity to 0.9999 "
                   << "because no PMT_Mirror material properties defined" << G4endl;
            propReMirror= new G4MaterialPropertiesTable();
            propReMirror->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
            //propMirror->AddEntry("REFLECTIVITY", twopi*hbarc/(800.0e-9*m), 0.9999);
            //propMirror->AddEntry("REFLECTIVITY", twopi*hbarc/(200.0e-9*m), 0.9999);
            propReMirror->AddEntry("REFLECTIVITY", 1.55*eV, 0.);
            propReMirror->AddEntry("REFLECTIVITY", 15.5*eV, 0.);
        }
        m_Remirror_opsurf->SetMaterialPropertiesTable( propReMirror );

    }
}

void
Ham8inchPMTManager::init_pmt() {
    // Refer to dyw_PMT_LogicalVolume.cc
    //

    ConstructPMT_UsingTorusStack ();

}

void
Ham8inchPMTManager::
ConstructPMT_UsingTorusStack()
{
  ////////////////////////////////////////////////////////////////
  // MAKE SOLIDS
  ////
  helper_make_solid();  

  ////////////////////////////////////////////////////////////////
  // MAKE LOGICAL VOLUMES (add materials)
  ////
  helper_make_logical_volume();
  
  
  ////////////////////////////////////////////////////////////////
  // MAKE PHYSICAL VOLUMES (place logical volumes)
  ////
  // TODO: face of tube 100 um from front of cylinder

  helper_make_physical_volume();

  ////////////////////////////////////////////////////////////////
  // Attach optical surfaces to borders
  ////
  helper_make_optical_surface();
  ////////////////////////////////////////////////////////////////
  // FastSimulationModel
  ////
  helper_fast_sim();
  
  ////////////////////////////////////////////////////////////////
  // Set colors and visibility
  ////
  helper_vis_attr();
}

void 
Ham8inchPMTManager::helper_make_solid() 
{
    pmt_solid = m_pmtsolid_maker->GetSolid(GetName() + "_pmt_solid", 1E-3*mm);
    body_solid = m_pmtsolid_maker->GetSolid(GetName() + "_body_solid");
    inner_solid= m_pmtsolid_maker->GetSolid(GetName()+"_inner_solid", -5*mm);

    G4double helper_sep_tube_r = m_pmt_r;
    G4double helper_sep_tube_h = m_z_equator;
    G4double helper_sep_tube_hh = helper_sep_tube_h/2;

    //G4double helper_sep_tube_r = 95.*mm;
    //G4double helper_sep_tube_h = 49.87*mm;
    //G4double helper_sep_tube_hh = m_z_equator-helper_sep_tube_h/2;

    // TODO: check the UNIT?

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
    /*
    G4double helper_Re_tube_r = m_pmt_r;
    G4double helper_Re_tube_h = m_z_equator-helper_sep_tube_h;
    G4double helper_Re_tube_hh = helper_Re_tube_h/2;
    G4VSolid * pInnerRe = new G4Tubs("Inner_Reflect",
            0.,
            helper_Re_tube_r+1E-9*mm,
            helper_Re_tube_hh+1E-9*mm,
            0.,360.*degree);
    G4ThreeVector innerDispl(0.,0.,helper_Re_tube_hh-1E-9*mm);
    innerRe_solid = new G4IntersectionSolid( GetName()
            + "_innerRe_solid", inner2_solid, pInnerRe, NULL, innerDispl);
    innerRe2_solid = new G4SubtractionSolid( GetName()
            + "_innerRe2_solid", inner2_solid, pInnerRe, NULL, innerDispl);    
    */

    // dynode volume
    //hh_dynode= (this->z_dynode - z_lowest_dynode)/2.0;
    //dynode_solid= new G4Tubs
    //    ( GetName()+"_dynode_solid",
    //      0.0, r_dynode,          // solid cylinder (fixme?)
    //      hh_dynode,              // half height of cylinder
    //      0., 2.*M_PI );          // cylinder complete in phi

}

void
Ham8inchPMTManager::helper_make_logical_volume()
{
    body_log= new G4LogicalVolume
        ( body_solid,
          GlassMat,
          GetName()+"_body_log" );

    m_logical_pmt = new G4LogicalVolume
        ( pmt_solid,
          GlassMat,
          GetName()+"_log" );

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
/*
    innerRe_log= new G4LogicalVolume
        ( innerRe_solid,
          PMT_Vacuum,
          GetName()+"_innerRe_log" );
    innerRe2_log= new G4LogicalVolume
        ( innerRe2_solid,
          PMT_Vacuum,
          GetName()+"_innerRe2_log" );
*/
    /*
    dynode_log= new G4LogicalVolume
        ( dynode_solid,
          Dynode_mat,
          GetName()+"_dynode_log" );
          */

}

void
Ham8inchPMTManager::helper_make_physical_volume()
{
    
    G4ThreeVector equatorTranslation(0.,0.,m_z_equator);
    G4ThreeVector noTranslation(0.,0.,0.);

    // place outer solids in envelope
    body_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,  // puts body equator in right place
          body_log,            // the logical volume
          GetName()+"_body_phys", // a name for this physical volume
          m_logical_pmt,                // the mother volume
          false,               // no boolean ops
          0 );                 // copy number
    G4cout<< " Active PMT_body_phys the overlapping=" << body_phys->CheckOverlaps()<<G4endl;
    // place inner solids in outer solid (vacuum)
    inner1_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          GetName()+"_inner1_phys",         // a name for this physical volume
          inner1_log,                    // the logical volume
          body_phys,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number
    G4cout<< " Active PMT_inner1_phys the overlapping=" << inner1_phys->CheckOverlaps()<<G4endl;
    inner2_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          GetName()+"_inner2_phys",         // a name for this physical volume
          inner2_log,                    // the logical volume
          body_phys,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number
    G4cout<< " Active PMT_inner2_phys the overlapping=" << inner2_phys->CheckOverlaps()<<G4endl;
/*
    innerRe_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          GetName()+"_innerRe_phys",         // a name for this physical volume
          innerRe_log,                    // the logical volume
          body_phys,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number
    G4cout<< " Active PMT_innerRe_phys the overlapping=" << innerRe_phys->CheckOverlaps()<<G4endl;
    innerRe2_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          GetName()+"_innerRe2_phys",         // a name for this physical volume
          innerRe2_log,                    // the logical volume
          body_phys,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number
    G4cout<< " Active PMT_innerRe2_phys the overlapping=" << innerRe2_phys->CheckOverlaps()<<G4endl;
*/
     /*
    // place dynode in stem/back
    dynode_phys = new G4PVPlacement
        ( 0,
          G4ThreeVector(0.0, 0.0, this->z_dynode - hh_dynode),
          GetName()+"_dynode_phys",
          dynode_log,
          inner2_phys,
          false,
          0 );

          */
}

void
Ham8inchPMTManager::helper_make_optical_surface()
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
/*
    new G4LogicalBorderSurface(GetName()+"_Remirror_logsurf1",
            innerRe_phys, body_phys,
            m_Remirror_opsurf);
    new G4LogicalBorderSurface(GetName()+"_Remirror_logsurf2",
            body_phys, innerRe_phys,
            m_Remirror_opsurf);
    new G4LogicalBorderSurface(GetName()+"_mirror_logsurf1",
            innerRe2_phys, body_phys,
            m_mirror_opsurf);
    new G4LogicalBorderSurface(GetName()+"_mirror_logsurf2",
            body_phys, innerRe2_phys,
            m_mirror_opsurf);
*/
}

void
Ham8inchPMTManager::helper_fast_sim()
{
    G4Region* body_region = new G4Region(this->GetName()+"_body_region");
    body_region->AddRootLogicalVolume(body_log);
    // new dywPMTOpticalModel( GetName()+"_optical_model", 
    //         body_phys, body_region);

}
void
Ham8inchPMTManager::helper_vis_attr()
{  
    G4VisAttributes * visAtt;
    m_logical_pmt -> SetVisAttributes (G4VisAttributes::Invisible);
    // PMT glass
    // visAtt= new G4VisAttributes(G4Color(0.0,1.0,1.0,0.05));
    // body_log->SetVisAttributes( visAtt );
    body_log->SetVisAttributes( G4VisAttributes::Invisible );
    // dynode is medium gray
    visAtt= new G4VisAttributes(G4Color(0.5,0.5,0.5,1.0));
    //dynode_log->SetVisAttributes( visAtt );
    // (surface of) interior vacuum is clear orangish gray on top (PC),
    // silvery blue on bottom (mirror)
    visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3,0.27));
    visAtt -> SetForceSolid(true);
    inner1_log->SetVisAttributes (visAtt);
    visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,0.67));
    visAtt -> SetForceSolid(true);
    inner2_log->SetVisAttributes (visAtt);
    //visAtt= new G4VisAttributes(G4Color(0.,1.,0.,0.67));
    //visAtt -> SetForceSolid(true);
    //innerRe_log->SetVisAttributes (visAtt);
    //visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,0.67));
    //visAtt -> SetForceSolid(true);
    //innerRe2_log->SetVisAttributes (visAtt);
}

void
Ham8inchPMTManager::init_cover() {
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
