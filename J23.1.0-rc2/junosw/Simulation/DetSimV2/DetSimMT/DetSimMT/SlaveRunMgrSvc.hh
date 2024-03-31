#ifndef SlaveRunMgrSvc_hh
#define SlaveRunMgrSvc_hh

/* 
 * SlaveRunMgrSvc is a service to manage slave run mgr.
 * 
 * Because the event loop is managed by SNiPER, this service
 * is also created in each SNiPER task. Then it is called by
 * detsim alg in each event.
 *
 * This class is like the tbbUserWorkerInitialization.
 * But the initialize is like tbbTask::execute.
 **/

#include "SniperKernel/SvcBase.h"
#include "DetSimMT/tbbWorkerRunManager.hh"

class SlaveRunMgrSvc: public SvcBase {
    public:
        SlaveRunMgrSvc(const std::string& name);
        ~SlaveRunMgrSvc();

        bool initialize();
        bool finalize();

        tbbWorkerRunManager* getRunMgr();

    private:
        tbbWorkerRunManager* m_runmgr;
        bool beamOnCondition;
};

#endif
