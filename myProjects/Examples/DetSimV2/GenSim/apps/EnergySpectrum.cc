
#include "HepEvtParser.hh"
#include "getEnergy.hh"
#include "globals.hh"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>

int main (int argc, char** argv) {

  if( argc == 1 ) {
    G4cout << "Usage: " << argv[0] 
           << " [-ns|--no-split]"
           << " HepEvtFile ..." << G4endl
           << "Default is split the event by time."
           << G4endl;
    exit(-1);
  }

  G4bool split = true;
  for (int i = 1; i < argc; ++i) {
    if ( strcmp(argv[i], "-ns") == 0) {
        split = false;
    } else if ( strcmp(argv[i], "--no-split") == 0 ) {
        split = false;
    }
  }

  for (int i = 1; i < argc; ++i) {
    if ( strcmp(argv[i], "-ns") == 0) {
        continue;
    } else if ( strcmp(argv[i], "--no-split") == 0 ) {
        continue;
    }

    //std::ifstream ifin(argv[i]);
    //Generator::Utils::EasyHepEvtParser ehep(ifin);
    Generator::Utils::EasyHepEvtParser ehep(argv[i], split);
    ehep.setVerbosity(0);
    Generator::Utils::ParticleInfoContainer pic_u;

    // Output data.
    std::string output_filename(argv[i]);
    output_filename = output_filename + std::string(".total_enegry.txt");
    std::ofstream ofout( output_filename.c_str() );

    while ( (pic_u = ehep.next()).size() ) {
      ofout << getEnergyFromContainer(pic_u) / GeV << std::endl;
    }

    ofout.close();

  }

}
