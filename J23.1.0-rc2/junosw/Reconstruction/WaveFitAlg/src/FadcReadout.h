#ifndef FadcReadout_h
#define FadcReadout_h

#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TMath.h"
#include "TF1.h"
#include "TLine.h"
#include "TTree.h"
#include "TStyle.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TAxis.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TProfile.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TApplication.h"
#include <TSelector.h>
#include <TROOT.h>
#include "FadcFit.h"

#include "IWaveFitTool.h"
#include "SniperKernel/ToolBase.h"
 
using namespace std;

class FadcWaveform : public IWaveFitTool, 
                     public ToolBase {
    typedef std::map<float,float> Peaks;
    typedef std::map<float,float> Pulses;
    struct Roi {
      int            start;
      int            end;
      int            preGap;
      int            postGap;
      float          maxAdc;
      float          qdc;
      float          charge;
      float          avgCharge;
      Peaks          peaks;
      Peaks          pulses;
      vector<double> fitPar;
      vector<double> lastFitPar;
      float          chi2;
    };
  public:
    bool fit(const float* waveform, int spantime);

    float get_first_time() {return GetTime();}
    float get_total_charge() {return GetCharge();}
  public:
    FadcWaveform(const std::string& name);
   ~FadcWaveform(){
     //m_fit->Delete(); 
     //delete m_fit;
     //m_graph.Delete();
    }
    TGraphErrors DrawWaveform(){return m_graph;}
    TGraph       DrawFit()     {
      if(m_fit.GetN()==0) m_fit.SetPoint(0,0,0);
      return m_fit;
    }
    TGraph       DrawRois();
    TGraph       DrawPeaks();
    TGraph       DrawPulses(float scale = 1.);
    TGraph       DrawRaw   (float scale = 1.){return m_raw;}
    TGraph       DrawDec   (float scale = 1.){return m_dec;}
    bool         IsSaturated(){return m_isSaturated;}
    void Analyze();
    void PreScan();
    void Deconvolve();
    void SetGain(float fadcGain){m_fadcGain = fadcGain;}
    int IsOsFixed(){return m_fixOs;}
    double GetOS() {
      if(m_fitPar.size()==0) return -1;
      return m_fitPar[0];
    }
    float GetChi2() {
      if(m_fitPar.size()==0) return -1;
      return m_chi2;
    }
    float GetCharge() {
      int nPulses = m_pulses.size();
      if (nPulses==0) {cout<<" !!! "<<"size 0"<<endl;return 0;}
      float charge = 0;
      Pulses::iterator pulseIt = m_pulses.begin();
      for(; pulseIt!=m_pulses.end();pulseIt++)  {
        if(pulseIt->first>200){
          charge += pulseIt->second;
          //cout << "----> " << pulseIt->first << endl;
        }
      }
      return charge/m_fadcGain;
    }


float GetTime() {
        int nPulses = m_pulses.size();
	if (nPulses==0) return 0;
	float time = 0;
	Pulses::iterator pulseIt = m_pulses.begin();
	return pulseIt->first;
}

    float GetPedestal(){return m_pedestal;}
    void GetPulses(float* time,float* charge,int& nPulse){
      Pulses::iterator pulseIt = m_pulses.begin();
      int count = 0;
      for(; pulseIt!=m_pulses.end();pulseIt++)  {
        if(count>49) break;
        time  [count] = pulseIt->first;
        charge[count] = pulseIt->second/m_fadcGain;
        count++;
      }
      nPulse = count;
    }

  private:
    void clear();
  private:
    static const unsigned int m_maxSize = 2000;
    int                m_nSamples;
    std::vector<float> m_rawWaveform;
    std::vector<float> m_waveform;
    float              m_pedestal;
    float              m_pedError;
    float              m_pedChi2;
    int                m_preWindow;
    int                m_bits;
    int                m_adcMax;
    float              m_fadcGain;
    int                m_minGap;
    int                m_isSaturated;
    int                m_fixOs;
    float              m_threshold;
    TGraphErrors       m_graph;
    TGraph             m_fit;
    TGraph             m_raw;
    TGraph             m_dec;

    std::vector<Roi>   m_rois;
    Peaks              m_peaks;
    Pulses             m_pulses;
    float              m_charge;
    Peaks  FindPeaks(Roi& roi,int itrnPf=5,int itrnDc=1000);
    Pulses FitPeaks (Roi& roi, Peaks peaks);

    vector<double> m_fitPar;
    float          m_chi2;
};

#endif
