#include "ElecSimAlg.h"

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/SniperException.h"

#include "EvtNavigator/NavBuffer.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "Event/GenHeader.h"
#include "Event/SimHeader.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtElecHeader.h"
#include "Event/CdSpmtElecHeader.h"
#include "Event/WpElecHeader.h"
#include "Event/TtElecHeader.h"

#include "Event/CdTriggerHeader.h"
#include "Event/WpTriggerHeader.h"
#include "Event/TtTriggerHeader.h"


#include "Event/CdLpmtElecTruthHeader.h"
#include "Event/CdSpmtElecTruthHeader.h"
#include "Event/TrackElecTruthHeader.h"
#include "Event/WpElecTruthHeader.h"
#include "Event/TtElecTruthHeader.h"


#include "Context/TimeStamp.h"
#include <TTimeStamp.h>
#include "EventKeeper.h"

DECLARE_ALGORITHM(ElecSimAlg);

ElecSimAlg::ElecSimAlg(const std::string& name)
    : AlgBase(name), m_elecsvc(0) {
    declProp("TriggerSPMTbyLPMT", m_triggerSPMTbyLPMT=false);
    declProp("Gen", m_Gen=true);
}

ElecSimAlg::~ElecSimAlg() {

}

bool
ElecSimAlg::initialize() {
    SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
    if (svc.invalid()) {
        LogError << "can't find service ElecSimSvc" << std::endl;
        return false;
    }
    m_elecsvc = svc.data();

    // register trigger handler
    m_trig_handler[kTriggerLpmt] = tool<ITriggerHandler>("TriggerHandlerLpmt");
    m_trig_handler[kTriggerSpmt] = tool<ITriggerHandler>("TriggerHandlerSpmt");
    m_trig_handler[kTriggerWP] = tool<ITriggerHandler>("TriggerHandlerWPLpmt");
    m_trig_handler[kTriggerTT] = tool<ITriggerHandler>("TriggerHandlerTT");


    // check whether hander exists
    for (std::map<TriggerType, ITriggerHandler*>::iterator it = m_trig_handler.begin();
         it != m_trig_handler.end(); ++it) {
        if (!it->second) {
            LogError << "Can't find handler for " << it->first << std::endl;
            return false;
        }

        it->second->init();
    }

    return true;
}

bool
ElecSimAlg::execute() {
    // check the status of Snoopy
    if (getRoot()->Snoopy().state()==Sniper::RunState::Stopped) {
        throw StopRunProcess();
    }


    // get a trigger from trigger buffer (invoke trigger algorithms)
    LogDebug << "check whether trigger buffer is empty." << std::endl;
    while (!m_elecsvc->has_trigger()) {

        // Following is an example to show how to add a new trigger:
        //   Trigger t; t.type = kTriggerLpmt; t.time = TimeStamp();
        //   m_elecsvc->put_trigger(t);

        // we will send an incident to to trigger task
        bool st = Incident::fire(*getRoot(),"TriggerSimTask");
        if (!st) {
            LogError << "failed to incident TriggerSimTask.";
            return false;
        }
        if (getRoot()->Snoopy().state()==Sniper::RunState::Stopped) {
            if (!m_elecsvc->has_trigger()) {
                throw StopRunProcess();
            }
        }

        SniperPtr<Task> triggersimtask(getRoot(), "TriggerSimTask");
        if (triggersimtask->Snoopy().state()==Sniper::RunState::Stopped) {
            // if no trigger found and the PMTSimTask is also stopped, we stop this task.
            dynamic_cast<Task*>(getParent())->stop(Sniper::StopRun::Promptly);
        }

    }
    Trigger trigger = m_elecsvc->get_trigger();
    LogDebug<< "get the trigger: "
             << trigger.type << " "
             << trigger.time << std::endl;

    const TimeStamp& cur_EvtTime =  m_elecsvc->get_cur_evttime(); 
    TimeStamp&  stop_Time = m_elecsvc->get_stop_time();

    if(cur_EvtTime>stop_Time) {
        return dynamic_cast<Task*>(getRoot())->stop(Sniper::StopRun::Promptly);
    }


    // handle trigger
    // produce and modify an event (readout)
    // note: in one header, we can have a lpmt event and a spmt event.

    // we need to create a new event navigator, due to event mixing will
    // break the 1-1 relationship.
    auto nav = new JM::EvtNavigator();
    nav->setTimeStamp(TTimeStamp(trigger.time.GetSec(),
                                 trigger.time.GetNanoSec()));

    // set the Run and Event number
    nav->setRunID(m_elecsvc->getCurrentRunID());
    nav->setEventID(m_elecsvc->getCurrentEventID());

    m_elecsvc->setCurrentEventID(m_elecsvc->getCurrentEventID()+1); // ++EventID

    SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");


    auto gen_hdr = new JM::GenHeader();
    auto sim_hdr = new JM::SimHeader();


    // TODO:
    // These headers should be created on demand. Otherwise it will waste resources. 
    // Tao Lin, 2022.03.09

    // save the event (readout) into buffer
    if (m_Gen) JM::addHeaderObject(nav, gen_hdr);
    JM::addHeaderObject(nav, sim_hdr);

    // =======================================================================
    // BEGIN Trigger Menu:
    // =======================================================================

    // =======================================================================
    // --- CD ---
    // =======================================================================
    bool isTriggeredByLPMT = trigger.type == kTriggerCD or 
                             trigger.type == kTriggerLpmt or 
                             trigger.type == kTriggerLpmtStd or 
                             trigger.type == kTriggerLpmtVFL or 
                             trigger.type == kTriggerForce or
                             trigger.type == kTriggerLaser;

    bool isTriggeredBySPMT = trigger.type == kTriggerCD or 
                             trigger.type == kTriggerSpmt;

    // no matter SPMT is triggered or not, trigger SPMT
    // * case 1: the kTriggerCD is given
    // * case 2: the m_triggerSPMTbyLPMT is enabled
    bool isTriggeredByCD = trigger.type == kTriggerCD or
                           (m_triggerSPMTbyLPMT and isTriggeredByLPMT);
    if (isTriggeredByCD) {
        isTriggeredByLPMT = false;
        isTriggeredBySPMT = false;
    }

    // =======================================================================
    // --- WP ---
    // =======================================================================
    bool isTriggeredByWP = trigger.type == kTriggerWP;

    // =======================================================================
    // --- TT ---
    // =======================================================================
    bool isTriggeredByTT = trigger.type == kTriggerTT;

    // =======================================================================
    // END   Trigger Menu
    // =======================================================================

    if (isTriggeredByCD) {
        LogDebug << "Produce event: LPMT and SPMT" << std::endl;
        m_trig_handler[kTriggerLpmt]->handle(trigger, nav);
        m_trig_handler[kTriggerSpmt]->handle(trigger, nav);

    } else if (isTriggeredByLPMT) {

        LogDebug << "Produce event: LPMT with waveforms" << std::endl;
        m_trig_handler[kTriggerLpmt]->handle(trigger, nav);

    } else if (isTriggeredBySPMT) {

        // only handle this case when m_triggerSPMTbyLPMT is not enabled
        if (not m_triggerSPMTbyLPMT) {
            LogDebug << "Produce event: SPMT with adc/tdc" << std::endl;
            m_trig_handler[kTriggerSpmt]->handle(trigger, nav);
        }

    } else if (isTriggeredByWP) {
       LogInfo<<"Produce event: WP trigger type!"<<std::endl;
       m_trig_handler[kTriggerWP]->handle(trigger, nav);

    } else if (isTriggeredByTT) {
       LogInfo<<"Produce event: TT trigger type!"<<std::endl;
       m_trig_handler[kTriggerTT]->handle(trigger, nav);
    } else {
        LogError << "Unknown trigger type. " << std::endl;
        return false;
    }


    // after readout, we need to commit the changes in EventKeeper.

    if (m_Gen) EventKeeper::Instance().rebuildGenEvent(nav);
    EventKeeper::Instance().rebuildSimEvent(nav);
    EventKeeper::Instance().commit();
    return true;
}

bool
ElecSimAlg::finalize() {
    return true;
}
