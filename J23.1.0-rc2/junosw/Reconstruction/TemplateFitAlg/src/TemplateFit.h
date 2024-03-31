#ifndef TemplateFit_h
#define TemplateFit_h

#include "TFitResultPtr.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TRandom.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TMultiGraph.h"
#include "TFitResult.h"
#include "TFile.h"
#include "TLine.h"
#include "TLegend.h"
#include "TFitResultPtr.h"
#include "TVirtualFitter.h"
#include <TROOT.h>

#include <sstream>
#include <vector>
#include <map>
#include <iostream>

using namespace std;
typedef std::map<float,float> Pair;//charge and time pairs

class TemplateFit{
    public:
        TemplateFit ();
        ~TemplateFit(){/*  */}
        void draw(TH1F* th,TH1F* th1,TH1F* th2);
        void SetChannel(int chId){m_channelcount=chId;}
        Pair SearchRoi(TH1F* thf);// return pulse region start-end 
        vector<float> NpeQ(float a, float b);// return npe&&totalQ
        vector<float> RoughQT(float a, float b,vector<float> npeQ);//deal with pulse region one by one return hittime as initial parameters
        void SetParameters(vector<float> peaks,float a, float b);// hittime and pulse region as fit region
        TFitResultPtr Fit(TH1F* thf);// Fit waveforms that substract already fitted part
        TFitResultPtr totalFit(TH1F* thf);
        double Get1st();
        bool InsertRegionResult(TFitResultPtr re);


    private:
        void calib();
        void genth1();
        float singletemplate(Double_t *x, Double_t *par);
        double gentemplate(Double_t *x, Double_t *par);


        TF1*  m_fit;
        TF1*  m_presentFit;
        
        int m_channelcount;
        int m_fittrycount;// in one region try several times
        int m_nfitpulse;

        Pair  m_peaks;// to store fit results
        TH1F* m_template;    
        TH1F* m_singlepecalib;
        TH1F* m_noise;
        TH1F* m_data;
        
        static const int m_absSlidingSumWidth=5;
       
        int m_SlidingSumWidth;
        int m_SWSumN;
        int m_totalpoints;
     
        float m_tstart;
        float m_tend;
        float m_xT;//sample delta T 0.1 1 2 ns 
        float m_initializeoffset;// to correct initialize parameter
        float m_firsthittime;
        float m_QcalibSpe;//spe Q
        float m_startthres;//pulse region start threshold
        float m_endthres;// pulse region stop threshold
        std::vector<float> m_adc_arr;// store waveform adc
        std::vector<float> m_SlidingSum;// sliding window sum
};

#endif
