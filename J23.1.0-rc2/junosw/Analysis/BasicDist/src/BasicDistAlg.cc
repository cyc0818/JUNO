#include "BasicDistAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "RootWriter/RootWriter.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Identifier/CdID.h"
#include "TMath.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH1F.h"

#include "Event/SimHeader.h"
#include "Event/SimEvt.h"
#include "Event/SimTrack.h"

#include <algorithm>

DECLARE_ALGORITHM(BasicDistAlg);

BasicDistAlg::BasicDistAlg(const std::string& name)
    : AlgBase(name), evt_nav(0), m_simheader(0), m_simevent(0)
    , m_response_time_tree(0), m_response_time_hist(0), m_ibd_tree(0)
    , m_npe_tree(0), m_npe_hist(0), m_evtnpe_tree(0)
{
    declProp("SplitTimeGap", m_split_time_gap = 300.); // 300ns
    declProp("TotalPECut", m_totalpe_cut);
    m_totalpe_cut.push_back(1000);
    m_totalpe_cut.push_back(4000);

    // 800us maybe not enough. 1ms is good.
    declProp("IBDTimeCut", m_ibd_time_cut = 800e3); // 800us
    declProp("IBDPosCut", m_ibd_pos_cut = 3e3); // 3m, 3000mm

}

BasicDistAlg::~BasicDistAlg()
{

}

bool
BasicDistAlg::initialize()
{
    SniperPtr<RootWriter> svc(getParent(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enalbe it in your job option file."
                 << std::endl;
        return false;
    }
    m_basic_tree = svc->bookTree(*getParent(), "CALIBEVT/calib", "calib supernova neutron");
    m_basic_tree->Branch("totalPE", &m_n_totalPE, "totalPE/D");
    m_basic_tree->Branch("edep", &m_n_edep, "edep/D");
    m_basic_tree->Branch("edep_x", &m_n_edep_x, "edepX/D");
    m_basic_tree->Branch("edep_y", &m_n_edep_y, "edepY/D");
    m_basic_tree->Branch("edep_z", &m_n_edep_z, "edepZ/D");

    m_basic_tree->Branch("init_x", &m_n_init_x, "initX/D");
    m_basic_tree->Branch("init_y", &m_n_init_y, "initY/D");
    m_basic_tree->Branch("init_z", &m_n_init_z, "initZ/D");

    m_basic_tree->Branch("evt_duration", &m_evt_duration, "evt_duration/D");

    // response time
    m_response_time_tree = svc->bookTree(*getParent(), "CALIBEVT/response", "response time");
    m_response_time_tree->Branch("responsetime", &m_response_time, "responsetime/D");

    m_response_time_hist = new TH1F("responsetime_hist", "responsetime", 3000, 0, 3000);
    svc->attach("CALIBEVT", m_response_time_hist);

    // nPE distribution
    m_npe_tree = svc->bookTree(*getParent(), "CALIBEVT/npe", "nPE distribution");
    m_npe_tree->Branch("npe", &m_npe_perpmt, "npe/I");

    m_npe_hist = new TH1I("npe_hist", "nPE distribution", 200, 0, 200);
    svc->attach("CALIBEVT", m_npe_hist);

    // m_evtnpe_tree = svc->bookTree("CALIBEVT/evtnpe", "max nPE per event");
    m_evtnpe_tree = m_basic_tree;
    m_evtnpe_tree->Branch("maxnpe", &m_maxnpe, "maxnpe/I");

    // IBD
    m_ibd_tree = svc->bookTree(*getParent(), "CALIBEVT/ibd", "IBD selection");
    m_ibd_tree->Branch("prompt_totalpe", &m_prompt_totalpe, "prompt_totalpe/D");
    m_ibd_tree->Branch("delay_totalpe", &m_delay_totalpe, "delay_totalpe/D");
    m_ibd_tree->Branch("time_diff", &m_time_diff, "time_diff/D");
    m_ibd_tree->Branch("dist", &m_dist, "dist/D");
    m_ibd_tree->Branch("prompt_dist", &m_prompt_dist, "prompt_dist/D");
    m_ibd_tree->Branch("delay_drift", &m_delay_drift, "delay_dist/D");
    m_ibd_tree->Branch("delay_dist_prod", &m_delay_dist_prod, "delay_dist_prod/D");
    m_ibd_tree->Branch("delay_dist_cap", &m_delay_dist_cap, "delay_dist_cap/D");
    return true;
}

bool
BasicDistAlg::execute()
{
    // = load sim event =
    bool st = false;
    st = load_sim_event();
    if (not st) { 
        LogError << "can't load sim event. " << std::endl;
        return false; 
    }
    // = filter the events =
    // == if no hits, just return ==
    st = filter_nonhits();
    if (not st) { 
        LogDebug << "skip event without any hits. " << std::endl;
        return true; 
    }

    LogInfo << "=====================================================" << std::endl;
    // == split by the time gap, and count the totalPE ==
    // reset
    m_n_totalPE = 0.0;
    cnts_cluster.clear();
    st = count_totalpe();
    if (not st) { return true; }

    // == only select the totalPE in [min, max] (totalpe_cut) ==
    st = select_totalpe();
    if (not st) { return true; }
    // == get the index and totalpe ==
    st = do_split();
    if (not st) { return true; }

    // = do more jobs =
    // == MCTruth of generator? ==
    do_mctruth();
    // == positron: energy spectrum ==
    // == positron: distance between production and visible ==
    // == neutron: distance between production and visible ==
    //
    // == select ibd ==
    do_ibd_selection();
    //
    // == PE distribution ==
    do_npe();
    // == hit time distribution ==
    do_hittime();

    // = save the results =
    st = save_tuple();
    LogInfo << "=====================================================" << std::endl;

    return true;
}

bool
BasicDistAlg::finalize()
{
    return true;
}

// --------------------------------------------------------------------------
// Load the Sim Event
// --------------------------------------------------------------------------
bool
BasicDistAlg::load_sim_event()
{
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        return false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        return false;
    }
    m_simheader = JM::getHeaderObject<JM::SimHeader>(evt_nav);
    LogDebug << "simhdr: " << m_simheader << std::endl;
    if (not m_simheader) {
        return false;
    }
    m_simevent = dynamic_cast<JM::SimEvt*>(m_simheader->event());
    LogDebug << "simevt: " << m_simevent << std::endl;
    if (not m_simevent) {
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// Filter the event with hits. 
// --------------------------------------------------------------------------
bool BasicDistAlg::filter_nonhits() 
{
    // get the hit collection
    const std::vector<JM::SimPMTHit*>& hits = m_simevent->getCDHitsVec();

    return hits.size() != 0;
}

// --------------------------------------------------------------------------
// Filter the event with neutron. 
// Also save the info from track
// --------------------------------------------------------------------------
bool
BasicDistAlg::filter_neutron()
{
    if (not m_simevent) {
        return false;
    }
    const std::vector<JM::SimTrack*>& trks = m_simevent->getTracksVec();
    bool is_neutron = false;
    for (std::vector<JM::SimTrack*>::const_iterator it = trks.begin();
            it != trks.end(); ++it) {
        int pdgid = (*it)->getPDGID();
        // PDGID 2112 -> neutron
        if ( pdgid == 2112) {
            LogInfo << "find neutron " << std::endl;
            is_neutron = true;

            // TODO get the energy
            m_n_edep = (*it)->getEdep();
            m_n_edep_x = (*it)->getEdepX();
            m_n_edep_y = (*it)->getEdepY();
            m_n_edep_z = (*it)->getEdepZ();
            
            m_n_init_x = (*it)->getInitX();
            m_n_init_y = (*it)->getInitY();
            m_n_init_z = (*it)->getInitZ();
            break;
        }
    }
    return is_neutron;
}

bool SortByhitTime(JM::SimPMTHit* hit1,JM::SimPMTHit* hit2){
    return hit1->getHitTime() < hit2->getHitTime();
}

// --------------------------------------------------------------------------
// count the total pe
// --------------------------------------------------------------------------
bool
BasicDistAlg::count_totalpe()
{
    // Due to the hits can be from e+ and n, we divide the 
    // hit collection into two parts.
    hits = m_simevent->getCDHitsVec();
    std::sort(hits.begin(),hits.end(), SortByhitTime);
    int hits_size = hits.size();
    if (hits_size == 0) { return false; }

    int startidx = -1;
    double cnts = 0;
    for (int i = 0; i < hits_size-1; ++i) {
        ++cnts;
        if ((hits[i+1]->getHitTime() - hits[i]->getHitTime()) > m_split_time_gap)  {
            startidx = i+1;
            
            cnts_cluster.push_back(cnts);
            // start a new count
            cnts = 0;
        }
    }
    ++cnts;
    cnts_cluster.push_back(cnts);
    LogInfo << "Show Info: " << std::endl;
    for (std::vector<double>::iterator it = cnts_cluster.begin();
            it != cnts_cluster.end(); ++it) {
        LogInfo << *it << std::endl;
    }
    LogInfo << "Show Info end " << std::endl;
    return true;
}

// --------------------------------------------------------------------------
// get the totalpe 
// --------------------------------------------------------------------------
bool 
BasicDistAlg::select_totalpe()
{
    // find out the right part from the cluster
    // the first part can be e+, so always skip
    int cnts_cluster_size = cnts_cluster.size();
    // start from the second part

    int is_found = 0;
    for (int i = 0; i < cnts_cluster_size; ++i) {
        double totalpe = cnts_cluster[i];
        if ( m_totalpe_cut[0] <= totalpe and totalpe <= m_totalpe_cut[1]) {
            LogInfo << "A candinate totalPE: " << totalpe << std::endl;
            m_n_totalPE = totalpe;
            ++is_found;
        }
    }
    if (is_found>1) {
        LogInfo << "multi totalPE candinates" << std::endl;
    }
    return is_found;
}

// --------------------------------------------------------------------------
// show the MC Truth
// --------------------------------------------------------------------------
bool
BasicDistAlg::do_mctruth()
{
    const std::vector<JM::SimTrack*>& trks = m_simevent->getTracksVec();

    int cnt_select_trks = 0;
    for (std::vector<JM::SimTrack*>::const_iterator it = trks.begin();
            it != trks.end(); ++it) {
        int pdgid = (*it)->getPDGID();
        double edep = (*it)->getEdep();

        // * -11, positron 
        // * 22, gamma (capture)
        if (pdgid == -11 or pdgid == 22) {
            ++cnt_select_trks;
        } else {
            LogInfo << "skip pdgid/edep: " 
                    << pdgid << "/" << edep << std::endl;
            continue;
        }

        LogInfo << "pdgid/edep: " 
                << pdgid << "/" << edep << std::endl;

        m_n_edep = (*it)->getEdep();
        m_n_edep_x = (*it)->getEdepX();
        m_n_edep_y = (*it)->getEdepY();
        m_n_edep_z = (*it)->getEdepZ();
            
        m_n_init_x = (*it)->getInitX();
        m_n_init_y = (*it)->getInitY();
        m_n_init_z = (*it)->getInitZ();
    }

    if (cnt_select_trks != 1) {
        LogWarn << "select tracks " << cnt_select_trks << std::endl;
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------
// ibd selection
// --------------------------------------------------------------------------
bool
BasicDistAlg::do_ibd_selection()
{
    std::vector<double>::iterator it  = 
        std::max_element(cnts_cluster.begin(), cnts_cluster.end());
    double hitsmax = *it;
    IBDSelection ibdsel = {
        evt_nav->TimeStamp(), // timestamp
        hitsmax, // totalPE
        m_n_edep_x, // edep x
        m_n_edep_y, // edep y
        m_n_edep_z, // edep z
        m_n_init_x, // init x
        m_n_init_y, // init y
        m_n_init_z  // init z
    };

    m_buffer_ibd.push_back(ibdsel);
    while (true) {
        const IBDSelection& ibd_first = m_buffer_ibd.front();
        Int_t first_s = ibd_first.timestamp.GetSec();
        Int_t first_ns = ibd_first.timestamp.GetNanoSec();

        const IBDSelection& ibd_last = m_buffer_ibd.back();
        Int_t last_s = ibd_last.timestamp.GetSec();
        Int_t last_ns = ibd_last.timestamp.GetNanoSec();

        Int_t diff_s = last_s - first_s;
        Int_t diff_ns = last_ns - first_ns;

        if (diff_ns<0) {
            diff_ns += 1000000000;
            diff_s -= 1;
        }

        double diff_pos = TMath::Sqrt(
                TMath::Power(ibd_first.edep_x - ibd_last.edep_x, 2)
              + TMath::Power(ibd_first.edep_y - ibd_last.edep_y, 2)
              + TMath::Power(ibd_first.edep_z - ibd_last.edep_z, 2)
             );

        if (diff_s != 0 || diff_ns > m_ibd_time_cut) {
            m_buffer_ibd.pop_front();
        } else if (diff_pos > m_ibd_pos_cut) {
            // position cut
            m_buffer_ibd.pop_front();
        } else {
            break;
        }
    }
    if (m_buffer_ibd.size() < 2) {
        return true;
    }
    if (m_buffer_ibd.size() > 2) {
        LogWarn << "multievents in a buffer. " << std::endl;
        return false;
    }
    // dump the info
    const IBDSelection& ibd_first = m_buffer_ibd.front();
    Int_t first_s = ibd_first.timestamp.GetSec();
    Int_t first_ns = ibd_first.timestamp.GetNanoSec();
    const IBDSelection& ibd_last = m_buffer_ibd.back();
    Int_t last_s = ibd_last.timestamp.GetSec();
    Int_t last_ns = ibd_last.timestamp.GetNanoSec();

    Int_t diff_s = last_s - first_s;
    Int_t diff_ns = last_ns - first_ns;

    if (diff_ns<0) {
        diff_ns += 1000000000;
        diff_s -= 1;
    }

    if (diff_s != 0) {
        LogWarn << "time selection can be problem." << std::endl;
        return false;
    }

    LogInfo << "first : " << ibd_first.timestamp 
            << " / " << ibd_first.totalPE << std::endl;
    LogInfo << "last : " << ibd_last.timestamp 
            << " / " << ibd_last.totalPE << std::endl;
    LogInfo << "time between first and last: " 
            << (ibd_last.timestamp-ibd_first.timestamp)
            << std::endl;
    LogInfo << "diff_s/ns: " << diff_s << "/" << diff_ns << std::endl;
    m_prompt_totalpe = ibd_first.totalPE;
    m_delay_totalpe = ibd_last.totalPE;
    m_time_diff = diff_ns;
    m_dist = TMath::Sqrt(
                TMath::Power(ibd_first.edep_x - ibd_last.edep_x, 2)
              + TMath::Power(ibd_first.edep_y - ibd_last.edep_y, 2)
              + TMath::Power(ibd_first.edep_z - ibd_last.edep_z, 2)
             );
    LogInfo << "distance: " << m_dist << " mm." << std::endl;

    // Ref to G4dyb basic distribution
    // Fig13 distance between visible energy and positron production position
    // distance between edep and init
    m_prompt_dist = TMath::Sqrt(
                TMath::Power(ibd_first.edep_x - ibd_first.init_x, 2)
              + TMath::Power(ibd_first.edep_y - ibd_first.init_y, 2)
              + TMath::Power(ibd_first.edep_z - ibd_first.init_z, 2)
             );

    // distance between delay init and prompt init
    m_delay_drift = TMath::Sqrt(
                TMath::Power(ibd_first.init_x - ibd_last.init_x, 2)
              + TMath::Power(ibd_first.init_y - ibd_last.init_y, 2)
              + TMath::Power(ibd_first.init_z - ibd_last.init_z, 2)
             );

    // Fig14(a) distance between gravity of visible energy 
    //       and neutron production position
    //       Here, assume production position of e+ and neutron are same.
    m_delay_dist_prod = TMath::Sqrt(
                TMath::Power(ibd_last.edep_x - ibd_first.init_x, 2)
              + TMath::Power(ibd_last.edep_y - ibd_first.init_y, 2)
              + TMath::Power(ibd_last.edep_z - ibd_first.init_z, 2)
             );

    
    // Fig14(b) distance between visible energy 
    //       and neutron capture position
    //       Assume capture position and gamma production position are same.
    // distance between delay init and delay edep
    // gamma
    m_delay_dist_cap = TMath::Sqrt(
                TMath::Power(ibd_last.edep_x - ibd_last.init_x, 2)
              + TMath::Power(ibd_last.edep_y - ibd_last.init_y, 2)
              + TMath::Power(ibd_last.edep_z - ibd_last.init_z, 2)
             );

    m_ibd_tree->Fill();
    return true;
}

// --------------------------------------------------------------------------
// select index and totalpe 
// --------------------------------------------------------------------------
bool
BasicDistAlg::do_split()
{
    // check the split size
    idx = -1;
    hitsmax = -1;
    for (std::vector<double>::iterator it = cnts_cluster.begin();
            it != cnts_cluster.end(); ++it) {
        if ((*it) > hitsmax) {
            hitsmax = (*it);
            idx = it-cnts_cluster.begin();
        }
    }
    if (idx == -1 or hitsmax == -1) {
        LogError << "can't find hits. " << std::endl;
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------
// NPE distribution
// --------------------------------------------------------------------------
bool
BasicDistAlg::do_npe()
{
    m_maxnpe = 0;
    // a) distribution of nPE
    typedef std::map<int, double> PMT2NPE;
    PMT2NPE pmt2npe;
    for (std::vector<JM::SimPMTHit*>::iterator it = hits.begin()+idx;
            it != hits.begin()+idx+hitsmax; ++it) {
        JM::SimPMTHit* hit = *it;
        int pmtid = hit->getPMTID();
        int npe = hit->getNPE();

        pmt2npe[pmtid] += npe;
    }
    for (PMT2NPE::iterator it = pmt2npe.begin();
            it != pmt2npe.end(); ++it) {

        m_npe_perpmt = it->second;
        // m_npe_tree->Fill();
        m_npe_hist->Fill(m_npe_perpmt);

        if (m_npe_perpmt > m_maxnpe) {
            m_maxnpe = m_npe_perpmt;
        }
    }
    LogInfo << "Max NPE (this event): " 
            << m_maxnpe << std::endl;
    // b) max pe
    return true;
}

// --------------------------------------------------------------------------
// Hit time distribution
// --------------------------------------------------------------------------
bool
BasicDistAlg::do_hittime() 
{

    // a) event last time. last hit - first hit time
    //    a tricky here is we already sort it
    double evt_first_hit_time = hits[idx]->getHitTime();
    double evt_last_hit_time = hits[idx+hitsmax-1]->getHitTime();
    m_evt_duration = evt_last_hit_time - evt_first_hit_time;
    LogInfo << "Event Last Time: " << m_evt_duration << std::endl;

    // b) response time: for every PMT, time between first and last
    // handle this collection
    typedef std::map<int, double> PMT2Time;
    PMT2Time pmt2time_min;
    PMT2Time pmt2time_max;
    for (std::vector<JM::SimPMTHit*>::iterator it = hits.begin()+idx;
            it != hits.begin()+idx+hitsmax; ++it) {
        JM::SimPMTHit* hit = *it;
        int pmtid = hit->getPMTID();
        double hittime = hit->getHitTime();
        // min
        if (pmt2time_min.count(pmtid) == 0) {
            pmt2time_min[pmtid] = hittime;
        }
        if (hittime < pmt2time_min[pmtid]) {
            pmt2time_min[pmtid] = hittime;
        }
        // max
        if (pmt2time_max.count(pmtid) == 0) {
            pmt2time_max[pmtid] = hittime;
        }
        if (hittime > pmt2time_max[pmtid]) {
            pmt2time_max[pmtid] = hittime;
        }
    }
    // calculate the diff
    for (PMT2Time::iterator it = pmt2time_max.begin();
            it != pmt2time_max.end(); ++it) {
        m_response_time = it->second - pmt2time_min[it->first];
        // m_response_time_tree->Fill();
        // TODO: add a cut 
        if (m_response_time > 0) {
            m_response_time_hist->Fill(m_response_time);
        }
    }
    return true;
}

bool
BasicDistAlg::save_tuple() 
{
    LogInfo << "save the tuple. " << std::endl;
    m_basic_tree->Fill();
    return true;
}
