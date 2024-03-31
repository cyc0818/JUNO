#ifndef MuonAnalysis_hh
#define MuonAnalysis_hh

#include <map>
#include <vector>
#include "globals.hh"
class G4Step;

namespace Juno {
    namespace Helper {

class MuonAnalysis {
public:
    static MuonAnalysis* getMuonAnalysis(G4int trackID);
    static void createMuonAnalysis(G4int trackID);
    static void deleteMuonAnalysis(G4int trackID);
    static const std::map<G4int, MuonAnalysis*>& getAll();
    static void destroyAll();
public:
    void analysisStep(const G4Step* step);
public:
    G4int getTrackID();
    G4int getPdgID();
    G4double getEdep();
    G4double getEdepNonIonizing();

    const std::vector<G4double>& getDeCol();
    const std::vector<G4double>& getDxCol();

private:
    MuonAnalysis(G4int trackID);
private:
    G4int m_trackID;
    G4int m_pdgID;
    G4double m_edep;
    G4double m_edep_non_ioni;

    std::vector<G4double> m_de_col;
    std::vector<G4double> m_dx_col;

private:
    static std::map<G4int, MuonAnalysis*> * m_manager;
};


    }
}

#endif
