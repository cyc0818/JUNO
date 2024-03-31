#include <boost/python.hpp>
#include <iostream>

#include "DepositEnergyCalibAnaMgr.hh"
#include "NormalTrackInfo.hh"
#include "BirksLawCalculator.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "RootWriter/RootWriter.h"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4LossTableManager.hh"
#include "G4SystemOfUnits.hh"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/SimHeader.h"

#include <MCParamsSvc/IMCParamsSvc.hh>


DECLARE_TOOL(DepositEnergyCalibAnaMgr);

DepositEnergyCalibAnaMgr::DepositEnergyCalibAnaMgr(const std::string& name)
    : ToolBase(name)
{
    declProp("EnableNtuple", m_flag_ntuple=false);
    m_evt_tree = 0;
}

DepositEnergyCalibAnaMgr::~DepositEnergyCalibAnaMgr()
{

}

void
DepositEnergyCalibAnaMgr::BeginOfRunAction(const G4Run* /*aRun*/) {
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
    m_evt_tree = svc->bookTree(*m_par, "SIMEVT/pelletron", "Deposit Energy Info for every primary track");

    m_evt_tree->Branch("evtID", &m_eventID, "evtID/I");
    m_evt_tree->Branch("nInitParticles", &m_init_nparticles, "nInitParticles/I");
    m_evt_tree->Branch("PDGID", m_pdgid, "PDGID[nInitParticles]/I");

    m_evt_tree->Branch("edep", m_edep, "edep[nInitParticles]/F");
    m_evt_tree->Branch("edepX", m_edep_x, "edepX[nInitParticles]/F");
    m_evt_tree->Branch("edepY", m_edep_y, "edepY[nInitParticles]/F");
    m_evt_tree->Branch("edepZ", m_edep_z, "edepZ[nInitParticles]/F");

    m_evt_tree->Branch("Qedep",  m_q_edep,   "Qedep[nInitParticles]/F");
    m_evt_tree->Branch("QedepX", m_q_edep_x, "QedepX[nInitParticles]/F");
    m_evt_tree->Branch("QedepY", m_q_edep_y, "QedepY[nInitParticles]/F");
    m_evt_tree->Branch("QedepZ", m_q_edep_z, "QedepZ[nInitParticles]/F");

    m_evt_tree->Branch("edepNotInLS", m_edep_notinLS, "edepNotInLS[nInitParticles]/F");

    m_evt_tree->Branch("edep_LS", m_edep_LS, "edep_LS[nInitParticles]/F");
    m_evt_tree->Branch("edep_Mylar", m_edep_Mylar, "edep_Mylar[nInitParticles]/F");
    m_evt_tree->Branch("edep_Acrylic", m_edep_Acrylic, "edep_Acrylic[nInitParticles]/F");

    m_evt_tree->Branch("travel_len_LS", m_travel_len_LS, "travel_len_LS[nInitParticles]/F");
    m_evt_tree->Branch("travel_len_Mylar", m_travel_len_Mylar, "travel_len_Mylar[nInitParticles]/F");
    m_evt_tree->Branch("travel_len_Acrylic", m_travel_len_Acrylic, "travel_len_Acrylic[nInitParticles]/F");
}

void
DepositEnergyCalibAnaMgr::EndOfRunAction(const G4Run* /*aRun*/) {

}

void
DepositEnergyCalibAnaMgr::BeginOfEventAction(const G4Event* evt) {
    m_eventID = evt->GetEventID();
    m_init_nparticles = 0;

    for (int i = 0; i < 100; ++i) {
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

        m_edep_LS[i] = 0.0;
        m_edep_Mylar[i] = 0.0;
        m_edep_Acrylic[i] = 0.0;

        m_travel_len_LS[i] = 0.0;
        m_travel_len_Mylar[i] = 0.0;
        m_travel_len_Acrylic[i] = 0.0;
    }

}

void
DepositEnergyCalibAnaMgr::EndOfEventAction(const G4Event* evt) {
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
DepositEnergyCalibAnaMgr::PreUserTrackingAction(const G4Track* aTrack) {
    G4ParticleDefinition* particle = aTrack->GetDefinition();
    G4int pdgid = particle->GetPDGEncoding();
    if (pdgid == 22 and aTrack->GetParentID() == 1) {
        LogDebug << "++++++++++++++ gamma " << std::endl;
    } else {
        return;
    }
    const G4VProcess* creatorProcess = aTrack->GetCreatorProcess();

    G4double trk_kine = aTrack->GetKineticEnergy();
    LogDebug << "+++++++++++++++++++" << creatorProcess->GetProcessName() << std::endl;
    LogDebug << "+++++++++++++++++++" << trk_kine << std::endl;

    if (creatorProcess->GetProcessName() == "annihil") {
        LogDebug << "++++++++++++++++++++++ " << aTrack->GetVolume() << std::endl;
        LogDebug << "++++++++++++++++++++++ " << aTrack->GetVolume()->GetName() << std::endl;
        // LogDebug << "++++++++++++++++++++++ " << aTrack->GetMaterial()->GetName() << std::endl;
    }

}

void
DepositEnergyCalibAnaMgr::PostUserTrackingAction(const G4Track* aTrack) {
    G4ParticleDefinition* particle = aTrack->GetDefinition();
    G4int pdgid = particle->GetPDGEncoding();
    if (pdgid == -11) {
        LogDebug << "++++++++++++++ positron " << std::endl;
    } else {
        return;
    }
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
        if (secparticle != G4Gamma::Definition()) {
            continue;
        }
        const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();

        LogDebug << "+++++++++++++++" << creatorProcess->GetProcessName() << std::endl;
    }

}

void
DepositEnergyCalibAnaMgr::UserSteppingAction(const G4Step* step) {
    G4Track* track = step->GetTrack();
    // = if the track is an optical photon, skip it =
    if (track->GetDefinition() == G4OpticalPhoton::Definition()) {
        return;
    }
    // = make sure the NormalAnaMgr is enabled =
    NormalTrackInfo* trackinfo = (NormalTrackInfo*)track->GetUserInformation();
    if (not trackinfo) {
        return;
    }
    G4int prmtrkid = trackinfo->GetOriginalTrackID();
    if (prmtrkid <= 0) {
        return;
    }

    // cache the material pointer, avoid comparison using string.
    static G4Material* s_mat_LS = nullptr;
    static G4Material* s_mat_Mylar = nullptr;
    static G4Material* s_mat_Acrylic = nullptr;
    if (!s_mat_LS) { s_mat_LS = G4Material::GetMaterial("LS"); }
    if (!s_mat_Mylar) { s_mat_Mylar = G4Material::GetMaterial("Mylar"); }
    if (!s_mat_Acrylic) { s_mat_Acrylic = G4Material::GetMaterial("Acrylic"); }

    G4int idx = prmtrkid - 1;
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep > 0 and track->GetDefinition()!= G4OpticalPhoton::Definition()
                 and track->GetMaterial() == s_mat_LS) {
        m_edep[idx] += edep;
        G4ThreeVector pos = step -> GetPreStepPoint() -> GetPosition();
        m_edep_x[idx] += edep * pos.x();
        m_edep_y[idx] += edep * pos.y();
        m_edep_z[idx] += edep * pos.z();

        G4double q_edep = m_birksLawCalculator->calQuenchedEnergy(step);
        m_q_edep[idx] += q_edep;
        m_q_edep_x[idx] += q_edep * pos.x();
        m_q_edep_y[idx] += q_edep * pos.y();
        m_q_edep_z[idx] += q_edep * pos.z();

        // deposit energy in LS
        m_edep_LS[idx] += edep;
        m_travel_len_LS[idx] += step->GetStepLength();

    } else if (edep > 0 and track->GetDefinition()!= G4OpticalPhoton::Definition()
                        and track->GetMaterial() != s_mat_LS) {
        m_edep_notinLS[idx] += edep;

        if (track->GetMaterial() == s_mat_Mylar) {
            m_edep_Mylar[idx] += edep;
            m_travel_len_Mylar[idx] += step->GetStepLength();
        } else if (track->GetMaterial() == s_mat_Acrylic) {
            m_edep_Acrylic[idx] += edep;
            m_travel_len_Acrylic[idx] += step->GetStepLength();
        }
    }
    // get the start and end position in the window.
    // only primary particle and gamma from e+ annihilation

    if ( step->GetPreStepPoint()->GetMaterial() != 
            step->GetPostStepPoint()->GetMaterial()) {
        LogDebug << "---- Pre/Post Materials (trk id: "
                 << track->GetTrackID()
                 << ") :"
                 << step->GetPreStepPoint()->GetMaterial()->GetName()
                 << " / "
                 << step->GetPostStepPoint()->GetMaterial()->GetName()
                 << std::endl;
    }

    // the post step will give the process name
    // try to find out when and where the position is annihil
    if ( track->GetDefinition() == G4Positron::Definition() 
     and step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "annihil"
     ) {
        LogDebug << "++++ e+ annihil @"
                 << step->GetPreStepPoint()->GetPosition()
                 << " / "
                 << step->GetPostStepPoint()->GetPosition()
                 << ", pre/post material: "
                 << step->GetPreStepPoint()->GetMaterial()->GetName()
                 << " / "
                 << step->GetPostStepPoint()->GetMaterial()->GetName()
                 << std::endl;
    }
}

bool DepositEnergyCalibAnaMgr::save_into_data_model() {
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
