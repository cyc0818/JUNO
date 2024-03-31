/**
 * \class NucUtil
 *
 * \brief Util functions for dealing nuclides / libmore
 *
 * 
 *
 *
 *
 * bv@bnl.gov Tue May 12 16:37:08 2009
 *
 */


#ifndef NUCUTIL_H
#define NUCUTIL_H

#include <more/phys/ens.h>
#include <map>
#include <string>
#include <vector>

namespace GenDecay {

    class NucState;
    struct NucDecay;
    class Radiation;

    double more_to_clhep_time(double more_time);
    double more_to_clhep_energy(double more_energy);

    /// Fill out a ens::nucleus given the phys::nucleus, return true if no error
    bool get_nucleus(const more::phys::nucleus& pn, more::phys::ens::nucleus& en);

    /// Return the state coresponding to given phys::nucleus, half
    /// life, relative energy and reference energy.
    NucState* get_state(const more::phys::nucleus& n, 
                        more::phys::ens::confiv_t hl = more::phys::ens::confiv_t(0.0,0.0), 
                        more::phys::ens::confiv_t rel = more::phys::ens::confiv_t(0.0,0.0), 
                        int ref=0.0);

    /// Get a ground state by name (eg "U-238")
    NucState* get_ground(std::string name);

    /// Return the NucState for the given nucleus that is at the ground state.
    NucState* get_ground(more::phys::nucleus nucl);

    /// Form decay chain starting from given mother state.  If depth
    /// is non-negative, do not chain further down than that many
    /// daughters.  If stop_nuc is given, do not chain beyond its
    /// ground state.
    void chain(NucState* mother, int depth=-1, 
               more::phys::nucleus stop_nuc = more::phys::nucleus());

    /// Map mother,daughter to the decays that joins them.  In most
    /// cases the vector will have only a single entry.  However,
    /// positron emission and electron capture are considered separate
    /// decays and both branches may be possible
    typedef std::map<std::pair<NucState*,NucState*>,std::vector<NucDecay*> > NucDecayMap_t;

    /// Get all decays that have been chained so far 
    const NucDecayMap_t& get_decays();
    /// Make a radiation object for given decay
    const Radiation* decay_radiation(const NucDecay& dk);

    /// Dump dataset on our terms.  Can also use ds.dump(cout) to use
    /// built-in dumper.
    void dump_dataset(const more::phys::ens::dataset& ds);

    // map of NucDecay* to int
    std::map<NucDecay*,int> getChainMap();

}

#endif  // NUCUTIL_H
