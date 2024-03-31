#ifndef SimAlg_hh
#define SimAlg_hh

#include "SniperKernel/AlgBase.h"

class tbbWorkerRunManager;
class SlaveRunMgrSvc;

class SimAlg: public AlgBase
{
    public:
        SimAlg(const std::string& name);
        ~SimAlg();

        bool initialize();
        bool execute();
        bool finalize();
    private:
        tbbWorkerRunManager* m_runmgr;
        SlaveRunMgrSvc* m_runmgrsvc;

        int i_event;
};

#endif
