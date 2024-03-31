#include "TriggerSimAlg.h"

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"


DECLARE_ALGORITHM(TriggerSimAlg);

TriggerSimAlg::TriggerSimAlg(const std::string& name)
    : AlgBase(name), m_elecsvc(0)
{
    declProp("PulseBufferLength",m_PulseBufferLength=2000);
    declProp("LpmtTrigger_alg", m_LpmtTrigger_alg="MC-based");
    declProp("TriggerSPMTbyLPMT", m_triggerSPMTbyLPMT=false);
    declProp("WPLpmtTrigger_alg", m_WPLpmtTrigger_alg="WP-logic");
}

TriggerSimAlg::~TriggerSimAlg()
{

}

bool
TriggerSimAlg::initialize()
{

    SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
    if (svc.invalid()) {
        LogError << "can't find service ElecSimSvc" << std::endl;
        return false;
    }
    m_elecsvc = svc.data();

    // register trigger tool
    // --- CD-LPMT ---
    if (m_LpmtTrigger_alg == "MC-based") {
        m_trig_tool[kTriggerLpmt] = tool<ITriggerTool>("TriggerToolLpmt");
    } else if (m_LpmtTrigger_alg == "real-logic") {
        m_trig_tool[kTriggerLpmt] = tool<ITriggerTool>("TriggerVFLToolLpmt");
    } else if (m_LpmtTrigger_alg == "MM-trigger") {
        m_trig_tool[kTriggerLpmt] = tool<ITriggerTool>("MMTriggerToolLpmt");
    }
    // --- CD-SPMT ---
    if (not m_triggerSPMTbyLPMT) {
        // only register this tool when SPMT is not triggered by LPMT
        m_trig_tool[kTriggerSpmt] = tool<ITriggerTool>("TriggerToolSpmt");
    } else {
        LogInfo << "TriggerToolSpmt is disabled. " << std::endl;
    }

    // --- WP ---
  if(m_WPLpmtTrigger_alg == "WP-logic") { 
   m_trig_tool[kTriggerWP]   = tool<ITriggerTool>("TriggerToolWPLpmt");
    }
    m_trig_tool[kTriggerTT]   = tool<ITriggerTool>("TriggerToolTT");
    // check whether trigger tools exists
    for (std::map<TriggerType, ITriggerTool*>::iterator it = m_trig_tool.begin();
         it != m_trig_tool.end(); ++it) {
        if (!it->second) {
            LogError << "Can't find trigger tool for " << it->first << std::endl;
            return false;
        }

        // initialize trigger tools
        ITriggerTool* tool = it->second;
        bool st = tool->init();
        if (!st) {
            LogError << "Failed to initialize tool " << it->first << std::endl;
            return false;
        }
    }


    return true;
}

bool
TriggerSimAlg::execute()
{

    // Example: create trigger and put it into buffer
    // Trigger t; t.type = kTriggerLpmt; t.time = TimeStamp();
    // m_elecsvc->put_trigger(t);


    // In the new design, the trigger tool will be responsible 
    // to return a trigger. 
    // Then trigger alg will put it into trigger buffer.

    // note: to simplify the code, in one execution,
    // we can even skip to generate a trigger.

    // prepare enough pulse first.
    LogDebug << "Going to check if enough pulses exist in buffer." << std::endl;
    while (!m_elecsvc->has_enough_pulse()) {
        bool st = Incident::fire(*getRoot(),"PMTSimTask");
        if (!st) {
            LogError << "failed to incident PMTSimTask" << std::endl;
            return false;
        }

        // the the status of PMTSimTask
        SniperPtr<Task> pmtsimtask(getRoot(), "PMTSimTask");
        if (pmtsimtask->Snoopy().state()==Sniper::RunState::Stopped) {
            break;
        }

    }

    bool has_trigger = false;

    // invoke tools to generate trigger
    for (std::map<TriggerType, ITriggerTool*>::iterator it = m_trig_tool.begin();
         it != m_trig_tool.end(); ++it) {
        ITriggerTool* tool = it->second;

        LogInfo << "invoke tool to generate trigger: " << it->first << std::endl;
        if (tool->has_trigger()) {
            Trigger t = tool->get_trigger();
            m_elecsvc->put_trigger(t);

            LogInfo << "!!! trigger found: " << t.time<<";trigger type:"<<it->first << std::endl;
            has_trigger = true;
        }
    }

    if (!has_trigger) {
        LogDebug << "No trigger found, keep to fire PMTSimTask and try to put more pulses in buffer." << std::endl;
        // update current time if not trigger found
        bool st = Incident::fire(*getRoot(),"PMTSimTask");
        if (!st) {
            LogError << "failed to incident PMTSimTask" << std::endl;
            return false;
        }
        SniperPtr<Task> pmtsimtask(getRoot(), "PMTSimTask");
        if (pmtsimtask->Snoopy().state()==Sniper::RunState::Stopped) {
            // if no trigger found and the PMTSimTask is also stopped, we stop this task.
            dynamic_cast<Task*>(getParent())->stop(Sniper::StopRun::Peacefully);
        }

    }

    return true;
}

bool
TriggerSimAlg::finalize()
{
    return true;
}

