#include "PMTGlassPosGenManager.hh"
#include "PMTGlassPosGen.hh"

#include <cassert>
#include "CLHEP/Random/RandFlat.h"
namespace DYB2 {

PMTGlassPosGenManager::PMTGlassPosGenManager(IPMTManager* pm)
{
    if (pm==NULL) {
        assert(pm);
    }
    m_rndm_pos_service = new PMTGlassPosGen(pm);
}

PMTGlassPosGenManager::~PMTGlassPosGenManager()
{
    if (m_rndm_pos_service ) {
        delete m_rndm_pos_service;
    }
}

PMTGlassPosGenManager&
PMTGlassPosGenManager::getInstance(IPMTManager* pm) {
    static PMTGlassPosGenManager _instance(pm);
    return _instance;
}

void
PMTGlassPosGenManager::appendPMTPhysical(G4VPhysicalVolume* anPMT){
    m_pmt_physical.push_back( anPMT );
}

G4VPhysicalVolume* 
PMTGlassPosGenManager::getPMTByID(G4int i){
    return m_pmt_physical[i];
}

G4ThreeVector 
PMTGlassPosGenManager::getPosInGlassByID(G4int i, G4String mat) {
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
PMTGlassPosGenManager::getPosInGlassByRndm(G4String mat) {
    G4int inum = CLHEP::RandFlat::shootInt(m_pmt_physical.size());
    return getPosInGlassByID(inum, mat);
}

}
