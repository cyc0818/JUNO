#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "NucUtil.h"
//#include "NucState.h"
//#include "NucDecay.h"
//#include "NucVisitor.h"
#include "Radiation.h"
#include "DecayRates.h"

//#include "CLHEP/Random/RandFlat.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenEvent.h"

#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <string>
#include <sstream>
#include "GtDecayerator.h"

DECLARE_TOOL(GtDecayerator);

using namespace std;
using namespace GenDecay;
using namespace more;
using namespace more::phys;


GtDecayerator::GtDecayerator(const std::string& name)
    	: ToolBase(name)
    	, m_rates(0)
{

    declProp("ParentNuclide", m_parentNuclide="");
    declProp("ParentAbundance",m_parentAbundance=0);
    declProp("AbundanceMap",m_abundanceMap);
    declProp("SecularEquilibrium",m_secularEquilibrium=true);
    declProp("CorrelationTime",m_correlationTime=1*CLHEP::second);
    declProp("DecayDepth", m_depth=-1);
    declProp("StopNuclide", m_stopNuclide="");

    m_rs = 0;
}

GtDecayerator::~GtDecayerator()
{

    // Clear out the cache at end of job.  This seems to work around bug #484
//    NucDecayMap_t& dks = const_cast<NucDecayMap_t&>(get_decays());
//    dks.clear();
}


HepMC::GenParticle* GtDecayerator::make_particle(GenDecay::NucDecay& dk)
{
    const Radiation* rad = decay_radiation(dk);
    if (!rad) return 0;

    double kine = rad->kineticEnergy();
    double mass = rad->mass();
    double mom = sqrt((kine+mass)*(kine+mass) - mass*mass);

    LogDebug << "make_particle: type " << rad->type() << " pid " << rad->pid() << " KE(MeV) " << kine/CLHEP::MeV << " mass(MeV) " << mass/CLHEP::MeV << endl;
    if ( rad->type() == EleCapture ) LogDebug << "make_particle: this type " << rad->type() << " is electron capture " << endl;

    // Pick random direction

    // cos(angle) from mean direction
//    double costh = CLHEP::RandFlat::shoot()*2.0 - 1.0;
    double costh = m_rs->random()*2.0 - 1.0;
    double sinth = sqrt(1-costh*costh);

    // azimuth angle around mean direction
    
//    double phi = CLHEP::RandFlat::shoot()*360.0*CLHEP::degree;
    double phi = m_rs->random()*360.0*CLHEP::degree;
    double cosphi = cos(phi);
    double sinphi = sin(phi);
        
    CLHEP::Hep3Vector dir(sinth*cosphi,sinth*sinphi,costh);

    CLHEP::HepLorentzVector lorvec(mom*dir,kine+mass);
    HepMC::FourVector fourmom(lorvec);

    // for electron capture, assign HEPEVT status code 3 signifying "a documentation line, defined separately from the event 
    // history. This could include the two incoming reacting particles, etc."
    // HEPEVT status code 1 is the standard code for particles to be propagated further
    // HEPEVT standard: http://cepa.fnal.gov/psm/simulation/mcgen/lund/pythia_manual/pythia6.3/pythia6301/node39.html
    int status = 1;
    if ( rad->type() == EleCapture && rad->pid() == 0 ) status = 3;
    LogDebug << "make_particle: assigning status " << status << " for this particle " << endl;

    return new HepMC::GenParticle(fourmom,rad->pid(),status);
}

HepMC::GenParticle* GtDecayerator::make_particle(GenDecay::NucState& state, int status)
{
  // PDG code for Ions (Geant4 version)
  // Nuclear codes are given as 10-digit numbers +-100ZZZAAAI.
  //For a nucleus consisting of np protons and nn neutrons
  // A = np + nn and Z = np.
  // I gives the isomer level, with I = 0 corresponding
  // to the ground state and I >0 to excitations
  //!!! I = 1 is assigned fo all excitation states !!!
 
    more::phys::nucleus mo_nuc = state.nuc();
    int mo_pid = 1000000000 + mo_nuc.n_prot()*10000 + mo_nuc.n_part()*10;  
    return new HepMC::GenParticle(HepMC::FourVector(0,0,0,0),mo_pid,status);
}

bool 
GtDecayerator::mutate(HepMC::GenEvent& event)
{
    phys::nucleus nucl;
    NucDecay* decay = 0;
    const Radiation* radiation = 0;
    do {
        decay = m_rates->decay();
        if (!decay) {
            LogError << "Failed to retrieve a decay!" << endl;
            return false;
        }

        // Check that primary radiation is known.  
        radiation = decay_radiation(*decay);
        if (!radiation) {
            LogWarn << "failed to get radiation from decay of " << *(decay->mother) 
                      << " via " << *decay << " skipping" << endl;
        }

    } while (!radiation);

    DecayRates::DecayValues_t decayTimes = m_rates->decayTimes(decay);
    HepMC::GenVertex* last_vertex = 0;
    NucState* last_daughter = 0;
    for (size_t ind = 0; ind < decayTimes.size(); ++ind) {

        NucDecay* dk = decayTimes[ind].first;

        HepMC::GenParticle* particle = make_particle(*dk);
        if (!particle) {
            LogWarn << "failed to get radiation from decay of " << *(dk->mother) 
                      << " via " << *dk << " with fraction=" << dk->fraction
                      << " skipping" << endl;
            continue;
        }

        double time = more_to_clhep_time(decayTimes[ind].second);
        HepMC::GenVertex* vertex = new HepMC::GenVertex(HepMC::FourVector(0,0,0,time));
        last_vertex = vertex;

        LogDebug << "Decay: " << *(dk->mother) 
                << " to " << *(dk->daughter)
                << " via " << *dk
                << " @ " << time/CLHEP::second << " seconds"
                << " (lifetime:" << dk->mother->lifetime() << " seconds)"
                << endl;

        event.add_vertex(vertex);
        if (decay == dk) {      // first in chain
            event.set_signal_process_vertex(vertex);
        }
        
        vertex->add_particle_out(particle);

        // Save mother particle as info particle (default status=2)
        particle = make_particle(*(dk->mother));
        if (particle) {
            vertex->add_particle_in(particle);
        }

        last_daughter = dk->daughter;
    }

    // Save final daughter as outgoing
    if (last_vertex && last_daughter) {
        HepMC::GenParticle* dp = make_particle(*last_daughter,1);
        if (dp) {
            last_vertex->add_particle_out(dp);
        }
    }

    LogDebug << "Decay of " << *(decay->mother) << " via " << *decay 
            << " produced " << event.particles_size() 
            << " particles in " << event.vertices_size() << " vertices" 
            << endl;
    //    event.print() ;  // djaffe



    return true;
}


bool
GtDecayerator::configure()
{ 
    SniperPtr<IRandomSvc> svc(*getParent(), "RandomSvc");
    if (svc.valid()) {
        m_rs = svc.data();
    }
    if ( 0 == m_rs )
    {
	LogError << "Can not load RandomSvc." << std::endl;
	return false;
    }

    phys::nucleus nucl;
    std::istringstream iss(m_parentNuclide);
//    istringstream iss("Th-234");
    iss >> nucl;
    LogDebug << "Staring with parent nuclide: \"" << nucl << "\"" << endl;
    
    NucState* head = get_ground(nucl);
    if (!head) {
        LogError << "Failed to get ground state for \"" << nucl << "\"" << endl;
        return false;
    }

    if (m_stopNuclide.empty()) {
        chain(head,m_depth);
    } else {
        LogInfo << "Decay From " << m_parentNuclide
                << " to " << m_stopNuclide
                << std::endl;
        std::stringstream ss(m_stopNuclide);
        phys::nucleus stopnucl;
        ss >> stopnucl;
        chain(head, m_depth, stopnucl);
    }
    std::map<NucDecay*,int> chain_map = getChainMap();

    map<phys::nucleus,double> abundance;
    map<string,double>::iterator sdit, sdend = m_abundanceMap.end();
    for (sdit=m_abundanceMap.begin(); sdit != sdend; ++sdit) {
        std::istringstream iss(sdit->first);
        phys::nucleus nuc;
        iss >> nuc;
        abundance[nuc] = sdit->second;
    }

    if (m_parentAbundance <= 0.0 ) {
        m_parentAbundance = abundance[nucl];
    }
    else {
        abundance[nucl] = m_parentAbundance;
    }

    m_rates = new DecayRates(abundance, m_secularEquilibrium, m_correlationTime/CLHEP::second, 1e-9/*1 ns*/,m_rs);
    m_rates -> setMap(chain_map);
//    try {
        m_rates->descend(head);
//    }
//    catch (const GaudiException& err) {
//        LogError << "Failed to calculate decay rates of chain" << endl;
//        error() << err.message() << endreq;
//        delete m_rates;
//        m_rates = 0;
//        return StatusCode::FAILURE;
//    }

    const vector<NucState*>& mothers = m_rates->mothers();
    const vector<double>& mother_rates = m_rates->motherRate();
    LogInfo << "I ("
           << this->objName() << ") got " 
           << mothers.size() << " uncorrelated mothers:\n";

    for (size_t ind=0; ind<mothers.size(); ++ind) {
        NucState* mom = mothers[ind];
        LogInfo << "\t" << *mom
               << " hl=" << mom->halflife() << 
		  " rate=" << mother_rates[ind] << "\n";
    }
    LogInfo << "total rate = " << m_rates->totalRate() << endl;





    if (!mothers.size()) return false;
    return true;
}

    

