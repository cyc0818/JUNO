#ifndef JUNO_QCTR_TOY_SIM_ALG_H
#define JUNO_QCTR_TOY_SIM_ALG_H

//
//  Toy Simulator for Charge Center Reconstruction Algorithm
//
//  Author: Zhengyun You  2013-12-26
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

class QCtrToySimAlg : public AlgBase
{
    public :

	QCtrToySimAlg(const std::string& name);
	virtual ~QCtrToySimAlg();

	virtual bool initialize();
	virtual bool execute();
	virtual bool finalize();

        // Initialization
        //-----------------------------------------------
        bool initGeom();
        bool initBuf();
        bool initOutput();

        // Test with toy data
        //-----------------------------------------------
        bool makeToyData();
        bool randomGenVtx(double rMax=1.0e4, double rMin=0.0,
                          double thetaMax=3.14159265359, double thetaMin=0.0, 
                          double phiMax=3.14159265359*2.0, double phiMin=0.0);
        bool propogate();
        void addSimHit(const Identifier &id, const double t);

        bool fillOutput();

        // Print event info
        //-----------------------------------------------
        void printEvent();

        // Getter, to be put in McTruth association algorithm
        //-----------------------------------------------    
        TVector3 getGenVtx()     { return m_genVtx; }
        int      getGenNPhoton() { return m_genNPhoton; }
        double   getSimAccpt()   { return m_simAccpt; }
        int      getSimNHit()    { return m_simNHit; }

    private :

        DataBufSvc* m_bufsvc;
        RWDataBuffer<JM::SimHeader>* m_shbuf; //buffer containing SimHeader object

        RecGeomSvc* m_recGeomSvc;
        CdGeom*  m_cdGeom; 

        int   m_iEvt;

        // GenEvent from toy sim
        TVector3 m_genVtx;
        int    m_genNPhoton;
        double m_genx;
        double m_geny;
        double m_genz;
        double m_genr;
        double m_gentheta;
        double m_genphi;

        // SimEvent
        std::map<int, JM::SimPMTHeader*> m_sphmap;
        JM::SimHeader* m_simheader;
 
        int     m_simNHit;
        double  m_simAccpt;                   // Acceptance ratio of gamma

        TTree* m_treeToySim;
};

#endif // JUNO_QCTR_TOY_SIM_ALG_H
