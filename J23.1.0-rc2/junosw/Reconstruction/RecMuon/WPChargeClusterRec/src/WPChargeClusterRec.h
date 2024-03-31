#ifndef WPChargeClusterRec_h
#define WPChargeClusterRec_h 1

#include "SniperKernel/AlgBase.h"

#include "EvtNavigator/NavBuffer.h"
#include "WPChargeClusterRec/PmtProp.h"
#include <string>
#include "TVector3.h"


class RecGeomSvc;
class WpGeom;

class WPChargeClusterRec : public AlgBase
{
    public:  

        WPChargeClusterRec(const std::string& name);
        virtual ~WPChargeClusterRec();

        virtual bool execute();
        virtual bool initialize();
        virtual bool finalize();


    private:

        bool iniBufSvc(); 
        bool iniGeomSvc(); 
        bool iniPmtPos(); 

        bool freshPmtData();
        bool printStats();
        bool writeUserOutput(TVector3 re_R0, TVector3 re_dir, double minResult, double minVal);
        int **findClustersSimple(int *nClusters);
        TVector3 *mergeClusters(TVector3 *meanPos, double *meanCharge, int *nClusters);

        // functions for analysis, debugging etc
        int sumToZero(int start);
        void writeClusters(TVector3* clusterPos, double* meanCharge, int nClusters);
        void exportClustersToTxt(TVector3* clusterPos, double* meanCharge, int nClusters, int option);
        void exportEventToTxt();
        TVector3 getIntersectionPoint(int option);
        bool getMCTruth();
        double getCharge(int option);
        int * get2HighCharges();
        double getDist(int pmtId);

    private:

        int m_iEvt; 

        unsigned int m_totPmtNum;
        double m_minCiC; 

        double m_sigmaPmt;
        bool m_flagUse20inch;
        TVector3 mc_R0, mc_dir;
 
        WpGeom*  m_wpGeom; 
        JM::NavBuffer* m_buf;


        PmtTable m_pmtTable; 

        std::string m_userOutfile; // small rec result filename 

        bool m_flagOpPmtpos;
};

#endif // WPChargeClusterRec_h
