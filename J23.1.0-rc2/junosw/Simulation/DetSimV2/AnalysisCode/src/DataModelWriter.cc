#include "DataModelWriter.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"

#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/SimHeader.h"
#include "Event/SimTrack.h"

#include "G4Event.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"

#include "junoHit_PMT.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

#include "DetSimAlg/ISimTrackSvc.h"

DECLARE_TOOL(DataModelWriter);

DataModelWriter::DataModelWriter(const std::string& name)
    : ToolBase(name)
{

}

DataModelWriter::~DataModelWriter()
{

}

void
DataModelWriter::BeginOfRunAction(const G4Run* /*aRun*/) {

    SniperPtr<ISimTrackSvc> simtracksvc_ptr(getParent(), "SimTrackSvc");
    if (simtracksvc_ptr.invalid()) {
        // create the service
        simtracksvc = dynamic_cast<ISimTrackSvc*>(getParent()->createSvc("SimTrackSvc"));
    } else {
        simtracksvc = simtracksvc_ptr.data();
    }


}

void
DataModelWriter::EndOfRunAction(const G4Run* /*aRun*/) {
    LogInfo << "size of PMT Hit (geant4): " << sizeof(junoHit_PMT) << std::endl;
    LogInfo << "size of PMT Hit for muon (geant4): " << sizeof(junoHit_PMT_muon) << std::endl;
    LogInfo << "size of PMT Hit (data model): " << sizeof(JM::SimPMTHit) << std::endl;
}

void
DataModelWriter::BeginOfEventAction(const G4Event* /*evt*/) {

}

void
DataModelWriter::EndOfEventAction(const G4Event* evt) {
    // FIXME: shall we get the navigator first?
    // Get the navigator with GenEvent from Buffer
    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        LogError << "Can't find the NavBuffer." << std::endl;
        return;
    }
    auto evt_nav = navBuf->curEvt();
    if (not evt_nav) {
        LogError << "Can't find the event navigator." << std::endl;
        return;
    }

    auto nav = evt_nav;

    // create header
    auto sim_header = new JM::SimHeader;
    // create event
    auto sim_event = new JM::SimEvt(evt->GetEventID());
    // == fill hits
    fill_hits(sim_event, evt);
    // == fill tracks
    collect_primary_track(evt);
    fill_tracks(sim_event);

    if (m_timewindow>0) {
        sim_header->setCDLPMTtimeWindow(m_timewindow);
    } else if (m_timewindow_muon>0) {
        sim_header->setCDLPMTtimeWindow(m_timewindow_muon);
    } else{
        sim_header->setCDLPMTtimeWindow(0);
    }
    if (m_nPhotons>0) {
        sim_header->setCDLPMTtotalHits(m_nPhotons);
    } else if (m_nPhotons_muon>0) {
        sim_header->setCDLPMTtotalHits(m_nPhotons_muon);
    } else {
        sim_header->setCDLPMTtotalHits(0);
    }
    // set the relation
    sim_header->setEvent(sim_event);
    nav->addHeader("/Event/Sim", sim_header);

}

void 
DataModelWriter::fill_hits(JM::SimEvt* dst, const G4Event* evt)
{

    LogDebug << "Begin Fill Hits" << std::endl;

    // header level 
    m_nPhotons = 0;
    m_timewindow = 0;
    m_nPhotons_muon = 0;
    m_timewindow_muon = 0;

    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    G4int CollID = SDman->GetCollectionID("hitCollection");
    junoHit_PMT_Collection* col = 0;
    G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
    if (!HCE) {
        LogError << "No hits collection found." << std::endl;
        return;
    }
    if (CollID >= 0) {
        col = (junoHit_PMT_Collection*)(HCE->GetHC(CollID));
    }
    if (col) {
        fill_hits_tmpl(col, dst);
    }
    // muon hit type
    CollID = SDman->GetCollectionID("hitCollectionMuon");
    junoHit_PMT_muon_Collection* col_muon = 0;
    if (CollID >= 0) {
        col_muon = (junoHit_PMT_muon_Collection*)(HCE->GetHC(CollID));
    }
    if (col_muon) {
        fill_hits_tmpl(col_muon, dst);
    }
    
    // fill evt data
    // int totPE = 0;
    LogDebug << "End Fill Hits" << std::endl;

}

void 
DataModelWriter::collect_primary_track(const G4Event* evt)
{
    LogDebug << "Begin Fill Tracks" << std::endl;
 //   std::vector<JM::SimTrack*>& m_tracks = simtracksvc->all();

    G4int nVertex = evt -> GetNumberOfPrimaryVertex();
    for (G4int index=0; index < nVertex; ++index) {
        G4PrimaryVertex* vtx = evt->GetPrimaryVertex( index );

        // Vertex info
        double x = vtx->GetX0();
        double y = vtx->GetY0();
        double z = vtx->GetZ0();
        double t = vtx->GetT0();

        // Loop Over Particle
        G4PrimaryParticle* pp = vtx -> GetPrimary();

        while (pp) {

            int trkid = pp -> GetTrackID();
            int pdgid = pp -> GetPDGcode();
            double px = pp -> GetPx();
            double py = pp -> GetPy();
            double pz = pp -> GetPz();
            double mass = pp -> GetMass();
            
             
            // new track
            auto jm_trk = simtracksvc->get(trkid);
            if(!jm_trk){
               auto trk=new JM::SimTrack();
               trk->setTrackID(trkid);
               trk->setParentID(0);
               simtracksvc->put(trk);
               jm_trk=trk;
            }
            jm_trk->setPDGID(pdgid);
            jm_trk->setInitPx(px);
            jm_trk->setInitPy(py);
            jm_trk->setInitPz(pz);
            jm_trk->setInitMass(mass);
            jm_trk->setInitX(x);
            jm_trk->setInitY(y);
            jm_trk->setInitZ(z);
            jm_trk->setInitT(t);

            pp = pp->GetNext();
        }
    }
    LogDebug << "End Fill Tracks" << std::endl;
}

void
DataModelWriter::fill_tracks(JM::SimEvt* dst)
{
    if (!simtracksvc) {
        LogWarn << "SimTrackSvc is not available to save additional tracks" << std::endl;
        return;
    }

    auto& alltracks = simtracksvc->all();
    for (auto track: alltracks) {
        auto jm_trk = dst->addTrack();
        
        jm_trk->setPDGID   (track->getPDGID());
        jm_trk->setTrackID (track->getTrackID());
        jm_trk->setParentID(track->getParentID());
        jm_trk->setInitMass(track->getInitMass());

        jm_trk->setInitPx  (track->getInitPx());
        jm_trk->setInitPy  (track->getInitPy());
        jm_trk->setInitPz  (track->getInitPz());
        jm_trk->setInitX   (track->getInitX());
        jm_trk->setInitY   (track->getInitY());
        jm_trk->setInitZ   (track->getInitZ());
        jm_trk->setInitT   (track->getInitT());

        jm_trk->setExitPx  (track->getExitPx());
        jm_trk->setExitPy  (track->getExitPy());
        jm_trk->setExitPz  (track->getExitPz());
        jm_trk->setExitX   (track->getExitX());
        jm_trk->setExitY   (track->getExitY());
        jm_trk->setExitZ   (track->getExitZ());
        jm_trk->setExitT   (track->getExitT());

        jm_trk->setTrackLength(track->getTrackLength());

        jm_trk->setEdep    (track->getEdep());
        jm_trk->setEdepX   (track->getEdepX());
        jm_trk->setEdepY   (track->getEdepY());
        jm_trk->setEdepZ   (track->getEdepZ());
        
        jm_trk->setQEdep   (track->getQEdep());
        jm_trk->setQEdepX  (track->getQEdepX());
        jm_trk->setQEdepY  (track->getQEdepY());
        jm_trk->setQEdepZ  (track->getQEdepZ());

        jm_trk->setEdepNotInLS(track->getEdepNotInLS());
    }

    simtracksvc->reset();
}
