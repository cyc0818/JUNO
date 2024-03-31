
#include "RooTrackerParser.hh"
#include <cmath>

namespace Generator {
    namespace Utils {

RooTrackerParser::RooTrackerParser(TString fname) 
    : m_file(NULL), m_tree(NULL), m_max_entries(0), 
      m_verbosity(4), m_current_index(0)
{
    m_fname = fname;
    m_file = new TFile(fname);
    if (m_file) {
        m_tree = (TTree*) m_file->Get("gRooTracker");
    }

    if (m_tree) {
        // set branch
        m_tree -> SetBranchAddress("StdHepN",   &m_StdHepN);
        m_tree -> SetBranchAddress("StdHepPdg", &m_StdHepPdg);
        m_tree -> SetBranchAddress("StdHepP4",  &m_StdHepP4);
    }

    if (m_tree) {
        m_max_entries = m_tree -> GetEntries();
        G4cout << "There are ["
               << m_max_entries
               << "] entries in the tree."
               << G4endl;
    }
}

RooTrackerParser::~RooTrackerParser()
{
    if (m_file) {
        m_file->Close();
    }
}

void
RooTrackerParser::setVerbosity(G4int verbosity)
{
    m_verbosity = verbosity;
}

ParticleInfoContainer 
RooTrackerParser::next()
{
    ParticleInfoContainer pic;

    // Check first.
    if (m_current_index < m_max_entries) {
        m_tree -> GetEntry(m_current_index);
        ++m_current_index;
    } else {
        if (m_verbosity > 3) {
            G4cout << "WARNING::There are no more entries in the ROOT File."
                   << G4endl;
        }
        return pic;
    }

    // Construct the Particle Info Container
    //
    G4int number_of_particles = getNumberOfParticles();
    if (m_verbosity > 3) {
      G4cout << "number_of_particles: " 
             << number_of_particles << G4endl;
    }

    for (G4int i=0; i < number_of_particles; ++i) {
        pic . push_back( getParticleInfoPerEntry(i) );
    }

    return pic;
}

G4int 
RooTrackerParser::getNumberOfParticles()
{
    return m_StdHepN;
}

ParticleInfo
RooTrackerParser::getParticleInfoPerEntry(G4int index)
{
    ParticleInfo pi_result;

    pi_result . stat = 1;
    pi_result . pid  = m_StdHepPdg[index];
    pi_result . px   = m_StdHepP4[index][0] * GeV;
    pi_result . py   = m_StdHepP4[index][1] * GeV;
    pi_result . pz   = m_StdHepP4[index][2] * GeV;
    G4double E = m_StdHepP4[index][3] * GeV;
    G4double mass2 =    ( pow(E, 2) -
                         (pow(pi_result.px, 2) +
                          pow(pi_result.py, 2) +
                          pow(pi_result.pz, 2)));
    if (mass2>=0.0) {
        pi_result . mass = sqrt(mass2);
    } else {
        pi_result . mass = 0.0 * GeV;
    }
    // TODO
    // Is the Time always ZERO
    pi_result . dt   = 0.0 * ns;

    if (m_verbosity > 3) {
        G4cout << "DUMP Record["
               << index
               << "]:" << G4endl;
        G4cout << "stat: "
               << pi_result.stat
               << G4endl;
        G4cout << "pid: "
               << pi_result.pid
               << G4endl;
        G4cout << "(px, py, pz):"
               << pi_result.px / GeV
               << " GeV"
               << ", ";
        G4cout << pi_result.py / GeV
               << " GeV"
               << ", ";
        G4cout << pi_result.pz / GeV
               << " GeV"
               << G4endl;
        G4cout << "E: "
               << E / GeV
               << " GeV"
               << G4endl;
        G4cout << "mass:"
               << pi_result.mass / GeV
               << " GeV"
               << G4endl;
        G4cout << "time:"
               << pi_result.dt / ns
               << "ns"
               << G4endl;
    }

    return pi_result;
}



    }
}
