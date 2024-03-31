#ifndef Single_SingleEventOut_H 
#define Single_SingleEventOut_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "TRandom.h"

class SingleEventOutAlg : public AlgBase
{
    public :
	
        SingleEventOutAlg(const std::string& name);
        virtual ~SingleEventOutAlg();

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
