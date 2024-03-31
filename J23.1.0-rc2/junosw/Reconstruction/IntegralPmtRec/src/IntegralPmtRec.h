#ifndef IntegralPmtRec_h
#define IntegralPmtRec_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "IPMTSimParamSvc/IPMTSimParamSvc.h"
#include <map>

namespace JM
{
    class ElecWaveform;
}


class IntegralPmtRec: public AlgBase
{
    public:
        IntegralPmtRec(const std::string& name);
        ~IntegralPmtRec();

    public:
        bool initialize();
        bool execute();
        bool finalize();

    private:
        double getNPE(const JM::ElecWaveform& channel, double gain);
        double getFHT(const JM::ElecWaveform& channel);//FHT, first hit time
   
    private:

        IDataMemMgr* m_memMgr;

        IPMTSimParamSvc* m_pmtsimsvc;
        std::map<int, double> m_gainMap;
        int m_totalPMT;
        double m_bin;
        double m_threshold;
};


#endif


