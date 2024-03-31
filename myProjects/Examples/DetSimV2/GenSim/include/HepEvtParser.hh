#ifndef HepEvtParser_hh
#define HepEvtParser_hh

#include <istream>
#include <fstream>
#include <iostream>
#include <map>
#include "ParticleDescription.hh"
#include "globals.hh"

namespace Generator {
  namespace Utils {

class EasyHepEvtParser {
public:
  //EasyHepEvtParser( std::istream& );
  EasyHepEvtParser( std::string );
  EasyHepEvtParser( std::string, G4bool split);

  void setVerbosity(G4int verbosity);

  ParticleInfoContainer next();

private:
  ParticleInfoContainer nextWhole();
  ParticleInfoContainer nextSplit();

private:
  G4int getNumberOfParticles();
  ParticleInfo getParticleInfoPerLine();

  G4bool checkOK( std::istream& );

  G4int getPDGID(G4int);
private:
  //std::istream& m_hepevt_src;
  std::ifstream m_hepevt_src;

  G4int m_verbosity;
  G4bool m_split;

  ParticleInfoContainer m_cache_whole;
  G4int m_next_index;
  G4bool m_new_event;
  G4double m_cache_time;

  // Need to reset per EVENT.
  G4double global_dtime;

  // For Map OLD PDG ID to NEW PDG ID
  //
  std::map< G4int, G4int > m_pdg_mapper;


};

  }

}

#endif
