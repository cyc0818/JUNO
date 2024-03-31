#include "DecayRates.h"
#include "NucState.h"
#include "NucDecay.h"
#include "NucUtil.h"
#include "SniperKernel/SniperLog.h"
//#include "CLHEP/Random/RandFlat.h"
#include <exception>

#include <algorithm>

using namespace GenDecay;
using namespace std;

using namespace more;
using namespace more::phys;

DecayRates::DecayRates(const map<phys::nucleus,double>& abundance,
                       bool secularEquilibrium, double corrTime, double epsTime,
		       IRandomSvc* rs
                      )
    : NucVisitor()
    , m_forcedAbundance(abundance)
    , m_secEq(secularEquilibrium)
    , m_correlationTime(corrTime)
    , m_epsilonTime(epsTime)
    , m_secEqRate(0.0)
    , m_rs(rs)
{
//    StatusCode sc = m_uni.initialize(rgs, Rndm::Flat(0,1));
//    if (sc.isFailure()) {
//        throw GaudiException("Failed to initialize uniform random numbers",
//                             "GtDecayerator::DecayRates",StatusCode::FAILURE);
//    }
}

DecayRates::DecayRates(bool secularEquilibrium, double corrTime, double epsTime
                      )
    : NucVisitor()
    , m_secEq(secularEquilibrium)
    , m_correlationTime(corrTime)
    , m_epsilonTime(epsTime)
    , m_secEqRate(0.0)
{
//    StatusCode sc = m_uni.initialize(rgs, Rndm::Flat(0,1));
//    if (sc.isFailure()) {
//        throw GaudiException("Failed to initialize uniform random numbers",
//                             "GtDecayerator::DecayRates",StatusCode::FAILURE);
//    }
}


DecayRates::~DecayRates() 
{
}

void DecayRates::addAbundance(more::phys::nucleus nuc, double abundance)
{
    m_forcedAbundance[nuc] = abundance;
}

void DecayRates::postDescend()
{
    // we should only be called once, guard against multiple calls.
    // If for some reason this must change, replace this return with a
    // wipe.
    if (m_motherRate.size()) return;

    m_totalDecayRate = 0.0;
    //cerr << "Calculating rates for " << m_mothers.size() << " mothers" << endl;
    for (size_t ind=0; ind<m_mothers.size(); ++ind) {
        NucState* mom = m_mothers[ind];
        double rate = m_bookkeeping[mom];
        
        if (rate == 0.0) {
//            stringstream err;
            LogError << "DecayRates::decay: no rate set for nuclide: " 
                << mom->nuc() << endl;
//            throw GaudiException(err.str(),
//                                 "GtDecayerator::DecayRates",StatusCode::FAILURE);
	    throw exception();
        }
            
        m_totalDecayRate += rate;
        m_motherRate.push_back(rate);
        m_meanDecayTimes[mom] = 1.0/rate;
        //cerr << ind << ": rate " << rate << " for mother " << *mom << endl;
    }
}

// Return a randomly chosen decay.  
GenDecay::NucDecay* DecayRates::decay() 
{
//    double target = CLHEP::RandFlat::shoot() * m_totalDecayRate;
    double target = m_rs -> random() * m_totalDecayRate;
    double arrow = 0.0;
    NucState* mother = 0;
    for (size_t ind=0; ind<m_mothers.size(); ++ind) {
        NucState* mom = m_mothers[ind];
        arrow += m_motherRate[ind];
        if (arrow > target) {
            mother = mom;
            break;
        }
    }
    if (!mother) {
        cerr << "DecayRates::decay() failed to find mother to decay out of " << m_mothers.size() 
             << " with target = " << target << " and total = " << m_totalDecayRate 
             << endl;
//        throw GaudiException("Failed to find mother to decay, this should not happen",
//                             "GtDecayerator::DecayRates",StatusCode::FAILURE);
	throw exception();
    }
    return decay_state(mother);
}

// Visit each possible decay in the chain.  Record uncorrelated
// mothers and determine their decay rates
bool GenDecay::DecayRates::visit(GenDecay::NucState* mother, GenDecay::NucState* daughter, 
                                 GenDecay::NucDecay* decay) 
{

    static int num = 0;
    num++;
    //cerr << "DecayRates::visit: visiting " 
    LogDebug << "Visited decay ID:" << num << std::endl;
    //     << mother->nuc() << " -> " << daughter->nuc() << " "
    //     << *decay << endl;
    
    if (decay->fraction <= 0.0 || decay->fraction > 1.0) {
//        stringstream err;
        cerr << "DecayRates::visit: illegal decay fraction: " << decay->fraction
            << " mother: " << mother->nuc() << " level:"<<mother->energy()
            << " daughter: " << daughter->nuc() << " level:"<<daughter->energy()<<endl;
//        throw GaudiException(err.str(),"GtDecayerator::DecayRates",StatusCode::FAILURE);
	LogError << "The daughter of this mother:" << "\n";
	for (int ind = 0; ind < mother->ndecays(); ++ind)
	{
	    LogError << "ID:" << ind << "\n";
	    LogError << mother->decay(ind)->daughter->nuc() << "\t";
	}
	LogError << "\n";
	LogError << "The mother of this daughter:" << "\n";
	for (int ind = 0; ind < mother->ndecays(); ++ind)
	{
	    LogError << "ID:" << ind << "\n";
	    LogError << daughter->origin_decay(ind)->mother->nuc() << "\t";
	}
	LogError << std::endl;
	throw exception();
    }
    // debugging
    if (true) {
        double totfrac = 0.0;
        std::vector<NucDecay*>& dks = mother->decays();
        for (size_t ind=0; ind < dks.size(); ++ind) {
            totfrac += dks[ind]->fraction;
        }
                 
        double diff = fabs(totfrac-1.0);
        if (diff > 0.000001) {
//            stringstream err;
            cerr << "DecayRates::visit: illegal total decay fraction, differs from 1.0 by: " << diff
                << " mother: " << mother->nuc() << endl;
//            throw GaudiException(err.str(),"GtDecayerator::DecayRates",StatusCode::FAILURE);
	    throw exception();
        }
    }

    // no moms yet, implies we are at the ultimate mother
    if (!m_mothers.size()) {  

        m_mothers.push_back(mother);
        double abundance = m_forcedAbundance[mother->nuc()];
        if (abundance == 0.0) { 
//            stringstream err;
            cerr << "DecayRates::visit: no abundance set for chain's mother nuclide: " 
                << mother->nuc() << endl;
//            throw GaudiException(err.str(),"GtDecayerator::DecayRates",StatusCode::FAILURE);
	    throw exception();
        }
        m_bookkeeping[mother] = abundance/mother->lifetime();
        cerr << "Ultimate mother rate: " << m_bookkeeping[mother] << " for " << *mother << endl;
    }
    else {
        if (mother->lifetime() > m_correlationTime) {
            if (find(m_mothers.begin(),m_mothers.end(),mother) == m_mothers.end()) {
                m_mothers.push_back(mother);
            }
        }
        // bookkeeping handled as a daughter
    }


    // Now the daughter

    // The user can force an abundance for ground states, check if
    // this has been done.
    if (daughter->ground_state()) {
        double abundance = m_forcedAbundance[daughter->nuc()];
        if (abundance > 0.0) {
            m_bookkeeping[daughter] = abundance/daughter->lifetime();
            return true;
        }
    }

    // if this decay is uncorrelated and the user hasn't provided an
    // abundance yet doesn't rely on secular equilibrium then we don't
    // know what to do. 

    if (daughter->lifetime() > m_correlationTime) {
        if (!m_secEq) {
//            stringstream err;
           cerr << "DecayRates::visit: no explicit abundance and no "
                << "secular equilibrium for uncorrelated decay. mother: " 
                << mother->nuc()
                << " daughter: " << daughter->nuc() << endl;
//            throw GaudiException(err.str(),"GtDecayerator::DecayRates",StatusCode::FAILURE);
//	    throw exception();
        }
    }
    
    double rate = m_bookkeeping[mother];
    if (0.0 == rate) {
//        stringstream err;
        cerr << "DecayRates::visit: mother, why u no have rate?"
            << " mother: " << mother->nuc() << " level:" << mother->energy()
            << " daughter: " << daughter->nuc() << " level:" << daughter->energy() << std::endl;
//        throw GaudiException(err.str(),"GtDecayerator::DecayRates",StatusCode::FAILURE);
//	throw exception();
    }
    m_bookkeeping[daughter] += rate * decay->fraction;
    


    LogDebug <<  (mother -> nuc()).name() << std::endl << "\n";

    std::cout.setf(std::ios::scientific);   
    LogDebug << "Decay:" << std::endl;
    LogDebug << *mother << "\t" << "Mother Rate:" << rate << "\n";  
    LogDebug << *daughter << "\n" << "Decay fraction:" << decay -> fraction << "\t" << *decay << "\n";    
//    LogDebug << "There are " << mother -> n_origin_decays() << " nucs decay into this mother." <<std::endl;
    LogDebug << "Contribute to daughter rate:" << std::setprecision(7) << rate * decay -> fraction << std::endl;  
    LogDebug << "Rate:" << std::setprecision(7) << m_bookkeeping[daughter] << std::endl;    


    LogDebug << "There are " << daughter -> n_origin_decays() << " nucs decay into this daughter." <<std::endl;
    LogDebug << "There are " << daughter -> ndecays() << " nucs this daughter can decay into." <<std::endl;
    if ( daughter->ndecays() == 0)
	LogDebug << "This dauhter is one of the final terminal." << std::endl;    
    LogDebug << "----------------------------------" << std::endl;



    return true;
}

/*
  For the given decay, pick a time for it to occur.  

  If the daughter is unstable with a correlated lifetime decay her and
  recuse.
*/

GenDecay::DecayRates::DecayValues_t GenDecay::DecayRates::decayTimes(NucDecay* decay) 
{ 
    DecayValues_t decayTime;
    get_decay_times(decay,decayTime,0.0);
    return decayTime;
}

void DecayRates::get_decay_times(NucDecay* decay, DecayRates::DecayValues_t& decayTime, double now) 
{
    double meanDecayTime = 0.0;

    if (decayTime.size()) {     // not the mother of the chain

        // check if this decay is considered correlated
        if (decay->mother->lifetime() > m_correlationTime) {
            return;    // uncorelated decay, stop descent
        }

        // got a specific decay, use specific branch lifetime
        meanDecayTime = decay->mother->lifetime() / decay->fraction;
        
        //cerr << "Daughter ";
    }
    else {                      // this is the mother of the chain
        // got a general decay, use abundance based lifetime
        meanDecayTime = m_meanDecayTimes[decay->mother];

        //cerr << "Mother:  ";
    }

    double dt = 0.0;
    if (meanDecayTime > 0.0) {
        //dt = (-1.0 * log(CLHEP::RandFlat::shoot())) * meanDecayTime;
        dt = (-1.0 * log(m_rs -> random())) * meanDecayTime;
    }
    now += dt;

    // cerr << "now="<<now << " meanDecayTime=" << meanDecayTime 
    //      << " decay=" << *decay << " mother=" << *(decay->mother) 
    //      << " daughter=" << *(decay->daughter)
    //      << endl;

    decayTime.push_back(DecayValuePair_t(decay, now));

    NucDecay* the_decay = decay_state(decay->daughter);    
    if (!the_decay) { return; }

    get_decay_times(the_decay,decayTime,now); // recurse on chosen daughter decay
}



NucDecay* DecayRates::decay_state(NucState* nuc)
{
    // Now, pick which daughter to follow
    vector<NucDecay*>& decays = nuc->decays();
    if (!decays.size()) return 0;

//    double total_fraction = 0, target = CLHEP::RandFlat::shoot();
    double total_fraction = 0, target = m_rs -> random();

    for (size_t ind=0; ind < decays.size(); ++ind) {
        NucDecay* dk = decays[ind];

        // Pick daughter
        total_fraction += dk->fraction;
        if (total_fraction > target) {
            //cerr << "Choose decay " << *dk << " at total_fraction = " << total_fraction
            //     << " and target fraction at " << target << endl;
            return dk;
        }
    }

    cerr << "Failed to choose a decay for " << *nuc
         << " target fraction = " << target << ", total fraction at end = " << total_fraction
         << " Precision error?\n";
    return 0;
}


// Local Variables: **
// c-basic-offset:4 **
// End: **
