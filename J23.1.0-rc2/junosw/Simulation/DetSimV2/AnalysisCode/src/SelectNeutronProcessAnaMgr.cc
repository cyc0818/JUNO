
#include "SelectNeutronProcessAnaMgr.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "RootWriter/RootWriter.h"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4Positronium.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "G4AntiNeutrinoE.hh"
#include "TMath.h"

#include "NormalTrackInfo.hh"

#include "TROOT.h"

#include "DetSimAlg/ISimTrackSvc.h"
#include "Event/SimTrack.h"
#include "np/NP.hh"
#include "G4SystemOfUnits.hh"


DECLARE_TOOL(SelectNeutronProcessAnaMgr);

SelectNeutronProcessAnaMgr::SelectNeutronProcessAnaMgr(const std::string& name)
    : ToolBase(name), simtracksvc(NULL)
{

}

SelectNeutronProcessAnaMgr::~SelectNeutronProcessAnaMgr()
{

}

// == Run Action
void
SelectNeutronProcessAnaMgr::BeginOfRunAction(const G4Run* /*aRun*/) {

}

void
SelectNeutronProcessAnaMgr::EndOfRunAction(const G4Run* /*aRun*/) {
   
   if(!m_tag_inelastic.size()){
        LogWarn<<"m_tag_inelastic.size == "
		  	   <<std::endl;
        return;
   }
   JUNO::NP* a = JUNO::NP::Make<int>(m_tag_inelastic);
   const char* FOLD = "/tmp";
   a->save(FOLD,"m_tag_inelastic.npy"); 

   if(m_tag_B11.size()){
	  
		JUNO::NP* b = JUNO::NP::Make<int>(m_tag_B11);
   		//const char* FOLD = "/tmp/huyuxiang/";
   		b->save(FOLD,"m_tag_B11.npy"); 

   }

   if(m_tag_C11.size()){
	  
		JUNO::NP* d = JUNO::NP::Make<int>(m_tag_C11);
   		//const char* FOLD = "/tmp/huyuxiang/";
   		d->save(FOLD,"m_tag_C11.npy"); 
	}
  
   if(m_tag_4_gamma.size()){
	  
		JUNO::NP* e = JUNO::NP::Make<int>(m_tag_4_gamma);
   		//const char* FOLD = "/tmp/huyuxiang/";
   		e->save(FOLD,"m_tag_4_gamma.npy"); 
	}

   if(m_tag_alpha.size()){
	  
		JUNO::NP* f = JUNO::NP::Make<int>(m_tag_alpha);
   		//const char* FOLD = "/tmp/huyuxiang/";
   		f->save(FOLD,"m_tag_alpha.npy"); 
	}
	
   if(m_tag_C12.size()){
	  
		JUNO::NP* g = JUNO::NP::Make<int>(m_tag_C12);
   		//const char* FOLD = "/tmp/huyuxiang/";
   		g->save(FOLD,"m_tag_C12.npy"); 
	}


   if(m_tag_noC12.size()){

		JUNO::NP* c = JUNO::NP::Make<int>(m_tag_noC12);
   		//const char* FOLD = "/tmp/huyuxiang/";
   		c->save(FOLD,"m_tag_noC12.npy"); 

	}



}

// == Event Action
void
SelectNeutronProcessAnaMgr::BeginOfEventAction(const G4Event* evt) {
   std::cout<<"SelectNeutronProcessAnaMgr::BeginOfEventAction [ "
	    <<std::endl;

   //G4int evtID = evt -> GetEventID();    
   //m_tag.push_back(evtID);
}

void
SelectNeutronProcessAnaMgr::EndOfEventAction(const G4Event* /*evt*/) {
   std::cout<<" SelectNeutronProcessAnaMgr::EndOfEventAction ] "
            <<std::endl;
}

// == Tracking Action
void
SelectNeutronProcessAnaMgr::PreUserTrackingAction(const G4Track* aTrack) {
//   SelectNeutronInelastic(aTrack); 
    
}

void
SelectNeutronProcessAnaMgr::PostUserTrackingAction(const G4Track* aTrack) {
}

// == Stepping Action
void
SelectNeutronProcessAnaMgr::UserSteppingAction(const G4Step* step) {
   SelectNeutronInelastic(step);
}

void
SelectNeutronProcessAnaMgr::SelectNeutronInelastic(const G4Step* aStep) {

    
    const G4Track* aTrack = aStep->GetTrack();
    G4ParticleDefinition* particle = aTrack->GetDefinition();
    G4int pdgid = particle->GetPDGEncoding();
    // only select neutron
    if (pdgid != 2112) {
        return;
    }
    // get the secondaries
    G4EventManager* em = G4EventManager::GetEventManager();
    G4TrackingManager* tm = em -> GetTrackingManager();
    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(not secondaries) {
        return;
    }

    size_t nSeco = secondaries->size();

    //find the neutron inelastic.

	bool find_neutron_inelastic = false;
	bool find_C12 = false;
	bool find_alpha = false;
	bool find_C11 = false;
	bool find_4_gamma = false;

	
	G4int evtID =  em->GetConstCurrentEvent()->GetEventID();

    for (size_t i = 0; i < nSeco; ++i) {
        G4Track* sectrk = (*secondaries)[i];
        G4ParticleDefinition* secparticle = sectrk->GetDefinition();
        const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();

		//find if it is neutronInelastics
        if (creatorProcess->GetProcessName() == "neutronInelastic" and !find_neutron_inelastic) {
	    	m_tag_inelastic.push_back(evtID);
            find_neutron_inelastic = true;
        }

		G4int pdgid = secparticle->GetPDGEncoding();
		G4double sec_kine = sectrk->GetKineticEnergy();
	
		//find final particle B11 in neutronInelastic process
		if(pdgid == 1000050110 and find_neutron_inelastic){
			m_tag_B11.push_back(evtID);
		}
	
		//find 4.4MeV gamma
		if(pdgid == 22 and find_neutron_inelastic and !find_4_gamma){

			if ((sec_kine >= 4.3*MeV) and (sec_kine <=4.5*MeV)){
				m_tag_4_gamma.push_back(evtID);
				find_4_gamma = true;
            }
		}
	
	
		//find alpha
		if(pdgid == 1000020040 and find_neutron_inelastic and !find_alpha){
            
			m_tag_alpha.push_back(evtID);
			find_alpha = true;
		}
		
		//find C11
		if(pdgid == 1000060110 and find_neutron_inelastic and !find_C11){
            m_tag_C11.push_back(evtID);
			find_C11 = true;
		}
		
		//find C12
		if(find_neutron_inelastic and pdgid == 1000060120 and !find_C12 ){
			m_tag_C12.push_back(evtID);
			find_C12 = true;
		}		
		
    }

	if (find_neutron_inelastic and !find_C12){
			m_tag_noC12.push_back(evtID);
	}

   
}

