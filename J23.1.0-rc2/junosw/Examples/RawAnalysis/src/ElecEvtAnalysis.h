#ifndef ElecEvtAnalysis_alg_h
#define ElecEvtAnalysis_alg_h

#include "SniperKernel/AlgBase.h"
#include "RootWriter/RootWriter.h"
#include "EvtNavigator/NavBuffer.h"
#include "TTree.h"

class TH1S;

class ElecEvtAnalysis : public AlgBase
{
    public :
        ElecEvtAnalysis(const std::string& name);
        ~ElecEvtAnalysis();

        bool initialize();
        bool execute();
        bool finalize();

    private :

        bool      m_fillRoot;
        int       m_count;
        unsigned int  m_nBins;

        TTree*    m_tree;
        TH1S*     m_waveform;
        UInt_t    m_chNum;
        UInt_t    m_triggerNumber;
        ULong64_t m_time;
        UShort_t  m_PMTId[100];
        UShort_t  m_GCUId[100];
        UShort_t  m_chSize[100];

        JM::NavBuffer* m_buf;
};

#endif
