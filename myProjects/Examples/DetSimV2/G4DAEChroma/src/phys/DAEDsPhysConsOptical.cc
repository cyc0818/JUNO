#define USE_CUSTOM_CERENKOV
#define USE_CUSTOM_SCINTILLATION

#include "DAEDsPhysConsOptical.h"
#include "DsG4OpRayleigh.h"

#ifdef USE_CUSTOM_CERENKOV
#include "DAEDsG4Cerenkov.h"
#else
#include "G4Cerenkov.hh"
#endif

#ifdef USE_CUSTOM_SCINTILLATION
#include "DAEDsG4Scintillation.h"
#else
#include "G4Scintillation.hh"
#endif

#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
//#include "DsG4OpBoundaryProcess.h"
#include "G4ProcessManager.hh"
#include "G4FastSimulationManagerProcess.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"

DECLARE_TOOL(DAEDsPhysConsOptical);

DAEDsPhysConsOptical::DAEDsPhysConsOptical(const G4String& name): G4VPhysicsConstructor(name)
                                                            , ToolBase(name)
{
    declProp("CerenMaxPhotonsPerStep", m_cerenMaxPhotonPerStep = 300);

    declProp("ScintDoReemission", m_doReemission = true);
    declProp("ScintDoScintAndCeren", m_doScintAndCeren = true);

    declProp("UseCerenkov", m_useCerenkov=true);
    declProp("ApplyWaterQe", m_applyWaterQe=true);

    declProp("UseScintillation", m_useScintillation=true);
    declProp("UseRayleigh", m_useRayleigh=true);
    declProp("UseAbsorption", m_useAbsorption=true);
    declProp("UseFastMu300nsTrick", m_useFastMu300nsTrick=false);
    declProp("ScintillationYieldFactor", m_ScintillationYieldFactor = 1.0);
   
    declProp("BirksConstant1", m_birksConstant1 = 6.5e-3*g/cm2/MeV);
    declProp("BirksConstant2", m_birksConstant2 = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV));

    declProp("GammaSlowerTime", m_gammaSlowerTime = 190*ns); 
    declProp("GammaSlowerRatio", m_gammaSlowerRatio = 0.15);

    declProp("NeutronSlowerTime", m_neutronSlowerTime = 220*ns);
    declProp("NeutronSlowerRatio", m_neutronSlowerRatio = 0.34);

    declProp("AlphaSlowerTime", m_alphaSlowerTime = 220*ns);
    declProp("AlphaSlowerRatio", m_alphaSlowerRatio = 0.35);

    declProp("UsePMTOpticalModel", m_doFastSim=true); // just the fast simulation

    //m_cerenPhotonScaleWeight = 3.125;
    //m_scintPhotonScaleWeight = 3.125;
    m_cerenPhotonScaleWeight = 1;
    m_scintPhotonScaleWeight = 1;
}

DAEDsPhysConsOptical::~DAEDsPhysConsOptical()
{
}

void DAEDsPhysConsOptical::ConstructParticle()
{
}

void DAEDsPhysConsOptical::ConstructProcess()
{
#ifdef USE_CUSTOM_CERENKOV
    
    DAEDsG4Cerenkov* cerenkov = 0;
    if (m_useCerenkov) {
        cerenkov = new DAEDsG4Cerenkov();
        cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
        cerenkov->SetApplyPreQE(m_cerenPhotonScaleWeight>1.);
        cerenkov->SetPreQE(1./m_cerenPhotonScaleWeight);
        
        // wangzhe   Give user a handle to control it.   
        cerenkov->SetApplyWaterQe(m_applyWaterQe);
        // wz
        cerenkov->SetTrackSecondariesFirst(true);
    }
#else
    
    G4Cerenkov* cerenkov = 0;
    if (m_useCerenkov) {
        cerenkov = new G4Cerenkov();
        cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
        cerenkov->SetTrackSecondariesFirst(true);
    }
#endif

#ifdef USE_CUSTOM_SCINTILLATION
    DAEDsG4Scintillation* scint = 0;
   
    scint = new DAEDsG4Scintillation();
    scint->SetBirksConstant1(m_birksConstant1);
    scint->SetBirksConstant2(m_birksConstant2);
    scint->SetGammaSlowerTimeConstant(m_gammaSlowerTime);
    scint->SetGammaSlowerRatio(m_gammaSlowerRatio);
    scint->SetNeutronSlowerTimeConstant(m_neutronSlowerTime);
    scint->SetNeutronSlowerRatio(m_neutronSlowerRatio);
    scint->SetAlphaSlowerTimeConstant(m_alphaSlowerTime);
    scint->SetAlphaSlowerRatio(m_alphaSlowerRatio);
    scint->SetDoReemission(m_doReemission);
    scint->SetDoBothProcess(m_doScintAndCeren);
    scint->SetApplyPreQE(m_scintPhotonScaleWeight>1.);
    scint->SetPreQE(1./m_scintPhotonScaleWeight);
    scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); //1.);
    scint->SetUseFastMu300nsTrick(m_useFastMu300nsTrick);
    scint->SetTrackSecondariesFirst(true);
    if (!m_useScintillation) {
        scint->SetNoOp();
    }
#else  // standard G4 scint
    G4Scintillation* scint = 0;
    if (m_useScintillation) {
  
        scint = new G4Scintillation();
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); // 1.);
        scint->SetTrackSecondariesFirst(true);
    }
#endif

    G4OpAbsorption* absorb = 0;
    if (m_useAbsorption) {
        absorb = new G4OpAbsorption();
    }

    DsG4OpRayleigh* rayleigh = 0;
    if (m_useRayleigh) {
        rayleigh = new DsG4OpRayleigh();
	//        rayleigh->SetVerboseLevel(2);
    }

    G4OpBoundaryProcess* boundproc = new G4OpBoundaryProcess();
    //DsG4OpBoundaryProcess* boundproc = new DsG4OpBoundaryProcess();
    boundproc->SetModel(unified);

    G4FastSimulationManagerProcess* fast_sim_man = 0;
    if (m_doFastSim) {
        fast_sim_man = new G4FastSimulationManagerProcess("fast_sim_man");
    }
    
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {

        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
    
        // Caution: as of G4.9, Cerenkov becomes a Discrete Process.
        // This code assumes a version of G4Cerenkov from before this version.

        if(cerenkov && cerenkov->IsApplicable(*particle)) {
            pmanager->AddProcess(cerenkov);
            pmanager->SetProcessOrdering(cerenkov, idxPostStep);
        }

        if(scint && scint->IsApplicable(*particle)) {
            pmanager->AddProcess(scint);
            pmanager->SetProcessOrderingToLast(scint, idxAtRest);
            pmanager->SetProcessOrderingToLast(scint, idxPostStep);
        }

        if (particle == G4OpticalPhoton::Definition()) {
            if (absorb)
                pmanager->AddDiscreteProcess(absorb);
            if (rayleigh)
                pmanager->AddDiscreteProcess(rayleigh);
            pmanager->AddDiscreteProcess(boundproc);
            //pmanager->AddDiscreteProcess(pee);
            if (m_doFastSim) {
                pmanager->AddDiscreteProcess(fast_sim_man);
            }
        }
    }
}
