#ifndef RandomSeedRecorder_h
#define RandomSeedRecorder_h

#include "SniperKernel/IIncidentHandler.h"

class Task;
class IRandomSvc;

class RandomSeedRecorder : public IIncidentHandler
{
    public:
        RandomSeedRecorder(Task* task);

        bool handle(Incident& incident);

    private:
        bool init();
    private:

        Task* m_task;
        bool m_1stCall;

        IRandomSvc* m_rndmsvc;
};

#endif
