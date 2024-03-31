#ifndef HECAtmAlg_H
#define HECAtmAlg_H

//DEBUG MOD
#define HXDEBUG

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

#include "OECData/OECTag.h"
#include "OECTagSvc/OECTagSvc.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "TTimeStamp.h"
#include "TH1.h"
#include "TTree.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <TVector3.h>

#ifdef HXDEBUG
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TTree.h"
#endif

#ifdef PERFORMANCE_CHECK
#include "TTree.h"
#include "RootWriter/RootWriter.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TSystem.h"
#endif

class HECAtmAlg: public AlgBase{
    public:
        HECAtmAlg(const std::string &name);
        ~HECAtmAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        int i_nEvt;
        double Qbf = 0.1;
        double d_dt = 2000.;
        double thre_E = 100.;
        JM::NavBuffer *m_buf;
        OECTagSvc* m_tagsvc;
        int FCtag;
        int CdPCtag;
        int WpPCtag;
        int MaxCorrEvtNum = 100;
        std::vector<JM::OecEvt*> corEvts;
        //private function
        int getTag(float, float, float);

#ifdef PERFORMANCE_CHECK
        // for resources test
        TTree* m_hecatmalg;
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent;
        int N_mem;
        float mem_resident[4];//memory usage of the initialize and execute functions            
        float time_total;//time usage of the execute function    
#endif  
};

#endif
