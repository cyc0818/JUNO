#include <boost/python.hpp>
#include "R12199PMTManager.hh"
#include "R12199_PMTSolid.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh" // for boolean solids
#include "G4SubtractionSolid.hh" // for boolean solids

#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh" // for G4VisAttributes::Invisible

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(R12199PMTManager);

R12199PMTManager::R12199PMTManager(const G4String& plabel)
    : ToolBase(plabel), m_logical_pmt(0), GlassMat(0), PMT_Vacuum(0)
{

}

R12199PMTManager::~R12199PMTManager() {

}

// Interface
G4LogicalVolume* 
R12199PMTManager::getLV() {
    LogInfo << __LINE__ << std::endl;
    if (!m_logical_pmt) {
        init();
    }
    LogInfo << __LINE__ << std::endl;
    return m_logical_pmt;
}

G4double
R12199PMTManager::GetPMTRadius() {
    return 40.*mm;
}

G4double
R12199PMTManager::GetPMTHeight() {
    return 110.*mm;
}

G4double
R12199PMTManager::GetZEquator() {
    return 22.5*mm;
}

G4ThreeVector
R12199PMTManager::GetPosInPMT() {
    // not used.
    return G4ThreeVector();
}

void
R12199PMTManager::init() {
    LogInfo << __LINE__ << std::endl;
    init_material();
    LogInfo << __LINE__ << std::endl;
    init_pmt();
    LogInfo << __LINE__ << std::endl;
}

void
R12199PMTManager::init_material() {
     GlassMat = G4Material::GetMaterial("Pyrex");
     PMT_Vacuum = G4Material::GetMaterial("Vacuum"); 
}

void
R12199PMTManager::init_pmt() {

    ////////////////////////////////////////////////////////////////
    // MAKE SOLIDS
    ////
    R12199_PMTSolid* pmtsolid_maker = new R12199_PMTSolid();
    LogInfo << __LINE__ << std::endl;
    G4VSolid* pmt_solid  = pmtsolid_maker->GetSolid(objName()+"_pmt_solid", 1E-3*mm);
    G4VSolid* body_solid = pmtsolid_maker->GetSolid(objName()+"_body_solid");
    G4VSolid* inner_solid= pmtsolid_maker->GetSolid(objName()+"_inner_solid", -1*mm);
    LogInfo << __LINE__ << std::endl;

    G4double helper_sep_tube_r = GetPMTRadius();
    G4double helper_sep_tube_h = GetZEquator();
    G4double helper_sep_tube_hh = helper_sep_tube_h/2; // half

    G4VSolid * pInnerSep = new G4Tubs(objName()+"_inner_separator",
            0.,
            helper_sep_tube_r+1E-9*mm,
            helper_sep_tube_hh+1E-9*mm,
            0.,360.*degree);
    G4ThreeVector innerSepDispl(0.,0.,helper_sep_tube_hh-1E-9*mm);
    G4VSolid* inner1_solid = new G4IntersectionSolid( objName()
            + "_inner1_solid", inner_solid, pInnerSep, NULL, innerSepDispl);
    G4VSolid* inner2_solid = new G4SubtractionSolid( objName()
            + "_inner2_solid", inner_solid, pInnerSep, NULL, innerSepDispl);

    ////////////////////////////////////////////////////////////////
    // MAKE LOGICAL VOLUMES (add materials)
    ////
    G4LogicalVolume* pmt_log = new G4LogicalVolume
        ( pmt_solid,
          GlassMat,
          objName()+"_log" );

    G4LogicalVolume* body_log= new G4LogicalVolume
        ( body_solid,
          GlassMat,
          objName()+"_body_log" );

    G4LogicalVolume* inner1_log= new G4LogicalVolume
        ( inner1_solid,
          PMT_Vacuum,
          objName()+"_inner1_log" );

    G4LogicalVolume* inner2_log= new G4LogicalVolume
        ( inner2_solid,
          PMT_Vacuum,
          objName()+"_inner2_log" );

    ////////////////////////////////////////////////////////////////
    // MAKE PHYSICAL VOLUMES (place logical volumes)
    ////
    G4ThreeVector noTranslation(0.,0.,0.);

    // body
    G4VPhysicalVolume* body_phys = new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          body_log,          // the logical volume
          objName()+"_body_phys", // a name for this physical volume
          pmt_log,            // the mother volume
          false,               // no boolean ops
          0 );                 // copy number;

    // inner 1
    G4VPhysicalVolume* inner1_phys = new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          inner1_log,          // the logical volume
          objName()+"_inner1_phys", // a name for this physical volume
          body_log,            // the mother volume
          false,               // no boolean ops
          0 );                 // copy number;

    // inner 2
    G4VPhysicalVolume* inner2_phys = new G4PVPlacement
        ( 0,                   // no rotation
          noTranslation,       // puts face equator in right place
          inner2_log,          // the logical volume
          objName()+"_inner2_phys", // a name for this physical volume
          body_log,            // the mother volume
          false,               // no boolean ops
          0 );                 // copy number;

    ////////////////////////////////////////////////////////////////
    // Attach optical surfaces to borders
    ////
    G4OpticalSurface* Photocathode_opsurf = 0;
    G4OpticalSurface* m_mirror_opsurf = 0;

    Photocathode_opsurf =  new G4OpticalSurface("Photocathode_opsurf");
    Photocathode_opsurf->SetType(dielectric_metal); // ignored if RINDEX defined
    Photocathode_opsurf->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode")->GetMaterialPropertiesTable() );
     
    if (!m_mirror_opsurf) {
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



    new G4LogicalBorderSurface(objName()+"_photocathode_logsurf1",
            inner1_phys, body_phys,
            Photocathode_opsurf);
    new G4LogicalBorderSurface(objName()+"_photocathode_logsurf2",
            body_phys, inner1_phys,
            Photocathode_opsurf);
    new G4LogicalBorderSurface(objName()+"_mirror_logsurf1",
            inner2_phys, body_phys,
            m_mirror_opsurf);
    new G4LogicalBorderSurface(objName()+"_mirror_logsurf2",
            body_phys, inner2_phys,
            m_mirror_opsurf);

    ////////////////////////////////////////////////////////////////
    // GET and ATTACH SENSITIVE DETECTOR 
    ////
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4VSensitiveDetector* m_detector = SDman->FindSensitiveDetector("PMTSDMgr");
    assert(m_detector);

    body_log->SetSensitiveDetector(m_detector);
    inner1_log->SetSensitiveDetector(m_detector);
  
    ////////////////////////////////////////////////////////////////
    // Set colors and visibility
    ////
    G4VisAttributes* visAtt = 0;
    visAtt= new G4VisAttributes(G4Color(0.7,0.5,0.3,1.00));
    visAtt -> SetForceSolid(true);
    inner1_log->SetVisAttributes (visAtt);
    visAtt= new G4VisAttributes(G4Color(0.6,0.7,0.8,0.99));
    visAtt -> SetForceSolid(true);
    inner2_log->SetVisAttributes (visAtt);

    m_logical_pmt = pmt_log;
}

