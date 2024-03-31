#ifndef JUNO_QCTR_EVAL_ALG_H
#define JUNO_QCTR_EVAL_ALG_H

//
//  Charge Center Evaluation Algorithm
//
//  Author: Zhengyun You  2013-12-20
//

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/DataBuffer.h"
#include "SniperKernel/RWDataBuffer.h"

namespace JM
{
    class RecHeader;
    class CalibPMTHeader;
    class CalibHeader;

    class SimHeader;
    class SimPMTHeader;
    class SimPMTHit;
}

class DataBufSvc;
class TTree;
class TH1F;
class TH2F;
class TProfile;

class QCtrEvalAlg : public AlgBase
{
    public :

	QCtrEvalAlg(const std::string& name);
	virtual ~QCtrEvalAlg();

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();

        // Initialization
        //-----------------------------------------------
        bool initInput();
        bool initOutput();

    private :

        //void f_bool_handler(MyProperty*);
        //void f_bool_updater(MyProperty*);
        //bool m_useToyData;

        DataBufSvc* m_bufsvc;
        DataBuffer<JM::RecHeader>* m_rhbuf;  // buffer containing RecHeader object
        DataBuffer<JM::SimHeader>* m_shbuf;  // buffer containing SimHeader object

        int   m_iEvt;

        JM::RecHeader* m_rh;
        JM::SimHeader* m_sh;

        TH1F* m_hdr;
        TH1F* m_hdx;        
        TH1F* m_hdy;
        TH1F* m_hdz;
        TH1F* m_hde;
        TH1F* m_hdeprec;
        TH1F* m_hrratio;
        TH2F* m_hnpevsr;
        TProfile *m_pnpevsr3;
        TH1F* m_heratio;
        TH1F* m_heprecratio;
};

#endif // JUNO_QCTR_REC_ALG_H
