#ifndef JUNO_QCTR_REC_ALG_H
#define JUNO_QCTR_REC_ALG_H

//
//  Charge Center Reconstruction Algorithm
//
//  Author: Zhengyun You  2013-11-20
//

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/DataBuffer.h"
#include "SniperKernel/RWDataBuffer.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "TVector3.h"

#include <vector>
#include <map>

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
class TF1;

class QCtrRecAlg : public AlgBase
{
    public :

	QCtrRecAlg(const std::string& name);
	virtual ~QCtrRecAlg();

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();

        // Initialization
        //-----------------------------------------------
        bool initGeom();
        bool initInput();
        bool initOutput();
        bool initParameters();

        // Read input, e.g. CalibHeader in an event
        //-----------------------------------------------
        bool readInput();

        // Reconstruction
        //-----------------------------------------------
        bool reconstruct();
        bool reconstructVtx();
        bool reconstructEnergy();
        bool correctEnergy();

        // Fill Reconstruction Output
        //-----------------------------------------------
        bool fillOutput();

        // Clear event based container
        //-----------------------------------------------
        void clear();

        // Print event info
        //-----------------------------------------------
        void printEvent();

        // Getter
        //-----------------------------------------------
        TVector3 getRecVtx()     { return m_recVtx; }

    private :

        void f_bool_handler(MyProperty*);
        void f_bool_updater(MyProperty*);
        bool m_usePerfectVtx;

        DataBufSvc* m_bufsvc;
        RWDataBuffer<JM::RecHeader>* m_rhbuf; // buffer containing RecHeader object
        DataBuffer<JM::CalibHeader>* m_chbuf; // buffer containing CalibHeader object

        RecGeomSvc* m_recGeomSvc;
        CdGeom*  m_cdGeom; 

        int   m_iEvt;

        // Correction functions
        TF1* m_efun;            // energy correction function
        TF1* m_non_li_positron; // non-linear correction for positron

        // CalibEvent
        JM::CalibHeader* m_chcol;

        // RecEvent
        TVector3 m_recVtx;
        double m_recx;
        double m_recy;
        double m_recz;
        double m_recr;
        double m_rectheta;
        double m_recphi;
        double m_recNPmt;
        double m_recPESum;
        double m_recEnergy;
        double m_eprec;
        JM::RecHeader* m_rh;
};

#endif // JUNO_QCTR_REC_ALG_H
