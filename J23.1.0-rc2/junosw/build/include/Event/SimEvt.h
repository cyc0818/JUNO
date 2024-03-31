#ifndef SimEvent_h
#define SimEvent_h

#include "Event/EventObject.h"
#include "Event/SimPMTHit.h"
#include "Event/SimTTHit.h"
#include "Event/SimTrack.h"
#include "Event/SimVertex.h"
#include <vector>


namespace JM
{
    /*
     * Description:
     *   This class represents an event in simulation. 
     *
     * History:
     *   - v11: add vertex collection
     *
     * Author:
     *   Tao Lin <lintao AT ihep.ac.cn>
     */


    class SimPMTHit;
    class SimTrack;
    class SimTTHit;

    class SimEvt: public EventObject
    {
        private:
            std::vector<SimTrack*> m_tracks;  
            std::vector<SimVertex*> m_vertices;  
            std::vector<SimPMTHit*> m_cd_hits; 
            std::vector<SimPMTHit*> m_wp_hits; 
            std::vector<SimTTHit*> m_tt_hits; 

            Int_t m_nhits;
            Int_t m_nhits_wp;
            Int_t m_ntrks;
            Int_t m_nbars;

            Int_t m_counter; // only for debug

            Int_t m_eventid;
 
            Int_t m_nhits_lpmt;
            Int_t m_nhits_spmt;
            Int_t m_nhits_nnvt;
            Int_t m_nhits_hama;

            // don't support the copy constructor
            SimEvt(const SimEvt& event);

        public:
            SimEvt();
            SimEvt(Int_t evtid);
            ~SimEvt();

            // == Initial Track Info ==
            JM::SimTrack *addTrack();
            const std::vector<JM::SimTrack*>& getTracksVec() const {return m_tracks;}
            JM::SimTrack *findTrackByTrkID(int trkid);

            JM::SimVertex *addVertex();
            const std::vector<JM::SimVertex*>& getVerticesVec() const {return m_vertices;}

            // == CD (Central Detector) Related ==
            JM::SimPMTHit *addCDHit();
            const std::vector<JM::SimPMTHit*>& getCDHitsVec() const {return m_cd_hits;}

            // == WP (Water Pool) Related ==
            JM::SimPMTHit *addWPHit();
            const std::vector<JM::SimPMTHit*>& getWPHitsVec() const {return m_wp_hits;}

            // == TT (Top Tracker) Related ==
            JM::SimTTHit *addTTHit();
            const std::vector<JM::SimTTHit*>& getTTHitsVec() const {return m_tt_hits;}

            // == Event ID ==
            Int_t getEventID() { return m_eventid; }
            void setEventID(Int_t val) { m_eventid = val; }
             
            Int_t getLpmtHits() { return m_nhits_lpmt; }
            void setLpmtHits(Int_t hits) { m_nhits_lpmt = hits ; }

            Int_t getSpmtHits() { return m_nhits_spmt; }
            void setSpmtHits(Int_t hits) { m_nhits_spmt = hits ; }

            Int_t getNNVTHits() { return m_nhits_nnvt; }
            void setNNVTHits(Int_t hits) { m_nhits_nnvt = hits ; }
 
            Int_t getHamaHits() { return m_nhits_hama; }
            void setHamaHits(Int_t hits) { m_nhits_hama = hits ; }


            ClassDef(SimEvt, 11)

    };
}

#endif
