
#include "EnergyDepositedCal.hh"
#include <iostream>

namespace DYB2 {
namespace Ball {

map< G4String, EnergyDepositedCal* > * EnergyDepositedCal::m_manager = 0;

EnergyDepositedCal* 
EnergyDepositedCal::getEnergyDepositedCal(G4String edep_name) {
    createEnergyDepositedCal( edep_name );
    // EnergyDepositedCal* tmpptr = 0;

    map< G4String, EnergyDepositedCal* >::iterator iter;
    iter = (*m_manager).find(edep_name);

    if (iter != (*m_manager).end()) {
        // Exist, Return
        return (*m_manager)[edep_name];
    }
    return 0;
}
void
EnergyDepositedCal::createEnergyDepositedCal(G4String edep_name) {
    if (m_manager == 0) {
        m_manager = new map< G4String, EnergyDepositedCal* >();
    }
    // find the key
    // if exist, return;
    // else, create a new instance.
    map< G4String, EnergyDepositedCal* >::iterator iter;
    iter = (*m_manager).find(edep_name);

    if (iter != (*m_manager).end()) {
        // Exist, Return
        return;
    }
    (*m_manager)[edep_name] = new EnergyDepositedCal();
}
void
EnergyDepositedCal::deleteEnergyDepositedCal(G4String edep_name) {
    // find the key
    // if exist, delete;
    // else, do nothing.
    map< G4String, EnergyDepositedCal* >::iterator iter;
    iter = (*m_manager).find(edep_name);

    if (iter != (*m_manager).end()) {
        // If Exist, Delete
        if ((*m_manager)[edep_name]) {
            delete (*m_manager)[edep_name];
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

EnergyDepositedCal::EnergyDepositedCal() {

}

void 
EnergyDepositedCal::reset() {
    m_pos.clear();
    m_edep.clear();

}

void
EnergyDepositedCal::append(G4double edep, G4ThreeVector pos) {
    appendEnergy(edep);
    appendPosition(pos);
}

void 
EnergyDepositedCal::appendEnergy(G4double edep) {
    m_edep.push_back(edep);
}

void 
EnergyDepositedCal::appendPosition(G4ThreeVector pos) {
    m_pos.push_back(pos);
}

G4double 
EnergyDepositedCal::getTotalEnergy() {
    G4double total = 0.0;

    vector< G4double >::iterator iter = m_edep.begin();

    for (; iter != m_edep.end(); ++iter) {
        total += (*iter);
    }

    return total;
}
G4ThreeVector 
EnergyDepositedCal::getMeanDepositedPos() {
    G4double total_edep = getTotalEnergy();
    G4ThreeVector total_pos = G4ThreeVector(0, 0, 0);

    vector< G4double >::iterator iter_edep = m_edep.begin();
    vector< G4ThreeVector >::iterator iter_pos = m_pos.begin();

    for( ;
         (iter_edep != m_edep.end()) && (iter_pos != m_pos.end());
         ++iter_edep, ++iter_pos) {
        total_pos += ( (*iter_edep) * (*iter_pos) );
    }

    if (total_edep > 0) {
        return total_pos / total_edep;
    }
    std::cout << "ERROR::total_edep is ZERO" << endl;
    return G4ThreeVector();


}


}
}
