#include "Event/DummyEvent.h"
#include "Event/DummyPMTHit.h"
#include "Event/DummyTTHit.h"
#include <cassert>

ClassImp(DummyEvent);

    static int counter = 0;

    DummyEvent::DummyEvent() {
        m_counter = ++counter;
        m_tracks.clear();
        m_cd_hits.clear();
        m_wp_hits.clear();
        m_tt_hits.clear();
        
        m_clones_tracks = 0;
        m_clones_cd_hits = 0;
        m_clones_wp_hits = 0;
        m_clones_tt_hits = 0;

        m_nhits = 0;
        m_nhits_wp = 0;
        m_ntrks = 0;
        m_eventid = 0;
        m_nbars = 0;
    }

    DummyEvent::DummyEvent(Int_t evtid) {
        m_counter = ++counter;
        m_tracks.clear();
        m_cd_hits.clear();
        m_wp_hits.clear();
        m_tt_hits.clear();
        m_clones_tracks = 0;
        m_clones_cd_hits = 0;
        m_clones_wp_hits = 0;
        m_clones_tt_hits = 0;

        m_nhits = 0;
        m_nhits_wp = 0;
        m_ntrks = 0;
        m_eventid = evtid;
        m_nbars = 0;

    }

    DummyEvent::~DummyEvent() {
        for (std::vector<DummyTrack*>::iterator it = m_tracks.begin();
                it != m_tracks.end(); ++it) {
            delete (*it);
        }
        m_tracks.clear();
        for (std::vector<DummyPMTHit*>::iterator it = m_cd_hits.begin();
                it != m_cd_hits.end(); ++it) {
            delete (*it);
        }
        m_cd_hits.clear();
        for (std::vector<DummyPMTHit*>::iterator it = m_wp_hits.begin();
                it != m_wp_hits.end(); ++it) {
            delete (*it);
        }
        m_wp_hits.clear();
        for (std::vector<DummyTTHit*>::iterator it = m_tt_hits.begin();
                it != m_tt_hits.end(); ++it) {
            delete (*it);
        }
        m_tt_hits.clear();
        
        if (m_clones_tracks) {
            m_clones_tracks->Clear("C");
            delete m_clones_tracks;
            m_clones_tracks = 0;
        }
        if (m_clones_cd_hits) {
            m_clones_cd_hits->Clear("C");
            delete m_clones_cd_hits;
            m_clones_cd_hits = 0;
        }
        if (m_clones_wp_hits) {
            m_clones_wp_hits->Clear("C");
            delete m_clones_wp_hits;
            m_clones_wp_hits = 0;
        }
        if (m_clones_tt_hits) {
            m_clones_tt_hits->Clear("C");
            delete m_clones_tt_hits;
            m_clones_tt_hits = 0;
        }
    }

    DummyTrack* DummyEvent::addTrack() {
        DummyTrack* track = new DummyTrack;
        m_tracks.push_back(track);
        ++m_ntrks;
        return track;
    }

    DummyTrack* DummyEvent::findTrackByTrkID(int trkid) {
        DummyTrack* trk = 0;
        for (int i = 0; i < m_tracks.size(); ++i) {
            trk = m_tracks[i];
            if (trk->getTrackID() == trkid) {
                break;
            }
        }
        return trk;
    }

    DummyPMTHit* DummyEvent::addCDHit() {
        DummyPMTHit* hit = new DummyPMTHit;
        m_cd_hits.push_back(hit);
        ++m_nhits;
        return hit;
    }

    DummyPMTHit* DummyEvent::addWPHit() {
        DummyPMTHit* hit = new DummyPMTHit;
        m_wp_hits.push_back(hit);
        ++m_nhits_wp;
        return hit;
    }

    DummyTTHit* DummyEvent::addTTHit() {
        DummyTTHit* hit = new DummyTTHit;
        m_tt_hits.push_back(hit);
        ++m_nbars;
        return hit;
    }

    TClonesArray* DummyEvent::getTracks() {
        if (m_clones_tracks) {
            assert(m_clones_tracks->GetEntriesFast() == m_tracks.size());
            return m_clones_tracks;
        }

        m_clones_tracks = new TClonesArray("DummyTrack", 10);
        std::vector<DummyTrack*>::iterator iter;
        int ntrks = 0;
        TClonesArray &tracks = *m_clones_tracks;        
        for (iter = m_tracks.begin(); iter != m_tracks.end(); ++iter) {
            DummyTrack* track = (DummyTrack*)tracks.ConstructedAt(ntrks++);
            *track = *(*iter);
        }

        return m_clones_tracks;
    }

    TClonesArray* DummyEvent::getCDHits() {
        if (m_clones_cd_hits) {
            assert(m_clones_cd_hits->GetEntriesFast() == m_cd_hits.size());
            return m_clones_cd_hits;
        }

        m_clones_cd_hits = new TClonesArray("DummyPMTHit", 1000);
        std::vector<DummyPMTHit*>::iterator iter;
        int nhits = 0;
        TClonesArray &hits = *m_clones_cd_hits;
        for (iter = m_cd_hits.begin(); iter != m_cd_hits.end(); ++iter) {
            DummyPMTHit* hit = (DummyPMTHit*)hits.ConstructedAt(nhits++);
            *hit = *(*iter);
            // hit->setPMTID ( (*iter)->getPMTID() );
            // hit->setNPE ( (*iter)->getNPE() );
            // hit->setHitTime ( (*iter)->getHitTime() );
            // hit->setTimeWindow ( (*iter)->getTimeWindow() );

        }
        return m_clones_cd_hits;
    }

    TClonesArray* DummyEvent::getWPHits() {
        if (m_clones_wp_hits) {
            assert(m_clones_wp_hits->GetEntriesFast() == m_wp_hits.size());
            return m_clones_wp_hits;
        }

        m_clones_wp_hits = new TClonesArray("DummyPMTHit", 1000);
        std::vector<DummyPMTHit*>::iterator iter;
        int nhits = 0;
        TClonesArray &hits = *m_clones_wp_hits;
        for (iter = m_wp_hits.begin(); iter != m_wp_hits.end(); ++iter) {
            DummyPMTHit* hit = (DummyPMTHit*)hits.ConstructedAt(nhits++);
            *hit = *(*iter);
            // hit->setPMTID ( (*iter)->getPMTID() );
            // hit->setNPE ( (*iter)->getNPE() );
            // hit->setHitTime ( (*iter)->getHitTime() );
            // hit->setTimeWindow ( (*iter)->getTimeWindow() );

        }
        return m_clones_wp_hits;
    }

  TClonesArray* DummyEvent::getTTHits() {
        if (m_clones_tt_hits) {
            assert(m_clones_tt_hits->GetEntriesFast() == m_tt_hits.size());
            return m_clones_tt_hits;
        }

        m_clones_tt_hits = new TClonesArray("DummyTTHit", 100);
        std::vector<DummyTTHit*>::iterator iter;
        int nhits = 0;
        TClonesArray &hits = *m_clones_tt_hits;
        for (iter = m_tt_hits.begin(); iter != m_tt_hits.end(); ++iter) {
            DummyTTHit* hit = (DummyTTHit*)hits.ConstructedAt(nhits++);
            *hit = *(*iter);
            // hit->setPMTID ( (*iter)->getPMTID() );
            // hit->setNPE ( (*iter)->getNPE() );
            // hit->setHitTime ( (*iter)->getHitTime() );
            // hit->setTimeWindow ( (*iter)->getTimeWindow() );

        }
        return m_clones_tt_hits;
    }
