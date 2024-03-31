#ifndef Single_SingleEventIn_H
#define Single_SingleEventIn_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class SingleEventInAlg : public AlgBase
{
    public :

        SingleEventInAlg(const std::string& name);
        virtual ~SingleEventInAlg();

        virtual bool initialize();
        virtual bool execute();
        virtual bool finalize();

    private :

        int m_iEvt;
        JM::NavBuffer* m_buf;
};

#endif
