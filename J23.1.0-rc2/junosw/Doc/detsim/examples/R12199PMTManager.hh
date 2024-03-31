#ifndef R12199PMTManager_hh
#define R12199PMTManager_hh

#include "SniperKernel/ToolBase.h"
#include "DetSimAlg/IPMTElement.h"

class G4OpticalSurface;
class G4Material;
class G4VSensitiveDetector;
class G4PVPlacement;
class G4VSolid;
class G4Tubs;

class R12199_PMTSolid;

class R12199PMTManager: public IPMTElement,
                        public ToolBase {
public:
    // Interface
    G4LogicalVolume* getLV();
    G4double GetPMTRadius();
    G4double GetPMTHeight();
    G4double GetZEquator();
    G4ThreeVector GetPosInPMT();

public:
    // Constructor
    R12199PMTManager
    (const G4String& plabel // label -- subvolume names are derived from this
    );
    ~R12199PMTManager();
private:
    void init();
    void init_material();
    void init_pmt();
private:
    G4LogicalVolume* m_logical_pmt;
    G4Material* GlassMat;
    G4Material* PMT_Vacuum;
};

#endif
