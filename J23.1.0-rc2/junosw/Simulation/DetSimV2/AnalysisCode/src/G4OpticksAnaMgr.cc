#include "G4OpticksAnaMgr.hh"
#include "SniperKernel/ToolFactory.h"

#ifdef WITH_G4OPTICKS
#include "G4OpticksRecorder.hh"
#endif

DECLARE_TOOL(G4OpticksAnaMgr);

G4OpticksAnaMgr::G4OpticksAnaMgr(const std::string& name)
    : 
    ToolBase(name),
#ifdef WITH_G4OPTICKS
    m_recorder(new G4OpticksRecorder)
#else
    m_recorder(nullptr)
#endif
{
}

G4OpticksAnaMgr::~G4OpticksAnaMgr()
{
#ifdef WITH_G4OPTICKS
    delete m_recorder ; 
#endif
}

#ifdef WITH_G4OPTICKS
void G4OpticksAnaMgr::BeginOfRunAction(const G4Run* run) {  BeginOfRun() ;  m_recorder->BeginOfRunAction(run);       }
void G4OpticksAnaMgr::EndOfRunAction(const G4Run* run) {           m_recorder->EndOfRunAction(run);         }
void G4OpticksAnaMgr::BeginOfEventAction(const G4Event* evt) {     m_recorder->BeginOfEventAction(evt);     }
void G4OpticksAnaMgr::EndOfEventAction(const G4Event* evt) {       m_recorder->EndOfEventAction(evt);       } 
void G4OpticksAnaMgr::PreUserTrackingAction(const G4Track* trk) {  m_recorder->PreUserTrackingAction(trk);  } 
void G4OpticksAnaMgr::PostUserTrackingAction(const G4Track* trk) { m_recorder->PostUserTrackingAction(trk); } 
void G4OpticksAnaMgr::UserSteppingAction(const G4Step* step) {     m_recorder->UserSteppingAction(step);    } 
#else
void G4OpticksAnaMgr::BeginOfRunAction(const G4Run* ) {} 
void G4OpticksAnaMgr::EndOfRunAction(const G4Run* ) {} 
void G4OpticksAnaMgr::BeginOfEventAction(const G4Event* ) {}
void G4OpticksAnaMgr::EndOfEventAction(const G4Event* ) {}
void G4OpticksAnaMgr::PreUserTrackingAction(const G4Track* ) {}
void G4OpticksAnaMgr::PostUserTrackingAction(const G4Track* ) {}
void G4OpticksAnaMgr::UserSteppingAction(const G4Step* ) {}
#endif




#ifdef WITH_G4OPTICKS

#include <cstdlib>
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"

#include "G4PhysicsOrderedFreeVector.hh"
#include "X4Array.hh"
#include "G4MaterialPropertyVector.hh"
#include "X4MaterialPropertyVector.hh"

#include "NPY.hpp"

#include "CProcessManager.hh"
#include "PLOG.hh"
#include "NP.hh"
#include "DsG4Scintillation.h"

void G4OpticksAnaMgr::BeginOfRun()
{
    const char* dir = getenv("G4OpticksAnaMgr_outdir") ; 
    if(dir)
    { 
        DsG4Scintillation* scint = getScintillationProcess() ; 
        std::cout 
            << " scint " << scint 
            << std::endl 
            ;   

        //scint->DumpPhysicsTable();  
        std::cout << " G4OpticksAnaMgr_outdir " << dir << std::endl ; 
        saveScintillationWavelengthSamples(dir, "WavelengthSamples.npy") ; 
        saveScintillationIntegral(dir, "ScintillationIntegral.npy"); 
        saveProperty("FASTCOMPONENT", dir, "FASTCOMPONENT.npy" ); 
        saveProperty("SLOWCOMPONENT", dir, "SLOWCOMPONENT.npy" ); 
        saveProperty("REEMISSIONPROB", dir, "REEMISSIONPROB.npy" ); 
    }
}

DsG4Scintillation* G4OpticksAnaMgr::getScintillationProcess() const 
{
    G4OpticalPhoton* particle =  G4OpticalPhoton::Definition() ; 
    G4ProcessManager* pmanager = particle->GetProcessManager(); 
    G4VProcess* proc = pmanager->GetProcess("Scintillation"); 

    std::cout << CProcessManager::Desc(pmanager) << std::endl ; 

    DsG4Scintillation* scint = dynamic_cast<DsG4Scintillation*>(proc) ;     
    return scint ; 
}

void  G4OpticksAnaMgr::saveScintillationIntegral(const char* dir, const char* name) const 
{
    DsG4Scintillation* scint = getScintillationProcess() ; 
    int scnt = 0 ; 
    int materialIndex = 1 ; 
    G4MaterialPropertyVector* ScintillationIntegral =  scint->getScintillationIntegral(scnt, materialIndex ); 
    X4Array* xvec = new X4Array(ScintillationIntegral);
    NPY<double>* d = xvec->convert<double>(); ;
    d->save(dir, name); 
}

void  G4OpticksAnaMgr::saveProperty(const char* prop, const char* dir, const char* name) const 
{
    DsG4Scintillation* scint = getScintillationProcess() ; 
    int materialIndex = 1 ; 
    G4MaterialPropertyVector* vec = scint->getMaterialProperty(prop, materialIndex) ;
    X4MaterialPropertyVector* xvec = new X4MaterialPropertyVector(vec) ; 
    NPY<double>* d = xvec->convert<double>(); ;
    d->save(dir, name); 
}

void  G4OpticksAnaMgr::saveScintillationWavelengthSamples(const char* dir, const char* name) const 
{
    DsG4Scintillation* scint = getScintillationProcess() ; 

    int scnt = 0 ; 
    int materialIndex = 1 ; 
    int num_samples = 1000000 ; 
    int edgeitems = 10 ; 

    std::vector<float> wavelength( num_samples, 0.f ); 
    for(int i=0 ; i < num_samples ; i++)
    {
        float wavelength_nm = scint->getSampledWavelength( scnt, materialIndex );  
        wavelength[i] = wavelength_nm ;  

        if( i < edgeitems || i > num_samples - edgeitems) 
        std::cout 
            << " i " << std::setw(5) << i 
            << " wavelength_nm " << std::setw(8) << std::fixed << std::setprecision(3) << wavelength_nm 
            << std::endl 
            ;
    }
    std::cout << " writing to " << dir << "/" << name << std::endl ; 
    NP::Write(dir, name, wavelength.data(), wavelength.size() ); 
}


#else
void G4OpticksAnaMgr::BeginOfRun(){}
#endif

