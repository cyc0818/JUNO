#ifndef MU_STEERING_H
#define MU_STEERING_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class Incident;

class MuSteering: public AlgBase 
{
    public:
        MuSteering(const std::string& name); 
        ~MuSteering(); 

        bool initialize(); 
        bool execute(); 
        bool finalize(); 

    private:

        JM::NavBuffer* m_buf; 

        Incident* m_recMuSingle;
        Incident* m_recMuBundle;

        //key: event type, value: task name
        std::map<std::string, std::string> m_taskMap;

        // the incident tasks are in top task or in sub task
        // - true: in top task
        // - false: in current task
        bool m_isInTopTask;

        ExecUnit* m_task;
};
#endif
