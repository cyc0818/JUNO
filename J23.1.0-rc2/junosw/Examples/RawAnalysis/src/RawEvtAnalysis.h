#ifndef RawEvtAnalysis_alg_h
#define RawEvtAnalysis_alg_h

#include "SniperKernel/AlgBase.h"
#include "RootWriter/RootWriter.h"
#include "EvtNavigator/NavBuffer.h"
#include "TTree.h"

class TH1S;

class RawEvtAnalysis : public AlgBase
{
    public :
        RawEvtAnalysis(const std::string& name);
        ~RawEvtAnalysis();

        bool initialize();
        bool execute();
        bool finalize();

    private :

        bool      m_fillRoot;
        bool      m_fillElec;

        int       m_count;
        unsigned int  m_nBins;

        TTree*    m_tree;
        TH1S*     m_waveform;
        UShort_t  m_PMTId;
        UShort_t  m_triggerNumber;
        UShort_t  m_triggerMode;
        ULong64_t m_time;
        UShort_t  m_version;
        UShort_t  m_dataType;
        UShort_t  m_errorRegister;
        UShort_t  m_highVoltageEnable;
        UShort_t  m_GCUId;

        JM::NavBuffer* m_buf;
};

#endif
