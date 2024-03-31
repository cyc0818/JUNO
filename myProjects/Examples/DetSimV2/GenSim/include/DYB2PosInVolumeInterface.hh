#ifndef DYB2PosInVolumeInterface_hh
#define DYB2PosInVolumeInterface_hh

#include <G4ThreeVector.hh>
#include <G4String.hh>
#include <globals.hh>

class G4VPhysicalVolume;

namespace DYB2 {

class IRndmPosInVolume {
public:
    virtual G4ThreeVector getRndmPosSurface(G4VPhysicalVolume* target,
                                            G4String solid_name)=0;
    virtual void setupParentPath(const G4String& /*path*/){};
    virtual ~IRndmPosInVolume(){}
};

}
#endif
