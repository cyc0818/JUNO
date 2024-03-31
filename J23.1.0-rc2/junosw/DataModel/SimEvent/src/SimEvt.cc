#include "Event/SimEvt.h"
#include "Event/SimPMTHit.h"
#include "Event/SimTTHit.h"
#include <cassert>

ClassImp(JM::SimEvt)

namespace JM
{
    static int counter = 0;

    SimEvt::SimEvt() {
        m_counter = ++counter;
        m_tracks.clear();
        m_vertices.clear();
        m_cd_hits.clear();
        m_wp_hits.clear();
        m_tt_hits.clear();

        m_nhits = 0;
        m_nhits_wp = 0;
        m_ntrks = 0;
        m_eventid = 0;
        m_nbars = 0;
    }

    SimEvt::SimEvt(Int_t evtid) {
        m_counter = ++counter;
        m_tracks.clear();
        m_vertices.clear();
        m_cd_hits.clear();
        m_wp_hits.clear();
        m_tt_hits.clear();

        m_nhits = 0;
        m_nhits_wp = 0;
        m_ntrks = 0;
        m_eventid = evtid;
        m_nbars = 0;
  
        m_nhits_lpmt = 0 ;
        m_nhits_spmt = 0 ;
        m_nhits_nnvt = 0 ;
        m_nhits_hama = 0 ;


    }

    SimEvt::~SimEvt() {
        for (std::vector<SimTrack*>::iterator it = m_tracks.begin();
                it != m_tracks.end(); ++it) {
            delete (*it);
        }
        m_tracks.clear();

        for (auto it = m_vertices.begin();
                it != m_vertices.end(); ++it) {
            delete (*it);
        }
        m_vertices.clear();


        for (std::vector<SimPMTHit*>::iterator it = m_cd_hits.begin();
                it != m_cd_hits.end(); ++it) {
            delete (*it);
        }
        m_cd_hits.clear();
        for (std::vector<SimPMTHit*>::iterator it = m_wp_hits.begin();
                it != m_wp_hits.end(); ++it) {
            delete (*it);
        }
        m_wp_hits.clear();
        for (std::vector<SimTTHit*>::iterator it = m_tt_hits.begin();
                it != m_tt_hits.end(); ++it) {
            delete (*it);
        }
        m_tt_hits.clear();
        
    }

    SimTrack* SimEvt::addTrack() {
        SimTrack* track = new SimTrack;
        m_tracks.push_back(track);
        track->setObjID(m_ntrks); // put the idx in collection
        ++m_ntrks;
        return track;
    }

    SimTrack* SimEvt::findTrackByTrkID(int trkid) {
        JM::SimTrack* trk = 0;
        for (size_t i = 0; i < m_tracks.size(); ++i) {
            trk = m_tracks[i];
            if (trk->getTrackID() == trkid) {
                break;
            }
        }
        return trk;
    }

    SimVertex* SimEvt::addVertex() {
        SimVertex* v = new SimVertex();
        m_vertices.push_back(v);
        return v;
    }

    SimPMTHit* SimEvt::addCDHit() {
        SimPMTHit* hit = new SimPMTHit;
        m_cd_hits.push_back(hit);
        ++m_nhits;
        return hit;
    }

    SimPMTHit* SimEvt::addWPHit() {
        SimPMTHit* hit = new SimPMTHit;
        m_wp_hits.push_back(hit);
        ++m_nhits_wp;
        return hit;
    }

    SimTTHit* SimEvt::addTTHit() {
        SimTTHit* hit = new SimTTHit;
        m_tt_hits.push_back(hit);
        ++m_nbars;
        return hit;
    }

}
