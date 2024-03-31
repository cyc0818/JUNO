#include "MuonAnalysis.hh"
#include "G4Step.hh"

#include <vector>

namespace Juno {
    namespace Helper {

std::map<G4int, MuonAnalysis*> * MuonAnalysis::m_manager = 0;

MuonAnalysis*
MuonAnalysis::getMuonAnalysis(G4int trackID) {
    createMuonAnalysis( trackID );

    std::map< G4int, MuonAnalysis* >::iterator iter;
    iter = (*m_manager).find(trackID);

    if (iter != (*m_manager).end()) {
        // Exist, Return
        return (*m_manager)[trackID];
    }
    return 0;
}

void
MuonAnalysis::createMuonAnalysis(G4int trackID) {
    if (m_manager == 0) {
        m_manager = new std::map< G4int, MuonAnalysis* >();
    }

    // check exists or not
    std::map< G4int, MuonAnalysis* >::iterator iter;
    iter = (*m_manager).find(trackID);

    if (iter != (*m_manager).end()) {
        // Exist, Return
        return;
    }
    (*m_manager)[trackID] = new MuonAnalysis(trackID);
}

void
MuonAnalysis::deleteMuonAnalysis(G4int trackID) {
    // find the key
    // if exist, delete;
    // else, do nothing.
    std::map< G4int, MuonAnalysis* >::iterator iter;
    iter = (*m_manager).find(trackID);

    if (iter != (*m_manager).end()) {
        // If Exist, Delete
        if ((*m_manager)[trackID]) {
            delete (*m_manager)[trackID];
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

const std::map<G4int, MuonAnalysis*>&
MuonAnalysis::getAll() {
    // make m_manager is created
    if (m_manager == 0) {
        m_manager = new std::map< G4int, MuonAnalysis* >();
    }
    return (*m_manager);
}

void
MuonAnalysis::destroyAll() {
    std::vector<G4int> keys;
    for (std::map<G4int, MuonAnalysis*>::iterator iter=(*m_manager).begin();
            iter != (*m_manager).end(); ++iter) {
        keys.push_back(iter->first);
    }
    for (std::vector<G4int>::iterator iter=keys.begin();
            iter != keys.end(); ++iter) {

        deleteMuonAnalysis(*iter);
    }
}

void
MuonAnalysis::analysisStep(const G4Step* step) {
    // make sure the trackID is right.
    if (step->GetTrack()->GetTrackID() != m_trackID) {
        return;
    }
    if (m_pdgID == 0) {
        m_pdgID = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    }

    G4double de = step->GetTotalEnergyDeposit();
    G4double dx = step->GetStepLength();

    m_edep += de;
    m_edep_non_ioni += step->GetNonIonizingEnergyDeposit();

    // if this is the primary muon track
    if (step->GetTrack()->GetParentID() == 0 and (m_pdgID == 13 or m_pdgID == -13)) {
        // save de and dx
        m_de_col.push_back(de);
        m_dx_col.push_back(dx);
    }

}

G4int
MuonAnalysis::getTrackID() {
    return m_trackID;
}

G4int
MuonAnalysis::getPdgID() {
    return m_pdgID;
}

G4double
MuonAnalysis::getEdep() {
    return m_edep;
}

G4double
MuonAnalysis::getEdepNonIonizing() {
    return m_edep_non_ioni;
}

const std::vector<G4double>&
MuonAnalysis::getDeCol() {
    return m_de_col;
}

const std::vector<G4double>&
MuonAnalysis::getDxCol() {
    return m_dx_col;
}

MuonAnalysis::MuonAnalysis(G4int trackID) 
    : m_trackID(trackID), m_pdgID(0), m_edep(0.0), m_edep_non_ioni(0.0) {

}

    }
}
