#ifndef Deconvolution_h
#define Deconvolution_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include <map>
#include "TH1D.h"
#include "TString.h"
#include "TTree.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <mutex>
#include "TStopwatch.h"
#include "TVirtualFFT.h"

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>
#include <CondDB/ICnvFromPayload.h>
#include <CondObj/CommonCondObj.h>

#ifdef PERFORMANCE_CHECK
#include "JunoTimer/IJunoTimerSvc.h"
#include "JunoTimer/JunoTimer.h"
#include "TSystem.h"
#endif

namespace JM {
class ElecWaveform;
}

class Deconvolution : public AlgBase {
  public:
  Deconvolution(const std::string& name);
  ~Deconvolution();

  public:
  bool initialize();
  bool execute();
  bool finalize();

  private:
  std::vector<bool> hmmtpmt;
  
  bool calibrate(int pmtId);
  double getInterpolateValue(double detlta_x, double y0, double y1);
  double subBSL_3TW();
  
  static std::mutex m_mutex;

  private:
  std::vector<double> m_filter;
  std::vector<double> m_SPERE;
  std::vector<double> m_SPEIM;
  int m_totalLPMT;
  double m_threshold;
  double m_threshold_mcp;
  double m_threshold_dyn;
  double m_widthThreshold;
  double m_widthThreshold_mcp;
  double m_widthThreshold_dyn;
  double m_para1;
  double m_para2;
  double m_para3;
  double m_para4;
  double m_para5;
  double m_para6;
  int m_calib_start_time;
  int m_length;
  int m_calib_length;
  int m_dft_length;
  int m_bsl_length;
  bool isLongerWave;
  bool isShorterWave;
  double m_f_scale;
  int m_window;
  int m_window_mcp;
  int m_window_dyn;
  int m_hc;
  int m_c;
  int N_filter;
  int N_freq;

  // user data definitions
  std::vector<float> m_charge;
  std::vector<float> m_time;
  std::vector<int> m_SimplePmtId;
  // std::vector<int> m_width;
  // std::vector<int> m_peak;
  std::vector<double> gain;     // gain correction from calib data
  std::vector<double> toffset;  // gain correction from calib data
  
  float m_totalpe;
  int m_trigtimesec;
  int m_trigtimenanosec;
  int save_features;
  double f_threshold;

  int debug;
  TTree* m_calib;
  TTree* m_feature;
  TTree* m_debug;

  int m_maxhits;
  int m_maxProfileN;
  int m_maxBSLN;

  std::unique_ptr<TH1D> debugWave;
  std::unique_ptr<TVirtualFFT> fft_forward;
  std::unique_ptr<TVirtualFFT> fft_back;
  std::unique_ptr<TH1D> hReco;

  std::vector<float> charge;
  std::vector<float> time;
  // std::vector<int> width;
  // std::vector<int> peak;

  std::vector<double> wf;
  std::vector<double> wfre;
  std::vector<double> wfim;
  std::vector<double> wfback;
  std::vector<int> adc_profile;
  std::vector<double> bsl_sums;
  std::vector<int> b_bsl;
  std::vector<int> HitBTime;
  std::vector<int> HitETime;

  struct ML_features{
    std::vector<int> pmtid_v;
    std::vector<double> npe_v;
    std::vector<int> fhtime_v;
    std::vector<int> fptime_v;
    std::vector<double> npemax_v;
    std::vector<int> timemax_v;
    std::vector<int> timemaxR_v;
    std::vector<double> slope_v;
    std::vector<double> slope4_v;
    std::vector<double> nperatio5_v;
    std::vector<double> nperatio4_v;
    std::vector<double> timeMedian_v;
    std::vector<double> timeMean_v;
    std::vector<double> timeStd_v;
    std::vector<double> timeSkewness_v;
    std::vector<double> timeKurtosis_v;

    void clear(){
      pmtid_v.clear();
      npe_v.clear();
      fhtime_v.clear();
      fptime_v.clear();
      npemax_v.clear();
      timemax_v.clear();
      timemaxR_v.clear();
      slope_v.clear();
      slope4_v.clear();
      nperatio5_v.clear();
      nperatio4_v.clear();
      timeMedian_v.clear();
      timeMean_v.clear();
      timeStd_v.clear();
      timeSkewness_v.clear();
      timeKurtosis_v.clear();
    }
  }ML_feature;

 	
#ifdef PERFORMANCE_CHECK
    // for resources test
    IJunoTimerSvc* m_junotimersvc;
    JunoTimerPtr m_timerevent;  
    int N_mem;
    float mem_resident[2];//memory usage of the initialize and execute functions            
    float time_total;//time usage of the execute function     
#endif 	

};

#endif

