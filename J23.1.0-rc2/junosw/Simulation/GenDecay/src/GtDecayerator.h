/**
 * \class GtDecayerator
 *
 * \brief Generate radioactive decay events.
 *
 * 
 *
 *
 *
 * bv@bnl.gov Wed May 13 16:05:04 2009
 *
 */

#ifndef GTDECAYERATOR_H
#define GTDECAYERATOR_H


#include "GenTools/IGenTool.h"
#include "SniperKernel/ToolBase.h"
//#include "GaudiAlg/GaudiTool.h"
#include "NucState.h"
#include "NucDecay.h"

#include "HepMC/GenParticle.h"

#include <string>
#include <map>
#include "RandomSvc/IRandomSvc.h"


namespace GenDecay {
    class DecayRates;
}

class GtDecayerator : public ToolBase,
                      public IGenTool
{

public:

    GtDecayerator(const std::string& name);
    ~GtDecayerator();

    bool configure();
    // HepMCEventMutator interface
    bool mutate(HepMC::GenEvent& event);

private:
    /// Property: ParentNuclide.  Name of the begining of a
    /// radioactive decay chain.  This can be set in any format that
    /// is understood by libmore.  This is basically a case insenstive
    /// combonation of the element abreviation and the atomic mass.
    /// Eg, u-238 U238 238u.
    std::string m_parentNuclide;
    

    /// Property: ParentAbundance.  Set the abundance of the parent
    /// nuclide.  That is, the number of nuclides of the parent's
    /// type.
    double m_parentAbundance;


    /// Property: AbundanceMap.  A map of string to double that gives
    /// abundances of specific nuclide types.  If the parent is
    /// listed and ParentAbundance is set the latter takes precedence.
    std::map<std::string,double> m_abundanceMap;


    /// Property: SecularEquilibrium.  Set abundances of uncorrelated
    /// daughter nuclides (see CorrelationTime property) to secular
    /// equilibrium with the parent.  If any values are given by the
    /// AbundanceMap property, they will take precedence.
    bool m_secularEquilibrium;


    /// Property: CorrelationTime.  Any nuclide in the chain that has
    /// a decay branch with a half life (total nuclide halflife *
    /// branching fraction) shorter than this correlation time will be
    /// considered correlated with the parent(s) that produced it and
    /// the resulting kinematics will include both parent and child
    /// decays together and with a time chosen based on the parent
    /// abundance.  Otherwise, the decay of the nuclide is considered
    /// independent from its parent and will decay based on its own
    /// abundance.
    double m_correlationTime;

    // Property: DecayDepth: decay depth in chain.
    int m_depth;

    std::string m_stopNuclide;
    
    GenDecay::DecayRates* m_rates;

    HepMC::GenParticle* make_particle(GenDecay::NucState& state, int status=2);

    HepMC::GenParticle* make_particle(GenDecay::NucDecay& dk);
    
    IRandomSvc* m_rs; 
};


#endif  // GTDECAYERATOR_H
