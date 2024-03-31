#include <boost/python.hpp>
#include "Tub3inchPMTV3Manager.hh"
#include "dywPMTOpticalModel.hh"
#include "Tub3inchPMTV3Solid.hh"

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

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"
using namespace CLHEP;
DECLARE_TOOL(Tub3inchPMTV3Manager);

// Interface
G4LogicalVolume* 
Tub3inchPMTV3Manager::getLV() {
    if (!m_logical_pmt) {
        init();
    }
    return m_logical_pmt;
}

G4double
Tub3inchPMTV3Manager::GetPMTRadius() {
    if (!getLV()) {
        LogError << "Can't initialize PMT." << std::endl;;
    }
    return m_pmt_R;
}

G4double
Tub3inchPMTV3Manager::GetPMTHeight() {
    if (!getLV()) {
        LogError << "Can't initialize PMT." << std::endl;;
    }
    return m_pmt_H - m_cntr_Z2;
}

G4double
Tub3inchPMTV3Manager::GetZEquator() {
    if (!getLV()) {
        LogError << "Can't initialize PMT." << std::endl;;
    }
    return m_z_equator;
}

G4ThreeVector
Tub3inchPMTV3Manager::GetPosInPMT() {
    G4ThreeVector rndm_pos;
    return rndm_pos;
}

// Constructor
Tub3inchPMTV3Manager::Tub3inchPMTV3Manager
    (const G4String& plabel
    )
    : ToolBase(plabel), m_label(plabel),
      m_pmtsolid_maker(0),
      pmt_solid(NULL), body_solid(NULL), inner_solid(NULL),
      inner1_solid(NULL), inner2_solid(NULL), cntr_solid(NULL),
      body_log(NULL), inner1_log(NULL), inner2_log(NULL), cntr_log(NULL),
      body_phys(NULL), inner1_phys(NULL), inner2_phys(NULL), cntr_phys(NULL),
      m_logical_pmt(NULL), m_mirror_opsurf(NULL), m_absorb_opsurf(NULL),
      ExteriorMat(0), GlassMat(0),
      Photocathode_opsurf(0),
      PMT_Vacuum(0), DynodeMat(0),
      MaskMat(0), m_detector(0),
      m_logical_cover(NULL)
{

}

Tub3inchPMTV3Manager::~Tub3inchPMTV3Manager() {
    if (m_pmtsolid_maker) {
        delete m_pmtsolid_maker;
    }
}
    

// Helper Methods
void
Tub3inchPMTV3Manager::init() {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    m_detector = SDman->FindSensitiveDetector("PMTSDMgr");
    assert(m_detector);
    init_material();
    // construct
    // * construct a mirror surface
    init_variables();
    init_mirror_surface();
    init_absorb_surface();
    init_pmt();
}

void
Tub3inchPMTV3Manager::init_material() {
     // note: outer virtual volume is used to speed up detector geometry construction.
     ExteriorMat = G4Material::GetMaterial("Water");

     GlassMat = G4Material::GetMaterial("Pyrex");
     PMT_Vacuum = G4Material::GetMaterial("Vacuum"); 
     DynodeMat = G4Material::GetMaterial("Steel");
     MaskMat = G4Material::GetMaterial("Steel");

     Photocathode_opsurf =  new G4OpticalSurface("Photocathode_opsurf_3inch");
     Photocathode_opsurf->SetType(dielectric_metal); // ignored if RINDEX defined
     Photocathode_opsurf->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode_3inch")->GetMaterialPropertiesTable() );
}


void
Tub3inchPMTV3Manager::init_variables() {
    m_pmt_R = 40.*mm;
    m_pmt_H = 24.*mm;
    m_cntr_R = 30.*mm;
    m_cntr_H = 60.*mm;
    m_cntr_Z1 = -1.*m_pmt_H*sqrt(1-m_cntr_R*m_cntr_R/m_pmt_R/m_pmt_R);
    m_cntr_Z2 = m_cntr_Z1 - m_cntr_H;
    m_glass_thickness = 2.*mm;
    m_photocathode_R = 36.*mm;
    G4double R_pc = m_photocathode_R;
    G4double R_in = m_pmt_R - m_glass_thickness;
    G4double H_in = m_pmt_H - m_glass_thickness;
    m_photocathode_Z = H_in*sqrt(1-R_pc*R_pc/R_in/R_in);

    m_pmtsolid_maker = new Tub3inchPMTV3Solid(
                            m_pmt_R,
                            m_pmt_H,
                            m_cntr_R,
                            m_cntr_Z1,
                            m_cntr_Z2
                            );
}

void
Tub3inchPMTV3Manager::init_mirror_surface() {
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
}

void
Tub3inchPMTV3Manager::init_absorb_surface() {
    if ( m_absorb_opsurf == NULL ) {
        // construct a static absorbant surface with idealized properties
        m_absorb_opsurf =  new G4OpticalSurface("Absorb_opsurf");
        m_absorb_opsurf->SetModel(glisur); 
        m_absorb_opsurf->SetType(dielectric_metal); 
        G4MaterialPropertiesTable* propAbsorb= NULL;
        if ( propAbsorb == NULL ) {
            propAbsorb= new G4MaterialPropertiesTable();
            propAbsorb->AddProperty("REFLECTIVITY", new G4MaterialPropertyVector());
            propAbsorb->AddEntry("REFLECTIVITY", 1.55*eV, 0.0);
            propAbsorb->AddEntry("REFLECTIVITY", 15.5*eV, 0.0);
        }
        m_absorb_opsurf->SetMaterialPropertiesTable( propAbsorb );
    }
}

void
Tub3inchPMTV3Manager::init_pmt() {
    // Refer to dyw_PMT_LogicalVolume.cc
    //

    ConstructPMT_UsingTorusStack ();

}

void
Tub3inchPMTV3Manager::
ConstructPMT_UsingTorusStack()
{
  ////////////////////////////////////////////////////////////////
  // MAKE SOLIDS
  ////
  G4cout << "\n   Making solids" << G4endl;
  helper_make_solid();  

  ////////////////////////////////////////////////////////////////
  // MAKE LOGICAL VOLUMES (add materials)
  ////
  G4cout << "\n   Making logical volumes" << G4endl;
  helper_make_logical_volume();
  
  
  ////////////////////////////////////////////////////////////////
  // MAKE PHYSICAL VOLUMES (place logical volumes)
  ////
  G4cout << "\n   Making physical volumes" << G4endl;
  helper_make_physical_volume();

  ////////////////////////////////////////////////////////////////
  // Attach optical surfaces to borders
  ////
  G4cout << "\n   Making optical surfaces" << G4endl;
  helper_make_optical_surface();

  ////////////////////////////////////////////////////////////////
  // FastSimulationModel
  ////
  G4cout << "\n   Fast simulation model" << G4endl;
  helper_fast_sim();
  
  ////////////////////////////////////////////////////////////////
  // Set colors and visibility
  ////
  G4cout << "\n   Visual attributes" << G4endl;
  helper_vis_attr();
}

void 
Tub3inchPMTV3Manager::helper_make_solid() 
{
    pmt_solid = m_pmtsolid_maker->GetUnionSolid(GetName() + "_pmt_solid", 1.e-3*mm);
    body_solid = m_pmtsolid_maker->GetEllipsoidSolid(GetName() + "_body_solid", 0.);
    inner1_solid = m_pmtsolid_maker->GetEllipsoidSolid(GetName()+"_inner1_solid", m_pmt_H, m_photocathode_Z, -1.*m_glass_thickness);
    inner2_solid = m_pmtsolid_maker->GetEllipsoidSolid(GetName()+"_inner2_solid", m_photocathode_Z, m_cntr_Z1, -1.*m_glass_thickness);
    cntr_solid = m_pmtsolid_maker->GetContainerSolid(GetName()+"_cntr_solid", -1.e-3*mm);
}

void
Tub3inchPMTV3Manager::helper_make_logical_volume()
{
    body_log= new G4LogicalVolume
        ( body_solid,
          GlassMat,
          GetName()+"_body_log" );

    m_logical_pmt = new G4LogicalVolume
        ( pmt_solid,
          ExteriorMat,
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

    assert(MaskMat);
    cntr_log= new G4LogicalVolume
        ( cntr_solid,
          MaskMat,
          GetName()+"_cntr_log" );

}

void
Tub3inchPMTV3Manager::helper_make_physical_volume()
{
    
    G4ThreeVector noTranslation(0.,0.,0.);

    // place outer solids in envelope
    assert(body_log);
    assert(m_logical_pmt);
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

    cntr_phys= new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          cntr_log,                    // the logical volume
          GetName()+"_cntr_phys",         // a name for this physical volume
          m_logical_pmt,           // the mother volume
          false,               // no boolean ops
          0 );                 // copy number

}

void
Tub3inchPMTV3Manager::helper_make_optical_surface()
{
    // inner1 / body
    new G4LogicalBorderSurface(GetName()+"_photocathode_logsurf1",
            inner1_phys, body_phys,
            Photocathode_opsurf);
    new G4LogicalBorderSurface(GetName()+"_photocathode_logsurf2",
            body_phys, inner1_phys,
            Photocathode_opsurf);

    // inner2 / body
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf1",
            inner2_phys, body_phys,
            m_absorb_opsurf);
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf2",
            body_phys, inner2_phys,
            m_absorb_opsurf);

    // container / body
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf3",
            cntr_phys, body_phys,
            m_absorb_opsurf);
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf4",
            body_phys, cntr_phys,
            m_absorb_opsurf);

    // inner1 / inner2
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf5",
            inner1_phys, inner2_phys,
            m_absorb_opsurf);
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf6",
            inner2_phys, inner1_phys,
            m_absorb_opsurf);

    // inner2 / container
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf7",
            cntr_phys, inner2_phys,
            m_absorb_opsurf);
    new G4LogicalBorderSurface(GetName()+"_absorb_logsurf8",
            inner2_phys, cntr_phys,
            m_absorb_opsurf);

}

void
Tub3inchPMTV3Manager::helper_fast_sim()
{
    G4Region* body_region = new G4Region(this->GetName()+"_body_region");
    body_region->AddRootLogicalVolume(body_log);
    // new dywPMTOpticalModel( GetName()+"_optical_model", 
    //         body_phys, body_region);
}
void
Tub3inchPMTV3Manager::helper_vis_attr()
{
    G4VisAttributes * visAtt;
    visAtt = new G4VisAttributes(G4Color(0.0,0.0,1.0,0.05));
    visAtt->SetForceSolid(true);
    m_logical_pmt->SetVisAttributes(visAtt);
//    m_logical_pmt->SetVisAttributes (G4VisAttributes::Invisible);
    // PMT glass
    visAtt = new G4VisAttributes(G4Color(0.0,1.0,1.0,0.5));
    visAtt->SetForceSolid(true);
    body_log->SetVisAttributes( visAtt );
    // body_log->SetVisAttributes( G4VisAttributes::Invisible );

    visAtt = new G4VisAttributes(G4Color(0.7,0.5,0.3,0.27));
    visAtt->SetForceSolid(true);
    inner1_log->SetVisAttributes (visAtt);

    visAtt = new G4VisAttributes(G4Color(0.4,0.3,0.8,0.05));
    visAtt->SetForceSolid(true);
    inner2_log->SetVisAttributes (visAtt);

    visAtt = new G4VisAttributes(G4Color(0.6,0.7,0.8,0.67));
    visAtt->SetForceSolid(true);
    cntr_log->SetVisAttributes(visAtt);

}
