#include <boost/python.hpp>
#include <iostream>

#include "DepositEnergyAnaMgr.hh"
#include "BirksLawCalculator.hh"
#include "NormalTrackInfo.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "RootWriter/RootWriter.h"
#include "G4Event.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4LossTableManager.hh"
#include "G4OpticalPhoton.hh"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/SimHeader.h"
#include "G4SystemOfUnits.hh"

#include "DetSimAlg/ISimTrackSvc.h"
#include "Event/SimTrack.h"
#include <MCParamsSvc/IMCParamsSvc.hh>

DECLARE_TOOL(DepositEnergyAnaMgr);

DepositEnergyAnaMgr::DepositEnergyAnaMgr(const std::string& name)
    : ToolBase(name), simtracksvc(NULL)
{
    declProp("EnableNtuple", m_flag_ntuple=true);
    declProp("UseQuenching", m_enableQuenching=true);
    m_evt_tree = 0;
}

DepositEnergyAnaMgr::~DepositEnergyAnaMgr()
{

}

void
DepositEnergyAnaMgr::BeginOfRunAction(const G4Run* /* aRun */) {
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

    // SimTrackSvc
    SniperPtr<ISimTrackSvc> simtracksvc_ptr(getParent(), "SimTrackSvc");
    if (simtracksvc_ptr.invalid()) {
        // create the service
        simtracksvc = dynamic_cast<ISimTrackSvc*>(getParent()->createSvc("SimTrackSvc"));
    } else {
        simtracksvc = simtracksvc_ptr.data();
    }

    if (not m_flag_ntuple) {
        return;
    }
    // check the RootWriter is Valid.
    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enalbe it in your job option file."
                 << std::endl;
        return;
    }
    m_evt_tree = svc->bookTree(*m_par, "SIMEVT/prmtrkdep", "Deposit Energy Info for every primary track");

    m_evt_tree->Branch("evtID", &m_eventID, "evtID/I");
    m_evt_tree->Branch("nInitParticles", &m_init_nparticles, "nInitParticles/I");
    m_evt_tree->Branch("PDGID", m_pdgid, "PDGID[nInitParticles]/I");
    m_evt_tree->Branch("TrackID", m_trkid, "TrackID[nInitParticles]/I");

    m_evt_tree->Branch("edep", m_edep, "edep[nInitParticles]/F");
    m_evt_tree->Branch("edepX", m_edep_x, "edepX[nInitParticles]/F");
    m_evt_tree->Branch("edepY", m_edep_y, "edepY[nInitParticles]/F");
    m_evt_tree->Branch("edepZ", m_edep_z, "edepZ[nInitParticles]/F");

    m_evt_tree->Branch("Qedep",  m_q_edep,   "Qedep[nInitParticles]/F");
    m_evt_tree->Branch("QedepX", m_q_edep_x, "QedepX[nInitParticles]/F");
    m_evt_tree->Branch("QedepY", m_q_edep_y, "QedepY[nInitParticles]/F");
    m_evt_tree->Branch("QedepZ", m_q_edep_z, "QedepZ[nInitParticles]/F");

    m_evt_tree->Branch("edepNotInLS", m_edep_notinLS, "edepNotInLS[nInitParticles]/F");
}

void
DepositEnergyAnaMgr::EndOfRunAction(const G4Run* /* aRun */) {

}

void
DepositEnergyAnaMgr::BeginOfEventAction(const G4Event* evt) {
    m_eventID = evt->GetEventID();
    m_init_nparticles = 0;

    for (int i = 0; i < MAX_PARTICLES; ++i) {
        m_pdgid[i] = 0;
        m_trkid[i] = 0;
        m_edep[i] = 0.0;
        m_edep_x[i] = 0.0;
        m_edep_y[i] = 0.0;
        m_edep_z[i] = 0.0;

        m_q_edep[i] = 0.0;
        m_q_edep_x[i] = 0.0;
        m_q_edep_y[i] = 0.0;
        m_q_edep_z[i] = 0.0;

        m_edep_notinLS[i] = 0.0;
    }
   
}

void
DepositEnergyAnaMgr::EndOfEventAction(const G4Event* evt) {
    G4int nVertex = evt -> GetNumberOfPrimaryVertex();
    int maxidx = 0;
    for (G4int index=0; index < nVertex; ++index) {
        G4PrimaryVertex* vtx = evt->GetPrimaryVertex( index );

        G4PrimaryParticle* pp = vtx -> GetPrimary();
        while (pp) {

            int trkid = pp -> GetTrackID();
            if (trkid < 0) {
                // skip the track
                pp = pp->GetNext();
                continue;
            }
            int idx = trkid - 1;

            if (idx > maxidx) {
                maxidx = idx;
            }

            m_pdgid[idx] = pp -> GetPDGcode();
            m_trkid[idx] = trkid;

            if (m_edep[idx] > 0) {
                m_edep_x[idx] /= m_edep[idx];
                m_edep_y[idx] /= m_edep[idx];
                m_edep_z[idx] /= m_edep[idx];
            }
            if (m_q_edep[idx] > 0) {
                m_q_edep_x[idx] /= m_q_edep[idx];
                m_q_edep_y[idx] /= m_q_edep[idx];
                m_q_edep_z[idx] /= m_q_edep[idx];
            }

            ++m_init_nparticles;
            pp = pp->GetNext();
        }

    }

    assert(m_init_nparticles >= maxidx);

    if (m_flag_ntuple and m_evt_tree) {
        m_evt_tree -> Fill();
    }
    save_into_data_model();
}

void
DepositEnergyAnaMgr::PreUserTrackingAction(const G4Track* aTrack) {
 
    if ( aTrack->GetParentID()==0 ) {
           
        int trkid= aTrack->GetTrackID();
        auto jm_trk = simtracksvc -> get(trkid);
        if(!jm_trk){
            jm_trk = new JM::SimTrack();
            jm_trk->setTrackID(trkid);
            jm_trk->setParentID(aTrack->GetParentID());
            simtracksvc->put(jm_trk);
        }  
    } 
  
}

void
DepositEnergyAnaMgr::PostUserTrackingAction(const G4Track* aTrack) {
  
    NormalTrackInfo* trackinfo = (NormalTrackInfo*)aTrack->GetUserInformation();
    if (not trackinfo) {
        return;
    }
    G4int prmtrkid = trackinfo->GetOriginalTrackID();

    if (prmtrkid <= 0) {
        return;
    }
    G4int idx = prmtrkid - 1;
    
    auto& m_tracks=simtracksvc->all();
    for (auto trk: m_tracks) {
  
        if ( prmtrkid == trk->getTrackID()) {
            if ( m_edep[idx] > 0 ){
                trk->setEdep(m_edep[idx]);
                trk->setEdepX( m_edep_x[idx]/m_edep[idx] );
                trk->setEdepY( m_edep_y[idx]/m_edep[idx] );
                trk->setEdepZ( m_edep_z[idx]/m_edep[idx] );
            }
            if( m_q_edep[idx] > 0){
                trk->setQEdep(  m_q_edep[idx] );
                trk->setQEdepX( m_q_edep_x[idx]/m_q_edep[idx] );
                trk->setQEdepY( m_q_edep_y[idx]/m_q_edep[idx] );
                trk->setQEdepZ( m_q_edep_z[idx]/m_q_edep[idx] );
            }
            trk->setEdepNotInLS( m_edep_notinLS[idx] );
        }
    } 

}

void
DepositEnergyAnaMgr::UserSteppingAction(const G4Step* step) {

    // cache the material pointer, avoid comparison using string.
    static G4Material* s_mat_LS = nullptr;
    if (!s_mat_LS) {
        s_mat_LS = G4Material::GetMaterial("LS");
    }

    G4Track* track = step->GetTrack();
    NormalTrackInfo* trackinfo = (NormalTrackInfo*)track->GetUserInformation();
    if (not trackinfo) {
        return;
    }
    G4int prmtrkid = trackinfo->GetOriginalTrackID();
    if (prmtrkid <= 0) {
        return;
    }
    G4int idx = prmtrkid - 1;

    if (idx>=MAX_PARTICLES) {
        LogError << "idx is greater than the MAX_PARTICLES." << std::endl;
        LogError << "Please consider to increate MAX_PARTICLES." << std::endl;
        return;
    }

    G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0 and track->GetDefinition() != G4OpticalPhoton::Definition()
                 and track->GetMaterial() == s_mat_LS) {
        // By default, the energies of secondaries are accumulated to the primary tracks.
        // If the track is also traced by the simtracksvc, then we don't accumulate the 
        // energies to the primary tracks.
        // TODO: in the future, we will use a unified way to manage all the tracks.
        bool accumulated_to_primary = true;

        if (simtracksvc) {
            const std::vector<G4int>& ancestors = trackinfo->getTracedAncestors();
            if (ancestors.size() && ancestors.back() != prmtrkid) {
                accumulated_to_primary = false;
            }
        }

        G4ThreeVector pos = step -> GetPreStepPoint() -> GetPosition();
        G4double q_edep = edep;
        if (m_enableQuenching){
            q_edep = m_birksLawCalculator->calQuenchedEnergy(step);
        }

        if (accumulated_to_primary) {
            m_edep[idx] += edep;

            m_edep_x[idx] += edep * pos.x();
            m_edep_y[idx] += edep * pos.y();
            m_edep_z[idx] += edep * pos.z();


            m_q_edep[idx] += q_edep;
            m_q_edep_x[idx] += q_edep * pos.x();
            m_q_edep_y[idx] += q_edep * pos.y();
            m_q_edep_z[idx] += q_edep * pos.z();
        }

        // Now, calculate the edep/Qedep of marked ancestors
        if (simtracksvc) {
            const std::vector<G4int>& ancestors = trackinfo->getTracedAncestors();
            // std::cout << "current track: " << track->GetTrackID() << std::endl;
            // for (auto trkid: ancestors) { // in the previous implementation, accumulate all
            //     std::cout << " -> " << trkid << std::endl;
            // }
            int trkid = -1;
            if (ancestors.size()) {
                // only get the last ancestor
                trkid = ancestors.back();
            } else {
                // other cases...
            }

            if (trkid != -1) {
                //for (auto trkid: ancestors) { // in the previous implementation, accumulate all
                auto jm_trk = simtracksvc->get(trkid);
                if (!jm_trk) {
                    LogWarn << "Failed to find track with trackID "
                            << trkid
                            << std::endl;
                    // continue;
                } else {
            
                    // before update
                    double total_edep = jm_trk->getEdep();
                    double x_edep = jm_trk->getEdepX() * total_edep;
                    double y_edep = jm_trk->getEdepY() * total_edep;
                    double z_edep = jm_trk->getEdepZ() * total_edep;

                    double total_Qedep = jm_trk->getQEdep();
                    double x_Qedep = jm_trk->getQEdepX() * total_Qedep;
                    double y_Qedep = jm_trk->getQEdepY() * total_Qedep;
                    double z_Qedep = jm_trk->getQEdepZ() * total_Qedep;

                
                    // after update: add the info
                    total_edep += edep;
                    x_edep += edep * pos.x();
                    y_edep += edep * pos.y();
                    z_edep += edep * pos.z();

                    x_edep /= total_edep;
                    y_edep /= total_edep;
                    z_edep /= total_edep;

                    total_Qedep += q_edep;
                    x_Qedep += q_edep * pos.x();
                    y_Qedep += q_edep * pos.y();
                    z_Qedep += q_edep * pos.z();

                    x_Qedep /= total_Qedep;
                    y_Qedep /= total_Qedep;
                    z_Qedep /= total_Qedep;

                    // update the track
                    jm_trk->setEdep  (total_edep);
                    jm_trk->setEdepX (x_edep);
                    jm_trk->setEdepY (y_edep);
                    jm_trk->setEdepZ (z_edep);

                    jm_trk->setQEdep (total_Qedep);
                    jm_trk->setQEdepX(x_Qedep);
                    jm_trk->setQEdepY(y_Qedep);
                    jm_trk->setQEdepZ(z_Qedep);


                }

            }
        }



    } else if (edep > 0 and track->GetDefinition() != G4OpticalPhoton::Definition()
                        and track->GetMaterial() != s_mat_LS) {
        m_edep_notinLS[idx] += edep;

        if (simtracksvc) {
            const std::vector<G4int>& ancestors = trackinfo->getTracedAncestors();

            for (auto trkid: ancestors) {
                auto jm_trk = simtracksvc->get(trkid);
                if (!jm_trk) {
                    LogWarn << "Failed to find track with trackID "
                            << trkid
                            << std::endl;
                    continue;
                }

                jm_trk->setEdepNotInLS(jm_trk->getEdepNotInLS() + edep);

            }
        }
    }

}

bool DepositEnergyAnaMgr::save_into_data_model() {
    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        return false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        return false;
    }

    const auto& m_paths= evt_nav->getPath();
    std::vector<std::string>::const_iterator pos = std::find(m_paths.begin(), m_paths.end(), "/Event/Sim");
    if ( m_paths.end() == pos ) {
        return false;
    }
    
    auto m_simheader = JM::getHeaderObject<JM::SimHeader>(evt_nav);
    LogDebug << "simhdr: " << m_simheader << std::endl;
    if (not m_simheader) {
        return false;
    }
    auto m_simevent = dynamic_cast<JM::SimEvt*>(m_simheader->event());
    LogDebug << "simevt: " << m_simevent << std::endl;
    if (not m_simevent) {
        return false;
    }

    // Fill the Deposit Energy and Quenched Energy
    for (int i = 0; i < m_init_nparticles; ++i) {
        int trkid = m_trkid[i];
        auto trk = m_simevent->findTrackByTrkID(trkid);
        trk->setEdep( m_edep[i] );
        trk->setEdepX( m_edep_x[i] );
        trk->setEdepY( m_edep_y[i] );
        trk->setEdepZ( m_edep_z[i] );

        trk->setQEdep(  m_q_edep[i] );
        trk->setQEdepX( m_q_edep_x[i] );
        trk->setQEdepY( m_q_edep_y[i] );
        trk->setQEdepZ( m_q_edep_z[i] );

        trk->setEdepNotInLS( m_edep_notinLS[i] );
    }
    return true;
}
