#ifndef FadcFit_h
#define FadcFit_h



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





using namespace std;

class FadcFit {
    typedef std::map<float,float> Peaks;
    typedef std::map<float,float> Pulses;
  public:
    FadcFit            (Peaks peaks,int nFix=0 );
    FadcFit            (int nPulseMax = 1      );
   ~FadcFit(){
     m_fit->Delete();
     //delete m_parameters;
     //delete m_peakTemplate;
    }
    void Reset         (int nPulseMax = 1      );
    float Fit          (TGraphErrors graph     );
    void  SetParameters(Peaks peaks,int fixN=0 );
    void  SetParameter (int i,float val){m_fit->SetParameter(i,val);}
    void  FixParameters(std::vector<double> par)
    {
      for(unsigned int i=0;i<par.size();i++)  
        m_fit->FixParameter(i,par[i]);
    }
    Pulses  GetPulses    () {return m_pulses;      }
    double* GetParameters() {return m_parameters;  }
    double*  GetTemplate  () {
      /// generate peak template
      for(unsigned int i=0; i<m_nSamples; i++)  {
        double x = double(i);
        m_peakTemplate[i] = m_fit->Eval(x);
      }
      return m_peakTemplate;
    }
    TGraph  DrawFit      ();
    TGraph  DrawPulses   ();
    float   GetChargeAna ();
    float   GetChargeInt ();
    //void    UnfixOS      () {m_fit->SetParLimits(0,0.008,0.01);}
    void    UnfixOS      () {m_fit->SetParLimits(0,0.005,0.2);}
  private:
    static       unsigned int s_count;
    static const unsigned int m_nSamples      = 2000;
    static const unsigned int s_nParOvershoot = 1;
    static const unsigned int s_nParPeak      = 3;
    
    TF1*         m_fit;
    unsigned int m_nPulseMax;
    unsigned int m_nPar;
    double*      m_parameters;
    double       m_peakTemplate[m_nSamples];
    Peaks        m_peaks;
    Pulses       m_pulses;
    
    float GetPulseExp (double* x, double* par);
    float GetOvershoot(double* x, double* par);
    double MultiPulse (double* x, double* par);
};

#endif
