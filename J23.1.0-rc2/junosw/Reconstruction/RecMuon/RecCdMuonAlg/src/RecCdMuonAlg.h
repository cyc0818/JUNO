/*=============================================================================
#
# Author: ZHANG Kun - zhangkun@ihep.ac.cn
# Last modified:	2015-05-11 01:39
# Filename:		RecCdMuonAlg.h
# Description: 
#
=============================================================================*/
#ifndef RecCdMuonAlg_h
#define RecCdMuonAlg_h 1

#include "SniperKernel/AlgBase.h"

#include "EvtNavigator/NavBuffer.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdSpmtCalibHeader.h"
#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include <string>
#include <memory>
#include "TH1F.h"

#ifdef WITH_OEC
#include "EvtDataTypes/CriteriaItemValue.h"
#endif

class RecGeomSvc; 
class CdGeom; 
class IRecMuonTool; 

class RecCdMuonAlg : public AlgBase
{
    public:  

        RecCdMuonAlg(const std::string& name);
        virtual ~RecCdMuonAlg();

        virtual bool execute();
        virtual bool initialize();
        virtual bool finalize();


    private:

        bool iniBufSvc(); 
        bool iniGeomSvc(); 
        bool iniPmtPos(); 
        bool iniRecTool(); 

        bool freshPmtData(); 
        bool freshPmtDataHelper(const std::list<JM::CalibPmtChannel*>& chlist);

    private:

        int m_iEvt; 

        unsigned int m_totPmtNum; 

        double m_sigmaPmt3inch;
        double m_sigmaPmt20inch; 

        bool m_flagUse3inch;
        bool m_flagUse20inch;

        CdGeom*  m_cdGeom; 
        JM::NavBuffer* m_buf; 


        PmtTable m_pmtTable; 
        Params m_params; //set of parameters' key/value

        std::string m_recToolName; 
        IRecMuonTool* m_recTool; 

        bool m_flagOpPmtpos;

        std::string s_mode;

        std::unique_ptr<TH1F> hits;

        //OEC related
#ifdef WITH_OEC
        CriteriaItemValue* n_cluster;
        CriteriaItemValue* dEdx;

        double TotaltrackL;
        double TotalPE;
        int ClusterNum; 
        std::vector<int> MuID;
        std::vector<float> MuInX;
        std::vector<float> MuInY;
        std::vector<float> MuInZ;
        std::vector<float> MuOutX;
        std::vector<float> MuOutY;
        std::vector<float> MuOutZ;
#endif
};

#endif // RecCdMuonAlg_h
