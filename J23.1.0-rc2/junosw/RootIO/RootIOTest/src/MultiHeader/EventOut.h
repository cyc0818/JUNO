#ifndef MultiEventOut_H 
#define MultiEventOut_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "TRandom.h"

class EventOutAlg : public AlgBase
{
    public :

        EventOutAlg(const std::string& name);
        virtual ~EventOutAlg();

        virtual bool initialize();
        virtual bool execute();
        virtual bool finalize();

    private :

        JM::NavBuffer* m_buf;
        int m_iEvt;
        int m_offset;
        TRandom m_r;
};

#endif
