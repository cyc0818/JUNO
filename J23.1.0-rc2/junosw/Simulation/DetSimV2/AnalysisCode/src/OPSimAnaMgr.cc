#include <boost/python.hpp>

#include "OPSimAnaMgr.hh"
//  for event
#include <sstream>
#include <cassert>
#include "junoHit_PMT.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4LossTableManager.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4SystemOfUnits.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "RootWriter/RootWriter.h"

#include "NormalTrackInfo.hh"
#include "EvtNavigator/NavBuffer.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/SimHeader.h"

#include "OPSimulator/IOPSimSvc.h"

#include "BirksLawCalculator.hh"
#include "G4Cerenkov_modified.hh"
#include <MCParamsSvc/IMCParamsSvc.hh>

DECLARE_TOOL(OPSimAnaMgr);

OPSimAnaMgr::OPSimAnaMgr(const std::string& name) 
    : ToolBase(name), m_opsimsvc(0)
{
 //   declProp("BirksConstant1", m_BirksConstant1 = 6.5e-3*g/cm2/MeV);
 //   declProp("BirksConstant2", m_BirksConstant2 = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV));
    declProp("TestingRatio", m_testing_ratio = 1.0);
}

OPSimAnaMgr::~OPSimAnaMgr()
{

}

void
OPSimAnaMgr::BeginOfRunAction(const G4Run* /*aRun*/) {
    // Birks' law calculator
    m_birksLawCalculator = BirksLawCalculator::Instance();

    if(not m_birksLawCalculator->getMCParamsSvc()){
        SniperPtr<IMCParamsSvc> mcgt(*getParent(), "MCParamsSvc");
        if(mcgt.invalid()){
            G4cout<<"Can't find MCParamsSvc."<<G4endl;
            assert(0);
        }
        m_birksLawCalculator->setMCParamsSvc(mcgt.data());
    }

    // check the RootWriter is Valid.

    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enalbe it in your job option file."
                 << std::endl;
        return;
    }

    SniperPtr<IOPSimSvc> opsimsvc(*getParent(), "OPSimSvc");
    if (opsimsvc.valid()) {
        LogInfo << "Get the OPSimSvc" << std::endl;
        m_opsimsvc = opsimsvc.data();
    }

}

void
OPSimAnaMgr::EndOfRunAction(const G4Run* /*aRun*/) {

}

void
OPSimAnaMgr::BeginOfEventAction(const G4Event* evt) {
    m_eventID = evt->GetEventID();
}

void
OPSimAnaMgr::EndOfEventAction(const G4Event* evt) {

    // invoke OPSimSvc
    if (m_opsimsvc) {

        opsimulator::IOPSimulator* opsim = m_opsimsvc->get_opsimulator();

        bool need_op = true; // user need to add their own logic here

        // if the ratio is set in [0, 1), then apply this ratio
        if (0<=m_testing_ratio && m_testing_ratio<1) {
            G4double r = G4UniformRand();
            if (r < m_testing_ratio) {
                need_op = true;
            } else {
                need_op = false;
            }
        }

        if (need_op) {
            LogInfo << "Start OP simulation using OP simulator. " << std::endl;
            opsim->simulate();
            LogInfo << "OP Simulator is done. " << std::endl;
        } else { 
            LogInfo << "Skip the OP simulator. " << std::endl;
            opsim->clear_gensteps();
        }

    }
}


void
OPSimAnaMgr::PreUserTrackingAction(const G4Track* aTrack) {
}

void
OPSimAnaMgr::PostUserTrackingAction(const G4Track* aTrack) {
}

void
OPSimAnaMgr::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    G4double edep = step->GetTotalEnergyDeposit();
    G4ParticleDefinition* aParticle = track->GetDefinition();

    // invoke OPSimSvc
    if (m_opsimsvc && track->GetDefinition() != G4OpticalPhoton::Definition()) {

        opsimulator::IOPSimulator* opsim = m_opsimsvc->get_opsimulator();

        opsimulator::IGenStep* gs = opsim->create_genstep();
        gs->eventid(m_eventID);

        gs->trackid(track->GetTrackID());
        gs->pdgcode(track->GetParticleDefinition()->GetPDGEncoding());
        gs->trackst(track->GetTrackStatus());
        gs->matname(track->GetMaterial()->GetName());

        gs->stepno(track->GetCurrentStepNumber());
        gs->edep(edep);

        double QuenchedTotalEnergyDeposit = 
            m_birksLawCalculator->calQuenchedEnergy(step);
        
        gs->evis(QuenchedTotalEnergyDeposit);
        // end 

        gs->steplen(step->GetStepLength());

        gs->pre_x(step->GetPreStepPoint()->GetPosition().x());
        gs->pre_y(step->GetPreStepPoint()->GetPosition().y());
        gs->pre_z(step->GetPreStepPoint()->GetPosition().z());
        gs->pre_t(step->GetPreStepPoint()->GetGlobalTime());
        gs->pre_velocity(step->GetPreStepPoint()->GetVelocity());

        gs->post_x(step->GetPostStepPoint()->GetPosition().x());
        gs->post_y(step->GetPostStepPoint()->GetPosition().y());
        gs->post_z(step->GetPostStepPoint()->GetPosition().z());
        gs->post_t(step->GetPostStepPoint()->GetGlobalTime());
        gs->post_velocity(step->GetPostStepPoint()->GetVelocity());

        gs->delta_x(step->GetDeltaPosition().x());
        gs->delta_y(step->GetDeltaPosition().y());
        gs->delta_z(step->GetDeltaPosition().z());


        // cerenkov related
        // - beta
        gs->pre_beta(step->GetPreStepPoint()->GetBeta());
        gs->post_beta(step->GetPreStepPoint()->GetBeta());
        // - num of cerenkov photons
        static bool ceren_proc_init = false;
        static G4Cerenkov_modified* ceren_proc = nullptr;
        if (!ceren_proc_init && !ceren_proc) {
            G4ProcessManager* ElectronManager =
                G4Electron::Electron()->GetProcessManager();
            if (ElectronManager) {
                G4int N =
                    ElectronManager->GetProcessList()->entries();
                G4ProcessVector* fProcessVector =
                    ElectronManager->GetProcessList();
                for ( G4int i=0; i<N; i++) {
                    G4VProcess* fCurrentProcess = (*fProcessVector)[i];
                    if (fCurrentProcess && fCurrentProcess->GetProcessName() == "Cerenkov") {
                        std::cout << __FILE__ << ":" << __LINE__ << ": "
                                  << "Found Cerenkov process " << fCurrentProcess << std::endl;
                        ceren_proc = dynamic_cast<G4Cerenkov_modified*>(fCurrentProcess);
                        std::cout << __FILE__ << ":" << __LINE__ << ": "
                                  << "dynamic_cast the Found Cerenkov process to G4Cerenkov_modified " << ceren_proc << std::endl;
                    }
                }
            }
            ceren_proc_init = true;
        }

        if (ceren_proc and ceren_proc->GetNumPhotons()>0) {
            LogDebug << " --> Cerenkov photons: " << ceren_proc->GetNumPhotons() << std::endl;
            gs->num_ceren(ceren_proc->GetNumPhotons());
            gs->num_ceren_1(ceren_proc->GetNumPhotons1());
            gs->num_ceren_2(ceren_proc->GetNumPhotons2());
            // in order to avoid the duplication, reset the number to zero.
            // This requires the modification on G4Cerenkov process
            ceren_proc->ResetNumPhotons();

        } else {
            gs->num_ceren(0);
            gs->num_ceren_1(0);
            gs->num_ceren_2(0);
        }
    }


}

