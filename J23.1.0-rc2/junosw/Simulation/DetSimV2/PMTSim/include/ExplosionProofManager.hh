
#ifndef ExplosionProofManager_hh
#define ExplosionProofManager_hh

#include "IPMTCoverManager.hh"
#include "ICoverManager.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include <vector>

class ExplosionProofSolid;
class G4LogicalVolume;
class G4Material;
class ICoverManager;

class ExplosionProofManager: public IPMTCoverManager {
public:
    // Interface
    G4LogicalVolume* GetLogicalPMT();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();
    
public:
    ExplosionProofManager
        (
         const G4String& plabel,
         IPMTManager* pmt_inner,
         G4Material* inner_material,
         G4Material* outer_top_material,
         G4Material* outer_bottom_material
         );
    ~ExplosionProofManager();
private:
    void initialize();

    void init_variables();
    void init_cover();

    G4String GetName() { return m_label;}
private:
    G4String m_label;
    G4LogicalVolume* m_logical_pmt;
    G4LogicalVolume * cover_out_top_log;
    G4LogicalVolume * top_log;
    G4LogicalVolume * bottom_log;
    G4LogicalVolume * cover_in_bottom_log; 
    G4LogicalVolume * cover_out_bottom_log;
    G4LogicalVolume * cover_in_top_log;
    G4LogicalVolume* m_logical_cover;
    G4Material* m_inner_material;
    G4Material* m_outer_top_material ;
    G4Material* m_outer_bottom_material;

private:
    /* solid maker */
    ExplosionProofSolid*  m_explosionproof_maker;


private:
    G4double m_z_equator;
    G4double m_pmt_r;
    G4double m_pmt_h;
};

#endif

