#ifndef PMTGlassPosGen_hh
#define PMTGlassPosGen_hh

#include "DYB2PosInVolumeInterface.hh"
#include "DYB2PositionGenInterface.hh"
#include "G4AffineTransform.hh"
#include "G4String.hh"

#include <map>

class G4VPhysicalVolume;
class IPMTManager;
class LocalPVTransform;

namespace DYB2 {

class PMTGlassPosGenV2 : public IRndmPosInVolume {
public:

    PMTGlassPosGenV2(IPMTManager* pm)
        : pmt_manager(pm) {
    }

    ~PMTGlassPosGenV2();

    void setupParentPath(const G4String& path) {
        m_parentpath = path;
    }

    G4ThreeVector getRndmPosSurface(G4VPhysicalVolume* target, 
                                    G4String material_name);

private:
    typedef std::map< G4VPhysicalVolume*, LocalPVTransform* > CACHE;
    G4bool isPositionInMaterial(G4ThreeVector pos, G4String material_name);
    IPMTManager* pmt_manager;
    G4String m_parentpath;

    CACHE m_cache;
};

}
#endif
