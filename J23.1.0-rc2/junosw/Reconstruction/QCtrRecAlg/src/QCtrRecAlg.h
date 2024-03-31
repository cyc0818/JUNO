//to do review
#ifndef QCTRRECALG_H
#define QCTRRECALG_H

#include "SniperKernel/AlgBase.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/CdLpmtCalibHeader.h"
#include "RecTools/PMTTable.h"
#include "RecTools/IRecTool.h"

#ifdef WITH_OEC
#include "EvtDataTypes/CriteriaItemValue.h"
#endif

#ifdef PERFORMANCE_CHECK
#include "TTree.h"
#include "RootWriter/RootWriter.h"
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TSystem.h"
#endif

class QCtrRecAlg: public AlgBase{
    public:
        QCtrRecAlg(const std::string &);
        ~QCtrRecAlg();

        virtual bool initialize();
        virtual bool execute();
        virtual bool finalize();

    private:
        int i_ithEvt;
        unsigned int i_totPMTs;
        JM::NavBuffer *m_buf = nullptr;
        PMTTable m_pmtTable;
        HitPMTVector m_hitPmtvector;
        RecInfo m_recInfo;

        //options
        std::string s_recMethod;
        std::string s_mode;

        std::map<std::string, IRecTool*> m_recTools;
        std::vector<std::string> m_toolNames;
        bool initPMTPars();
        bool initRecTools();
        void fillTable(JM::CdLpmtCalibEvt*);
        void store_normal();

        //OEC related
#ifdef WITH_OEC
        CriteriaItemValue* m_energy = nullptr;
        CriteriaItemValue* m_detID = nullptr;
        CriteriaItemValue* m_vtxR = nullptr;

        void store_OEC();
#endif
#ifdef PERFORMANCE_CHECK
        // for resources test
        TTree* m_qctr;
        IJunoTimerSvc* m_junotimersvc;
        JunoTimerPtr m_timerevent;
        int N_mem;
        float mem_resident[2];//memory usage of execute function
        float time_total;//time usage of the execute function
#endif
};
#endif
