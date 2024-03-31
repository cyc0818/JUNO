
#include "InteresingProcessAnaMgr.hh"

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
#include "Event/SimVertex.h"

DECLARE_TOOL(InteresingProcessAnaMgr);

InteresingProcessAnaMgr::InteresingProcessAnaMgr(const std::string& name)
    : ToolBase(name), simtracksvc(NULL)
{
    declProp("ExtraProcNames", m_extra_procnames);
    declProp("legacyMichelElectron", m_legacy_michel_electron);
    declProp("legacyNeutronCapture", m_legacy_ncapture);
    declProp("legacyDecay", m_legacy_decay);

}

InteresingProcessAnaMgr::~InteresingProcessAnaMgr()
{

}

// == Run Action
void
InteresingProcessAnaMgr::BeginOfRunAction(const G4Run* /*aRun*/) {
    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");

    gROOT->ProcessLine("#include <vector>");

    if (m_legacy_michel_electron) {
        m_michel_tree = svc->bookTree(*m_par, "SIMMICHEL/michael", "michel");
        m_michel_tree -> Branch("michelID", &m_eventID, "michelID/I");
        m_michel_tree -> Branch("pdgid", &michael_pdgid);
        m_michel_tree -> Branch("kine", &michael_kine);
        m_michel_tree -> Branch("px", &michael_px);
        m_michel_tree -> Branch("py", &michael_py);
        m_michel_tree -> Branch("pz", &michael_pz);
        m_michel_tree -> Branch("x", &michael_pos_x);
        m_michel_tree -> Branch("y", &michael_pos_y);
        m_michel_tree -> Branch("z", &michael_pos_z);
        m_michel_tree -> Branch("t", &michael_t);
    }

    if (m_legacy_ncapture) {
        m_neutron_tree = svc->bookTree(*m_par, "SIMEVT/nCapture", "Neutron Capture");
        m_neutron_tree -> Branch("evtID", &m_eventID, "evtID/I");
        m_neutron_tree -> Branch("NeutronTrkid", &neutron_only_trkid);
        m_neutron_tree -> Branch("NeutronKine",  &neutron_only_kine );
        m_neutron_tree -> Branch("NeutronCaptureT", &neutron_only_t );
        m_neutron_tree -> Branch("NCStartX", &neutron_only_start_x );
        m_neutron_tree -> Branch("NCStartY", &neutron_only_start_y );
        m_neutron_tree -> Branch("NCStartZ", &neutron_only_start_z );
        m_neutron_tree -> Branch("NCStopX", &neutron_only_stop_x );
        m_neutron_tree -> Branch("NCStopY", &neutron_only_stop_y );
        m_neutron_tree -> Branch("NCStopZ", &neutron_only_stop_z );
        m_neutron_tree -> Branch("NCTrackLength", &neutron_only_track_length );
        // secondaries
        m_neutron_tree -> Branch("trkid", &neutron_trkid);
        m_neutron_tree -> Branch("pdgid", &neutron_pdgid);
        m_neutron_tree -> Branch("kine", &neutron_kine);
        m_neutron_tree -> Branch("px", &neutron_px);
        m_neutron_tree -> Branch("py", &neutron_py);
        m_neutron_tree -> Branch("pz", &neutron_pz);
        m_neutron_tree -> Branch("x", &neutron_pos_x);
        m_neutron_tree -> Branch("y", &neutron_pos_y);
        m_neutron_tree -> Branch("z", &neutron_pos_z);
        m_neutron_tree -> Branch("t", &neutron_t);
    }

    // SimTrackSvc
    SniperPtr<ISimTrackSvc> simtracksvc_ptr(getParent(), "SimTrackSvc");
    if (simtracksvc_ptr.invalid()) {
        // create the service
        simtracksvc = dynamic_cast<ISimTrackSvc*>(getParent()->createSvc("SimTrackSvc"));
    } else {
        simtracksvc = simtracksvc_ptr.data();
    }

    // initialize InterestedProcessMgr
    ipm.particle2skip[G4OpticalPhoton::Definition()] = 1;
    ipm.particle2skip[G4Positronium::Definition()] = 1;

    // register the proc name
    ipm.procname2use["Decay"] = 1;
    ipm.procname2use["RadioactiveDecay"] = 1;
    ipm.procname2use["nCapture"] = 1;
    ipm.procname2use["neutronInelastic"] = 1;
    ipm.procname2use["muMinusCaptureAtRest"] = 1;
    ipm.procname2use["muPairProd"] = 1;

    // extra proc name
    for (auto procname: m_extra_procnames) {
        if (ipm.procname2use.count(procname)) {
            LogInfo << "SKIP duplicating process name: " << procname << std::endl;
            continue;
        }
        LogInfo << "REGISTER extra process name " << procname << std::endl;
        ipm.procname2use[procname] = 1;
    }
}

void
InteresingProcessAnaMgr::EndOfRunAction(const G4Run* /*aRun*/) {
    LogInfo << "All the collected process names: " << std::endl;
    for (auto [k,v]: proc_map) {
        std::string flagrecorded = " [ ] ";
        if (ipm.procname2use[k->GetProcessName()]) {
            flagrecorded = " [x] ";
        }
        LogInfo << " - " 
                << flagrecorded
                << k->GetProcessName() 
                << std::endl;
    }
}

// == Event Action
void
InteresingProcessAnaMgr::BeginOfEventAction(const G4Event* evt) {
    m_eventID = evt->GetEventID();
    michael_pdgid.clear();
    michael_kine .clear();
    michael_px   .clear();
    michael_py   .clear();
    michael_pz   .clear();
    michael_pos_x.clear();
    michael_pos_y.clear();
    michael_pos_z.clear();
    michael_t    .clear();

    neutron_only_trkid.clear();
    neutron_only_kine .clear();
    neutron_only_t    .clear();
    neutron_only_start_x.clear();
    neutron_only_start_y.clear();
    neutron_only_start_z.clear();
    neutron_only_stop_x.clear();
    neutron_only_stop_y.clear();
    neutron_only_stop_z.clear();
    neutron_only_track_length.clear();

    neutron_trkid.clear();
    neutron_pdgid.clear();
    neutron_kine .clear();
    neutron_px   .clear();
    neutron_py   .clear();
    neutron_pz   .clear();
    neutron_pos_x.clear();
    neutron_pos_y.clear();
    neutron_pos_z.clear();
    neutron_t    .clear();

   
    michel_trkid .clear();
    neutron_sec_trkid.clear();
}

void
InteresingProcessAnaMgr::EndOfEventAction(const G4Event* /*evt*/) {
    if (m_legacy_michel_electron) {
        m_michel_tree -> Fill();
    }

    if (m_legacy_ncapture) {
        m_neutron_tree -> Fill();
    }
}

// == Tracking Action
void
InteresingProcessAnaMgr::PreUserTrackingAction(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    // LogDebug << "NormalTrackInfo: " << info << std::endl;

    if (info) {
        // select michael electron
        if (info->isMichelElectron() && m_legacy_michel_electron) {
            saveMichel(aTrack);
        }
        if (info->isNeutronCapture() && m_legacy_ncapture) {
            saveNeutronCapture(aTrack);
        }

        if (info->getDecayMode() && m_legacy_decay) {
            // if it is the secondaries of g4decay
            saveDecayInit(aTrack);
        }


        if (info->isInterestedProcess()) {
            saveSecondaryInit(aTrack);
        }
    }
}

void
InteresingProcessAnaMgr::PostUserTrackingAction(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());

    if (m_legacy_michel_electron) {
        selectMichel(aTrack);
    }
    if (m_legacy_ncapture) {
        selectNeutronCapture(aTrack);
    }
    if (m_legacy_decay) {
        selectDecay(aTrack);
    }

    selectInterested(aTrack);

    if (info) {
        // as secondaries, store the exit info
        if (info->isNeutronCapture() && m_legacy_ncapture) {
            saveNeutronCaptureSecondary(aTrack);
        }

        if (info->getDecayMode() && m_legacy_decay) {
            // if it is the secondaries of g4decay
            saveDecayExit(aTrack);
        }

        if (info->isInterestedProcess()) {
            saveSecondaryExit(aTrack);
        }

    }
}

// == Stepping Action
void
InteresingProcessAnaMgr::UserSteppingAction(const G4Step* step) {

}

// Helper
// == Select Michel
void 
InteresingProcessAnaMgr::selectMichel(const G4Track* aTrack) {
    G4ParticleDefinition* particle = aTrack->GetDefinition();
    G4int pdgid = particle->GetPDGEncoding();
    // only select the mu- and mu+
    if (pdgid != 13 and pdgid != -13) {
        return;
    }
    // get the secondaries
    G4TrackingManager* tm = G4EventManager::GetEventManager() 
                                            -> GetTrackingManager();
    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(not secondaries) {
        return;
    }
    size_t nSeco = secondaries->size();

    bool find_michael = false;
    int idx_michael = -1;
    G4Track* trk_michael = 0;
    // what particle is to select
    // -- mu-
    // --- Capture at Rest
    if (pdgid == 13) {
        int idx_anti_nu_e = -1;
        double kine_michael_electron = -1;
        for (size_t i = 0; i < nSeco; ++i) {
            G4Track* sectrk = (*secondaries)[i];
            G4ParticleDefinition* secparticle = sectrk->GetDefinition();
            // const G4String& sec_name = secparticle->GetParticleName();
            const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();
            G4double sec_kine = sectrk->GetKineticEnergy();
       
            if (creatorProcess->GetProcessName()!="muMinusCaptureAtRest") {
                continue;
            }
            if (secparticle == G4AntiNeutrinoE::Definition() ) {
                idx_anti_nu_e = i;
            } else if (secparticle == G4Electron::Definition() and sec_kine > kine_michael_electron) {
                idx_michael = i;
                trk_michael = sectrk;
            }
        }
        // find the michael electron
        if (idx_michael != -1 and idx_anti_nu_e != -1 and trk_michael) {
            find_michael = true;
        }

    } else if (pdgid == -13) {
        for (size_t i = 0; i < nSeco; ++i) {
            G4Track* sectrk = (*secondaries)[i];
            G4ParticleDefinition* secparticle = sectrk->GetDefinition();
            // const G4String& sec_name = secparticle->GetParticleName();
            const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();

            if (creatorProcess->GetProcessName()!="Decay") {
                continue;
            }
            if (secparticle == G4Positron::Definition()) {
                idx_michael = i;
                trk_michael = sectrk;
            }

        }
        // find the michael positron
        if (idx_michael != -1 and trk_michael) {
            find_michael = true;
        }

    }

    // Fill the info
    if (not find_michael) {
        return;
    }
    NormalTrackInfo* info = (NormalTrackInfo*)(trk_michael->GetUserInformation());
    if (not info) {
        LogWarn << "--- update the track. create new Track Info" << std::endl;
        info = new NormalTrackInfo(aTrack);
        trk_michael->SetUserInformation(info);
    }
    info -> setMichelElectron();

}

// == save Michel Electron
void
InteresingProcessAnaMgr::saveMichel(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    if ((not info) or (not info->isMichelElectron())) {
        return;
    }
    LogDebug << "!-- Michel Electron: " << std::endl;
    // Please note, in Post Tracking Action, we can't get the right
    // track ID of the secondaries.
    LogDebug << "!--- TrkID: " << aTrack->GetTrackID() << std::endl;
    LogDebug << "!--- Kine : " << aTrack->GetKineticEnergy() << std::endl;
    
    //Please note: Geant4 may invoke PreUsertrackAction and PostUserTrackingAction many 
    //times when we open optical simulation. Thus we should check the track ID when we want 
    //to store variables.   
    int ID = aTrack->GetTrackID();
    std::vector<int>::iterator ret;
    ret = std::find(michel_trkid.begin(), michel_trkid.end(), ID);
    if(ret!= michel_trkid.end()){
        return;
    } else{
        michel_trkid.push_back(ID);      
    }
    //-----------------------------------//

    michael_pdgid.push_back(aTrack->GetDefinition()->GetPDGEncoding());
    michael_kine .push_back(aTrack->GetKineticEnergy());
    const G4ThreeVector& mom = aTrack->GetMomentum();
    michael_px   .push_back(mom.x());
    michael_py   .push_back(mom.y());
    michael_pz   .push_back(mom.z());
    const G4ThreeVector& pos = aTrack->GetPosition();
    michael_pos_x.push_back(pos.x());
    michael_pos_y.push_back(pos.y());
    michael_pos_z.push_back(pos.z());
    michael_t    .push_back(aTrack->GetGlobalTime());

    LogDebug << "!---- Kine : " << michael_kine .back() << std::endl;
    LogDebug << "!---- Mome : " << michael_px.back()
                        << ", " << michael_py.back()
                        << ", " << michael_pz.back() << std::endl;
    LogDebug << "!---- Posi : " << michael_pos_x.back()
                        << ", " << michael_pos_y.back()
                        << ", " << michael_pos_z.back() << std::endl;
    LogDebug << "!---- Time : " << michael_t    .back() << std::endl;

}
// == Select Neutron Capture
void
InteresingProcessAnaMgr::selectNeutronCapture(const G4Track* aTrack) {
    G4ParticleDefinition* particle = aTrack->GetDefinition();
    G4int pdgid = particle->GetPDGEncoding();
    // only select neutron
    if (pdgid != 2112) {
        return;
    }
    // get the secondaries
    G4TrackingManager* tm = G4EventManager::GetEventManager() 
                                            -> GetTrackingManager();
    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(not secondaries) {
        return;
    }
    size_t nSeco = secondaries->size();

    bool find_neutron_capture = false;

    for (size_t i = 0; i < nSeco; ++i) {
        G4Track* sectrk = (*secondaries)[i];
        G4ParticleDefinition* secparticle = sectrk->GetDefinition();
        const G4String& sec_name = secparticle->GetParticleName();
        const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();

        if (creatorProcess->GetProcessName()!="nCapture") {
            continue;
        }
        if (not find_neutron_capture) {
            find_neutron_capture = true;
        }
        LogDebug << "*--- Neutron Capture: " << std::endl;
        LogDebug << "*---- particle: " << sec_name << std::endl;
        LogDebug << "*---- kinetic : " << sectrk->GetKineticEnergy() << std::endl;
        // set flag
        NormalTrackInfo* info = (NormalTrackInfo*)(sectrk->GetUserInformation());

        if (not info) {
            LogWarn << "--- get Track Info failed, create new Track Info" << std::endl;
            info = new NormalTrackInfo(aTrack);
            sectrk->SetUserInformation(info);
        }

        info -> setNeutronCapture();
    }

    // save the current neutron
    if (find_neutron_capture) {
        // save the current track (the neutron self)
        neutron_only_trkid.push_back(aTrack->GetTrackID());
        neutron_only_kine .push_back(aTrack->GetVertexKineticEnergy());
        neutron_only_t    .push_back(aTrack->GetGlobalTime());
        const G4ThreeVector& start_pos = aTrack->GetVertexPosition();
        const G4ThreeVector& stop_pos  = aTrack->GetPosition();
        neutron_only_start_x.push_back(start_pos.x());
        neutron_only_start_y.push_back(start_pos.y());
        neutron_only_start_z.push_back(start_pos.z());
        neutron_only_stop_x.push_back(stop_pos.x());
        neutron_only_stop_y.push_back(stop_pos.y());
        neutron_only_stop_z.push_back(stop_pos.z());
        neutron_only_track_length.push_back(aTrack->GetTrackLength());

        LogDebug << "***--- Neutron Capture: " << std::endl;
        LogDebug << "***---- Vkinetic: " << aTrack->GetVertexKineticEnergy() << std::endl;
        LogDebug << "***----  kinetic: " << aTrack->GetKineticEnergy() << std::endl;
        LogDebug << "***---- StartPos: " << start_pos.x() << " " 
                                         << start_pos.y() << " "
                                         << start_pos.z() << std::endl;
        LogDebug << "***---- Stop Pos: " << stop_pos.x() << " " 
                                         << stop_pos.y() << " "
                                         << stop_pos.z() << std::endl;
        LogDebug << "***---- Length  : " << aTrack->GetTrackLength() << std::endl;

    }
}
// == save Neutron Capture
void
InteresingProcessAnaMgr::saveNeutronCapture(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    if ((not info) or (not info->isNeutronCapture())) {
        return;
    }

    // Tag the current track ID will be traced,
    // so that the edep will be accumulated in DepositEnergyAnaMgr

    LogDebug << "!-- Neutron Capture: " << std::endl;
    LogDebug << "!--- TrkID: " << aTrack->GetTrackID() << std::endl;
    LogDebug << "!--- Kine : " << aTrack->GetKineticEnergy() << std::endl;
  
    //-------------------------------------//
    int ID = aTrack->GetTrackID();
    std::vector<int>::iterator ret;
    ret = std::find(neutron_sec_trkid.begin(), neutron_sec_trkid.end(), ID);
    if(ret!= neutron_sec_trkid.end()){
     return;
     }
    else{
          neutron_sec_trkid.push_back(ID);
        }   
    //-------------------------------------//

    neutron_trkid.push_back(aTrack->GetParentID());
    neutron_pdgid.push_back(aTrack->GetDefinition()->GetPDGEncoding());
    neutron_kine .push_back(aTrack->GetKineticEnergy());
    const G4ThreeVector& mom = aTrack->GetMomentum();
    neutron_px   .push_back(mom.x());
    neutron_py   .push_back(mom.y());
    neutron_pz   .push_back(mom.z());
    const G4ThreeVector& pos = aTrack->GetPosition();
    neutron_pos_x.push_back(pos.x());
    neutron_pos_y.push_back(pos.y());
    neutron_pos_z.push_back(pos.z());
    neutron_t    .push_back(aTrack->GetGlobalTime());

    saveSecondaryInit(aTrack);
}

void
InteresingProcessAnaMgr::saveNeutronCaptureSecondary(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    if ((not info) or (not info->isNeutronCapture())) {
        return;
    }

    saveSecondaryExit(aTrack);
}

// ===========================================================================
// Decay related
// ===========================================================================

void InteresingProcessAnaMgr::selectDecay(const G4Track* aTrack) {
    G4ParticleDefinition* particle = aTrack->GetDefinition();
    // skip optical photon
    if (particle == G4OpticalPhoton::Definition()) {
        return;
    }
    // skip positronium
    if (particle == G4Positronium::Definition()) {
        return;
    }

    // get the secondaries
    G4TrackingManager* tm = G4EventManager::GetEventManager() 
                                            -> GetTrackingManager();
    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(not secondaries) {
        return;
    }

    size_t nSeco = secondaries->size();

    for (size_t i = 0; i < nSeco; ++i) {
        G4Track* sectrk = (*secondaries)[i];
        G4ParticleDefinition* secparticle = sectrk->GetDefinition();
        // const G4String& sec_name = secparticle->GetParticleName();
        const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();

        if (creatorProcess->GetProcessName()!="Decay"
            && creatorProcess->GetProcessName()!="RadioactiveDecay") {
            continue;
        }

        NormalTrackInfo* info = (NormalTrackInfo*)(sectrk->GetUserInformation());

        if (not info) {
            LogWarn << "--- get Track Info failed, create new Track Info" << std::endl;
            info = new NormalTrackInfo(aTrack);
            sectrk->SetUserInformation(info);
        }

        info -> setDecayMode();


    }

}

void InteresingProcessAnaMgr::saveDecayInit(const G4Track* aTrack) {
    saveSecondaryInit(aTrack);
}

void InteresingProcessAnaMgr::saveDecayExit(const G4Track* aTrack) {
    saveSecondaryExit(aTrack);
}

void
InteresingProcessAnaMgr::saveSecondaryInit(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    if ((not info)) {
        return;
    }
    // Create SimTrack for the secondaries
    if (simtracksvc) {
        // NOTE: because pre tracking action could be invoked
        //       multiple times for the same track, we need to
        //       check whether the sim track already exists or not.
        if (simtracksvc->get(aTrack->GetTrackID())) {
            return;
        }

        // NOTE: The ancestor is used to control whether accumulate the edep
        //       to primary track or not.
        info->markTracedAncestors(aTrack->GetTrackID());
        // info->markTracedAncestors(aTrack->GetParentID());
        JM::SimTrack* jm_trk = new JM::SimTrack();
        jm_trk->setPDGID   (aTrack->GetDefinition()->GetPDGEncoding());
        jm_trk->setTrackID (aTrack->GetTrackID());
        jm_trk->setParentID(aTrack->GetParentID());
        // if (info->getTracedAncestors().size()) {
        //     // the first element is assumed to be the ancestor
        //     jm_trk->setAncestorID(info->getTracedAncestors()[0]);
        // }
        jm_trk->setAncestorID(info->GetOriginalTrackID());

        jm_trk->setInitMass(aTrack->GetDefinition()->GetPDGMass());

        // FIXME:
        //   In some cases, this method is not invoked by Pre Tracking Action.
        //   So the Init Px/Py/Pz and X/Y/Z/T are not correct. 
        //
        //   A possible solution is saving these info in the user track info.
        //   However, it will use 56 bytes (2x G4ThreeVector and 1x double). 
        //
        //   Maybe in the future, we need to divide the user track info into
        //   two parts. One could be shared by both OP (optical photons) and 
        //   non-OP. Another are used by non-OP only.
        //
        //   Tao Lin, 22 March 2023

        jm_trk->setInitPx  (aTrack->GetMomentum().x());
        jm_trk->setInitPy  (aTrack->GetMomentum().y());
        jm_trk->setInitPz  (aTrack->GetMomentum().z());
        jm_trk->setInitX   (aTrack->GetPosition().x());
        jm_trk->setInitY   (aTrack->GetPosition().y());
        jm_trk->setInitZ   (aTrack->GetPosition().z());
        jm_trk->setInitT   (aTrack->GetGlobalTime());

        // Exit info is collected in InteresingProcessAnaMgr::saveNeutronCaptureSecondary
        // Edep/Qedep is collected in DepositEnergyAnaMgr

        // associate track and vertex in SimTrackSvc
        auto trkidx = simtracksvc->put(jm_trk);
        auto vertexidx = info->getOriginalVertexIDX();
        if (vertexidx != -1 && vertexidx < simtracksvc->allVertices().size()) {
            auto vertex = simtracksvc->allVertices()[vertexidx];
            vertex->addTrackIdxOut(trkidx);
            LogWarn << "associate vertexidx -> trkidx " << vertexidx << " -> " << trkidx << std::endl;
        }
    }


}

void
InteresingProcessAnaMgr::saveSecondaryExit(const G4Track* aTrack) {
    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    if ((not info)) {
        return;
    }

    // Create SimTrack for the secondaries
    if (simtracksvc) {
        int current_track_id = aTrack->GetTrackID();

        JM::SimTrack* jm_trk = simtracksvc->get(current_track_id);
        if (!jm_trk) {
            LogWarn << "Failed to find track with trackID "
                    << current_track_id
                    << std::endl;
            return;
        }

        jm_trk->setExitPx  (aTrack->GetMomentum().x());
        jm_trk->setExitPy  (aTrack->GetMomentum().y());
        jm_trk->setExitPz  (aTrack->GetMomentum().z());
        jm_trk->setExitX   (aTrack->GetPosition().x());
        jm_trk->setExitY   (aTrack->GetPosition().y());
        jm_trk->setExitZ   (aTrack->GetPosition().z());
        jm_trk->setExitT   (aTrack->GetGlobalTime());

        jm_trk->setTrackLength(aTrack->GetTrackLength());

    }

}


// ===========================================================================
// A generic selector
// ===========================================================================
void InteresingProcessAnaMgr::selectInterested(const G4Track* parentTrack) {
    // -----------------------------------------------------------------------
    // skip some particles
    // -----------------------------------------------------------------------
    G4ParticleDefinition* parentParticle = parentTrack->GetDefinition();
    // skip optical photon
    if (ipm.particle2skip.count(parentParticle)) {
        return;
    }

    LogDebug << " selectInterested " << std::endl;


    // -----------------------------------------------------------------------
    // loop the secondaries
    // -----------------------------------------------------------------------

    G4TrackingManager* tm = G4EventManager::GetEventManager() 
                                            -> GetTrackingManager();
    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(not secondaries) {
        return;
    }

    size_t nSeco = secondaries->size();

    // following temp map is used to create vertex
    std::unordered_map<const G4VProcess*, std::vector<int>> group_by_proc;

    for (size_t i = 0; i < nSeco; ++i) {
        G4Track* sectrk = (*secondaries)[i];
        G4ParticleDefinition* secparticle = sectrk->GetDefinition();

        if (ipm.particle2skip.count(secparticle)) {
            continue;
        }

        const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();

        // if the creator is empty, that particle is not created by G4
        if (!creatorProcess) {
            continue;
        }

        if (proc_map.count(creatorProcess) == 0) {
            proc_map[creatorProcess] = 1;
        }

        LogDebug << " creatorProcess Name: " << creatorProcess->GetProcessName() << std::endl;

        // in order to speed up this part, please avoid to use string.
        // if the creatorProcess is not in the map, just skip
        if (ipm.procname2use.count(creatorProcess->GetProcessName()) == 0) {
            continue;
        }

        NormalTrackInfo* info = (NormalTrackInfo*)(sectrk->GetUserInformation());

        if (not info) {
            LogWarn << "--- get Track Info failed, create new Track Info" << std::endl;
            info = new NormalTrackInfo(parentTrack);
            sectrk->SetUserInformation(info);
        }

        // check if the vertex is already created
        group_by_proc[creatorProcess].push_back(i);
        LogWarn << "creatorProcess " << creatorProcess << " " << i << std::endl;

    }

    // if there is no vertex found, just skip this parent track.
    if (group_by_proc.size() == 0) {
        return;
    }


    int parentTrkIdx = simtracksvc->getTrackIdx(parentTrack->GetTrackID());
    if (parentTrkIdx == -1) {
        // if the track is not in simtracksvc, need to register it. 
        saveSecondaryInit(parentTrack);
        saveSecondaryExit(parentTrack);
        parentTrkIdx = simtracksvc->getTrackIdx(parentTrack->GetTrackID());
    }
    if (parentTrkIdx == -1) {
        LogWarn << "Can't find parent track in simtracksvc. "
                << " parent track id: " << parentTrack->GetTrackID() 
                << std::endl;
    }

    // now, we can create the vertex according to the group_by_proc
    for (auto [procptr, trkidcol]: group_by_proc) {
        std::vector<JM::SimVertex*> vertices;
        std::vector<G4ThreeVector> posvertices;

        for (auto trkidx: trkidcol) {
            auto sectrack = (*secondaries)[trkidx];

            auto& vertexpos = sectrack->GetPosition();

            // find a vertex with same vertex pos
            JM::SimVertex* v = nullptr;
            for (size_t i = 0; i < posvertices.size(); ++i) {
                if (posvertices[i] == vertexpos) {
                    v = vertices[i];
                }
            }
            if (not v) {
                v = new JM::SimVertex();
                v->setXYZT(vertexpos.x(), vertexpos.y(), vertexpos.z(), sectrack->GetGlobalTime());
                v->setProcName(procptr->GetProcessName());
                v->addTrackIdxIn(parentTrkIdx);

                LogWarn << "Creating a new vertex "
                        << v->getX() << "," << v->getY() << "," << v->getZ() << "," << v->getT()
                        << " " << v->getProcName() << std::endl;

                vertices.push_back(v);
                posvertices.push_back(vertexpos);

                auto idx_vertex = simtracksvc->putVertex(v); // idx in the vertex collection
                v->setObjID(idx_vertex);
                
            }

            // update the relationship
            auto idx_vertex = v->getObjID();

            NormalTrackInfo* info = (NormalTrackInfo*)(sectrack->GetUserInformation());
            info->setOriginalVertexIDX(idx_vertex);
            info->setInterestedProcess();
            LogWarn << "Associate vertex " << idx_vertex << " with track " << trkidx << std::endl;
        }

    }

}
