#ifndef BasicDistAlg_h
#define BasicDistAlg_h

#include "SniperKernel/AlgBase.h"
#include "TTimeStamp.h"
#include <vector>
#include <deque>
#include <map>

class TTree;
class TH1I;
class TH1F;
namespace JM {
    class EvtNavigator;
    class SimHeader;
    class SimEvt;
    class SimPMTHit;
}

class BasicDistAlg: public AlgBase {
public:
    BasicDistAlg(const std::string& name);
    ~BasicDistAlg();

    bool initialize();
    bool execute();
    bool finalize();

private:

    bool load_sim_event();
    bool filter_nonhits();
    bool filter_neutron();
    bool count_totalpe();
    bool select_totalpe();
    bool do_split();
    bool do_mctruth();
    bool do_ibd_selection();
    bool do_hittime();
    bool do_npe();
    bool save_tuple();

private:
    JM::EvtNavigator* evt_nav;
    JM::SimHeader* m_simheader;
    JM::SimEvt* m_simevent;
    // = cache position and deposit energy =
    double m_n_edep;
    double m_n_edep_x;
    double m_n_edep_y;
    double m_n_edep_z;

    double m_n_init_x;
    double m_n_init_y;
    double m_n_init_z;

    double m_evt_duration; // last hit - first hit time per event
    double m_response_time; // last hit - first hit time per pmt

    double m_prompt_totalpe;
    double m_delay_totalpe;
    double m_time_diff;
    double m_dist;
    double m_prompt_dist;
    double m_delay_drift;
    double m_delay_dist_prod;
    double m_delay_dist_cap;

    int m_npe_perpmt;
    int m_maxnpe; // max in one event
    // 

    std::vector<double> cnts_cluster;
    double m_n_totalPE;
    // gap to split the event
    double m_split_time_gap;
    std::vector<double> m_totalpe_cut;
    // cache all hits
    std::vector<JM::SimPMTHit*> hits;

    // for select hits
    int idx;
    double hitsmax;

    // cache for ibd
    struct IBDSelection {
        TTimeStamp timestamp;
        double totalPE;
        double edep_x;
        double edep_y;
        double edep_z;
        double init_x;
        double init_y;
        double init_z;
    };
    std::deque<IBDSelection> m_buffer_ibd;
    // need further study, the value of time cut
    // depends on neutron capture time
    double m_ibd_time_cut;
    double m_ibd_pos_cut;

    TTree* m_basic_tree;
    TTree* m_response_time_tree;
    TH1F*  m_response_time_hist;
    TTree* m_ibd_tree;
    TTree* m_npe_tree; // FIXME: TH1I is better
    TH1I*  m_npe_hist;
    TTree* m_evtnpe_tree; // in current code, just reuse the basic tree
};

#endif
