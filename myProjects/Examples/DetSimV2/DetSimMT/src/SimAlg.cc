#include <boost/python.hpp>
#include <thread>
#include <mutex>
#include <DetSimMT/SimAlg.hh>
#include <DetSimMT/SlaveRunMgrSvc.hh>

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/Incident.h"

DECLARE_ALGORITHM(SimAlg);

std::mutex mutex_simalg_init;

SimAlg::SimAlg(const std::string& name)
    : AlgBase(name)
    , m_runmgr(0)
    , i_event (0)
{

}

SimAlg::~SimAlg()
{

}

bool
SimAlg::initialize()
{
    // because our task is wrapped by boost python,
    // when we enter boost python, we need lock.
    std::unique_lock<std::mutex> lock_access_boostpython(mutex_simalg_init);
    SniperPtr<SlaveRunMgrSvc> drSvc(*getParent(), "SlaveRunMgrSvc");
    lock_access_boostpython.unlock();
    if (drSvc.invalid()) {
        LogError << "Failed to load Slave RunMgr Svc." << std::endl;
        return false;
    }
    m_runmgr = drSvc->getRunMgr();
    m_runmgrsvc = drSvc.data();

    m_runmgr->SimulateEventInit();
    return true;
}

bool
SimAlg::execute()
{
    if (m_runmgr->SimulateEvent(i_event)) {
        ++i_event;
        return true;
    } else {
        LogError << "m_runmgr->SimulateEvent(" << i_event << ") failed." << std::endl;
        return Incident::fire(*getParent(), "StopRun");
    }
    return true;
}

bool
SimAlg::finalize()
{
    m_runmgr->RunTermination();
    return true;
}
