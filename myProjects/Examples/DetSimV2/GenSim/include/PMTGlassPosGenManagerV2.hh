#ifndef PMTGlassPosGenManagerV2_hh
#define PMTGlassPosGenManagerV2_hh

#include <vector>
#include "G4ThreeVector.hh"

class IPMTManager;
class G4VPhysicalVolume;

namespace DYB2 {

class IRndmPosInVolume;

class PMTGlassPosGenManagerV2 {

public:
    static PMTGlassPosGenManagerV2& getInstance(IPMTManager* pm=NULL);

    void registerParentPath(const G4String& path);
    void appendPMTPhysical(G4VPhysicalVolume* anPMT);
    G4VPhysicalVolume* getPMTByID(G4int i); 

    G4ThreeVector getPosInGlassByID(G4int i, G4String mat);
    G4ThreeVector getPosInGlassByRndm(G4String mat);

private:
    PMTGlassPosGenManagerV2(IPMTManager* pm);
    ~PMTGlassPosGenManagerV2();
    PMTGlassPosGenManagerV2(PMTGlassPosGenManagerV2&);
    void operator=(PMTGlassPosGenManagerV2 const&);
private:
    std::vector< G4VPhysicalVolume* > m_pmt_physical;
    IRndmPosInVolume* m_rndm_pos_service;
    G4String m_path;

};

}

#endif
