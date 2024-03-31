#include "PackSplitEventAlg.h"
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

DECLARE_ALGORITHM(PackSplitEventAlg);

PackSplitEventAlg::PackSplitEventAlg(const std::string& name)
    : AlgBase(name)
    , cache_evtnav(0), tmp_simheader(0), tmp_simevent(0), iotask(0)
{
    declProp("InputTask", m_task_name);
}

PackSplitEventAlg::~PackSplitEventAlg()
{

}

bool
PackSplitEventAlg::initialize()
{
    if (m_task_name.size() == 0) {
        LogError << "No Input Task!" << std::endl;
        return false;
    }
    // look for the task.
    // Here, the scope is another I/O Task 

    iotask = dynamic_cast<Task*>(getRoot()->find(m_task_name));
    if (iotask == 0) {
        LogError << "Can't find the task for I/O." << std::endl;
        throw SniperException("Make sure the IO task is created");
    }
    return true;
}

bool
PackSplitEventAlg::execute()
{
    // == create a new navigator ==
    JM::EvtNavigator* nav = 0; 
    // == create header ==
    JM::SimHeader* sim_header = 0;
    // == create event ==
    JM::SimEvt* sim_event = 0;
    // == iterate the evt nav from the normal io task. ==
    while (true) {
        // === incident the normal io task if there is no evt nav ===
        if (cache_evtnav == 0) {
            // ==== trigger the iotask ====
            Incident::fire(*getRoot(), m_task_name);

            if (getRoot()->Snoopy().state() != Sniper::RunState::Running) {
                LogWarn << "Stop Running." << std::endl;
                return true;
            }


            // === load the buffer ===
            SniperDataPtr<JM::NavBuffer>  navBufPtr(getRoot(), m_task_name+":/Event");
            if (navBufPtr.invalid() and nav == 0) {
                LogError << "can't find the buffer in task[" << m_task_name << "]" << std::endl;
                return dynamic_cast<Task*>(getRoot())->stop();;
            }
            if (navBufPtr->size() == 0 and nav == 0) {
                LogError << "There is nothing in Buffer[" << m_task_name << "]." << std::endl;
                return dynamic_cast<Task*>(getRoot())->stop();
            }
            cache_evtnav = navBufPtr->curEvt();
            if (not cache_evtnav and nav == 0) {
                LogError << "EvtNav is none in Buffer[" << m_task_name << "]." << std::endl;
                return dynamic_cast<Task*>(getRoot())->stop();
            }
            tmp_simheader = JM::getHeaderObject<JM::SimHeader>(cache_evtnav);
            if (not tmp_simheader and nav == 0) {
                LogError << "No SimHeader is none in Buffer[" << m_task_name << "]." << std::endl;
                return dynamic_cast<Task*>(getRoot())->stop();
            }
            // === get the SimEvent ===
            tmp_simevent = dynamic_cast<JM::SimEvt*>(tmp_simheader->event());
            if (not tmp_simevent and nav == 0) {
                LogError << "No SimEvent is none in Buffer[" << m_task_name << "]." << std::endl;
                return dynamic_cast<Task*>(getRoot())->stop();
            }
        }
        if (cache_evtnav == 0 or tmp_simheader == 0 or tmp_simevent == 0) {
            LogDebug << "no data in the normal I/O task." << std::endl;
            break;
        }
        // === create the nav if it does not exist ===
        if (nav == 0) {
            LogDebug << "create new EvtNavigator" << std::endl;

            nav = new JM::EvtNavigator(); 
            sim_header = new JM::SimHeader;
            LogDebug << "Current Event ID : " << tmp_simevent->getEventID() << std::endl;
            sim_event = new JM::SimEvt(tmp_simevent->getEventID());
        }
        // === compare the new created nav with the evt nav ===
        // FIXME the current data model can't return event id.
        //       so we just merge all the hits. 
        //       This is only for the muon event.
        // === merge the event ===
        if ( sim_event->getEventID() == tmp_simevent->getEventID()) {
            merge_evt(sim_event, tmp_simevent);
            cache_evtnav = 0;
            tmp_simheader = 0;
            tmp_simevent = 0;
        } else {
        // === break the loop if they are different ===
            break;
        }
    }
    // == put the nav into the buffer ==
    SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");

    sim_header->setEvent(sim_event);
    JM::addHeaderObject(nav, sim_header);
    LogDebug << "Save the event into buffer." << std::endl;
    return true;
}

bool
PackSplitEventAlg::finalize()
{
    return true;
}

bool
PackSplitEventAlg::merge_evt(JM::SimEvt* dest_evt, JM::SimEvt* src_evt)
{
    const auto& tmp_hits_col = src_evt->getCDHitsVec();
    const auto& tmp_hits_wp_col = src_evt->getWPHitsVec();

    if (logLevel()<1) {
        LogDebug << "Size of CD[" << m_task_name << "] Hits Collection: " << tmp_hits_col.size() << std::endl;
        LogDebug << "Size of WP[" << m_task_name << "] Hits Collection: " << tmp_hits_wp_col.size() << std::endl;
    }

    // === merge the SimEvent ===
    // FIXME: The pointer is copied, not the object.
    // ==== copy hits ====
    for (std::vector<JM::SimPMTHit*>::const_iterator ithit = tmp_hits_col.begin();
            ithit != tmp_hits_col.end(); ++ithit) {
        auto old_sph = *ithit;
        auto new_sph = dest_evt->addCDHit();
        new_sph->setPMTID( old_sph->getPMTID() );
        new_sph->setNPE( old_sph->getNPE() );
        new_sph->setHitTime( old_sph->getHitTime() );

    }

    for (std::vector<JM::SimPMTHit*>::const_iterator ithit = tmp_hits_wp_col.begin();
            ithit != tmp_hits_wp_col.end(); ++ithit) {
        auto old_sph = *ithit;
        auto new_sph = dest_evt->addWPHit();
        new_sph->setPMTID( old_sph->getPMTID() );
        new_sph->setNPE( old_sph->getNPE() );
        new_sph->setHitTime( old_sph->getHitTime() );

    }
    return true;
}
