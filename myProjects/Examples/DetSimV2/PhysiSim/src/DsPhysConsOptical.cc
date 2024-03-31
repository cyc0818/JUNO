#include <boost/python.hpp>

#include "DsPhysConsOptical.h"

#include "G4Cerenkov.hh"
#ifdef WITH_G4OPTICKS
#include "LocalG4Cerenkov1042.hh"
#endif

#include "G4Scintillation.hh"
#include "DsG4Scintillation.h"
#include "DsG4ScintSimple.h"
#include "DsG4OpAbsReemit.h"


#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4FastSimulationManagerProcess.hh"



#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

DECLARE_TOOL(DsPhysConsOptical);

DsPhysConsOptical::DsPhysConsOptical(const G4String& name): G4VPhysicsConstructor(name)
                                                            , ToolBase(name)
{
    declProp("OpticksMode", m_opticksMode=0);
    declProp("CerenMaxPhotonsPerStep", m_cerenMaxPhotonPerStep = 300);

    declProp("ScintDoReemission", m_doReemission = true);
    declProp("ScintDoScintAndCeren", m_doScintAndCeren = true);
    declProp("ScintDoReemissionOnly", m_doReemissionOnly = false);

    declProp("UseCerenkov", m_useCerenkov=true);
    declProp("ApplyWaterQe", m_applyWaterQe=true);

    declProp("UseScintillation", m_useScintillation=true);
    declProp("UseScintSimple", m_useScintSimple=false);
    declProp("UseRayleigh", m_useRayleigh=true);
    declProp("UseAbsorption", m_useAbsorption=true);
    declProp("UseAbsReemit", m_useAbsReemit=false);
    declProp("UseFastMu300nsTrick", m_useFastMu300nsTrick=false);
    declProp("ScintillationYieldFactor", m_ScintillationYieldFactor = 1.0);
   
    declProp("UseQuenching", m_enableQuenching=true);
    declProp("BirksConstant1", m_birksConstant1 = 6.5e-3*g/cm2/MeV);
    declProp("BirksConstant2", m_birksConstant2 = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV));

    declProp("GammaSlowerTime", m_gammaSlowerTime = 190*ns); 
    declProp("GammaSlowerRatio", m_gammaSlowerRatio = 0.15);

    declProp("NeutronSlowerTime", m_neutronSlowerTime = 220*ns);
    declProp("NeutronSlowerRatio", m_neutronSlowerRatio = 0.34);

    declProp("AlphaSlowerTime", m_alphaSlowerTime = 220*ns);
    declProp("AlphaSlowerRatio", m_alphaSlowerRatio = 0.35);

    declProp("UsePMTOpticalModel", m_doFastSim=false); // just the fast simulation

    declProp("doDecayTimeFast", flagDecayTimeFast=true);
    declProp("doDecayTimeSlow", flagDecayTimeSlow=true);

    declProp("doTrackSecondariesFirst", m_doTrackSecondariesFirst=true);

    //m_cerenPhotonScaleWeight = 3.125;
    //m_scintPhotonScaleWeight = 3.125;
    m_cerenPhotonScaleWeight = 1;
    m_scintPhotonScaleWeight = 1;
}

DsPhysConsOptical::~DsPhysConsOptical()
{
}

void DsPhysConsOptical::ConstructParticle()
{
}

void DsPhysConsOptical::ConstructProcess()
{
    G4VProcess* cerenkov_ = 0;
    if (m_useCerenkov) {
        if( m_opticksMode == 0 )
        {
            G4Cerenkov* cerenkov = new G4Cerenkov() ;
            cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
            cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
            cerenkov_ = cerenkov ;

        } else {
#ifdef WITH_G4OPTICKS
            LocalG4Cerenkov1042* cerenkov = new LocalG4Cerenkov1042(m_opticksMode) ;
            cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
            cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
            cerenkov_ = cerenkov ;
#else
            G4cout 
               << __FILE__ << ":" << __LINE__ 
               << " DsPhysConsOptical::ConstructProcess "
               << " FATAL "
               << " non-zero opticksMode requires compilation -DWITH_G4OPTICKS " 
               << " m_useCerenkov " << m_useCerenkov 
               << " m_opticksMode " << m_opticksMode 
               << G4endl
               ;
            assert(0) ;   
#endif
        }  
    }

    G4VProcess* scint_ = 0;

    if (m_useScintillation && 1) { // DsG4 (with re-emission)
        DsG4Scintillation* scint = new DsG4Scintillation(m_opticksMode);

        scint->SetDoQuenching(m_enableQuenching);
        scint->SetBirksConstant1(m_birksConstant1);
        scint->SetBirksConstant2(m_birksConstant2);
        scint->SetGammaSlowerTimeConstant(m_gammaSlowerTime);
        scint->SetGammaSlowerRatio(m_gammaSlowerRatio);
        scint->SetNeutronSlowerTimeConstant(m_neutronSlowerTime);
        scint->SetNeutronSlowerRatio(m_neutronSlowerRatio);
        scint->SetAlphaSlowerTimeConstant(m_alphaSlowerTime);
        scint->SetAlphaSlowerRatio(m_alphaSlowerRatio);
        scint->SetDoReemission(m_doReemission);
        scint->SetDoReemissionOnly(m_doReemissionOnly);
        scint->SetDoBothProcess(m_doScintAndCeren);
        scint->SetApplyPreQE(m_scintPhotonScaleWeight>1.);
        scint->SetPreQE(1./m_scintPhotonScaleWeight);
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); //1.);
        scint->SetUseFastMu300nsTrick(m_useFastMu300nsTrick);
        scint->SetTrackSecondariesFirst(true);
        scint->SetFlagDecayTimeFast(flagDecayTimeFast);
        scint->SetFlagDecayTimeSlow(flagDecayTimeSlow);
        scint->SetVerboseLevel(0);


        scint_ = scint;

    }else if (m_useScintillation && 0) { // default G4
        G4Scintillation* scint = 0;  
        scint = new G4Scintillation();
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); // 1.);
        scint->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
        scint_ = scint;
    } 

 if (1 && m_useScintSimple){
        LogInfo<<"Scintillation physics process : ScintSample is used"<<std::endl;
        DsG4ScintSimple * scint= new DsG4ScintSimple();
        scint->SetDoQuenching(m_enableQuenching);
        scint->SetBirksConstant1(m_birksConstant1);
        scint->SetBirksConstant2(m_birksConstant2);
        scint->SetGammaSlowerTimeConstant(m_gammaSlowerTime);
        scint->SetGammaSlowerRatio(m_gammaSlowerRatio);
        scint->SetNeutronSlowerTimeConstant(m_neutronSlowerTime);
        scint->SetNeutronSlowerRatio(m_neutronSlowerRatio);
        scint->SetAlphaSlowerTimeConstant(m_alphaSlowerTime);
        scint->SetAlphaSlowerRatio(m_alphaSlowerRatio);
        scint->SetDoReemission(m_doReemission);
        scint->SetDoReemissionOnly(m_doReemissionOnly);
        scint->SetDoBothProcess(m_doScintAndCeren);
        scint->SetApplyPreQE(m_scintPhotonScaleWeight>1.);
        scint->SetPreQE(1./m_scintPhotonScaleWeight);
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor);
        scint->SetUseFastMu300nsTrick(m_useFastMu300nsTrick);
        scint->SetTrackSecondariesFirst(true);
        scint->SetFlagDecayTimeFast(flagDecayTimeFast);
        scint->SetFlagDecayTimeSlow(flagDecayTimeSlow);
        scint->SetVerboseLevel(0);

        scint_ = scint;   


      }

     DsG4OpAbsReemit* absreemit_PPO =0;
     DsG4OpAbsReemit* absreemit_bisMSB =0;
      if (m_useAbsReemit){
                absreemit_PPO= new DsG4OpAbsReemit("PPO");
                absreemit_bisMSB= new DsG4OpAbsReemit("bisMSB");
                 absreemit_PPO->SetVerboseLevel(0);
                 absreemit_bisMSB->SetVerboseLevel(0);
              }


    

    G4OpAbsorption* absorb = 0;
    if (m_useAbsorption) {
        absorb = new G4OpAbsorption();
    }

    G4OpRayleigh* rayleigh = 0;
    if (m_useRayleigh) {
        rayleigh = new G4OpRayleigh();
	//        rayleigh->SetVerboseLevel(2);
    }

    G4OpBoundaryProcess* boundproc = new G4OpBoundaryProcess();
    boundproc->SetInvokeSD(false);


    G4FastSimulationManagerProcess* fast_sim_man = 0;
    if (m_doFastSim) {
        fast_sim_man = new G4FastSimulationManagerProcess("fast_sim_man");
    }

    auto theParticleIterator=G4ParticleTable::GetParticleTable()->GetIterator();
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {

        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
    
        // Caution: as of G4.9, Cerenkov becomes a Discrete Process.
        // This code assumes a version of G4Cerenkov from before this version.

        if(cerenkov_ && cerenkov_->IsApplicable(*particle)) {
            pmanager->AddProcess(cerenkov_);
            pmanager->SetProcessOrdering(cerenkov_, idxPostStep);
        }

        if(scint_ && scint_->IsApplicable(*particle)) {
            if (m_useScintSimple) LogInfo << "Associate Scintillation with Particle " << (particle->GetParticleName()) << std::endl;

            pmanager->AddProcess(scint_);
            pmanager->SetProcessOrderingToLast(scint_, idxAtRest);
            pmanager->SetProcessOrderingToLast(scint_, idxPostStep);
        }

       /* if (particle == G4OpticalPhoton::Definition()) {
            if (absorb)
                pmanager->AddDiscreteProcess(absorb);
            if (rayleigh)
                pmanager->AddDiscreteProcess(rayleigh);
            pmanager->AddDiscreteProcess(boundproc);
            //pmanager->AddDiscreteProcess(pee);
            if (m_doFastSim) {
                std::cout << "############## Using new PMT optical model!!! " << std::endl;
                pmanager->AddDiscreteProcess(fast_sim_man);
            } else {
                std::cout << "############## Using old PMT optical model!!! " << std::endl;
            }
        }*/
          if(particle==G4OpticalPhoton:: Definition()){
              if (absreemit_PPO)
                  pmanager->AddDiscreteProcess(absreemit_PPO);
              if (absreemit_bisMSB)
                  pmanager->AddDiscreteProcess(absreemit_bisMSB);
              if (absorb)
                  pmanager->AddDiscreteProcess(absorb);
              if (rayleigh)
                  pmanager->AddDiscreteProcess(rayleigh);
              pmanager->AddDiscreteProcess(boundproc);
             //pmanager->AddDiscretePrcess(pee);
              if (m_doFastSim)
                  pmanager->AddDiscreteProcess(fast_sim_man);
           }
  }
}
