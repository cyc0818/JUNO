#ifndef TestBuffDataAlg_h
#define TestBuffDataAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include "Event/CdLpmtElecTruthHeader.h"
#include "Context/TimeStamp.h"
class ElecFeeCrate;
class DataBufSvcV2;
class LpmtElecTruth;

namespace JM{
    class ElecFeeCrate;
}

class TestBuffDataAlg: public AlgBase
{
    public:
        TestBuffDataAlg(const std::string& name);
        ~TestBuffDataAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        
        TTree* m_evt_tree;

        int m_EvtID;
        int m_Fired_PMT_Num;
        int m_PMTID[20000];
        double m_tdc[20000][1250];
        double m_adc[20000][1250];
        TH1D* h_1;

        //ElecTruth
        TTree* m_evt_truth;
        JM::CdLpmtElecTruthHeader* elec_truth_header;
        std::vector<std::string> m_pulsetype;
        std::vector<int> m_npe;
        std::vector<int> m_pmtId;
        std::vector<double>  m_amplitude;
        std::vector<double>  m_hitTime;
        std::vector<double>  m_TTS;
        std::vector<double>  m_timeoffset;
        std::vector<TimeStamp>  m_pulseHitTime;

        std::vector<JM::LpmtElecTruth>*  truths;
        
};

#endif
