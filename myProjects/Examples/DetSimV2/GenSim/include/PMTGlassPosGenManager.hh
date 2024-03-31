#ifndef PMTGlassPosGenManager_hh
#define PMTGlassPosGenManager_hh

// TODO
// Not Implement
//
// Current Place in Analysis Manager.
//
// + Add PMT
// + Get Position in Specific PMT
// + Get Position in Random PMTs
//
//
#include <vector>

#include <G4ThreeVector.hh>

class IPMTManager;
class G4VPhysicalVolume;

namespace DYB2 {

class IRndmPosInVolume;

class PMTGlassPosGenManager {
    /* Singleton Class */
public:
    static PMTGlassPosGenManager& getInstance(IPMTManager* pm=NULL);

    void appendPMTPhysical(G4VPhysicalVolume* anPMT); 
    G4VPhysicalVolume* getPMTByID(G4int i); 

    G4ThreeVector getPosInGlassByID(G4int i, G4String mat);
    G4ThreeVector getPosInGlassByRndm(G4String mat);

private:
    PMTGlassPosGenManager(IPMTManager* pm);
    ~PMTGlassPosGenManager();
    PMTGlassPosGenManager(PMTGlassPosGenManager&);
    void operator=(PMTGlassPosGenManager const&);

private:
    std::vector< G4VPhysicalVolume* > m_pmt_physical;
    IRndmPosInVolume* m_rndm_pos_service;
};
}

#endif
