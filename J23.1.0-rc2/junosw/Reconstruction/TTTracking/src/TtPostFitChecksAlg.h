/*
 * Calculate fit quality based on *all* hits and not just the fitted hits.
 *
 * Author: J. P. A. M. de Andre <jpandre@iphc.cnrs.fr>
 */


#ifndef TtPostFitChecksAlg_h
#define TtPostFitChecksAlg_h

#include "Geometry/TtGeom.h"
#include "SniperKernel/AlgBase.h"
#include "TTree.h"
#include <string>

class NavBuffer;

class TtPostFitChecksAlg: public AlgBase
{
    public:
        TtPostFitChecksAlg(const std::string& name);
        ~TtPostFitChecksAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:

        // Parameters
        bool m_compute_resolution;

        // Sniper
        JM::NavBuffer* m_buf;

        TtGeom * m_ttGeom;
        double m_tt_height;
        double m_wp_height;

        int m_evt_id;

        // TREE output
        TTree* m_tree_TTQuality;
        int m_eventID;
        int m_NTracks;
#define MAX_NTRACKS 2000
        int m_Rank_MissTime[MAX_NTRACKS];
        int m_Rank_TimeOnly[MAX_NTRACKS];
        int m_PtsInTrack   [MAX_NTRACKS];
        int m_evt_topo     [MAX_NTRACKS];
        int m_NHitStrips   [MAX_NTRACKS];
        int m_NMissedStrips[MAX_NTRACKS];
        int m_NMiss1SStrips[MAX_NTRACKS];
        int m_NNoiseStrips [MAX_NTRACKS];
        float m_TimeRMS    [MAX_NTRACKS];
        float m_AngularRes [MAX_NTRACKS];
        float m_DistWPRes  [MAX_NTRACKS];
        int m_true_evt_topo;

        TTree * m_tree_TTMissedStrips;
        int m_trackNum;
        int m_NMissed;
#define MAX_NMISSED 10000
        unsigned int m_missed_ChID[MAX_NMISSED];

        TTree * m_tree_evaluate_distance;
        int          m_nCrossed;
#define MAX_CROSSED 10000
        unsigned int m_ChID[MAX_CROSSED];
        unsigned int m_type[MAX_CROSSED];
        float        m_dist[MAX_CROSSED];

        TTree * m_tree_timeinfo;
        int m_nHitCrossed;
#define MAX_HIT_CROSS 10000
        unsigned int m_ChID_HX    [MAX_HIT_CROSS];
        float        m_distInStrip[MAX_HIT_CROSS];
        float        m_distToStrip[MAX_HIT_CROSS];
        float        m_timeProp   [MAX_HIT_CROSS];
        float        m_hitTime    [MAX_HIT_CROSS];
        float        m_corrTime   [MAX_HIT_CROSS];

};

#endif // TtPostFitChecksAlg_h
// vim: expandtab shiftwidth=4 softtabstop=4 tabstop=4
