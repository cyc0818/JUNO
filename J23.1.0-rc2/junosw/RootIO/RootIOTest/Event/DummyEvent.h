#ifndef DummyEvent_h
#define DummyEvent_h

#include "Event/EventObject.h"
#include "Event/DummyPMTHit.h"
#include "Event/DummyTTHit.h"
#include "Event/DummyTrack.h"
#include "TClonesArray.h"
#include <vector>


    class DummyPMTHit;
    class DummyTrack;
    class DummyTTHit;

    class DummyEvent
    {
        private:
            std::vector<DummyTrack*> m_tracks;  
            std::vector<DummyPMTHit*> m_cd_hits; 
            std::vector<DummyPMTHit*> m_wp_hits; 
            std::vector<DummyTTHit*> m_tt_hits; 

            TClonesArray* m_clones_tracks;  //!
            TClonesArray* m_clones_cd_hits; //!
            TClonesArray* m_clones_wp_hits; //!
            TClonesArray* m_clones_tt_hits; //!

            Int_t m_nhits;
            Int_t m_nhits_wp;
            Int_t m_ntrks;
            Int_t m_nbars;

            Int_t m_counter; // only for debug

            Int_t m_eventid;

            // don't support the copy constructor
            DummyEvent(const DummyEvent& event);

        public:
            DummyEvent();
            DummyEvent(Int_t evtid);
            virtual ~DummyEvent();

            // == Initial Track Info ==
            DummyTrack *addTrack();
            const std::vector<DummyTrack*>& getTracksVec() const {return m_tracks;}
            TClonesArray* getTracks() ;
            DummyTrack *findTrackByTrkID(int trkid);

            // == CD (Central Detector) Related ==
            DummyPMTHit *addCDHit();
            const std::vector<DummyPMTHit*>& getCDHitsVec() const {return m_cd_hits;}
            TClonesArray* getCDHits() ;

            // == WP (Water Pool) Related ==
            DummyPMTHit *addWPHit();
            const std::vector<DummyPMTHit*>& getWPHitsVec() const {return m_wp_hits;}
            TClonesArray* getWPHits() ;

            // == TT (Top Tracker) Related ==
            DummyTTHit *addTTHit();
            const std::vector<DummyTTHit*>& getTTHitsVec() const {return m_tt_hits;}
            TClonesArray* getTTHits() ;

            // == Event ID ==
            Int_t getEventID() { return m_eventid; }
            void setEventID(Int_t val) { m_eventid = val; }
            ClassDef(DummyEvent, 9)

    };

#endif
