#ifndef Single_MultiEventOut_H 
#define Single_MultiEventOut_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "TRandom.h"

class MultiEventOutAlg : public AlgBase
{
    public :

        MultiEventOutAlg(const std::string& name);
        virtual ~MultiEventOutAlg();

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
