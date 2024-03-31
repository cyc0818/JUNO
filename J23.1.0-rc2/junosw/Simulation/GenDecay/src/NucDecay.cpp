#include "NucDecay.h"
#include "NucState.h"

#include <more/phys/ens.h>
#include "CLHEP/Units/SystemOfUnits.h"

using namespace std;
using namespace GenDecay;
using namespace more::phys;

NucDecay::NucDecay(NucState* mo, NucState* da, 
                   const string& typ, double e, double f) 
    : mother(mo), daughter(da), type(typ), energy(e), fraction(f) 
{
    mother->decays().push_back(this); 
//    daughter->origin_decays().push_back(this);
}

double NucDecay::clhep_energy() const
{
    return energy/SI::MeV*CLHEP::MeV;
}


namespace GenDecay {
ostream& operator<<(ostream& o, const NucDecay& ns)
{
    o << ns.type << " E=" << ns.clhep_energy()/CLHEP::MeV << " MeV br=" << ns.fraction*100 << "%";
    return o;
}
}
