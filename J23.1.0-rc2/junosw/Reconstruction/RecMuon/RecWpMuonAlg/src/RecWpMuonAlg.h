/*=============================================================================
#
# Author: Jilei Xu - xujl@ihep.ac.cn
# Last modified:	2016-11-13 01:39
# Filename:		RecWpMuonAlg.h
# Description: reference RecCdMuonAlg
#
=============================================================================*/
#ifndef RecWpMuonAlg_h
#define RecWpMuonAlg_h 1

#include "SniperKernel/AlgBase.h"

#include "EvtNavigator/NavBuffer.h"
#include "RecTools/PmtProp.h"
#include "RecTools/Params.h"
#include <string>

#ifdef WITH_OEC
#include "EvtDataTypes/CriteriaItemValue.h"
#endif

class RecGeomSvc; 
class WpGeom; 
class IRecMuonTool; 

class RecWpMuonAlg : public AlgBase
{
    public:  

        RecWpMuonAlg(const std::string& name);
        virtual ~RecWpMuonAlg();

        virtual bool execute();
        virtual bool initialize();
        virtual bool finalize();


    private:

        bool iniBufSvc(); 
        bool iniGeomSvc(); 
        bool iniPmtPos(); 
        bool iniRecTool(); 

        bool freshPmtData(); 

    private:

        int m_iEvt; 

        unsigned int m_totPmtNum; 

        double m_sigmaPmt3inch;
        double m_sigmaPmt20inch; 

        bool m_flagUse3inch;
        bool m_flagUse20inch;

        WpGeom*  m_wpGeom; 
        JM::NavBuffer* m_buf; 


        PmtTable m_pmtTable; 
        Params m_params; //set of parameters' key/value

        std::string m_recToolName; 
        IRecMuonTool* m_recTool; 

        bool m_flagOpPmtpos;

        std::string s_mode;

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

#endif // RecWpMuonAlg_h
