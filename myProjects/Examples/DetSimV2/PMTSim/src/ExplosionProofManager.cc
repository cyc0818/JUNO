
#include "ExplosionProofManager.hh"
#include "dywPMTOpticalModel.hh"
#include "ExplosionProofSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4IntersectionSolid.hh" // for boolean solids
#include "G4SubtractionSolid.hh" // for boolean solids
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh" // for G4VisAttributes::Invisible
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

using namespace CLHEP;
// Interface
G4LogicalVolume* 
ExplosionProofManager::GetLogicalPMT() {
    return m_logical_pmt;
}

G4double
ExplosionProofManager::GetPMTRadius() {
    return m_pmt_r;
}

G4double
ExplosionProofManager::GetPMTHeight() {
    return m_pmt_h;
}

G4double
ExplosionProofManager::GetZEquator() {
    return m_z_equator;
}

G4ThreeVector
ExplosionProofManager::GetPosInPMT() {
  return m_pmt_inner->GetPosInPMT();
}

// Constructor
ExplosionProofManager::ExplosionProofManager
    (const G4String& plabel, 
     IPMTManager* pmt_inner,
     G4Material* inner_material ,
     G4Material* outer_top_material, 
     G4Material* outer_bottom_material
    ): IPMTCoverManager(pmt_inner)
       , m_label(plabel)
       , m_logical_pmt(NULL)
       , m_inner_material(inner_material)
       , m_outer_top_material(outer_top_material)
       , m_outer_bottom_material(outer_bottom_material)
       , m_explosionproof_maker(NULL)
{

  if (pmt_inner) {
    initialize();
  } else {
    assert(pmt_inner);
  }

}

ExplosionProofManager::~ExplosionProofManager() {
    if (m_explosionproof_maker) {
        delete m_explosionproof_maker;
    }
}
    

// Helper Methods
void
ExplosionProofManager::initialize() {
    init_variables();
    init_cover();
}

void
ExplosionProofManager::init_variables() {
    m_pmt_r = 254.*mm;
    m_pmt_h = 700.*mm;
    m_z_equator = m_pmt_r; // From top to equator

    m_explosionproof_maker = new ExplosionProofSolid(
                            m_pmt_r,
                            100.0*mm,
                            50.0*mm,
                            m_pmt_h
                            );
}

void
ExplosionProofManager::init_cover() {

    G4VSolid* cover_out_top_solid=NULL;
    G4VSolid* cover_out_bottom_solid=NULL;
    G4VSolid* cover_in_top_solid=NULL;
    G4VSolid* cover_in_bottom_solid=NULL;
    G4VSolid* cover_out_solid=NULL;

    cover_out_top_solid = m_explosionproof_maker->GetSolidTop(GetName() + "_Cov_out_top",10.5*mm);
    cover_in_top_solid = m_explosionproof_maker->GetSolidTop(GetName() + "_Cov_in_top",0.5*mm);
    cover_out_bottom_solid = m_explosionproof_maker->GetSolidBottom(GetName() + "_Cov_out_bottom",10.5*mm);
    cover_in_bottom_solid = m_explosionproof_maker->GetSolidBottom(GetName() + "_Cov_in_bottom",0.5*mm);
    //cover_out_solid = m_explosionproof_maker->GetSolid(GetName() + "_Cov_out",10.7*mm);
    cover_out_solid = m_explosionproof_maker->GetSolid(GetName() + "_Cov_out",266*mm,480*mm);
 
  G4SubtractionSolid* top_solid = new G4SubtractionSolid(
      "_part1",
      cover_out_top_solid,
      cover_in_top_solid,
      0,
      G4ThreeVector(0,0,0)
      );

  G4SubtractionSolid* bottom_soild = new G4SubtractionSolid(
      "_part2",
      cover_out_bottom_solid,
      cover_in_bottom_solid,
      0,
      G4ThreeVector(0,0,0)
      );

  G4LogicalVolume * top_log = new G4LogicalVolume(
      top_solid,
      m_outer_top_material,
       "_Cover_TOP_OUT");

  G4LogicalVolume * bottom_log = new G4LogicalVolume(
      bottom_soild ,
      m_outer_bottom_material,
       "_Cover_BOTTOM_OUT");

  G4LogicalVolume * cover_out_log = new G4LogicalVolume(
      cover_out_solid,
      m_inner_material,
       "_Cover_OUT");

  m_logical_pmt = cover_out_log;
  
   new G4PVPlacement(0,
      G4ThreeVector(0,0,0),
      top_log,
       "_Cover_top_Phy",
      cover_out_log,
      false,
      0,
      true
      );
   new G4PVPlacement(0,
      G4ThreeVector(0,0,0),
      bottom_log,
       "_in_Bottom_Phy",
      cover_out_log,
      false,
      0,
      true
      );
  
      new G4PVPlacement(0,
      G4ThreeVector(0,0,0),
      m_pmt_inner->GetLogicalPMT(),
       "_in_pmt_Phy",
      cover_out_log,
      false,
      0,
      true
      );
   
 /*
  G4VisAttributes* pmtcover_visatt = new G4VisAttributes(G4Colour(1.0, 0, 1.0));
  pmtcover_visatt -> SetForceWireframe(true);  
  pmtcover_visatt -> SetForceAuxEdgeVisible(true);
 //   pmtcover_visatt -> SetForceSolid(true);
  //  pmtcover_visatt -> SetForceLineSegmentsPerCircle(4);
  bottom_log -> SetVisAttributes(pmtcover_visatt);

  G4VisAttributes* cover_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0));
  cover_visatt -> SetForceWireframe(true);  
  cover_visatt -> SetForceAuxEdgeVisible(true);
  //  pmtcover_visatt -> SetForceSolid(true);
  //  pmtcover_visatt -> SetForceLineSegmentsPerCircle(4);
  top_log -> SetVisAttributes(cover_visatt);
 */ 
  //  G4VisAttributes* cover_visatt = new G4VisAttributes(G4Colour(1.0, 1.0, 0));
  //  cover_visatt -> SetForceWireframe(true);  
  //  cover_visatt -> SetForceAuxEdgeVisible(true);
  //  cover_visatt -> SetForceSolid(true);
  //  cover_visatt -> SetForceLineSegmentsPerCircle(4);
  //  cover_out_log -> SetVisAttributes(cover_visatt);
}

