#ifndef EventRecAlgMT_h
#define EventRecAlgMT_h

/*
 * Description:
 *   This is a dummy reconstruction algorithm running in event-level MT mode.
 *   - access global geometry information
 *   - access global buffer to retrieve one calib event
 *   - mark the event done 
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn>
 */

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"

#include "SniperMuster/GlobalBuffer.h"

#include "RecGeomSvcMT.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"

#include "TVector3.h"
#include <vector>
#include <tuple>

#include "EvtNavMT.h"

class EventRecAlgMT: public AlgBase {
public:

    EventRecAlgMT(const std::string& name);
    ~EventRecAlgMT();

    bool initialize();
    bool execute();
    bool finalize();

private:
    bool Load_LikeFun();
    void VectorPush();
    void VectorClear();

    bool ChargeCenterRec();
    bool ChargeCenterFitter(double percent);
    bool ChargeCenterRecCorr();
    
private:
    GlobalBuffer* m_gbuf;

    //center detector geometry
    CdGeom*  m_cdGeom;

    // initialized
    int Total_num_PMT;
    double PMT_R ;
    double Ball_R;
    double LS_R;
    double pmt_r;

    int m_Pdf_Value;


    std::vector<TVector3> ALL_PMT_pos;


    // event loop
    int num_PMT;
    double PMT_HIT[20000]; 
    double PMT_HITTIME[20000];
    double PMT_HITTIMETTS[20000];
    double PMT_DarkRate[20000];
    double TimeSpread[20000];
    double DarkRate[20000];

    int PMT_isDNHit[20000]; //dn filter study

    //Charge and hit time information
    std::vector<double> Readout_PE;
    std::vector<double> Readout_hit_time; 
    std::vector<TVector3> PMT_pos;
    std::vector<double>  PMT_TTS; 
    std::vector<int>  PMT_ID; 

    std::vector<std::tuple<double, double, double, double, double> > FirstHitTime_NPE_PMTPos;


    double min_hit_time;


    //Charge Center Reconstruction
    double ChaCenRec_x;
    double ChaCenRec_y;
    double ChaCenRec_z;
    double ChaCenRec_r;


    // Fit Result
    double m_ccr_x;
    double m_ccr_y;
    double m_ccr_z;
    double m_rec_x;
    double m_rec_y;
    double m_rec_z;
    double m_rec_r;
    double m_rec_theta;
    double m_rec_phi;
    double m_rec_t;
    double m_like_vtx;
    double m_rec_n;
    double m_rec_E;
    double m_like_ene;
    double m_pe_sum;
    double m_time;
    double m_ene_time;
    double m_memory;

};

#endif
