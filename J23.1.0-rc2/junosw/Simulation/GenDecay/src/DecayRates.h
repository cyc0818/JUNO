/**
 * \class DecayRates
 *
 * \brief A NucVisitor that will calculate decay rates of a chain
 * given abundances.
 *
 * 
 *
 *
 * bv@bnl.gov Mon May 18 12:42:36 2009
 *
 */

#ifndef DECAYRATES_H
#define DECAYRATES_H

#include "NucVisitor.h"

//#include "GaudiKernel/RndmGenerators.h"

#include <more/phys/ens.h>
#include <map>
#include <vector>
#include "RandomSvc/IRandomSvc.h"
//class IRndmGenSvc;

namespace GenDecay {

/// Visit the chain, calculate mean and total rates given abundances
/// and collect correlated decays.
class DecayRates : public GenDecay::NucVisitor
{
public:
    typedef std::map<more::phys::nucleus,double> AbundanceMap_t;
    typedef std::pair<NucDecay*,double> DecayValuePair_t;
    typedef std::vector<DecayValuePair_t> DecayValues_t;
    typedef std::map<NucState*,double> StateValueMap_t;

    /// Create a decay rate with given abundance, whether to use
    /// secular equilibrium or not, correlation time (seconds,
    /// compared to lifetime not half life) and pointer to random
    /// number generator.  epsTime is a small epsilon such that any
    /// lifetime less is considered zero.
    DecayRates(const AbundanceMap_t& abundance,
               bool secularEquilibrium, double corrTime, double epsTime, IRandomSvc* rs);

    /// As above, but no abundance map specified.  Use addAbundance()
    /// before calling anything else.
    DecayRates(bool secularEquilibrium, double corrTime, double epsTime);

    virtual ~DecayRates();

    // Set an entry in to the abundance map.  It is assumed this is an
    // abundance for a ground state.
    void addAbundance(more::phys::nucleus nuc, double abundance);

    // Return a randomly chosen decay from the chain
    NucDecay* decay();

    // Return a randomly chosen decay from a particular mother
    NucDecay* decay_state(NucState* nuc);

    // Access the collection of uncorrelated mothers from this chain
    const std::vector<NucState*>& mothers() const { return m_mothers; }
    double totalRate() const { return m_totalDecayRate; }

    /// Access resulting decay times.  Times are in seconds.
    DecayValues_t decayTimes(NucDecay* decay);

//    double uni() { return m_uni(); }

    /// Mostly exposed for debugging purposes
    const std::vector<double>& motherRate() { return m_motherRate; }

protected:

    // Visitor pattern.  Client should call descend()

    // Build up map from decay to mean rate
    bool visit(NucState* mother, NucState* /*daughter*/, NucDecay* decay);

    // Call once after top-level descend() is called
    virtual void postDescend();


private:
    void get_decay_times(NucDecay* decay, DecayValues_t& decayTime, double now);

    AbundanceMap_t m_forcedAbundance; // user input abundance
    std::map<NucState*,double> m_bookkeeping;

    bool m_secEq;
    double m_correlationTime;   // seconds
    double m_epsilonTime;        // seconds

    // secular equilibrium rate, take from chain's mother
    double m_secEqRate;

    // Cache mean decay rate of each mother
    StateValueMap_t m_meanDecayTimes;

    // uncorrelated mothers
    std::vector<NucState*> m_mothers; 
    std::vector<double> m_motherRate; // abundance corrected mean decay rate
    double m_totalDecayRate;          // cached sum of above
    IRandomSvc* m_rs;
//    Rndm::Numbers m_uni;

};


}
#endif  // DECAYRATES_H
