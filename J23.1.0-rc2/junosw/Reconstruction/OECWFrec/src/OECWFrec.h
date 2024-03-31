#ifndef OECWFrec_h
#define OECWFrec_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include <map>
#include "RecTools/IWFRecTool.h"
#include "OECWFrecTool.h"
// #include "TStopwatch.h"

#ifdef PERFORMANCE_CHECK
#include "TTree.h"
#include "RootWriter/RootWriter.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TSystem.h"
#endif

namespace JM
{
    class ElecWaveform;
}


class OECWFrec: public AlgBase
{
    public:
        OECWFrec(const std::string& name);
        ~OECWFrec();

    public:
        bool initialize();
        bool execute();
        bool finalize();

    private:
        // TStopwatch timeWatch;
        int m_evt;
        std::vector<std::string> m_subTools;
        OECWFrecTool* oecwfrecTool;

        int m_totalLPMT = 17612;
       
#ifdef PERFORMANCE_CHECK        
        // for resources test
        TTree* m_calib;
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent;
        int N_mem;
        float mem_resident[2];//memory usage of the initialize and execute functions            
        float time_total;//time usage of the execute function             
#endif        
};


#endif


