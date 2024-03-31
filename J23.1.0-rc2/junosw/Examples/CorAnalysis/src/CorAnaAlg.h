#ifndef COR_ANA_ALG_H
#define COR_ANA_ALG_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class CorAnaAlg : public AlgBase
{
    public :

        CorAnaAlg(const std::string& name);

        bool initialize();
        bool execute();
        bool finalize();

    private :

        int m_iEvt;
        JM::NavBuffer* m_buf;

};

#endif
