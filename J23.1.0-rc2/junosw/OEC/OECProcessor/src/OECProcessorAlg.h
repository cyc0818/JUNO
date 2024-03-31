#ifndef OECPROCESSORALG_H
#define OECPROCESSORALG_H

//DEBUG MOD
#define HXDEBUG

#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "OECProcessor/OECBuffer.h"
#include "OECTagSvc/OECTagSvc.h"

#include "OECOutputHelper.hh"
//T/Q data model
#include "Event/CdLpmtElecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtElecHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "Event/WpElecHeader.h"
#include "Event/WpCalibHeader.h"
#include "Event/TtElecHeader.h"
#include "Event/TtCalibHeader.h"

#ifdef HXDEBUG
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "RootWriter/RootWriter.h"
#include "TSystem.h"
#include "TTree.h"
#include "TBranch.h"
#endif

#include <vector>

class OECProcessorAlg: public AlgBase{
    public:
        OECProcessorAlg(const std::string &name);
        ~OECProcessorAlg();

        bool initialize();
        bool execute();
        bool finalize();

    private:
        bool fireTask(std::string);
        bool writeOutput();

        //constant parameters
        std::vector<double> v_bufRange;

        //The buffers
        OECBuffer *m_buf;
        JM::NavBuffer* m_navbuf_input;
        IDataMemMgr* m_memmgr_lec;

        OECTagSvc* m_tagsvc;

        OECOutputHelper* m_output_helper;

        //for DEBUG
#ifdef HXDEBUG
        //for resources test
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent[2];
        TTree *m_OECProcessor[2];
        float f_memory;//memory usage at the begining of execute
        float f_memory2;//memory usage in the loop
        float f_time[5];//time usage of the execute function
        
        int N_mem;
        float f_memory3[2];//memory usage of HEC in execute function        
                  
#endif
};
#endif
