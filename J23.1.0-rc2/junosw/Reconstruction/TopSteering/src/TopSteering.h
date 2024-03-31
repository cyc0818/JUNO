#ifndef TOP_STEERING_H
#define TOP_STEERING_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class Incident;

class TopSteering: public AlgBase 
{
    public:
        TopSteering(const std::string& name); 
        ~TopSteering(); 

        bool initialize(); 
        bool execute(); 
        bool finalize(); 

    private:

        JM::NavBuffer* m_buf; 

        Incident* m_recPoint;
        Incident* m_recTrack;
        Incident* m_recWP;
        Incident* m_recTT;

        //key: event type, value: task name
        std::map<std::string, std::string> m_taskMap;
};
#endif
