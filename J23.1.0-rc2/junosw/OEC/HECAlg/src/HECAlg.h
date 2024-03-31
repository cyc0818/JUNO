#ifndef HECALG_H
#define HECALG_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "OECConfigSvc/OECConfigSvc.h"
#include "OECData/OECTag.h"
#include "HECAlg/IClassTool.h"
#include "SniperKernel/ToolBase.h"

#ifdef PERFORMANCE_CHECK
#include "TTree.h"
#include "RootWriter/RootWriter.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TSystem.h"
#endif

class HECAlg: public AlgBase{
    public:
        HECAlg(const std::string&);
        ~HECAlg();

        virtual bool initialize();
        virtual bool execute();
        virtual bool finalize();

    private:
        bool findTag(const IClassTool::EventPair&, const classifyQuantity&);


        int i_nEvt;
        JM::NavBuffer *m_buf;

        //For the classification tool
        std::vector<IClassTool*> v_classTool;
        std::vector<std::string> v_toolName;

        //For the tag name and its tag value
        std::vector<OECTag*> m_tagList;
        OECConfigSvc* m_OECConfigSvc;
        
#ifdef PERFORMANCE_CHECK
        // for resources test
        TTree* m_hecalg;
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent;
        int N_mem;
        float mem_resident[2];//memory usage of the execute function     
        float time_total;//time usage of the execute function    
        // for IBD information
        TTree* m_ibd;
        float m_ibd_penergy;
        float m_ibd_denergy;
        float m_ibd_distance;
        float m_ibd_deltaT;
#endif        
};
#endif
