#ifndef CDWaveFormSaving_H
#define CDWaveFormSaving_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

class CDWaveFormSaving : public AlgBase
{
    public :

        CDWaveFormSaving(const std::string& name);

        bool initialize();
        bool execute();
        bool finalize();

    private :
        float m_wf_nPEcut;

        double m_pe_sum;
        JM::NavBuffer* m_buf;
};

#endif
