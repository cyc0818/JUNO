#include "RandomSeedRecorder.h"
#include "SniperKernel/Task.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SvcBase.h"
#include "RandomSvc/IRandomSvc.h"

RandomSeedRecorder::RandomSeedRecorder(Task* task)
    : IIncidentHandler(task),
      m_task(task), m_1stCall(true), m_rndmsvc(0)
{

}

bool
RandomSeedRecorder::handle(Incident& /*incident*/)
{
    // == initialize the recorder ==
    if (m_1stCall) {
        bool st = init();
        if (not st) {
            LogWarn << "Initialize the RandomSeedRecorder failed." << std::endl;
            return false;
        }
        m_1stCall = false;
    }
    // == get the current seed ==
    long seed = m_rndmsvc->getSeed();
    LogDebug << "RandomSeedRecorder current seed: " << seed << std::endl;
    return true;
}

bool 
RandomSeedRecorder::init()
{
    // == check the random svc ==
    SniperPtr<IRandomSvc> rndm_svc(m_task, "RandomSvc");
    if (rndm_svc.invalid()) {
        LogError << "Can't find the RandomSvc" << std::endl;
        throw SniperException("RandomSvc is invalid");
    }
    m_rndmsvc = rndm_svc.data();
    // == check the root writer svc ==
    return true;
}
