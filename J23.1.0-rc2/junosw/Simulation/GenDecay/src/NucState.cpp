#include <NucState.h>
//#include <GenDecay/NucUtil.h>
#include <more/phys/ens.h>

#include "CLHEP/Units/SystemOfUnits.h"

#include <cmath>

using namespace GenDecay;
using namespace std;
using namespace more;
using namespace more::phys;

namespace GenDecay {
ostream& operator<<(ostream& o, const NucState& ns)
{
    o << ns.nuc().name();
    if (ns.erel().is_known()) {
        o << " level=" << ns.erel().cent()/SI::keV
          << " keV [" << ns.eref() << "]";
    }
    else
        o << " level=(unknown)";
    //o << " with " << ns.decays.size() << " decays\n";
    return o;
}
}

int NucState::Z()
{
    return m_nuc.n_prot();
}
int NucState::A()
{
    return m_nuc.n_prot() + m_nuc.n_neut();
}
double NucState::lifetime()
{
    const double ln2 = log(2.0);
    double hl = m_halflife.cent();
    if (::isnan(hl)) return 0;
    return hl / ln2;
}
double NucState::lifetime_clhep()
{
    return this->lifetime()/SI::s*CLHEP::second;
}
double NucState::energy()
{
    // fixme: ignores eref!
    return m_erel.cent();
}
double NucState::energy_clhep()
{
    return this->energy()/SI::MeV*CLHEP::MeV;
}
int NucState::ndecays()
{
    return m_decays.size();
}
int NucState::n_origin_decays()
{
    return m_origin_decays.size();
}


GenDecay::NucDecay* NucState::decay(int ind)
{
    if (ind<0 || ind>= (int)m_decays.size()) return 0;
    return m_decays[ind];
}

GenDecay::NucDecay* NucState::origin_decay(int ind)
{
    if (ind<0 || ind>= (int)m_decays.size()) return 0;
    return m_origin_decays[ind];
}



bool NucState::ground_state()
{
    return 0.0 == energy();
}
