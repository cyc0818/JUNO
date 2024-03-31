#ifndef RooTrackerParser_hh
#define RooTrackerParser_hh

#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "ParticleDescription.hh"
#include "globals.hh"

namespace Generator {
    namespace Utils {

class RooTrackerParser {

public:
    RooTrackerParser( TString );
    ~RooTrackerParser();

    void setVerbosity(G4int verbosity);
    ParticleInfoContainer next();

private:
    G4int getNumberOfParticles();
    ParticleInfo getParticleInfoPerEntry(G4int);

private:
    TString m_fname;
    TFile* m_file;
    TTree* m_tree;

    G4int m_max_entries;
    G4int m_verbosity;
    G4int m_current_index;
private:
    // variables for the branches.
    G4int m_StdHepN;
    G4int m_StdHepPdg[100];
    G4double m_StdHepP4[100][4];
};

    }
}

#endif
