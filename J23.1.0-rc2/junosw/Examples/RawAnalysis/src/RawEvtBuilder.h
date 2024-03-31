#ifndef RAW_EVT_BUILDER_alg_h
#define RAW_EVT_BUILDER_alg_h

#include "EvtNavigator/NavBuffer.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "SniperKernel/AlgBase.h"
#include <vector>

class RawEvtBuilder : public AlgBase
{
    public :
        RawEvtBuilder(const std::string& name);
        ~RawEvtBuilder();

        bool initialize();
        bool execute();
        bool finalize();

    private :

        unsigned int  m_ngcus;
        unsigned int  m_ndone;
        unsigned int  m_count;

        IDataMemMgr*   m_mmgr;

        std::vector<JM::NavBuffer*> m_ibuf;
        std::vector<std::string> m_iname;
};

#endif
