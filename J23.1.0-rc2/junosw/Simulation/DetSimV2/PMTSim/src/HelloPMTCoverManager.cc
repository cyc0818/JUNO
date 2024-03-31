
#include "HelloPMTCoverManager.hh"
#include "Hello_PMTSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"

#include <cassert>
using namespace CLHEP;

// Interface
G4LogicalVolume* 
HelloPMTCoverManager::GetLogicalPMT() {
    return m_logical_pmt;
}

G4double
HelloPMTCoverManager::GetPMTRadius() {
    return m_pmt_r;
}

G4double
HelloPMTCoverManager::GetPMTHeight() {
    return m_pmt_h;
}

G4double
HelloPMTCoverManager::GetZEquator() {
    return m_z_equator;
}

G4ThreeVector
HelloPMTCoverManager::GetPosInPMT() {
    return m_pmt_inner->GetPosInPMT();
}

// Constructor
HelloPMTCoverManager::HelloPMTCoverManager
    (
     const G4String& plabel,
     IPMTManager* pmt_inner,
     G4Material* outer_mat,
     G4Material* front_mat,
     G4Material* bottom_mat,
     HelloPMTSolidParameter hpsp
    ): IPMTCoverManager(pmt_inner)
       , m_label(plabel)
       , m_logical_pmt(NULL)
       , m_outer_mat(outer_mat)
       , m_front_mat(front_mat)
       , m_bottom_mat(bottom_mat)
       , m_pmtsolid_maker(NULL)
       , pmt_solid_par(hpsp)
{
    if (pmt_inner) {
        initialize();
    } else {
        assert(pmt_inner);
    }
}

HelloPMTCoverManager::~HelloPMTCoverManager()
{
    if (m_pmtsolid_maker) {
        delete m_pmtsolid_maker;
    }
}

// Helper Methods
void
HelloPMTCoverManager::initialize() {
    // construct
    // * construct a mirror surface
    init_variables();
    init_pmt();
}

void
HelloPMTCoverManager::init_variables() {
    m_pmt_r = 10.*mm + m_pmt_inner->GetPMTRadius();
    m_pmt_h = 200.*mm + m_pmt_inner->GetPMTHeight();
    m_z_equator = 200.*mm + m_pmt_r;

    m_pmtsolid_maker = new Hello_PMTSolid(
                            m_pmt_inner->GetPMTRadius(),
                            100*mm,
                            50*mm,
                            m_pmt_inner->GetPMTHeight()
                            );
    m_pmtsolid_maker->SetCoverEdge(pmt_solid_par.numSide);
}

void
HelloPMTCoverManager::init_pmt() {
    G4VSolid* pmttube_top_solid=NULL;
    G4VSolid* pmttube_bottom_solid=NULL;
    G4VSolid* pmttube_whole_solid=NULL;

    pmttube_top_solid = m_pmtsolid_maker->GetCoverTop(
                                            GetName() + "_Cover_TOP",
                                            pmt_solid_par.h1,
                                            pmt_solid_par.r1,
                                            pmt_solid_par.h2,
                                            pmt_solid_par.r2,
                                            pmt_solid_par.th_in);
    pmttube_bottom_solid = m_pmtsolid_maker->GetCoverBottom(
                                            GetName() + "_Cover_BOT",
                                            pmt_solid_par.h2,
                                            pmt_solid_par.r2,
                                            pmt_solid_par.h3,
                                            pmt_solid_par.r3,
                                            pmt_solid_par.th_out,
                                            pmt_solid_par.th_in);
    pmttube_whole_solid = m_pmtsolid_maker->GetCoverSolid(
                                            GetName() + "_Cover",
                                            pmt_solid_par.h1,
                                            pmt_solid_par.r1,
                                            pmt_solid_par.h2,
                                            pmt_solid_par.r2,
                                            pmt_solid_par.h3,
                                            pmt_solid_par.r3,
                                            pmt_solid_par.th_out
                                            );
    G4LogicalVolume* pmttube_front = new G4LogicalVolume(
                                      pmttube_top_solid,
                                      m_front_mat/*Material*/,
                                      GetName() + "Cover_Top");
    G4LogicalVolume* pmttube_bottom = new G4LogicalVolume(
                                      pmttube_bottom_solid,
                                      m_bottom_mat/*Material*/,
                                      GetName() + "Cover_Bottom");
 
    G4LogicalVolume* pmttube_whole = new G4LogicalVolume(
                                      pmttube_whole_solid,
                                      m_outer_mat/*Material*/,
                                      GetName() + "Cover_Whole");
    m_logical_pmt = pmttube_whole;

    new G4PVPlacement(0,
                        G4ThreeVector(),
                        pmttube_front,
                        GetName() + "Cover_Top_Phy",
                        pmttube_whole,
                        false,
                        0);
    new G4PVPlacement(0,
                        G4ThreeVector(),
                        pmttube_bottom,
                        GetName() + "Cover_Bottom_Phy",
                        pmttube_whole,
                        false,
                        0);
    new G4PVPlacement(0,
                        G4ThreeVector(),
                        m_pmt_inner->GetLogicalPMT(),
                        GetName() + "PMTTube_Phy",
                        pmttube_whole,
                        false,
                        0);


}
