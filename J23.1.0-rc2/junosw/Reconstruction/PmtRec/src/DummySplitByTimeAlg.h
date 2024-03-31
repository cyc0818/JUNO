#ifndef DummySplitByTimeAlg_h
#define DummySplitByTimeAlg_h

#include "SniperKernel/AlgBase.h"
#include <vector>
#include <list>
#include "TTimeStamp.h"
namespace JM {
    class EvtNavigator;
    class SimHeader;
    class SimEvt;
    class SimPMTHit;
    //class SimTTHit;
	class SimTrack;
}

class DummySplitByTimeAlg : public AlgBase {
    public:
        DummySplitByTimeAlg(const std::string& name);
        ~DummySplitByTimeAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        bool execute_one();
        bool load_detsim_data();
        bool find_gap();
        bool split_evts();
        //bool split_evts_TopTracker(bool cleanup=true);
        bool save_into_buffer();

    private:
        struct m_split_related {
            int count;
            bool is_first;
            std::list<JM::SimEvt*> buffer;
        } m_split_related;

        JM::EvtNavigator* m_simnav;
        JM::SimHeader* m_simheader;
        JM::SimEvt* m_simevent;
        std::vector<JM::SimPMTHit*> m_hits_col;
        // a trick here, the 3inch PMTs are in the WP collection.
        std::vector<JM::SimPMTHit*> m_hits_wp_col;
        std::vector<int> m_split_hits;

		//lizy::2019.02.26 Fix the bug that calib.root do not contain tracks info
		std::vector<JM::SimTrack*> m_tracks;

        // cache hits from TT
		//std::vector<JM::SimTTHit*> m_hits_tt_col;

        std::string detsimtask_name;
        double m_split_time_gap;

        TTimeStamp m_current_timestamp;
};

#endif
