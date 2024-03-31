/**
 * \class NucDecay
 *
 * \brief tie together two NucStates with a decay
 *
 * 
 *
 *
 *
 * bv@bnl.gov Tue May 12 14:25:26 2009
 *
 */

#ifndef NUCDECAY_H
#define NUCDECAY_H

#include <string>

namespace GenDecay {

class NucState;

struct NucDecay {
    NucState *mother, *daughter;
    // Type can be:
    // "A DECAY"
    // "B- DECAY"
    // "B+ DECAY"
    // "IT DECAY"
    // "Gamma"
    std::string type;
    double energy;
    double fraction;

    NucDecay(NucState* mo, NucState* da, const std::string& typ, double e=0, double f=0);

    // return energy in CLHEP energy units
    double clhep_energy() const;
};

std::ostream& operator<<(std::ostream& o, const NucDecay& ns);

}



#endif  // NUCDECAY_H
