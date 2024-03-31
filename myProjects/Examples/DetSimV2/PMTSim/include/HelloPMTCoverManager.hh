#ifndef HelloPMTCoverManager_hh
#define HelloPMTCoverManager_hh

#include "IPMTCoverManager.hh"

#include "G4String.hh"

class Hello_PMTSolid;
class G4LogicalVolume;
class G4Material;

struct HelloPMTSolidParameter {
    G4int    numSide;
    G4double h1;
    G4double r1;
    G4double h2;
    G4double r2;
    G4double h3;
    G4double r3;
    G4double th_in;
    G4double th_out;
};


class HelloPMTCoverManager: public IPMTCoverManager {
public:
    // Interface
    G4LogicalVolume* GetLogicalPMT();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();
public:
    HelloPMTCoverManager
        (
         const G4String& plabel,
         IPMTManager* pmt_inner,
         G4Material* outer_mat,
         G4Material* front_mat,
         G4Material* bottom_mat,
         HelloPMTSolidParameter hpsp
        );
    ~HelloPMTCoverManager();
private:
    void initialize();

    void init_variables();
    void init_pmt();

    G4String GetName() { return m_label;}
private:
    G4String m_label;
    G4LogicalVolume* m_logical_pmt;
    G4Material* m_outer_mat;
    G4Material* m_front_mat;
    G4Material* m_bottom_mat;

private:
    /* solid maker */
    Hello_PMTSolid* m_pmtsolid_maker;
    

private:
    G4double m_z_equator;
    G4double m_pmt_r;
    G4double m_pmt_h;
    HelloPMTSolidParameter pmt_solid_par;
};

#endif
