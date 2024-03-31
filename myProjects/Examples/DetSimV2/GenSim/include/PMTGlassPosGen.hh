#ifndef PMTGlassPosGen_hh
#define PMTGlassPosGen_hh

#include "DYB2PosInVolumeInterface.hh"
#include "DYB2PositionGenInterface.hh"
#include "G4AffineTransform.hh"

class G4VPhysicalVolume;
class IPMTManager;

namespace DYB2 {

class PMTGlassPosGen : public IRndmPosInVolume {
public:

    PMTGlassPosGen(IPMTManager* pm)
        : pmt_manager(pm) {
    }

    G4ThreeVector getRndmPosSurface(G4VPhysicalVolume* target, 
                                    G4String material_name);

private:
    G4AffineTransform getAffineTransform(G4VPhysicalVolume* target); 
    G4bool isPositionInMaterial(G4ThreeVector pos, G4String material_name);
    IPMTManager* pmt_manager;
};

}

#endif
