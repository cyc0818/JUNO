//////////////////////////////////
/*
Algorithm for PMT waveform reconstruction, with the deconvolution method.
v1, Zeyuan Yu.
Author: yuzy@ihep.ac.cn
July 12, 2016
*/
/*
v2. Add a simple hit counting algorithm on the deconvolution results.
If a hit has a charge integral less than 1.5 p.e., and it is far away from other
other hit, its charge is set to 1 p.e.
Energy resolution is expected to be improved by at least 0.1% at 1MeV.
By Zeyuan Yu.
Oct. 18, 2016
*/
/*
v3. Update for the J17v1:
    1) Modify according to the FADC configurations and ouput format
By Zeyuan Yu.
Apr. 6, 2017
*/
/*
v4. Input PMT calibration parameters
By Xuantong Zhang.
zhangxuantong@ihep.ac.cn
Sept. 29, 2019
*/
/*
v5. Add hits discrimination
By Xuantong Zhang.
zhangxuantong@ihep.ac.cn
Mar. 24, 2020
*/
//////////////////////////////////
#ifndef Deconvolution_cc
#define Deconvolution_cc
#include "Deconvolution.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Task.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "Identifier/CdID.h"
#include <TFile.h>
#include "TROOT.h"
#include <iostream>
#include <TTree.h>
#include <TSystem.h>
#include <sstream>
#include "math.h"
#include "Geometry/PMTParamSvc.h"

#include "PMTCalibSvc/PMTCalibSvc.h"
using namespace std;

DECLARE_ALGORITHM(Deconvolution);
std::mutex Deconvolution::m_mutex;

Deconvolution::Deconvolution(const string &name) : AlgBase(name), m_totalLPMT(17612), m_threshold_mcp(0.030), m_threshold_dyn(0.025), m_widthThreshold_mcp(12), m_widthThreshold_dyn(12), m_para1(110.), m_para2(160.), m_para3(15.), m_para4(80.), m_para5(130.), m_para6(15.), m_calib_start_time(300), m_calib_length(1000), m_window_mcp(7), m_window_dyn(8), m_hc(0), m_c(0), save_features(0), f_threshold(0.25), debug(0), m_calib(nullptr), m_feature(nullptr), m_debug(nullptr), debugWave(nullptr), fft_forward(nullptr), fft_back(nullptr), hReco(nullptr)
{
  m_maxBSLN = 3;
  m_maxhits = 99;
  m_maxProfileN = 100;
  m_bsl_length = 50;

  declProp("TotalPMT", m_totalLPMT);
  declProp("Threshold1", m_threshold_mcp);
  declProp("Threshold2", m_threshold_dyn);
  declProp("Para1", m_para1);
  declProp("Para2", m_para2);
  declProp("Para3", m_para3);
  declProp("Para4", m_para4);
  declProp("Para5", m_para5);
  declProp("Para6", m_para6);
  declProp("Calib_Length", m_calib_length);
  declProp("Calib_Start_Time", m_calib_start_time);
  declProp("Window1", m_window_mcp);
  declProp("Window2", m_window_dyn);
  declProp("WidthThreshold1", m_widthThreshold_mcp);
  declProp("WidthThreshold2", m_widthThreshold_dyn);
  declProp("HitCounting", m_hc);
  declProp("Calib", m_c);
  declProp("DebugMode", debug);
  declProp("SaveFeatures", save_features);
  declProp("ReadFeaturesThreshold", f_threshold);
}

Deconvolution::~Deconvolution()
{
}

bool Deconvolution::initialize()
{

  // prepare PMT gain calib data
  SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
  if (calSvc.invalid()) {
    LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
    return false;
  }
  if (m_c == 0) {
    gain = calSvc->getGain();
    toffset = calSvc->getTimeOffset();
    LogInfo << "Load gain and timeoffset correction successfully" << std::endl;
  }

  m_filter = calSvc->getLPMTfilter();
  N_filter = int(m_filter.size()/2);
  for(int i=0;i<4;i++) LogInfo << "Read filter bin " << i+1 << " nnvt/hmmt value: " << m_filter[i] << "/" << m_filter[N_filter+i] << std::endl;
  if(int(m_filter.size()%2)!=0){
    LogError << " Filters size wrong! " << m_filter.size() << std::endl;
    return false;
  }

  m_SPERE = calSvc->getLPMTre();
  m_SPEIM = calSvc->getLPMTim();
  N_freq  = int(m_SPERE.size())/m_totalLPMT;
  LogInfo << "SPE freq NbinX: " << N_freq << std::endl;
  for(int i=1;i<4;i++) LogInfo << "Read SPE freq of PMT3, bin " << i+1 << " re/im value: " << m_SPERE[3*N_freq+i] << "/" << m_SPEIM[3*N_freq+i] << std::endl;
  if(int(m_SPERE.size()%m_totalLPMT)!=0 || int(m_SPEIM.size()%m_totalLPMT)!=0){
    LogError << " SPE frequency size wrong! " << m_SPERE.size() << " | " << m_SPEIM.size() << std::endl;
    return false;
  }

  if (m_SPERE.size() == 0) {
    LogError << "NO PMT SPE WAVEFORM!" << endl;
    return false;
  }

  // get hmmt pmt id
  hmmtpmt.clear();

  SniperPtr<PMTParamSvc> pmtParamSvc(getParent(), "PMTParamSvc");
  if (pmtParamSvc.invalid()) {
    LogError << "Failed to get PMTParamSvc instance!" << std::endl;
    return false;
  }

  for (int i = 0; i < m_totalLPMT; i++) {
    bool isHama = pmtParamSvc->isHamamatsu(i);
    hmmtpmt.push_back(isHama);
  }

#ifndef BUILD_ONLINE
#ifdef PERFORMANCE_CHECK 
   //timer service
   SniperPtr<IJunoTimerSvc>  junotimersvc(getParent(), "JunoTimerSvc");
   m_junotimersvc = junotimersvc.data();
   m_timerevent=m_junotimersvc->get("CALIBEVT/timerevt");
#endif

  // user data definitions
  SniperPtr<RootWriter> svc(getParent(), "RootWriter");
  if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return false;
  }
  gROOT->ProcessLine("#include <vector>");

  m_calib = svc->bookTree(*m_par, "USER_OUTPUT/calibevt", "simple output of calibration data");
  m_calib->Branch("Charge", &m_charge);
  m_calib->Branch("Time", &m_time);
  m_calib->Branch("PMTID", &m_SimplePmtId);
  m_calib->Branch("TotalPE", &m_totalpe, "TotalPE/F");
  // m_calib->Branch("Width", &m_width);
  // m_calib->Branch("Peak", &m_peak);
  m_calib->Branch("TrigTimeSec", &m_trigtimesec, "TrigTimeSec/I");
  m_calib->Branch("TrigTimeNanoSec", &m_trigtimenanosec, "TrigTimeNanoSec/I");

#ifdef PERFORMANCE_CHECK
  m_calib->Branch("time_total",&time_total,"time_total/F");
  N_mem = 2;   
  m_calib->Branch("N_mem", &N_mem, "N_mem/I");   
  m_calib->Branch("mem_resident",mem_resident,"mem_resident[N_mem]/F"); 
#endif    

  if(save_features != 0){
    m_feature= svc->bookTree(*m_par, "USER_OUTPUT/features", "features of calibration data");
    m_feature->Branch("pmtid", &ML_feature.pmtid_v);
    m_feature->Branch("npe", &ML_feature.npe_v);
    m_feature->Branch("fht", &ML_feature.fhtime_v);
    m_feature->Branch("fpt", &ML_feature.fptime_v);
    m_feature->Branch("npemax", &ML_feature.npemax_v);
    m_feature->Branch("timemax", &ML_feature.timemax_v);
    m_feature->Branch("peaktime", &ML_feature.timemaxR_v);
    m_feature->Branch("slope", &ML_feature.slope_v);
    m_feature->Branch("slope4", &ML_feature.slope4_v);
    m_feature->Branch("nperatio5", &ML_feature.nperatio5_v);
    m_feature->Branch("nperatio4", &ML_feature.nperatio4_v);
    m_feature->Branch("vTimeMedian", &ML_feature.timeMedian_v);    
    m_feature->Branch("vTimeMean", &ML_feature.timeMean_v);    
    m_feature->Branch("vTimeStd", &ML_feature.timeStd_v);            
    m_feature->Branch("vTimeSkewness", &ML_feature.timeSkewness_v);        
    m_feature->Branch("vTimeKurtosis", &ML_feature.timeKurtosis_v);
    hReco = std::unique_ptr<TH1D>(new TH1D("", "", m_calib_length, 0, m_calib_length));
  }
#endif

  wf.resize(m_length);
  bsl_sums.resize(m_maxBSLN);
  charge.reserve(m_maxhits);
  time.reserve(m_maxhits);
  // peak.reserve(m_maxhits);
  // width.reserve(m_maxhits);

  LogInfo << "Initialized successfully" << endl;
  return true;
}

bool Deconvolution::execute()
{
#ifndef BUILD_ONLINE
#ifdef PERFORMANCE_CHECK
  // for resources test
  ProcInfo_t mem_info2;
  gSystem->GetProcInfo(&mem_info2);
  mem_resident[0] = mem_info2.fMemResident / 1024./1024.;
  m_timerevent->start();
#endif
  // preparation of the user output
  m_charge.clear();
  m_time.clear();
  m_SimplePmtId.clear();
  // m_width.clear();
  // m_peak.clear();
  if(save_features != 0) ML_feature.clear();
#endif
  LogInfo << "Start PMT waveform unfolding" << endl;
  m_totalpe = 0;
  // read the electronics event
  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if (navBuf.invalid()) {
    LogWarn << "can't find the buffer. " << std::endl;
    return false;
  }
  auto jstat = dynamic_cast<Task*>(getRoot())->Snoopy().state();
  if (jstat != Sniper::RunState::Running && jstat != Sniper::RunState::Ready) {
    LogWarn << "Stop running." << std::endl;
    return true;
  }
  auto nav = navBuf->curEvt();
  // LogInfo << "EvtNav: " << nav << std::endl;
  if (!nav) {
    LogWarn << "can't load event navigator." << std::endl;
    return dynamic_cast<Task*>(getRoot())->stop();
  }
  auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
  if(eh==NULL) return true;
  // only use large pmts
  if (!eh->hasEvent()) return true;
  auto ee = (eh->event());
  const auto &feeChannels = ee->channelData();

  list<JM::CalibPmtChannel *> cpcl;  // CalibPMTChannel list

  // map<int, vector<double>>::iterator spe = m_SPERE.begin();

  for (auto it = feeChannels.begin(); it != feeChannels.end(); ++it) {

    const auto &channel = *(it->second);
    if (channel.adc().size() == 0) {
      continue;
    }
    if(!fft_forward.get()){
      m_length = channel.adc().size();
      isLongerWave = m_length > m_calib_length;
      isShorterWave= m_length < m_calib_length;
      m_dft_length = isLongerWave ? m_length : m_calib_length;
      m_f_scale = m_calib_length*1./(m_length*1.);
      {
        std::unique_lock<std::mutex> lock(m_mutex);
        fft_forward = std::unique_ptr<TVirtualFFT>(TVirtualFFT::FFT(1, &m_dft_length, "R2C EX K"));
        fft_back    = std::unique_ptr<TVirtualFFT>(TVirtualFFT::FFT(1, &m_dft_length, "C2R EX K"));
      }

      wf  .resize(m_dft_length);
      wfre.resize(m_dft_length);
      wfim.resize(m_dft_length);
      wfback.resize(m_dft_length);
      HitBTime.resize(m_maxhits);
      HitETime.resize(m_maxhits);
      adc_profile.resize(m_maxProfileN); // 100 is enough
      b_bsl.resize(m_dft_length);

      if (debug == 1) {
        debugWave = std::unique_ptr<TH1D>(new TH1D("", "debugWave", m_calib_length, 0, m_calib_length));
        SniperPtr<RootWriter> svc(getParent(), "RootWriter");
        m_debug = svc->bookTree(*m_par, "USER_OUTPUT/debug", "debug waves");
        // m_debug->Branch("BugWaves", "TH1D", &debugWave);
        m_debug->Branch("BugWaves", "TH1D", debugWave.get());
      }

      LogInfo << "Length of the electronics simulation data is " << channel.adc().size() << " ns" << endl;
      LogInfo << "Length of the calibration length of deconvolution is " << m_calib_length << " ns" << endl;
    }
    if (int(channel.adc().size()) != m_length) {
      LogError << "Length of the electronics simulation data is " << channel.adc().size() << " ns" << endl;
      LogError << "Length of the pre-defined length of deconvolution is " << m_length << " ns" << endl;
      LogError << "ERROR: inconsistent waveform length in the unfolding." << endl;
      return false;
    }
    int pmtID = it->first;  // remeber to check the conversion from electronics id to pmt id
    if (pmtID >= m_totalLPMT) continue;
    const auto &adc_int = channel.adc();
    if (adc_int.size() == 0) {
      return false;
    }
    double nPE = 0, firstHitTime = 0;

    charge.clear();
    time.clear();
    // width.clear();
    // peak.clear();

    if (hmmtpmt[pmtID] == true){
      m_threshold = m_threshold_dyn;
      m_widthThreshold = m_widthThreshold_dyn;
      m_window = m_window_dyn;
    }
    else{
      m_threshold = m_threshold_mcp;
      m_widthThreshold = m_widthThreshold_mcp;
      m_window = m_window_mcp;
    }

    for (int i = 0; i < m_length; i++) {
      double tmpadc=0.;
      if (adc_int.at(i) >> 14 == 0) tmpadc =  double(adc_int.at(i));
      if (adc_int.at(i) >> 14 == 1) tmpadc =( double(adc_int.at(i))-(1<<14) ) * .55/.08;
      if (tmpadc > 4e9) tmpadc = 0;  // Robust for unsigned int adc value becomes very large. If this bug is fixed, this part can be deleted.
      wf[i] = tmpadc;
    }
    if(isShorterWave){
      double bsl = subBSL_3TW();
      std::fill(wf.begin()+m_length, wf.end(), bsl);
    }

    bool StatusC = calibrate(pmtID);

    if (StatusC == false) {
      LogError << "ERROR when unfolding the waveform." << endl;
      return false;
    }

    if (charge.size() == 0) continue;  // Does not pass the threshold. Do not save.

    unsigned int detID = CdID::id(static_cast<unsigned int>(pmtID), 0);
    // Identifier id = Identifier(pmtID);
    for (int j = 0; j < int(charge.size()); j++) {
      // nPE += charge[j];
      // PMT calib
      if (m_c == 0) {
        charge[j] /= gain.at(pmtID);
        time[j] -= toffset.at(pmtID);
      }

      nPE += charge[j];
#ifndef BUILD_ONLINE
      m_charge.push_back(charge[j]);
      m_time.push_back(time[j]);
      m_SimplePmtId.push_back(pmtID);
      // m_width.push_back(width[j]);
      // m_peak.push_back(peak[j]);
#endif
    }

    m_totalpe += nPE;

    if (charge.size() > 0) firstHitTime = time[0];
    auto cpc = new JM::CalibPmtChannel;
    cpc->setNPE(nPE);
    cpc->setPmtId(detID);
    cpc->setFirstHitTime(firstHitTime);
    cpc->setTime(time);
    cpc->setCharge(charge);
    cpcl.push_back(cpc);
  }
  // TODO: the TriggerTime is not in crate any more, need to update.
  // Tao Lin, 2022.03.08
  TimeStamp trigTimeStamp; // = m_crate->TriggerTime();
  m_trigtimesec = (int)trigTimeStamp.GetSec();
  m_trigtimenanosec = (int)trigTimeStamp.GetNanoSec();

  //m_calib->Fill();
  auto ce = new JM::CdLpmtCalibEvt;
  ce->setCalibPMTCol(cpcl);
  auto ch = new JM::CdLpmtCalibHeader;
  ch->setEvent(ce);

  JM::addHeaderObject(nav, ch);

  // LogDebug << "End of the PMT Waveform Unfolding" << endl;
  ProcInfo_t info;
  gSystem->GetProcInfo(&info);
  double m_memory = info.fMemResident / 1024;
  LogInfo << "Memory cost: " << m_memory << endl;
  LogInfo << "End of the PMT Waveform Unfolding" << endl;
#ifndef BUILD_ONLINE
#ifdef PERFORMANCE_CHECK  
  // for resources test
  ProcInfo_t mem_info3;
  gSystem->GetProcInfo(&mem_info3);
  mem_resident[1] = mem_info3.fMemResident / 1024./1024.;
  m_timerevent->stop();
  time_total=m_timerevent->elapsed()/1000.;
  LogInfo << "Time consumes: " << time_total << " s"<<endl;  
#endif
  m_calib->Fill();
  if (save_features != 0){
    m_feature->Fill();
  }
#endif
      
  return true;
}

double Deconvolution::getInterpolateValue(double detlta_x, double y0, double y1)
{
  return y0 + (y1-y0)*detlta_x;
}

bool Deconvolution::finalize()
{
  fft_back.reset();
  fft_forward.reset();
  debugWave.reset();
  hReco.reset();
  return true;
}

bool Deconvolution::calibrate(int pmtId)
{ 
  if ( !fft_back || !fft_forward )
  {
      LogError << "Can't create fft plan properly!!" << endl;
      return false;
  }
    
  const int N = m_dft_length;

 // cout<<"check fft_forward in calib"<<endl;
  fft_forward->SetPoints(wf.data());
  fft_forward->Transform();
  fft_forward->GetPointsComplex(wfre.data(), wfim.data());

  auto pmt_filter = &m_filter[hmmtpmt[pmtId]*N_filter];
  for (int i = 0; i < N_filter; i++) {
    if(!isLongerWave){
      wfre[i] *= pmt_filter[i];
      wfim[i] *= pmt_filter[i];
    }else{
      double tmp_x = i*m_f_scale;
      int x0 = int(tmp_x);
      wfre[i] *= getInterpolateValue(tmp_x-1.*x0,pmt_filter[x0],pmt_filter[x0+1]);
      wfim[i] *= getInterpolateValue(tmp_x-1.*x0,pmt_filter[x0],pmt_filter[x0+1]);
    }
  }

  // division in frequency domain
  auto spe_re = &m_SPERE[N_freq*pmtId];
  auto spe_im = &m_SPEIM[N_freq*pmtId];
  for (int i = 1; i < N_freq; i++) {
    double c11, d11;
    if(!isLongerWave){
      c11 = spe_re[i];
      d11 = spe_im[i];
    }else{
      double tmp_x = i*m_f_scale;
      int x0 = int(tmp_x);
      c11 = getInterpolateValue(tmp_x-1.*x0,spe_re[x0],spe_re[x0+1]);
      d11 = getInterpolateValue(tmp_x-1.*x0,spe_im[x0],spe_im[x0+1]);
    }
    if (c11 != 0 || d11 != 0) {
      double a11 = wfre[i], b11 = wfim[i];
      double f11 = c11 * c11 + d11 * d11;
      wfre[i] = (a11 * c11 + b11 * d11) / f11;
      wfim[i] = (b11 * c11 - a11 * d11) / f11;
    } else {
      wfre[i] = 0;
      wfim[i] = 0;
    }
  }
  for (int i = N_freq; i < N; i++) {
    wfre[i] = 0;
    wfim[i] = 0;
  }
  wfre[0] = 0;
  wfim[0] = 0;

 // cout<<"check fft_back in calib"<<endl;
  fft_back->SetPointsComplex(wfre.data(), wfim.data());
  fft_back->Transform();
  fft_back->GetPoints(wfback.data());

  // integral
  // time shift 100ns, because phase shifts in deconvolution, shifting time is based on average waveform starting time
  for(int i=m_calib_start_time;i<N;i++) wf[i] = wfback[i  -m_calib_start_time]/N;
  for(int i=0;i<m_calib_start_time;i++) wf[i] = wfback[i+N-m_calib_start_time]/N;

  // rough baseline
  double wf_min = *min_element(wf.begin(), wf.end());
  double wf_max = *max_element(wf.begin(), wf.end());
  const double bslUnder0_cut = 1.;
  double tmp_scale = double(m_maxProfileN);
  if(wf_max-wf_min>bslUnder0_cut){
      // diff > 1 is enough to identify big pulse
      wf_max = 0.5;
      tmp_scale = bslUnder0_cut*m_maxProfileN/(wf_max-wf_min);
  }
  const int imin = int(wf_min*tmp_scale);
  std::fill(adc_profile.begin(), adc_profile.end(), 0);
  for(int i=0;i<N;i++){
      int tmpidx = int(wf[i]*tmp_scale)-imin;
      if(tmpidx<0 || tmpidx>=m_maxProfileN) continue;
      ++adc_profile[tmpidx];
  }
  double baseline = (max_element(adc_profile.begin(), adc_profile.end())-adc_profile.begin()+imin) / tmp_scale;
  for(int i=0;i<N;i++) wf[i] -= baseline;

  // rough hits
  int HitCount = 0;
  int Pass = 0;
  for (int i = 0; i < N; i++) {
    if (Pass == 0 && wf[i] > m_threshold) {
      for (int j = i; j >= 0; j--) {
        if (wf[j] <= 0 || j == 0){
          HitBTime[HitCount] = j;
          break;
        }
      }
      Pass = 1;
      continue;
    }
    if (Pass == 1) {
      if (wf[i] <= 0 || i == N - 1) {
        HitETime[HitCount] = i;
        if (HitETime[HitCount]-HitBTime[HitCount]+1 > m_widthThreshold) ++HitCount;  // 12 ns width for J19
	      Pass = 0;
      }
    }
    if (HitCount >= m_maxhits) {
      LogWarn << "Too much hits recognized! Move to next waveform." << std::endl;
      break;  // if it really happend, it should print out warning and move to next waveform.
    }
  }

  // accurate baseline
  int counter = 0;
  baseline = 0;
  std::fill(b_bsl.begin(), b_bsl.end(), 1);
  for(int j=0;j<HitCount;j++){
    int bL = HitBTime[j] - 9;
    int bR = HitETime[j] + 9;
    if(bL<0)   bL = 0;
    if(bR>N-1) bR = N-1;
    counter += bR-bL+1;
    for(int i=bL;i<=bR;i++) b_bsl[i] = 0;
  }
  counter = N-counter;
  for (int i = 0; i < N; i++) baseline += wf[i]*b_bsl[i];
  if (counter < 40) {
    LogWarn << "No enough points for baseline calculation, baseline will set to be the default one." << std::endl;  // make sure to have enough points to calculate baseline.
    if (debug == 1) {
      for (int i = 0; i < N; i++) debugWave->SetBinContent(i + 1, wf[i]);
      TString debugTitle = Form("Error: No enough baseline points, Counted points = %d", counter);
      debugWave->SetTitle(debugTitle);
      m_debug->Fill();
    }
  } else if (counter != 0) {
    baseline /= (counter + 0.);
    for (int i = 0; i < N; i++) wf[i] -= baseline;
  }

#ifndef BUILD_ONLINE
  if(save_features != 0){
    double npetot = 0;
    double npe = 0;
    double nperatio5 = 0;
    double nperatio4 = 0;
    int fht = -1;
    int fpt = -1;    
    int timemax = 0;
    int timemaxR = 0;
    double npemax = 0;
    double slope = 0;
    double slope4 = 0;
    double timeMedian = 0;
    double timeMean = 0;
    double timeStd = 0;
    double timeSkewness = 0;        
    double timeKurtosis = 0;   
    //extract features
    auto hReco_arr = hReco->GetArray();
    for (int i = 0; i < N; i++) {
      if (wf[i]>0) hReco_arr[i + 1] = wf[i];
      else hReco_arr[i + 1] = 0;
    }
    hReco->ResetStats();
    double rec_baseline = hReco->Integral(13, 40 + 13) / 40.0;
    for(int j = 13; j < N; ++j){
      if(hReco_arr[j + 1] > rec_baseline + f_threshold){     
        if(hReco_arr[j + 1] > hReco_arr[j]) {
          fpt = j;
        }
        else break;
      } 
    }//end loop over bins
    //FHT:  CFD method https://jupyter.ihep.ac.cn/vYhy8phkTQGaZmNxwM2ujg
    for(int j = fpt; j > 0; j--){
      if((hReco_arr[j + 1] - rec_baseline) < 0.2 * (hReco_arr[fpt + 2] - rec_baseline)){     
        fht = j;  
        break;	                    
      }
    }//end loop over bins	        
    int fhtime = fht;
    int fptime = fpt;
    ////if (fht>=0 && hittime_min>fht)  hittime_min=fht;
    for(int j = 1; j < N; j++){	        	        
      if(hReco_arr[j + 1] > npemax ){
        npemax = hReco_arr[j + 1];
        timemax = j - fht;
        timemaxR = j;
      } 
    }//end loop over bins
  
    if(timemax>0) { 
      slope = npemax / timemax; 
      if (fht > 0) slope4 = (hReco_arr[fht + 5]-hReco_arr[fht + 1]) / 4;
      if (slope4 < 0) slope4 = 0;
    }
    npetot = hReco->Integral();
    if(npetot > 0){
      npe = npetot;
      if( fht >= 0 && (fht - 4 < N) ) nperatio5 = hReco->Integral(hReco->FindBin(fht), hReco->FindBin(fht) + 4)/npetot; //each bin is 1 ns
      if( fht >= 0 && (fht - 3 < N) ) nperatio4 = hReco->Integral(hReco->FindBin(fht), hReco->FindBin(fht) + 3)/npetot;
      
      timeMean = hReco->GetMean(); 
      timeStd = hReco->GetStdDev();  
      if(timeStd){
        double quantile = 0.5;
        hReco->GetQuantiles(1, &timeMedian, &quantile);                
        timeSkewness = hReco->GetSkewness(1); 
        timeKurtosis = hReco->GetKurtosis(1);       
      }
    }
    ML_feature.pmtid_v.push_back(pmtId);
    ML_feature.npe_v.push_back(npe);
    ML_feature.fhtime_v.push_back(fhtime);
    ML_feature.fptime_v.push_back(fptime);
    ML_feature.npemax_v.push_back(npemax);
    ML_feature.timemax_v.push_back(timemax);
    ML_feature.timemaxR_v.push_back(timemaxR);
    ML_feature.slope_v.push_back(slope);
    ML_feature.slope4_v.push_back(slope4);
    ML_feature.nperatio5_v.push_back(nperatio5);
    ML_feature.nperatio4_v.push_back(nperatio4);
    ML_feature.timeMedian_v.push_back(timeMedian);
    ML_feature.timeMean_v.push_back(timeMean);
    ML_feature.timeStd_v.push_back(timeStd);
    ML_feature.timeSkewness_v.push_back(timeSkewness);
    ML_feature.timeKurtosis_v.push_back(timeKurtosis);
  }
#endif

  // accurate hits
  HitCount = 0;
  Pass = 0;
  for (int i = 0; i < N; i++) {
    if (Pass == 0 && wf[i] > m_threshold) {
      for (int j = i; j >= 0; j--) {
        if (wf[j] <= 0 || j == 0){
          HitBTime[HitCount] = j;
          break;
        }
      }
      Pass = 1;
      continue;
    }
    if (Pass == 1) {
      if (wf[i] <= 0 || i == N - 1) {
        HitETime[HitCount] = i;
        if (HitETime[HitCount]-HitBTime[HitCount]+1 > m_widthThreshold) ++HitCount;  // 12 ns width for J19
        Pass = 0;
      }
    }
    if (HitCount >= m_maxhits) {
      LogWarn << "Too much hits recognized! Move to next waveform." << std::endl;
      if (debug == 1) {
        for (int k = 0; k < N; k++) debugWave->SetBinContent(k + 1, wf[k]);
        TString debugTitle = Form("Error: Too much hits! hits = %d", HitCount);
        debugWave->SetTitle(debugTitle);
        m_debug->Fill();
      }
      break;  // if it really happend, it should print out warning and move to next waveform.
    }
  }

  int inteW = m_window;
  if (HitCount == 0) return true;
  for (int i = 0; i < HitCount; i++) {
    time.push_back(HitBTime[i]);
    int inte_begin = 0, inte_end = 0;
    int isSingleHit = 0;
    if (HitCount == 1) {
      inte_begin = HitBTime[i] - inteW + 1;
      if (inte_begin < 0) inte_begin = 0;
      inte_end = HitETime[i] + inteW;
      if (inte_end > N) inte_end = N;
      isSingleHit = 1;
    } else {
      if (i == 0) {
        inte_begin = HitBTime[i] - inteW + 1;
        if (inte_begin < 0) inte_begin = 0;
        int interval = HitBTime[1] - HitETime[0];
        if (interval > 2 * inteW) {
          inte_end = HitETime[0] + inteW;
          isSingleHit = 1;
        } else
          inte_end = int(HitETime[0] + interval / 2.);
      } else if (i == HitCount - 1) {
        inte_end = HitETime[i] + inteW;
        if (inte_end > N) inte_end = N;
        int interval = HitBTime[i] - HitETime[i - 1];
        if (interval > 2 * inteW) {
          inte_begin = HitBTime[i] - inteW + 1;
          isSingleHit = 1;
        } else
          inte_begin = int(HitBTime[i] - interval / 2.);
      } else {
        int single1 = 0, single2 = 0;
        int interval = HitBTime[i + 1] - HitETime[i];
        if (interval > inteW * 2) {
          inte_end = HitETime[i] + inteW;
          single1 = 1;
        } else
          inte_end = int(HitETime[i] + interval / 2.);
        interval = HitBTime[i] - HitETime[i - 1];
        if (interval > 2 * inteW) {
          inte_begin = HitBTime[i] - inteW + 1;
          single2 = 1;
        } else
          inte_begin = int(HitBTime[i] - interval / 2.);
        if (single1 == 1 && single2 == 1) isSingleHit = 1;
      }
    }
    double tempC = 0;
    for (int j = inte_begin; j < inte_end; j++) tempC += wf[j];
    if (m_hc) {  // the hit counting method is opened
      if (isSingleHit) {
        if (tempC < 1.55) tempC = 1;  // A simple hit counting, to remove the PMT amplitude fluctuations.
                                      // else if(tempC<2.5) tempC = 2;
                                      // else if(tempC<3.5) tempC = 3;
      }
    }
    charge.push_back(tempC);
  }

  return true;
}

double Deconvolution::subBSL_3TW()
{
    // JUNO-doc-8875
    const int m_nbsl = 3;
    auto adc_begin = wf.begin();
    std::fill(bsl_sums.begin(), bsl_sums.end(), 0);
    for(int i=0;i<m_nbsl;i++) for(auto it=adc_begin+i*m_bsl_length; it!=adc_begin+(i+1)*m_bsl_length; ++it) bsl_sums[i] += *it;
    auto minbsl_pos = min_element(bsl_sums.begin(), bsl_sums.end());
    double baseline = (*minbsl_pos)/m_bsl_length;
    return baseline;
}
#endif

