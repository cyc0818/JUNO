#ifndef R3600PMTTubeManager_hh
#define R3600PMTTubeManager_hh

#include "IPMTCoverManager.hh"
class G4LogicalVolume;
class G4Material;

class R3600PMTTubeManager: public IPMTCoverManager {
public:
    // Interface
    G4LogicalVolume* GetLogicalPMT();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();
public:
    R3600PMTTubeManager
        (
         const G4String& plabel,
         IPMTManager* pmt_inner,
         G4Material* outer_mat
        );
private:
    void initialize();

    void init_variables();
    void init_pmt();

    G4String GetName() { return m_label;}
private:
    G4String m_label;

    G4LogicalVolume* m_logical_pmt;
    G4Material* m_outer_mat;

    G4double m_z_equator;
    G4double m_pmt_r;
    G4double m_pmt_h;

};

#endif
