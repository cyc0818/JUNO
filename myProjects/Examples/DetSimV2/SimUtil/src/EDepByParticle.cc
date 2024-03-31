
#include "EDepByParticle.hh"

namespace Juno {
    namespace Helper {

std::map<G4String, EDepByParticle*> * EDepByParticle::m_manager = 0;

EDepByParticle* 
EDepByParticle::getEdepByParticle(G4String primaryTrack) {
    createEdepByParticle( primaryTrack );

    std::map< G4String, EDepByParticle* >::iterator iter;
    iter = (*m_manager).find(primaryTrack);

    if (iter != (*m_manager).end()) {
        // Exist, Return
        return (*m_manager)[primaryTrack];
    }
    return 0;
}

void
EDepByParticle::createEdepByParticle(G4String primaryTrack) {
    if (m_manager == 0) {
        m_manager = new std::map< G4String, EDepByParticle* >();
    }

    // check exists or not
    std::map< G4String, EDepByParticle* >::iterator iter;
    iter = (*m_manager).find(primaryTrack);

    if (iter != (*m_manager).end()) {
        // Exist, Return
        return;
    }
    (*m_manager)[primaryTrack] = new EDepByParticle();
}

void
EDepByParticle::deleteEdepByParticle(G4String primaryTrack) {
    // find the key
    // if exist, delete;
    // else, do nothing.
    std::map< G4String, EDepByParticle* >::iterator iter;
    iter = (*m_manager).find(primaryTrack);

    if (iter != (*m_manager).end()) {
        // If Exist, Delete
        if ((*m_manager)[primaryTrack]) {
            delete (*m_manager)[primaryTrack];
        }
        // Remove the key !!!
        m_manager->erase(iter);
    }

    if ((*m_manager).size() == 0) {
        // If there is no elements in the map,
        // then delete it.
        delete m_manager;
        m_manager = 0;
    }
    return;

}

void
EDepByParticle::reset() {
    m_pdgid2edep.clear();
}

void
EDepByParticle::append(G4int pdgid, G4double edep) {
    std::map<G4int, G4double>::iterator iter;
    iter = m_pdgid2edep.find(pdgid);

    // if not exists, init it = 0.
    if ( iter == m_pdgid2edep.end() ) {
        m_pdgid2edep[pdgid] = 0.0;
    }

    m_pdgid2edep[pdgid] += edep;
}

EDepByParticle::EDepByParticle() {
    reset();
}


    }
}
