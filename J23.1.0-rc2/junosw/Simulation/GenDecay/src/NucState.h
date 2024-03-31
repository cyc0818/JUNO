/**
 * \class NucState
 *
 * \brief A nuclide state.
 *
 * A, Z, and energy level
 *
 * bv@bnl.gov Fri Mar 20 14:23:02 2009
 *
 */


#ifndef NUCSTATE_H
#define NUCSTATE_H

#include <more/phys/ens.h>

#include <vector>
#include <ostream>
#include <string>

namespace GenDecay {

class NucDecay;

class NucState {
    more::phys::nucleus m_nuc;            // Z/A
    more::phys::ens::confiv_t m_halflife; // half life of this state
    more::phys::ens::confiv_t m_erel; // Nuclear energy relative to reference
    int m_eref;                       // Energy reference
    std::vector<NucDecay*> m_decays;
//  2014/3/20
//  To solve the problem of decay rates
private:
    std::vector<NucDecay*> m_origin_decays;
public:
    std::vector<NucDecay*>& origin_decays() { return m_origin_decays;}
    NucDecay* origin_decay(int ind);
    int n_origin_decays();

//  END

public:

    NucState(const more::phys::nucleus& n, 
             more::phys::ens::confiv_t hl = more::phys::ens::confiv_t(0.0,0.0),
             more::phys::ens::confiv_t rel = more::phys::ens::confiv_t(0.0,0.0),
             int ref=0.0)
        : m_nuc(n), m_halflife(hl), m_erel(rel), m_eref(ref) {}

    more::phys::nucleus nuc() const { return m_nuc; }
    more::phys::ens::confiv_t halflife() const { return m_halflife; }
    more::phys::ens::confiv_t erel() const { return m_erel; }
    int eref() const { return m_eref; }
    std::vector<NucDecay*>& decays() { return m_decays; }


    int Z();
    int A();
    /// Return lifetime (not halflife) in seconds
    double lifetime();
    /// Returns lifetime (not halflife) in CLHEP units (ns)
    double lifetime_clhep();
    /// Return energy level in eV
    double energy();
    /// Returns energy level in CLHEP units (MeV)
    double energy_clhep();
    int ndecays();
    NucDecay* decay(int ind);       

    /// Return true if this is a ground state
    bool ground_state();

};

std::ostream& operator<<(std::ostream& o, const NucState& ns);

}




#endif  // NUCSTATE_H
