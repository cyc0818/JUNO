#ifndef IPMTManager_hh
#define IPMTManager_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
class G4LogicalVolume;

class IPMTManager {
public:
    virtual G4LogicalVolume* GetLogicalPMT() = 0;
    virtual G4double GetPMTRadius() = 0;
    virtual G4double GetPMTHeight() = 0;
    virtual G4double GetZEquator() = 0;
    virtual G4ThreeVector GetPosInPMT() = 0;
    virtual ~IPMTManager() {}
};

#endif
