#include "GenTools/GtOpticksTool.h"

#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#ifdef WITH_G4CXOPTICKS
#include "SLOG.hh"
#include "NP.hh"
#include "scuda.h"
#include "squad.h"
#include "sphoton.h"
#include "SStr.hh"
#include "SVec.hh"
#include "SEvt.hh"
#endif

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Types.hh"

using namespace CLHEP;

DECLARE_TOOL(GtOpticksTool);


const GtOpticksTool* GtOpticksTool::fInstance = nullptr ; 

const GtOpticksTool* GtOpticksTool::Get()
{
    return fInstance ; 
}

GtOpticksTool::GtOpticksTool(const std::string& name)
    : 
    ToolBase(name),
    m_input_photon(nullptr)
{
    fInstance = this ; 
}

GtOpticksTool::~GtOpticksTool() {}



const char* GtOpticksTool::configure_FAIL_NOTES = R"(
GtOpticksTool::configure_FAIL_NOTES
=====================================

GtOpticksTool integrates junosw with Opticks input photon 
machinery including the frame targetting functionality using 
the Opticks translated Geant4 geometry.  

Getting this to work requires:

1. compilation WITH_G4CXOPTICKS
2. SEvt::Exists true, this typically requires 
   an opticksNode greater than zero, configure with 
   the tut_detsim.py option "--opticks-mode 1/2/3"  
3. OPTICKS_INPUT_PHOTONS envvar identifying an 
   existing .npy file containing the photons

To disable use of GtOpticksTool input photons simply replace 
the "opticks" argument on the tut_detsim.py commandline 
with for example "gun". 


)" ;


bool GtOpticksTool::configure() 
{ 
    bool ret = false  ; 
#ifdef WITH_G4CXOPTICKS
    ret = SEvt::Exists() && SEvt::HasInputPhoton() ; 
    if(ret == false) std::cerr 
       << "GtOpticksTool::configure WITH_G4CXOPTICKS"
       << " : ERROR : something is missing "
       << std::endl 
       << " SEvt::Exists " << SEvt::Exists() 
       << " SEvt::HasInputPhoton " << SEvt::HasInputPhoton() 
       << std::endl 
       << "SEvt::Brief" 
       << std::endl 
       << SEvt::Brief() 
       << std::endl 
       << "SEvt::DescInputPhoton"
       << SEvt::DescInputPhoton()
       << std::endl 
       << configure_FAIL_NOTES 
       ;
#else
    std::cerr
        << "GtOpticksTool::configure NOT WITH_G4CXOPTICKS"
        << " ret " << ( ret ? "true" : "false" )
        << std::endl 
        ; 
#endif
    return ret ; 
}


/**
GtOpticksTool::getInputPhoton
-------------------------------

The m_input_photon is set by GtOpticksTool::mutate 
as it is too soon at initialization time because the frame targetting 
requires an Opticks CSGFoundry geometry. 

**/
NP* GtOpticksTool::getInputPhoton() const { return m_input_photon ;  }


#ifdef WITH_G4CXOPTICKS
void GtOpticksTool::add_optical_photon(HepMC::GenEvent& event, int idx ) 
{
    bool dump = idx < 10 ; 
    int ebyte = m_input_photon ? m_input_photon->ebyte : 0 ; 

    HepMC::FourVector post ; 
    HepMC::ThreeVector dir ; 
    HepMC::ThreeVector pol ; 
    G4double wavelength_nm(0.) ; 

    if( ebyte == sizeof(float) )
    {
        sphoton p ;   
        sphoton::Get(p, m_input_photon, idx );  
        post.set(p.pos.x,p.pos.y,p.pos.z,p.time); 
        dir.set(p.mom.x, p.mom.y, p.mom.z);  
        pol.set(p.pol.x, p.pol.y, p.pol.z);  
        wavelength_nm = p.wavelength  ; 
    }
    else if( ebyte == sizeof(double) )
    {
        sphotond p ;   
        sphotond::Get(p, m_input_photon, idx );  
        post.set(p.pos.x,p.pos.y,p.pos.z,p.time); 
        dir.set(p.mom.x, p.mom.y, p.mom.z);  
        pol.set(p.pol.x, p.pol.y, p.pol.z);  
        wavelength_nm = p.wavelength  ; 
    }
    else
    {
        std::cerr
            << "GtOpticksTool::add_optical_photon" 
            << " FATAL "
            << " no input_photon OR invalid type "
            << " ebyte " << ebyte 
            << std::endl 
            ;
        assert(0);  
    }

    G4double wavelength = wavelength_nm * CLHEP::nm ; 
    G4double energy = h_Planck*c_light/wavelength ;

    HepMC::GenVertex* vertex = new HepMC::GenVertex(post) ; 
    HepMC::FourVector momentum(dir.x()*energy, dir.y()*energy, dir.z()*energy, energy );  
    HepMC::Polarization polarization(pol); 

    int pdg_id = 20022 ; // optical photon 
    int status = 1 ;  // ?
    HepMC::GenParticle* particle = new HepMC::GenParticle(momentum, pdg_id, status);

    particle->set_polarization(polarization);
    vertex->add_particle_out(particle);
    event.add_vertex(vertex); 

    if(dump) std::cout
        << " idx " << idx 
        << " event.event_number " << event.event_number()
        << " wavelength_nm " << std::setw(5) << wavelength_nm
        << " wavelength " << std::fixed << std::setw(10) << std::setprecision(3) << wavelength
        << " energy " << std::fixed << std::setw(10) << std::setprecision(3) << energy
        << " energy/eV " << std::fixed << std::setw(10) << std::setprecision(3) << energy/eV
        << std::endl 
        ;

    LOG_IF(info, dump) << " check log " ; 

}
#else
void GtOpticksTool::add_optical_photon(HepMC::GenEvent&, int ) 
{
}
#endif



#ifdef WITH_G4CXOPTICKS
bool GtOpticksTool::mutate(HepMC::GenEvent& event)
{
    int event_number = event.event_number() ; // is this 0-based ? 
    if(m_input_photon == nullptr)
    {
        m_input_photon = SEvt::GetInputPhoton() ; 
        std::cerr 
            << "GtOpticksTool::mutate"
            << " event_number " << event_number
            << " deferred SEvt::GetInputPhoton "
            << " " << SEvt::Brief()
            << " m_input_photon " << ( m_input_photon ? m_input_photon->sstr() : "-" )
            << std::endl
            ;
    }

    int numPhotons = m_input_photon ? m_input_photon->shape[0] : 0 ; 
    //LOG(info)
    std::cerr
        << "GtOpticksTool::mutate"
        << " event_number " << event_number
        << " numPhotons " << numPhotons 
        << std::endl 
        ; 

    for(int idx = 0; idx < numPhotons ; ++idx) add_optical_photon(event, idx); 
    return true;
}
#else
bool GtOpticksTool::mutate(HepMC::GenEvent&)
{
    std::cerr
        << "GtOpticksTool::mutate"
        << " NOT WITH_G4CXOPTICKS "
        << std::endl  
        ;

    return false ; 
}
#endif

