#ifndef EnergyDepositedCal_hh
#define EnergyDepositedCal_hh

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <map>
using namespace std;

namespace DYB2 {
    namespace Ball {
class EnergyDepositedCal {
public:
    static EnergyDepositedCal* getEnergyDepositedCal(G4String edep_name);
    static void createEnergyDepositedCal(G4String edep_name);
    static void deleteEnergyDepositedCal(G4String edep_name);

    void reset();
    void append(G4double, G4ThreeVector);
private:
    // I think, Energy and Position should push_back at the same time!!!
    void appendEnergy(G4double);
    void appendPosition(G4ThreeVector);
public:

    G4double getTotalEnergy();
    G4ThreeVector getMeanDepositedPos();

private:
    EnergyDepositedCal();
    vector< G4ThreeVector > m_pos;
    vector< G4double > m_edep;

    static map< G4String, EnergyDepositedCal* > * m_manager;

};
    }

}

#endif
