#ifndef ICoverManager_hh
#define ICoverManager_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
class G4LogicalVolume;

class ICoverManager {
public:
    virtual G4LogicalVolume* GetLogicalCover() = 0;

};

#endif

