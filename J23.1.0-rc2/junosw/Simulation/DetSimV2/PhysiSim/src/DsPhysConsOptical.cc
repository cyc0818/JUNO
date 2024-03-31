#include <boost/python.hpp>

#include "EGet/EGet.h"
#include "DsPhysConsOptical.h"

#include "G4Cerenkov.hh"
#include "G4Cerenkov_modified.hh"
#include "G4Scintillation.hh"
#include "DsG4Scintillation.h"
#include "DsG4ScintSimple.h"
#include "DsG4OpAbsReemit.h"


#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"

#include "G4OpBoundaryProcess.hh"
#include "C4OpBoundaryProcess.hh"

#include "G4ProcessManager.hh"
#include "G4FastSimulationManagerProcess.hh"



#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhotonSource.hh"
#include "G4PhotonConversion.hh"

DECLARE_TOOL(DsPhysConsOptical);

DsPhysConsOptical::DsPhysConsOptical(const G4String& name): G4VPhysicsConstructor(name)
                                                            , ToolBase(name)
{
    declProp("OpticksMode", m_opticksMode=0);
    declProp("CerenMaxPhotonsPerStep", m_cerenMaxPhotonPerStep = 300);
    declProp("CerenPhotonStack", m_cerenPhotonStack = true);

    declProp("ScintDoReemission", m_doReemission = true);
    declProp("ScintDoScintAndCeren", m_doScintAndCeren = true);
    declProp("ScintDoReemissionOnly", m_doReemissionOnly = false);

    declProp("UseCerenkov", m_useCerenkov=true);
    declProp("UseCerenkovType", m_useCerenkovType="modified");
    declProp("ApplyWaterQe", m_applyWaterQe=true);

    declProp("UseScintillation", m_useScintillation=true);
    declProp("UseScintSimple", m_useScintSimple=false);
    declProp("UseRayleigh", m_useRayleigh=true);
    declProp("UseAbsorption", m_useAbsorption=true);
    declProp("UseAbsReemit", m_useAbsReemit=false);
    declProp("UseFastMu300nsTrick", m_useFastMu300nsTrick=false);
    declProp("ScintillationYieldFactor", m_ScintillationYieldFactor = 1.0);
   
    declProp("UseQuenching", m_enableQuenching=true);


    declProp("UsePMTOpticalModel", m_UsePMTOpticalModel=true);
    declProp("UsePMTNaturalGeometry", m_UsePMTNaturalGeometry=true);
    m_doFastSim = false ;  // maybe changed later based on config of above two 


    declProp("doDecayTimeFast", flagDecayTimeFast=true);
    declProp("doDecayTimeSlow", flagDecayTimeSlow=true);

    declProp("doTrackSecondariesFirst", m_doTrackSecondariesFirst=true);

    declProp("UsePhotonConversion", m_photonconversion_flag=false);
    declProp("EnableDfball", m_dfball_flag=false);
    declProp("nProducedPhotons", m_nProducedPhotons=10);
    declProp("FixedEnergy", m_fixed_energy=0.00000467);
    declProp("OffsetInX", m_dfball_x=0.);
    declProp("OffsetInY", m_dfball_y=0.);
    declProp("OffsetInZ", m_dfball_z=0.);

    //m_cerenPhotonScaleWeight = 3.125;
    //m_scintPhotonScaleWeight = 3.125;
    m_cerenPhotonScaleWeight = 1;
    m_scintPhotonScaleWeight = 1;

    SetVerboseLevel(EGet::Get<int>("DsPhysConsOptical_verboseLevel",0));
}

DsPhysConsOptical::~DsPhysConsOptical()
{
}


void DsPhysConsOptical::ConstructParticle()
{
}

void DsPhysConsOptical::SetParameters(){
     

      SniperPtr<IMCParamsSvc> mcgt(*getParent(), "MCParamsSvc");
      if (mcgt.invalid()) {
          G4cout << "Can't find MCParamsSvc." << G4endl;
          assert(0);
       }
      IMCParamsSvc::map_s2d LS_constant_map;
      bool st = mcgt->Get("Material.LS.ConstantProperty", LS_constant_map);
        if (st && LS_constant_map.count("eBirksConstant1") && LS_constant_map.count("eBirksConstant2")
               && LS_constant_map.count("pBirksConstant1") && LS_constant_map.count("pBirksConstant2")
               && LS_constant_map.count("aBirksConstant1") && LS_constant_map.count("aBirksConstant2")) {

           m_birksConstant1.insert(std::pair<int, double> (11, LS_constant_map["eBirksConstant1"]));
           m_birksConstant2.insert(std::pair<int, double> (11, LS_constant_map["eBirksConstant2"]));
           m_birksConstant1.insert(std::pair<int, double> (2212, LS_constant_map["pBirksConstant1"]));
           m_birksConstant2.insert(std::pair<int, double> (2212, LS_constant_map["pBirksConstant2"]));
           m_birksConstant1.insert(std::pair<int, double> (1000020040, LS_constant_map["aBirksConstant1"]));
           m_birksConstant2.insert(std::pair<int, double> (1000020040, LS_constant_map["aBirksConstant2"]));
          }
        else{
         LogError<<" BirksConstant doesn't exist.Please check."<<std::endl;
        }
        
        LogInfo<<"Electron birksConstant1 = "<<m_birksConstant1[11]<<std::endl;
        LogInfo<<"Electron birksConstant2 = "<<m_birksConstant2[11]<<std::endl;
        LogInfo<<"Proton birksConstant1 = "<<m_birksConstant1[2212]<<std::endl;
        LogInfo<<"Proton birksConstant2 = "<<m_birksConstant2[2212]<<std::endl;
        LogInfo<<"Alpha birksConstant1 = "<<m_birksConstant1[1000020040]<<std::endl;
        LogInfo<<"Alpha birksConstant2 = "<<m_birksConstant2[1000020040]<<std::endl;
}




/**
DsPhysConsOptical::doFastSim
------------------------------

See Simulation/DetSimV2/PMTSim/include/CommonPMTManager.h for notes on the PMT*POM quadrants 

**/

bool DsPhysConsOptical::doFastSim() const 
{
    return m_UsePMTOpticalModel == true && m_UsePMTNaturalGeometry == false ; 
}

void DsPhysConsOptical::ConstructProcess()
{
    SetParameters();
    m_doFastSim = doFastSim() ; 
   
    G4VProcess* cerenkov_ = 0;
  
    LogInfo<<"check: m_useCerenKov == "<< m_useCerenkov <<std::endl;
    LogInfo<<"check: m_useScintillation == "<< m_useScintillation  <<std::endl;
    LogInfo<<"check:  m_useScintSimple == "<<  m_useScintSimple <<std::endl;
    if (m_useCerenkov) 
    {
        if (m_useCerenkovType == "modified") {
            G4Cerenkov_modified* cerenkov = new G4Cerenkov_modified() ;
            cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
            cerenkov->SetStackPhotons(m_cerenPhotonStack);
            cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
            cerenkov->SetOpticksMode(m_opticksMode); 
            cerenkov->SetVerboseLevel(EGet::Get<int>("G4Cerenkov_modified_verboseLevel",0));
            cerenkov_ = cerenkov ;
        } else if (m_useCerenkovType == "original") {
            G4Cerenkov* cerenkov = new G4Cerenkov() ;
            cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
            cerenkov->SetStackPhotons(m_cerenPhotonStack);
            cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
            cerenkov->SetVerboseLevel(EGet::Get<int>("G4Cerenkov_verboseLevel",0));
            cerenkov_ = cerenkov ;
        } else {
            G4cerr << __FILE__ << ":" << __LINE__
                   << " Unknown m_useCerenkovType: '"
                   << m_useCerenkovType << "'"
                   << G4endl;
            assert(0);
        }

    }

    G4VProcess* scint_ = 0;

    if (m_useScintillation && 1) { // DsG4 (with re-emission)
        DsG4Scintillation* scint = new DsG4Scintillation(m_opticksMode);

        scint->SetDoQuenching(m_enableQuenching);
        scint->SetBirksConstant1(m_birksConstant1);
        scint->SetBirksConstant2(m_birksConstant2);
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
        scint->SetVerboseLevel(EGet::Get<int>("DsG4Scintillation_verboseLevel",0));


        scint_ = scint;

    }else if (m_useScintillation && 0) { // default G4
        G4Scintillation* scint = 0;  
        scint = new G4Scintillation();
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); // 1.);
        scint->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
        scint->SetVerboseLevel(EGet::Get<int>("G4Scintillation_verboseLevel",0));
        scint_ = scint;
    } 

 if (1 && m_useScintSimple){
        LogInfo<<"Scintillation physics process : ScintSample is used"<<std::endl;
        DsG4ScintSimple * scint= new DsG4ScintSimple();
        scint->SetDoQuenching(m_enableQuenching);
        scint->SetBirksConstant1(m_birksConstant1);
        scint->SetBirksConstant2(m_birksConstant2);
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
        scint->SetVerboseLevel(EGet::Get<int>("DsG4ScintSimple_verboseLevel",0));
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
	    rayleigh->SetVerboseLevel(EGet::Get<int>("G4OpRayleigh_verboseLevel",0));
    }



    G4VProcess* boundproc_ = nullptr ; 
    G4FastSimulationManagerProcess* fast_sim_man = 0;

    G4int level = GetVerboseLevel(); 



    if(level>0)
    {
        std::cout 
            << "DsPhysConsOptical::ConstructProcess" 
            << " DsPhysConsOptical_verboseLevel " << level 
            << " m_UsePMTNaturalGeometry " << m_UsePMTNaturalGeometry
            << " m_UsePMTOpticalModel " << m_UsePMTOpticalModel
            << " m_doFastSim " << m_doFastSim
            << std::endl
            ;
    }

    LogInfo<<"check:  level                   : "<<  level       <<std::endl;
    LogInfo<<"check:  m_UsePMTNaturalGeometry : "<<  m_UsePMTNaturalGeometry <<std::endl;
    LogInfo<<"check:  m_UsePMTOpticalModel    : "<<  m_UsePMTOpticalModel <<std::endl;
    LogInfo<<"check:  m_doFastSim             : "<<  m_doFastSim <<std::endl;

    if(m_doFastSim)  // using m_doFastSim to configure use of the old impl 
    {
        fast_sim_man = new G4FastSimulationManagerProcess("fast_sim_man");
    }

    C4OpBoundaryProcess* boundproc = CreateCustomG4OpBoundaryProcess();
    boundproc->SetInvokeSD(false);
    boundproc_ = boundproc ;  


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
              pmanager->AddDiscreteProcess(boundproc_);
             //pmanager->AddDiscretePrcess(pee);
              if (fast_sim_man)
                  pmanager->AddDiscreteProcess(fast_sim_man);
           }
  }

//   Setting for "Photon Source Particle"
  G4PhotonConversion* conversion = 0;
  if (m_photonconversion_flag) {
      conversion = new G4PhotonConversion();
      conversion->SetnProducedPhotons(m_nProducedPhotons);
      conversion->SetOpEnergy(m_fixed_energy);
      conversion->SetDfBall(m_dfball_flag);
      conversion->SetDfBallPosX(m_dfball_x);
      conversion->SetDfBallPosY(m_dfball_y);
      conversion->SetDfBallPosZ(m_dfball_z);

      G4ParticleDefinition* photonsource = G4PhotonSource::PhotonSource();
      G4ProcessManager* man = photonsource->GetProcessManager();
      man->AddProcess(conversion,  0,-1,1);
  }
}


#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#include "PMTSimParamSvc/PMTSimParamData.h"
#include "PMTSimParamSvc/PMTAccessor.h"

C4OpBoundaryProcess* DsPhysConsOptical::CreateCustomG4OpBoundaryProcess()
{
    SniperPtr<IPMTSimParamSvc> psps_ptr(*getParent(), "PMTSimParamSvc"); 

    if(psps_ptr.invalid()) 
    {   
        std::cout << "invalid" << std::endl ;   
        return nullptr ; 
    }   

    IPMTSimParamSvc* ipsps = psps_ptr.data(); 
    PMTSimParamData* pspd = ipsps->getPMTSimParamData() ; 

    C4IPMTAccessor* accessor = new PMTAccessor(pspd) ; 
    C4OpBoundaryProcess* boundproc = new C4OpBoundaryProcess(accessor) ; 
    std::cout << "DsPhysConsOptical::CreateCustomG4OpBoundaryProcess" << std::endl ; 

    return boundproc ; 
}
