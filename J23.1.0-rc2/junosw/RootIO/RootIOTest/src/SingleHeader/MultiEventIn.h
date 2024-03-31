#ifndef Single_MultiEventIn_H
#define Single_MultiEventIn_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class MultiEventInAlg : public AlgBase
{
    public :

        MultiEventInAlg(const std::string& name);
        virtual ~MultiEventInAlg();

        virtual bool initialize();
        virtual bool execute();
        virtual bool finalize();

    private :

        int m_iEvt;
        JM::NavBuffer* m_buf;
};

#endif
