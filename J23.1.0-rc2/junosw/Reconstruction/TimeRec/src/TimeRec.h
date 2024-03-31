#ifndef TimeRec_h
#define TimeRec_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "EvtNavigator/NavBuffer.h"
#include "Event/CalibPmtChannel.h"
#include "TTree.h"
#include "TProfile.h"
#include "TH2.h"
#include "TGraph.h"
#include "TCanvas.h"
#include <fstream>
//#include "TPDF.h"


namespace JM
{
    class ElecWaveform;
}

class TimeRec: public AlgBase
{
    public:
        TimeRec(const std::string& name);
        ~TimeRec();

    public:
        bool initialize();
        bool execute();
        bool finalize();

    private:
        JM::NavBuffer* m_buf;
        
        std::vector<double> tOffset;

        double m_windowLength;
        std::string algOption;

        //center detector geometry
        CdGeom*  m_cdGeom;

        std::vector<TVector3> ALL_PMT_pos;
        double Ball_R;
        double PMT_R;
        
        bool m_save;
        std::string mode;
        bool m_doCorrection;
        bool m_elecTruth;

        int m_nlpmt;

    private:
        // user's definition
        
        TTree* m_calib;
        std::vector<float> m_decon;
        std::vector<float> m_linear;
        std::vector<int> m_pulse;

        std::vector<float> vecTime;
        

        TH1D* deltaT[20000];
        TH1D *recCharge;
        TH1D* recFirstCharge;
        TH1D *time;
        TH1D* time_force;
        TH1D* thre;    // baseline fluctuation
        TH1D* basel;   // baseline 
        TH1D* diff_fht;
        TH1D* hist_allhitsTime;

        TCanvas* cc = new TCanvas();

        int firstHit;   //first hit number in vector

        int m_evt;
    private:
        double npe, first_npe, firstHitTime;
        // search pulse
        std::vector<int> vec_pass_time;
        std::vector<int> vec_start_time;
        std::vector<int> vec_end_time;
        std::vector<double> vec_time;

        std::vector<float> vec_decon_time;

    private:

        // search pulse algorithm
        void getPulse(const JM::ElecWaveform& channel,double baseline, double threshold);

        // chargeRec algorithm
         
        double getNPE(const JM::ElecWaveform& channel, double baseline, double threshold);
        double getNPE_FH(const JM::ElecWaveform& channel, double baseline, double threshold);

        // timeRec algorithm
        
        double getFHT_fixed(/*const JM::ElecWaveform& channel, double baseline, double threshold,*/ int hitNum);
        double getFHT_fitting1(const JM::ElecWaveform& channel, double baseline, double threshold,int hitNum);
        double getFHT_CFD(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum);
        double getFHT_peakFitting(const JM::ElecWaveform& channel, double baseline, double threshold, int hitNum);

    


        // little test
        int count;
        std::vector<int> m_adc;
        std::vector<int> m_tdc;

};

#endif
