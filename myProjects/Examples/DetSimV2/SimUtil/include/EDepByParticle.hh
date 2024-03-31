#ifndef EDepByParticle_hh
#define EDepByParticle_hh

#include "globals.hh"
#include <map>

/*  This is a variant of 'EnergyDepositedCal' 
 *  It will save the energy deposite of primary track and its secondaries
 **/

namespace Juno {
    namespace Helper {
class EDepByParticle {
public:
    static EDepByParticle* getEdepByParticle(G4String primaryTrack);
    static void createEdepByParticle(G4String primaryTrack);
    static void deleteEdepByParticle(G4String primaryTrack);

public:
    void reset();
    void append(G4int pdgid, G4double edep);
    const std::map<G4int, G4double>& getResult() {
        return m_pdgid2edep;
    }

private:
    EDepByParticle();

    std::map<G4int, G4double> m_pdgid2edep;
private:
    static std::map<G4String, EDepByParticle*> * m_manager;

};
    }

}

#endif
