#include "R3600PMTTubeManager.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4Tubs.hh"

#include <cassert>
using namespace CLHEP;
// Interface
G4LogicalVolume* 
R3600PMTTubeManager::GetLogicalPMT() {
    return m_logical_pmt;
}

G4double
R3600PMTTubeManager::GetPMTRadius() {
    return m_pmt_r;
}

G4double
R3600PMTTubeManager::GetPMTHeight() {
    return m_pmt_h;
}

G4double
R3600PMTTubeManager::GetZEquator() {
    return m_z_equator;
}

G4ThreeVector
R3600PMTTubeManager::GetPosInPMT() {
    return m_pmt_inner->GetPosInPMT() 
            + G4ThreeVector(0, 0, 
                            m_z_equator-m_pmt_inner->GetZEquator());
}

// Constructor 
R3600PMTTubeManager::R3600PMTTubeManager
    (
     const G4String& plabel,
     IPMTManager* pmt_inner,
     G4Material* outer_mat
    ): IPMTCoverManager(pmt_inner)
       , m_label(plabel)
       , m_logical_pmt(NULL)
       , m_outer_mat(outer_mat)
{
    if (pmt_inner) {
        initialize();
    } else {
        assert(pmt_inner);
    }
}

// Helper Methods
void
R3600PMTTubeManager::initialize() {
    // construct
    // * construct a mirror surface
    init_variables();
    init_pmt();
}

void
R3600PMTTubeManager::init_variables() {
    m_pmt_r = 1e-3*mm + m_pmt_inner->GetPMTRadius();
    m_pmt_h = 680*mm;

    m_z_equator = m_pmt_h/2;
}

void
R3600PMTTubeManager::init_pmt() {
    G4VSolid* pmttube_whole_solid=NULL;
    pmttube_whole_solid = new G4Tubs(
                                GetName() + "_Whole",
                                0,
                                m_pmt_r,
                                m_pmt_h / 2,
                                0,
                                360*deg);
    G4LogicalVolume* pmttube_whole = new G4LogicalVolume(
                                      pmttube_whole_solid,
                                      m_outer_mat/*Material*/,
                                      GetName() + "Cover_Whole");
    m_logical_pmt = pmttube_whole;

    new G4PVPlacement(0,
                        G4ThreeVector(0, 0,
                              m_z_equator-m_pmt_inner->GetZEquator()),
                        m_pmt_inner->GetLogicalPMT(),
                        GetName() + "_PMTTube_Phy",
                        pmttube_whole,
                        false,
                        0);

}
