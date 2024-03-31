#include "PMTGlassPosGenManagerV2.hh"
#include "PMTGlassPosGen.hh"

#include <cassert>
#include "CLHEP/Random/RandFlat.h"
namespace DYB2 {
PMTGlassPosGenManagerV2::PMTGlassPosGenManagerV2(IPMTManager* pm)
    : m_rndm_pos_service(NULL)
{
    if (pm==NULL) {
        assert(pm);
    }
    // XXX
    m_rndm_pos_service = new PMTGlassPosGen(pm);
}

PMTGlassPosGenManagerV2::~PMTGlassPosGenManagerV2()
{
    if (m_rndm_pos_service ) {
        delete m_rndm_pos_service;
    }
}

PMTGlassPosGenManagerV2&
PMTGlassPosGenManagerV2::getInstance(IPMTManager* pm) {
    static PMTGlassPosGenManagerV2 _instance(pm);
    return _instance;
}

void
PMTGlassPosGenManagerV2::registerParentPath(const G4String& path) {
    m_path = path;
    m_rndm_pos_service->setupParentPath(m_path);
}

void
PMTGlassPosGenManagerV2::appendPMTPhysical(G4VPhysicalVolume* anPMT){
    m_pmt_physical.push_back( anPMT );
}

G4VPhysicalVolume* 
PMTGlassPosGenManagerV2::getPMTByID(G4int i){
    return m_pmt_physical[i];
}

G4ThreeVector 
PMTGlassPosGenManagerV2::getPosInGlassByID(G4int i, G4String mat) {
    // We need make sure the parent path is ok.
    assert(m_path.size());
    G4VPhysicalVolume* pv = getPMTByID(i);
    if (pv) {
        return m_rndm_pos_service->getRndmPosSurface(
                                                    pv,
                                                    mat);
    } else {
        G4cout << "WARNING:"
               << "In PMTGlassPosGenManager, can't find the "
               << i << " PMT.";
       return G4ThreeVector();
    }
}

G4ThreeVector
PMTGlassPosGenManagerV2::getPosInGlassByRndm(G4String mat) {
    G4int inum = CLHEP::RandFlat::shootInt(m_pmt_physical.size());
    return getPosInGlassByID(inum, mat);
}

}
