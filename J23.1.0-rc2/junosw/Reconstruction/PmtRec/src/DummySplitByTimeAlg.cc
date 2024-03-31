#include "DummySplitByTimeAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Event/SimHeader.h"

#include <algorithm>
#include <iterator>

DECLARE_ALGORITHM(DummySplitByTimeAlg);

DummySplitByTimeAlg::DummySplitByTimeAlg(const std::string& name)
    : AlgBase(name)
    , m_simnav(0), m_simheader(0), m_simevent(0)
{
    declProp("DetSimTaskName", detsimtask_name);
    declProp("SplitTimeGap", m_split_time_gap=10000); // 10us
    m_split_related.count = 0;
    m_split_related.is_first = true;
}

DummySplitByTimeAlg::~DummySplitByTimeAlg()
{

}

bool
DummySplitByTimeAlg::initialize()
{
    return true;
}

bool
DummySplitByTimeAlg::execute()
{
    while (m_split_related.count == 0) {
        // Full simulation
        bool st = execute_one();
        m_split_related.count = m_split_related.buffer.size();
        //execute_();
        LogDebug << "After Execute_ , m_split_related.count: "
            << m_split_related.count 
            << std::endl;
        if (not st) {
            LogError << "Execute_ Failed!" << std::endl;
            return dynamic_cast<Task*>(getRoot())->stop();
        } 
        if (m_hits_col.size() != 0) {
            break;
        }
    }
    // TT needs now to be used with ElecSim and it is no longer supported with the DummySplitByTimeAlg
    if (m_hits_col.size() == 0 /*&& m_hits_tt_col.size()==0*/) {
        // create a dummy event
        // FIXME: shall we get the navigator first?
        LogWarn << "There is no hit in current event. " << std::endl;
        LogWarn << "Create a dummy one." << std::endl;
        return true;
        auto nav = new JM::EvtNavigator();
        TTimeStamp ts = m_current_timestamp;
        nav->setTimeStamp(ts);

        SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
        mMgr->adopt(nav, "/Event");

        // create header
        auto sim_header = new JM::SimHeader;
        // create event
        auto sim_event = new JM::SimEvt();

        // set the relation
        sim_header->setEvent(sim_event);
        JM::addHeaderObject(nav, sim_header);
        return true;
    }
    // yield data
    // put data into buffer
    --m_split_related.count;
    save_into_buffer();

    return true;
}

bool
DummySplitByTimeAlg::finalize()
{
    return true;
}

bool
DummySplitByTimeAlg::execute_one()
{
    // = get detsim event data from buffer =
    bool st_load_det = load_detsim_data();
    if (not st_load_det) {
        return false;
    }
    // = split the event =
    //if (m_hits_col.size()==0) {
    //  if(m_hits_tt_col.size() > 0) split_evts_TopTracker();
    //  return true;
    //}
    // == find the gap ==
    find_gap();
    // == save the event into different SimEvents
    split_evts();
    return true;
}

bool 
DummySplitByTimeAlg::load_detsim_data()
{
    // trigger detector simulation task
    // * do simulation,
    // or 
    // * load data.
    JM::NavBuffer* navBuf = 0;
    if (detsimtask_name.empty()) {
        SniperDataPtr<JM::NavBuffer>  navBufPtr(getParent(), "/Event");
        if (navBufPtr.invalid()) {
            return false;
        }
        navBuf = navBufPtr.data();
        LogDebug << "navBuf: " << navBuf << std::endl;

    } else {
        LogDebug << "Trigger the detsimtask." << std::endl;
        Incident::fire(*getRoot(), detsimtask_name);
        SniperDataPtr<JM::NavBuffer>  navBufPtr(getRoot(), detsimtask_name+":/Event");
        if (navBufPtr.invalid()) {
            return false;
        }
        navBuf = navBufPtr.data();
        LogDebug << "navBuf: " << navBuf << std::endl;
    }
    if (navBuf->size() == 0) {
        LogError << "There is nothing in Cur Buffer." << std::endl;
        return false;
    }
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        return false;
    }
    m_simnav = evt_nav;
    m_current_timestamp = evt_nav->TimeStamp();
    LogDebug << "current timestamp: '" 
             << m_current_timestamp << "'." << std::endl;
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

    m_hits_col = m_simevent->getCDHitsVec();
    m_hits_wp_col = m_simevent->getWPHitsVec();

	//lizy::2019.02.26 Fix the bug that calib.root do not contain tracks info
	m_tracks = m_simevent->getTracksVec();

    // put the hits in WP into CD
    std::copy(m_hits_wp_col.begin(), m_hits_wp_col.end(),
                          std::back_inserter(m_hits_col));

    //m_hits_tt_col = m_simevent->getTTHitsVec();
    return true;
}

bool SortByhitTime(JM::SimPMTHit* hit1,JM::SimPMTHit* hit2){
    return hit1->getHitTime() < hit2->getHitTime();
}
bool
DummySplitByTimeAlg::find_gap() 
{
    // == sort the collection ==
    std::sort(m_hits_col.begin(),m_hits_col.end(), SortByhitTime);
    // == split the event if the gap is great than m_split_time_gap ==
    m_split_hits.clear();
    m_split_hits.push_back(0);
    for (int i = 1; i < m_hits_col.size(); ++i) {
        if ( (m_hits_col[i]->getHitTime() - m_hits_col[i-1]->getHitTime()) 
                > m_split_time_gap) {
            // save the start index
            m_split_hits.push_back(i);
        }
    }
    m_split_hits.push_back(m_hits_col.size());

    return true;
}

bool
DummySplitByTimeAlg::split_evts()
{
    m_split_related.buffer.clear();

    //if(m_hits_tt_col.size() > 0) split_evts_TopTracker(false);

    for (int i = 0 ; i < m_split_hits.size() - 1; ++i) {
        auto se = new JM::SimEvt();
        for (int j = m_split_hits[i]; j < m_split_hits[i+1]; ++j) {
            auto old_sph = m_hits_col[j];
            auto new_sph = se->addCDHit();
            new_sph->setPMTID( old_sph->getPMTID() );
            new_sph->setNPE( old_sph->getNPE() );
            new_sph->setHitTime( old_sph->getHitTime() );
        }

		//lizy::2019.02.26 Fix the bug that calib.root do not contain tracks info
		for (int j = 0; j < m_tracks.size(); j++){
			auto old_sph = m_tracks[j];
			auto new_sph = se->addTrack();
			new_sph->setInitX( old_sph->getInitX() );
			new_sph->setInitY( old_sph->getInitY() );
			new_sph->setInitZ( old_sph->getInitZ() );
			new_sph->setInitT( old_sph->getInitT() );
			new_sph->setExitX( old_sph->getExitX() );
			new_sph->setExitY( old_sph->getExitY() );
			new_sph->setExitZ( old_sph->getExitZ() );
			new_sph->setExitT( old_sph->getExitT() );
			new_sph->setEdep( old_sph->getEdep() );
			new_sph->setEdepX( old_sph->getEdepX() );
			new_sph->setEdepY( old_sph->getEdepY() );
			new_sph->setEdepZ( old_sph->getEdepZ() );
			new_sph->setQEdep( old_sph->getQEdep() );
			new_sph->setQEdepX( old_sph->getQEdepX() );
			new_sph->setQEdepY( old_sph->getQEdepY() );
			new_sph->setQEdepZ( old_sph->getQEdepZ() );
		}

        m_split_related.buffer.push_back(se);
    }
    return true;
}


//bool
//DummySplitByTimeAlg::split_evts_TopTracker(bool cleanup)
//{
    /*
     * Note: Top Tracker events not split really at this point.
     * TT hits will get separated from CD and WP hits which will have to be handled
     * by the reconstruction at reconstruction level. That being said there is no splitting
     * of hits within a TT event.
     */
    //if(cleanup) m_split_related.buffer.clear();

    //auto se = new JM::SimEvt();

    //for (int j=0; j < m_hits_tt_col.size(); ++j) {
// auto old_sph = m_hits_tt_col[j];
//      auto new_sph = se->addTTHit();
//       new_sph->setChannelID(old_sph->getChannelID());
//      new_sph->setPE(old_sph->getPE());
//      new_sph->setTime(old_sph->getTime());
//      new_sph->setADC(old_sph->getADC());
//      new_sph->setX(old_sph->getX());
//      new_sph->setY(old_sph->getY());
//      new_sph->setZ(old_sph->getZ());
//  }

//  m_split_related.buffer.push_back(se);

//  return true;
//}

bool
DummySplitByTimeAlg::save_into_buffer()
{
    // FIXME: shall we get the navigator first?
    auto nav = new JM::EvtNavigator();
    TTimeStamp ts = m_current_timestamp;
    LogInfo << "current timestamp: '" 
             << m_current_timestamp << "'." << std::endl;
    nav->setTimeStamp(ts);

    SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");
    // create header
    auto sim_header = new JM::SimHeader;
    // create event
    auto sim_event = m_split_related.buffer.front();
    m_split_related.buffer.pop_front();

    // set the relation
    sim_header->setEvent(sim_event);
    // LT:
    //   To distinguish SimEvent before and after event split,
    //   we use two different path.
    //
    //   Before: /Event/SimOrig
    //   After : /Event/Sim
    //
    // 2016.10.27
    JM::addHeaderObject(nav, sim_header);
    nav->copyHeader(m_simnav, "/Event/Sim", "/Event/SimOrig");
    return true;
}
