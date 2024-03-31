#ifndef MultiEventIn_H
#define MultiEventIn_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class EventInAlg : public AlgBase
{
    public :

        EventInAlg(const std::string& name);
        virtual ~EventInAlg();

        virtual bool initialize();
        virtual bool execute();
        virtual bool finalize();

    private :

        int m_iEvt;
        JM::NavBuffer* m_buf;
};

#endif
