#ifndef TimeCorrAlg_h
#define TimeCorrAlg_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TTree.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include <vector>
#include <fstream>
#include <string>
#include "EvtNavigator/NavBuffer.h"
#include "Context/TimeStamp.h"
#include "Geometry/PMTParamSvc.h"
//////////////////////////////////////////////////////////////////////////
class ElecFeeCrate;
class DataBufSvcV2;
using  std::string;
class RecGeomSvc;
class CdGeom;

namespace JM
{
    class ElecWaveformEvt;
    class ElecWaveform;
}

class TimeCorrAlg: public AlgBase
{
    public:
        TimeCorrAlg(const std::string& name);
        ~TimeCorrAlg();

    public:
        bool initialize();
        bool execute();
        bool finalize();

    private:
        double npe, first_npe, firstHitTime;
        int firstHit;   //first hit number in vector
        // search pulse
        std::vector<int> vec_pass_time;
        std::vector<int> vec_start_time;
        std::vector<int> vec_end_time;
        std::vector<double> vec_time;

        // variables in PMTCalibSvc
        std::vector<int> pmtId;
        std::vector<double> relaDE;
        std::vector<double> gain;
        std::vector<double> toffset;
        std::vector<double> darkrate;

    private:
        // search pulse algorithm
        void getPulse(const JM::ElecWaveform& channel,double baseline, double threshold);

        // chargeRec algorithm
         
        double getNPE(const JM::ElecWaveform& channel, double baseline, double threshold, int pmtid);
        double getNPE_FH(const JM::ElecWaveform& channel, double baseline, double threshold, int pmtid);
        //double getNPE(JM::ElecFeeChannel& channel, double baseline);
        //double getNPE_FH(JM::ElecFeeChannel& channel,double threshold, double baseline);
        // timeRec algorithm
        
        double getFHT_fixed(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum);
        double getFHT_linear(const JM::ElecWaveform& channel, double baseline, double threshold,int hitNum);
        double getFHT_linearFit(const JM::ElecWaveform& channel, double baseline, double threshold,int hitNum, int fitOption);
        double getFHT_CFD(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum);
        double getFHT_peakFitting(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum);
        bool   ChargeCenterRec();
        double CalculateTOF(int pmtid, double source_x ,double source_y, double source_z);
        // draw waveform
        void draw_waveform(const JM::ElecWaveform& channel, int pmtID, double time, int fht, double baseline, double threshold, double p0, double p1);

    private:

        JM::NavBuffer* m_buf; 

        TCanvas* cc = new TCanvas();
        // variables in PMTCalibSvc

        std::vector<double> tOffset;
        int m_totalPMT;
        double m_length;
        double Ball_R;
        double PMT_R;
        double LS_R;
        double ChaCenRec_ratio;
        string option;
        string trigger;
        std::vector< std::vector<double> > m_particlePositions;
        string hitsOption;
        double forceTriggerSigma;

        //center detector geometry
        CdGeom*  m_cdGeom;

        std::vector<TVector3> ALL_PMT_pos;

        //pmt param svc
        PMTParamSvc* m_PMTParamsvc;

        // tof correction strategy
        // calculate in initialization, all calib in same position
        double relativeTOF[20000];


    private:
        int m_evt;
        TTree* m_corr;
        std::vector<int> m_pmtId;
        std::vector<double> m_time;
        double m_nPE[20000];
        double m_nPEforFH[20000];
        double m_baseline[20000];
        double m_threshold[20000];
        std::vector<double> m_charge;
        double m_reftime;
        

        int num_PMT;
        double ChaCenRec_x;
        double ChaCenRec_y;
        double ChaCenRec_z;

        double spescale;


        double chi2ndf;
        double chi2;
        double t_bias;

    private:
        //user's definition
        TH1D* deltaT[20000];

        int peak_time;

        int pass_time;

        TH1D* thre;
        double threshold;
        TH1D* basel;

        double x1,x2,y1,y2;
        double xx1,xx2,yy1,yy2;
        double p0,p1;

        

        // for Q-T effect
        TH1D* time1hist[30];  // hittime by hamamatsu
        TH1D* time2hist[30];  // hittime by nnvt
        int count_qt;


    private:
        // for single channel fitting
        double MIN_BIN = -200;
        double MAX_BIN = 1300;
        int BIN_NUM = 1500;

        const double nominal_baseline = 100;
        const double nominal_threshold = 37;

        double cut_baseline = 105;
        double cut_threshold = 20;
        //double cut_threshold = 60;
        
    private: 
        //double m_charge[17613];
        double gHeight;

};

#endif
