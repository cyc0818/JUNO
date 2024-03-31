#ifndef HECMUONALG_H
#define HECMUONALG_H

//DEBUG MOD
#define HXDEBUG

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"

#include "Event/OECHeader.h"
#include "OECData/OECTag.h"
#include "OECConfigSvc/OECConfigSvc.h"
//#include "OECProcessor/OECBuffer.h"
#include "Event/OECEvent.h"

#include "TTimeStamp.h"
#include "TH1.h"
#include "TTree.h"
#include <string>
#include <vector>
#include <deque>
#include <TVector3.h>

#ifdef HXDEBUG
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "RootWriter/RootWriter.h"
#include "TTree.h"
#endif

class HECMuonAlg: public AlgBase{
    public:
        HECMuonAlg(const std::string &name);
        ~HECMuonAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        JM::NavBuffer *m_buf;
        //OECBuffer *m_buf;
        JM::OECEvent *m_tEvent;

        //parameters
        double Mu_lowE;
        double Mu_highE;
        double d_muwindow;
        double d_buffwindow;
        double d_distpEvt2tEvt; 

        //tag list
        vector<OECTag*> m_tagList;
        OECConfigSvc* m_OECConfigSvc;

        //private function
        bool relatedTTorWP();
        int getTag(float, float, float);


        //for DEBUG
#ifdef HXDEBUG
        //for time test
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent;
        TTree *m_HECMuon;
        TVector3 MuTrack;
        TVector3 EventPos;
        TVector3 cross_vector;
        float f_time;//time usage of the execute function
        int f_tag;
#endif
};

#endif
