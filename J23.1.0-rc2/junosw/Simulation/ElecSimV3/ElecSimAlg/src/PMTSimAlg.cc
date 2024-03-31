#include "PMTSimAlg.h"
#include <boost/python.hpp> 
#include <ios> 

#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperException.h"

#include "InputReviser/InputReviser.h"
#include "SniperKernel/Task.h"

#include "Event/SimHeader.h"

#include "EvtNavigator/EvtNavHelper.h"

#include <TTimeStamp.h>
#include "Context/TimeStamp.h"

#include <TRandom.h>
#include <TMath.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include "SPMTTool.h"
#include "TF1.h"

#include "Geometry/TTGeomSvc.h"
#include "Geometry/PMT.h"
#include "TTDigitization.h"

DECLARE_ALGORITHM(PMTSimAlg);

using namespace std;

PMTSimAlg::PMTSimAlg(const std::string& name)
    : AlgBase(name), m_elecsvc(0), m_pmtsimsvc(0), m_pmtsvc()
{
    // declare properties that can be configured in python script
    declProp("Seed", m_init_seed);
    declProp("RateMap", m_rateMap);
    declProp("TaskMap", m_taskMap);
    declProp("LoopMap", m_loopMap);
    declProp("StartIdxMap", m_startIndexMap);
    declProp("SaveTruthsMap",m_savetruthsMap);

    // please fix me, this information should be gotten from service!!!
    m_total_Lpmt = 17613;
    m_total_Spmt = 25600;
    m_total_WPLpmt =2400;

    declProp("TotalLpmt",m_total_Lpmt=17613);
    declProp("enableLPMT", m_enableLPMT=true);
    declProp("enableSPMT", m_enableSPMT=true);
    declProp("enableTT", m_enableTT=true);
    declProp("enableWP", m_enableWP=false);
    declProp("enableSNMode", m_enableSNMode=false);
    declProp("enableEfficiency", m_enableEfficiency=false);
    declProp("enableAfterPulse", m_enableAfterPulse=false);
    declProp("enableDarkPulse", m_enableDarkPulse=false);
    declProp("enablePmtTTS", m_enablePmtTTS=true);
    declProp("enableDynodeTTS2theta", m_enableDynodeTTS2theta=true);
    declProp("enableMCPTTS2theta", m_enableMCPTTS2theta=true);
    declProp("enableDynodeTT2theta", m_enableDynodeTT2theta=true);
    declProp("enableMCPTT2theta", m_enableMCPTT2theta=true);
    declProp("enableWPDarkPulse", m_enableWPDarkPulse=false);
    declProp("enableMergeLPMTPulse", m_enableMergeLPMTPulse=false);
    declProp("enableAssignGain", m_enableAssignGain=false);
    declProp("enableAssignDarkPulse", m_enableAssignDarkPulse=false);
    declProp("enableAssignSigmaGain", m_enableAssignSigmaGain=false);
    declProp("enableMCPLargeSignal", m_enableMCPLargeSignal=true);
    declProp("enableChargeSmearing", m_enableChargeSmearing=true);
    declProp("inputGain", m_Gain=1);
    declProp("inputSigmaGain", m_SigmaGain=0.3);
    declProp("darkRate", m_darkRate=30e3);
    declProp("darkRateScale", m_darkRateScale=1.);
    declProp("darkNoiseWindows", m_darkNoiseWindows=15e5);
    declProp("darkRate_WP", m_darkRate_WP=70e3);
    declProp("darkRateScale_WP", m_darkRateScale_WP=1.);
    declProp("darkRateScale_SPMT", m_darkRateScale_SPMT=1.);
    declProp("evtBufferLength", m_evtBufferLength=5000);
    declProp("nHitsThreshold", m_nHitsThreshold=200);
    declProp("doForceTrigger", m_doForceTrigger=false);
    declProp("doLaserTrigger", m_doLaserTrigger=false);
    declProp("enableMultiLaserLoad", m_enableMultiLaserLoad=false);
    declProp("nLaserEventLoad", m_nLaserEventLoad=1);
    declProp("enableSaveDarkPulseTruths", m_enableSaveDarkPulseTruths=true);
    declProp("enableSaveTruths", m_enableSaveTruths=true);

    // prepare pdf of after pulse
    m_afterPulsePdf.clear();
    m_afterPulseEdges.clear();

    for(int ii=0; ii < NUM_BINS_DIST; ii++) {
        m_afterPulsePdf.push_back(afterPusleTimingDist[ii]);
        m_afterPulseEdges.push_back(ii*100.+500.);
    }

    declProp("N_LIMIT_COUNTS", m_nlimit_counts=3);
    m_count_errs = 0;

    // it is true when no events loaded from input files, however, pulses may exist in pulse buffer.
    m_stoppingRun = false;
}

PMTSimAlg::~PMTSimAlg()
{

}

bool
PMTSimAlg::initialize()
{
    // to configure seed in PMT alg 
    LogInfo << "Initialize the SEED = " << m_init_seed <<std::endl;
    gRandom->SetSeed(m_init_seed);
 
    SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
    if (svc.invalid()) {
        LogError << "can't find service ElecSimSvc" << std::endl;
        return false;
    }
    m_elecsvc = svc.data();

    SniperPtr<IPMTSimParamSvc> pmtsimsvc(*getRoot(), "PMTSimParamSvc");
    if (pmtsimsvc.invalid()) {
        LogError << "can't find service PMTSimParamSvc" << std::endl;
        assert(0);
    }  else {
        LogInfo << "Retrieve PMTSimParamSvc Successfully. " << std::endl;
        m_pmtsimsvc = pmtsimsvc.data();
    }

    SniperPtr<IPMTParamSvc> pmtsvc(*getRoot(), "PMTParamSvc");
    if (pmtsvc.invalid()) {
        LogError << "can't find service PMTParamSvc" << std::endl;
        assert(0);
    }  else {
        LogInfo << "Retrieve PMTParamSvc Successfully. " << std::endl;
        m_pmtsvc = pmtsvc.data();
    }

    m_total_Lpmt = m_pmtsvc->get_NTotal_CD_LPMT();


    curEvtTimeStamp = m_elecsvc->get_start_time();
    nextEvtTime = m_elecsvc->get_start_time();
    nextSNEvtTime = m_elecsvc->get_start_time();
    nextBGKEvtTime = m_elecsvc->get_start_time();
    nextLaserEvtTime = m_elecsvc->get_start_time();
    m_elecsvc->setFirstPulseTimeInBuffer(curEvtTimeStamp);

    isSNEvt = true;
    isSNMixingWithBKG = false;
    isFirstTry = true;
    SN_aNav = nullptr;
    
    isLaserEvt = true;
    isLaserMixingWithBKG = false;

    gNNVTAmp = m_pmtsimsvc->get_MCPAmp();
    gTT_MCP = m_pmtsimsvc->get_MCPTT();

    // register pmt tool
    // here, we use the same pmt tool, with different configurations.
    m_pmt_tool[kPMT_CD_Lpmt] = tool<IPMTTool>("LPMTTool"); // -> PMTTool
    m_pmt_tool[kPMT_CD_Spmt] = tool<IPMTTool>("SPMTTool"); // -> PMTTool
    m_pmt_tool[kPMT_WP] =      tool<IPMTTool>("LPMTTool"); // -> PMTTool
    m_pmt_tool[kPMT_TT] =      tool<IPMTTool>("MaPMTTool"); // -> PMTTool
    // otherwise, we can use the same tool instead
    // m_pmt_tool[kPMT_CD_Lpmt] = tool<IPMTTool>("PMTTool"); // -> PMTTool
    // m_pmt_tool[kPMT_CD_Spmt] = tool<IPMTTool>("PMTTool"); // -> PMTTool

    // check whether hander exists
    for (std::map<PMTType, IPMTTool*>::iterator it = m_pmt_tool.begin();
         it != m_pmt_tool.end(); ++it) {
        if (!it->second) {
            LogError << "Can't find pmt tool for " << it->first << std::endl;
            return false;
        }
    }

    m_SN_sampleName = "";
    m_Laser_sampleName = "";

    m_totalRate = 0;
    for(map<string,double>::iterator it=m_rateMap.begin();it!=m_rateMap.end();it++){ //key is sample name ,value is the rate of sample
        m_firstMap.insert(std::pair<string,bool>(it->first,true));// I define this map just for find the begin entry of the sample
        //check weather SN is enabled, if yes, reset the rate of SN to be zero.
        if(m_enableSNMode) {
          if(strstr(it->first.c_str(), "SN") == NULL) { m_totalRate += it->second; isSNMixingWithBKG = true; } //first is sample name , second is rate 
          else {
            m_SN_sampleName = it->first;
            it->second = 0;
          }
        }
        else if (m_doLaserTrigger) {
          if(strstr(it->first.c_str(), "Laser") == NULL) { m_totalRate += it->second; isLaserMixingWithBKG = true; } //first is sample name , second is rate 
          else {
            m_Laser_sampleName = it->first;
            m_LaserRate = it->second;
          }
        }
        else m_totalRate+=it->second;  //first is sample name , second is rate 
    }
    LogInfo<<"event totalRate= "<< m_totalRate << endl;
    m_mainTau=1.0/m_totalRate;
    LogInfo<< "mainTau= " << m_mainTau <<endl;
    
    if(m_enableSNMode) {
      if(isSNMixingWithBKG) LogInfo<< "Going to mixing background events with SN." << std::endl;
      else LogInfo << "Going to simulate SN without background mixing" << std::endl;
    }
    
    LogInfo<< "m_doLaserTrigger = " << m_doLaserTrigger <<endl;
    if(m_doLaserTrigger) {
      if(isLaserMixingWithBKG) LogInfo<< "Going to mixing background events with Laser." << std::endl;
      else LogInfo << "Going to simulate Laser without background mixing" << std::endl;
    }
    if (m_enableMultiLaserLoad) LogInfo << m_nLaserEventLoad << " detsim Laser events will be loaded into one timing window." << std::endl;

    // prepare input reviser
    // m_taskMap["default"] = "default";
    //        ^- sample name      ^- task name

    Task* toptask = dynamic_cast<Task*>(getRoot());

    for(std::map<std::string,std::string>::iterator it=m_taskMap.begin();
        it!=m_taskMap.end();it++){
        m_loop = false; // false: don't reuse sample.

        map<string,int>::iterator i=m_loopMap.find(it->first);
        if (i!=m_loopMap.end()) {
            if (i->second==1) m_loop = true;else if(i->second==0) m_loop = false;else m_loop = true;
        }
        Task* task = dynamic_cast<Task*>(toptask->find(it->second));
        if (!task) {
            LogError << "can't find task " << it->second << std::endl;
            return false;
        }
        InputReviser* aInci = new InputReviser(*toptask, it->second, m_loop);  //first is task name 
        m_taskObjMap[it->first] = task;
        m_incidentMap.insert(make_pair(it->first, aInci));//first is sample name，second is the InputReviser which related to the sample name. In test I use sample as task name.
    }
    if(!m_enableSaveTruths)
    {
	m_enableSaveDarkPulseTruths = false;
	map<std::string, bool>::iterator it = m_savetruthsMap.begin();
	map<std::string, bool>::iterator it_end = m_savetruthsMap.end();
	for(;it != it_end;it++)
	{
		it->second = false;
	}
    }
	
    // interface to Spmt Configuration
    SniperPtr<SpmtElecConfigSvc> spmtSvc(*getRoot(), "SpmtElecConfigSvc");
    if (spmtSvc.invalid()) {
        LogError << "can't find service SpmtElecConfigSvc" << std::endl;
        return false;
    }
    m_SpmtConfigSvc = spmtSvc.data();
 
    SPMTTool* spmtTool = (SPMTTool *)m_pmt_tool[kPMT_CD_Spmt]; 
    spmtTool->setSpmtConfig(m_SpmtConfigSvc);

    m_do_subdet_sim = false;
    m_elecsvc->iniCDDrakPulseStartTime(m_pre_darkpulse);

    // Get optical crosstalk information for TT
    TTGeomSvc* tt_ct_svc = 0;
    SniperPtr<TTGeomSvc> svcct(getParent(), "TTGeomSvc");
    if (svcct.invalid()) {
      LogError << "Can't get TTGeomSvc. We can't initialize Cross Talk." << std::endl;
      assert(0);
    } else {
      LogInfo << "Retrieve TTGeomSvc successfully." << std::endl;
      tt_ct_svc = svcct.data();
    }
    
    for(int ii=0;ii<64;ii++)
      {
        std::vector<int> stripCT;
        for(int kk=0;kk<21;kk++)
          {
            stripCT.push_back(tt_ct_svc->getCT(ii,kk));
          }
        
        CTchannels.push_back(stripCT);
      }
    
    //Initialize pointer to TTDigitization class
    TTDigit = new TTDigitization();

    return true;
}

bool
PMTSimAlg::execute()
{
    // event mixing is also here.
    // we will mix the events, then convert the hit into pulse.
    // so we can avoid duplication of SimHit 
    // 

    LogDebug << "In PMTSimAlg, going to load events and unpacking hits." << std::endl;

    std::deque<Pulse>& CDSPMT_pulse_buffer = m_elecsvc->get_CDSPMT_pulse_buffer();
    std::deque<Pulse>& CDLPMT_pulse_buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
    std::deque<Pulse>& WPLPMT_pulse_buffer = m_elecsvc->get_WPLPMT_pulse_buffer();
    std::deque<Pulse>& TT_pulse_buffer = m_elecsvc->get_TT_pulse_buffer();

    int cnt_loaded_evt = 0;
    m_do_subdet_sim = false;

    while(true) {
        //events pre select optimizate simulation speed.
        int hitsSum = 0;
        TimeStamp timeStart, timeCurrent, timeStop, TempT;
        double delta = 0;

        vec_current_event.clear();
        
        if(m_enableSNMode and isFirstTry == true) {
          isFirstTry = false;
          SN_aNav = get_one_SN_event();
          if (SN_aNav == nullptr) {
             LogWarn << "Get empty navigator of SN events. Run is stopping." << std::endl;
             m_stoppingRun = true;
             break;
          }
          //JM::SimHeader* first_hdr = dynamic_cast<JM::SimHeader*>(SN_aNav->getHeader("/Event/Sim"));
          //JM::SimEvt* first_evt = dynamic_cast<JM::SimEvt*>(first_hdr->event());
          //const std::vector<JM::SimTrack*>& trc = first_evt->getTracksVec();
          //std::vector<JM::SimTrack*>::const_iterator it_track = trc.begin();
          //JM::SimTrack* track = *it_track;
          //double track_initT = track->getInitT();
          //TimeStamp evtTime(track_initT*1e-9);

          TimeStamp evtTime(SN_aNav->TimeStamp().GetSec(), SN_aNav->TimeStamp().GetNanoSec());
          firstSNEvtTime = evtTime;
          nextSNEvtTime = evtTime;
          nextEvtTime = evtTime;
          m_elecsvc->setFirstPulseTimeInBuffer(nextEvtTime);

          double run_time = (m_elecsvc->get_stop_time() - m_elecsvc->get_start_time()).GetSeconds();
          m_elecsvc->set_start_time(firstSNEvtTime);
    	  m_elecsvc->iniCDDrakPulseStartTime(m_pre_darkpulse);
          evtTime = firstSNEvtTime;
          evtTime.Add(run_time);
          m_elecsvc->set_stop_time(evtTime);

          if(isSNMixingWithBKG) {
            nextBGKEvtTime = m_elecsvc->get_start_time();
            delta = gRandom -> Exp(m_mainTau);
            nextBGKEvtTime.Add(delta);
            LogInfo << "Time of the next background event to be mixed is : " << nextBGKEvtTime << std::endl;
          }
          else {
            nextBGKEvtTime = m_elecsvc->get_stop_time();
            nextBGKEvtTime.Add(10);
          }

          LogInfo << "Since this is a SN simulaiton, the start time and stop time are updated according to the time given by SN: " << firstSNEvtTime << ", " << evtTime << std::endl;
        } else if (m_doLaserTrigger and isFirstTry == true) {
          isFirstTry = false;
          if (isLaserMixingWithBKG) {
            nextBGKEvtTime = m_elecsvc->get_start_time();
            delta = gRandom -> Exp(m_mainTau);
            nextBGKEvtTime.Add(delta);
          }
        }

        // for case of empty buffer
        if(CDLPMT_pulse_buffer.size() == 0 && m_stoppingRun == false) m_elecsvc->setFirstPulseTimeInBuffer(nextEvtTime);

        //set the marker time which will be used to indicate the end point of the pulses in buffer to be processed.
        //The events after the marker time will be stopped to be loaded.
        //It will be used by the followed modules, such as TriggerSim.
        TimeStamp markerT;
        if(m_elecsvc->getFirstPulseTimeInBuffer() >= nextEvtTime && m_stoppingRun == false) {
          markerT = nextEvtTime;
          markerT.Add(m_evtBufferLength*1e-9);
        }
        else {
          markerT = m_elecsvc->getFirstPulseTimeInBuffer();
          markerT.Add(m_evtBufferLength*1e-9);
        }
        m_elecsvc->set_marker_time(markerT);

        LogDebug << "The marker time stamp is: " << markerT << std::endl;
        LogDebug << "First pulse time in buffer: " << m_elecsvc->getFirstPulseTimeInBuffer()  << std::endl;

        if(m_stoppingRun) {
          nextEvtTime = m_elecsvc->get_stop_time();
          nextEvtTime.Add(10);
        }

        if(m_stoppingRun) {
          //start time of this round of event sampling
          timeStart =  m_elecsvc->getFirstPulseTimeInBuffer();
          
          //event time of the current event
          timeCurrent =  m_elecsvc->getFirstPulseTimeInBuffer();
        }
        else {
          //start time of this round of event sampling
          timeStart = nextEvtTime;
          
          //event time of the current event
          timeCurrent = nextEvtTime;
        }

        LogDebug << "The time stamp of the first event that might be loaded: " << nextEvtTime << std::endl;

        //number of hits in the interested time window, within marker time.
        int nhitsInWoI = 0;

        if(m_elecsvc->getFirstPulseTimeInBuffer() < m_elecsvc->get_marker_time()) {
          for(int ipulse = 0; ipulse < CDLPMT_pulse_buffer.size(); ipulse++) {
            if(CDLPMT_pulse_buffer[ipulse].pulseHitTime < m_elecsvc->get_marker_time()) nhitsInWoI++;
            else break;
          }
        }
        
        LogDebug << "Number of hits in WoI: " << nhitsInWoI << std::endl;
        LogDebug << "Number of hits in loaded event: " << hitsSum << std::endl;

        while( (hitsSum + nhitsInWoI) < m_nHitsThreshold or 
               nextEvtTime < m_elecsvc->get_marker_time()) {
	      if(m_do_subdet_sim && nextEvtTime > m_elecsvc->get_marker_time()) break;
            // skip the low energy events with no overlap with the next sampled event
          if(nextEvtTime > m_elecsvc->get_marker_time() and 
             (hitsSum + nhitsInWoI) < m_nHitsThreshold) {
              while(true) {
                if(CDLPMT_pulse_buffer.size() == 0) {
                  timeStart = nextEvtTime;
                  m_elecsvc->setFirstPulseTimeInBuffer(nextEvtTime);
                  break;
                }
                if(CDLPMT_pulse_buffer[0].pulseHitTime > m_elecsvc->get_marker_time()) {
                  if(CDLPMT_pulse_buffer[0].pulseHitTime > nextEvtTime) timeStart = nextEvtTime;
                  else timeStart = CDLPMT_pulse_buffer[0].pulseHitTime;
                  if(m_stoppingRun) timeStart = CDLPMT_pulse_buffer[0].pulseHitTime;
                  m_elecsvc->setFirstPulseTimeInBuffer(CDLPMT_pulse_buffer[0].pulseHitTime);
                  break;
                }
                CDLPMT_pulse_buffer.pop_front();
              }
              markerT = timeStart;
              markerT.Add(m_evtBufferLength*1e-9);
              m_elecsvc->set_marker_time(markerT);

                  nhitsInWoI = 0;
                  if(m_elecsvc->getFirstPulseTimeInBuffer() < m_elecsvc->get_marker_time()) {
                    for(int ipulse = 0; ipulse < CDLPMT_pulse_buffer.size(); ipulse++) {
                      if(CDLPMT_pulse_buffer[ipulse].pulseHitTime < m_elecsvc->get_marker_time()) nhitsInWoI++;
                      else break;
                    }
                  }

                  LogDebug << "Marker time is updated to: " << markerT << std::endl;
                  LogDebug << "First pulse time in buffer: " << m_elecsvc->getFirstPulseTimeInBuffer() << std::endl;
                  LogDebug << "Number of pulse in WoI: " << nhitsInWoI << std::endl;

                  vecNav.clear();
                  vecEvtTime.clear();
                  vec_current_event.clear();
                  hitsSum = 0;
              }

              if(m_stoppingRun == true) break;
              if(nextEvtTime > m_elecsvc->get_marker_time()) continue;

              //the next event is loaded
              std::shared_ptr<JM::EvtNavigator> aNav = nullptr;
              std::deque<std::shared_ptr<JM::EvtNavigator>> multiaNav(m_nLaserEventLoad, nullptr);
              if(m_enableSNMode) {
                if(nextSNEvtTime == m_elecsvc->get_start_time()) { 
                  aNav = SN_aNav;
                  vec_current_event.push_back(vec_current_SNevent[0]); vec_current_SNevent.clear();
                }
                else {
                  if(isSNEvt)  {
                      aNav = SN_aNav;
                      if( !vec_current_SNevent.empty()) {
                        vec_current_event.push_back(vec_current_SNevent[0]); vec_current_SNevent.clear();
                      }
                  }
                  else { 
                      aNav = get_one_event();
                  }
                }
              }
              else if (m_doLaserTrigger) {
                  if (isLaserEvt) {
                      if (m_enableMultiLaserLoad) {
                          for (int i = 0; i < m_nLaserEventLoad; i++) {
                              aNav = get_one_Laser_event();
                              multiaNav[i] = aNav;
                          }
                          if (aNav != nullptr) {
                              m_elecsvc->set_Laser_evttime(nextLaserEvtTime);
                              markerT = nextLaserEvtTime;
                              markerT.Add(m_evtBufferLength*1e-9);
                              m_elecsvc->set_marker_time(markerT);
                          }
                      } else {
                          aNav = get_one_Laser_event();
                          if (aNav != nullptr) {
                              m_elecsvc->set_Laser_evttime(nextLaserEvtTime);
                              markerT = nextLaserEvtTime;
                              markerT.Add(m_evtBufferLength*1e-9);
                              m_elecsvc->set_marker_time(markerT);
                          }
                      }
                  } else {
                      aNav = get_one_event();
                  }
              }
              else {
                aNav = get_one_event();
              }

              if (aNav == nullptr) {
                  LogWarn << "Get empty navigator. Run is stopping." << std::endl;
                  m_stoppingRun = true;
                  break;
              }

              ++cnt_loaded_evt;

              //to get number of hits of LPMT via simHeader
              auto hdr = JM::getHeaderObject<JM::SimHeader>(aNav.get());
              int nHits = hdr->getCDLPMTtotalHits();
              if (m_enableMultiLaserLoad&&isLaserEvt) {
                  nHits = 0;
                  for (int i = 0; i < m_nLaserEventLoad; i++) {
                      hdr = JM::getHeaderObject<JM::SimHeader>(multiaNav[i].get());
                      nHits += hdr->getCDLPMTtotalHits();
                  }
              }
	      //
	      check_subdet_hits(aNav);

              timeCurrent = nextEvtTime;
              if(!m_enableSNMode) aNav->setTimeStamp(TTimeStamp(timeCurrent.GetSec(), timeCurrent.GetNanoSec()));
              if (m_enableMultiLaserLoad&&isLaserEvt) {
                  for (int i = 0; i < m_nLaserEventLoad; i++) {
                      vecNav.push_back(multiaNav[i]);
                      vecEvtTime.push_back(timeCurrent); 
                  }
              } else {
                  vecNav.push_back(aNav); 
                  vecEvtTime.push_back(timeCurrent); 
              }

              hitsSum += nHits;
              
              LogDebug << "one event is loaded." << std::endl;
              LogDebug << "Time stamp of this event: " << timeCurrent << std::endl;
              LogDebug << "Number of hits in this event: " << hitsSum << std::endl;
              LogDebug << "Number of hits of loaded events: " << nHits << std::endl;

              if(m_enableSNMode) {
                if(isSNEvt) {
                  SN_aNav = get_one_SN_event();
                  if (SN_aNav == nullptr) {
                      LogWarn << "Get empty navigator of SN events. Run is stopping." << std::endl;
                      if(isSNMixingWithBKG) {
                        isSNEvt = false;
                        delta = gRandom -> Exp(m_mainTau);
                        nextBGKEvtTime.Add(delta);
                        nextSNEvtTime = m_elecsvc->get_stop_time();
                        nextSNEvtTime.Add(10);
                        m_stoppingRun = true;
                      }
                      else {
                        isSNEvt = true;
                        m_stoppingRun = true;
                        break;
                      }
                  }
                  else {
                    TimeStamp evtTime(SN_aNav->TimeStamp().GetSec(), SN_aNav->TimeStamp().GetNanoSec());
                    nextSNEvtTime = evtTime;
                    LogInfo << "##### Next SN time is " << nextSNEvtTime << std::endl;
                  }
                }
                else {
                  delta = gRandom -> Exp(m_mainTau);
                  nextBGKEvtTime.Add(delta);
                }

                if(nextBGKEvtTime > nextSNEvtTime) {
                  nextEvtTime = nextSNEvtTime;
                  isSNEvt = true;
                }
                else {
                  nextEvtTime = nextBGKEvtTime;
                  isSNEvt = false;
                }
              }
              else if (m_doLaserTrigger) {
                  if (isLaserMixingWithBKG) {
                      if (isLaserEvt) {
                          delta = 1/m_LaserRate;
                          nextLaserEvtTime.Add(delta);
                      } else {
                          delta = gRandom -> Exp(m_mainTau);
                          nextBGKEvtTime.Add(delta);
                      }
                      if (nextBGKEvtTime > nextLaserEvtTime) {
                          nextEvtTime = nextLaserEvtTime;
                          isLaserEvt = true;
                      } else {
                          nextEvtTime = nextBGKEvtTime;
                          isLaserEvt = false;
                      }
                  } else {
                      delta = 1/m_LaserRate;
                      nextLaserEvtTime.Add(delta);
                      nextEvtTime = nextLaserEvtTime;
                      isLaserEvt = true;
                  }
              }
              else {
                delta = gRandom -> Exp(m_mainTau);
                LogInfo << "delta = " << delta << ", nextEvtTime = " << nextEvtTime << std::endl;
                nextEvtTime = timeCurrent;
                nextEvtTime.Add(delta);
              }
              LogDebug << "Time of the next event is sampled to be : " << nextEvtTime << std::endl;
        }

        if(vecEvtTime.size() == 0 && CDLPMT_pulse_buffer.size() == 0) {
            LogWarn << "nothing is loaded." << std::endl;
            break;
        } 
        if(vecEvtTime.size() == 0 && CDLPMT_pulse_buffer.size() > 0) {
            m_elecsvc->set_cur_evttime(m_elecsvc->getFirstPulseTimeInBuffer());
        }
        if(vecEvtTime.size() > 0) {
            m_elecsvc->set_cur_evttime(vecEvtTime.back());
        }

        LogDebug << "=== Testing: start time, first pulse time and marker time: " << timeStart << ", " << m_elecsvc->getFirstPulseTimeInBuffer() << ", " << m_elecsvc->get_marker_time() << std::endl;
        if(m_enableDarkPulse){
           if(m_elecsvc->getFirstPulseTimeInBuffer() > timeStart) generate_cd_dark_pulses(timeStart, m_elecsvc->get_marker_time());
           else generate_cd_dark_pulses(m_elecsvc->getFirstPulseTimeInBuffer(), m_elecsvc->get_marker_time());
        }

        if(m_enableWPDarkPulse){
          if(m_elecsvc->getFirstPulseTimeInBuffer() > timeStart) generate_wp_dark_pulses(timeStart, m_elecsvc->get_marker_time());
          else generate_wp_dark_pulses(m_elecsvc->getFirstPulseTimeInBuffer(), m_elecsvc->get_marker_time());
        }

        LogDebug << "=== size of navigator vector: " << vecNav.size() << std::endl;
        //loop EvtNavigator buffer and genetate pulses
        JM::SimHeader* curr_hdr = 0;
        JM::SimEvt* evt = 0;
        int eventNum = 0;
        for(std::deque<std::shared_ptr<JM::EvtNavigator>>::iterator it_nav = vecNav.begin();it_nav != vecNav.end();++it_nav) {
            std::shared_ptr<JM::EvtNavigator> currNav = *it_nav;
            curr_hdr = JM::getHeaderObject<JM::SimHeader>(currNav.get());
            evt = dynamic_cast<JM::SimEvt*>(curr_hdr->event());
            //vertex fitting
            JM::SimTrack* tra = 0;
            edepX =0;
            edepY =0;
            edepZ =0;
            const auto&  tracks = evt->getTracksVec();
            for( auto it_tra = tracks.begin();
                 it_tra != tracks.end();++it_tra) {
                    tra = *it_tra;
                    edepX = tra->getEdepX();
                    edepY = tra->getEdepY();
                    edepZ = tra->getEdepZ();
            }

            if(abs(edepX)>17500) edepX = edepX/abs(edepX)*15000;
            else edepX = 5000*round(edepX/5000);
            if(abs(edepY)>17500) edepY = edepY/abs(edepY)*15000;
            else edepY = 5000*round(edepY/5000);
            if(abs(edepZ)>17500) edepZ = edepZ/abs(edepZ)*15000;
            else edepZ = 5000*round(edepZ/5000);

            //Event keeper

                   
            EventKeeper& m_keeper = EventKeeper::Instance();
            m_keeper.set_current_entry(vec_current_event[eventNum]);
            
            //generate cd pulses
            generate_cd_pulses(evt,vecEvtTime[eventNum]);  

            if(m_enableWP) generate_wp_pulses(evt,vecEvtTime[eventNum]);

            if(m_enableTT) generate_tt_pulses(evt,vecEvtTime[eventNum]);
            eventNum++;
        }
       
        vecNav.clear();
        vecEvtTime.clear();

        std::sort(CDSPMT_pulse_buffer.begin(), CDSPMT_pulse_buffer.end());
        std::sort(CDLPMT_pulse_buffer.begin(), CDLPMT_pulse_buffer.end());

        if(m_enableMergeLPMTPulse){
            merge_cd_LPMT_pulses();
        }
        // need to sort pulse
        std::sort(WPLPMT_pulse_buffer.begin(), WPLPMT_pulse_buffer.end());
        // merge pulses for wp LPMT
        if(m_enableMergeLPMTPulse){
            merge_wp_LPMT_pulses();
        }
        std::sort(TT_pulse_buffer.begin(), TT_pulse_buffer.end());
        break;
    }

    // if in this event, nothing is loaded,
    // we need to throw the exception again.
    if (!cnt_loaded_evt && CDLPMT_pulse_buffer.size() == 0) {
        LogWarn << "Nothing is loaded, propose to stop the simulation. " << std::endl;
        LogWarn << "The current status of top task in Sniper is "
                << static_cast<unsigned short>(dynamic_cast<Task*>(getRoot())->Snoopy().state())
                << std::endl;
        for (const auto& subtask: m_taskObjMap) {
            LogWarn << "The current status of sub task ["
                    << subtask.first << "]: "
                    << static_cast<unsigned short>(subtask.second->Snoopy().state())
                    << std::endl;
        }

        LogWarn << "Number of pulses in CDSPMT: "
                << CDSPMT_pulse_buffer.size()
                << std::endl;
        LogWarn << "Number of pulses in CDLPMT: "
                << CDLPMT_pulse_buffer.size()
                << std::endl;
        LogWarn << "Number of pulses in WPLPMT: "
                << WPLPMT_pulse_buffer.size()
                << std::endl;
        LogWarn << "Number of pulses in TT: "
                << TT_pulse_buffer.size()
                << std::endl;

        ++m_count_errs;

        if (m_count_errs>=m_nlimit_counts) {
            // mark current task as stop
            dynamic_cast<Task*>(getParent())->stop(Sniper::StopRun::Peacefully);
        }
    }

    return true;
}

bool
PMTSimAlg::finalize()
{
    delete gNNVTAmp;
    return true;
}

int 
PMTSimAlg::generate_cd_pulses(JM::SimEvt* evt, TimeStamp curTimeStamp)
{
    std::deque<Pulse>& SPMT_pulse_buffer = m_elecsvc->get_CDSPMT_pulse_buffer();
    std::deque<Pulse>& LPMT_pulse_buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
    TimeStamp stop_time = m_elecsvc->get_stop_time();
    TimeStamp start_time = m_elecsvc->get_start_time();

    int pulses_sum = 0; // generated pulses

    const auto& cd_hits = evt->getCDHitsVec();

    EventKeeper& keeper = EventKeeper::Instance();
    const EventKeeper::Entry& entry = keeper.current_entry();
    shared_ptr<EventKeeper::Entry> entry_p{new EventKeeper::Entry};
    if(m_savetruthsMap[entry.tag]) 
    {
	*entry_p = entry;
    }


    for (auto it_hit = cd_hits.begin();
         it_hit != cd_hits.end(); ++it_hit) {

        auto hit=*it_hit;
        int hit_pmtid = hit->getPMTID();
        TimeStamp hitTimeStamp = curTimeStamp;
        TimeStamp corrhitTimeStamp = curTimeStamp;
        double hit_amplitude = 0;
        
        double m_efficiency = 0;
        double m_afterPulseProb = 0;
        double m_timeSpread = 0;
        double m_timeOffset = 0;
        double m_gain = 0;
        double m_sigmaGain = 0;
        double hit_timeSpread = 0;
        double m_tof = 0;
        if(hit_pmtid<=m_total_Lpmt){//LPMT
            if(!m_enableLPMT) continue;
            //LPMT data service
            if(m_enableAssignGain){
                m_gain = m_Gain;
            }else{
                m_gain = m_pmtsimsvc->get_gain(hit_pmtid);
            }
            if(m_enableAssignSigmaGain){
                m_sigmaGain = m_SigmaGain;
            }else{
                m_sigmaGain = m_pmtsimsvc->get_sigmaGain(hit_pmtid);
            }
            m_efficiency = m_pmtsimsvc->get_pde(hit_pmtid);
            m_afterPulseProb = m_pmtsimsvc->get_afterPulseProb(hit_pmtid);


            // TTS changes with theta:
            if(m_pmtsvc->isHamamatsu(hit_pmtid)) { // Hamamatsu
                if(m_enableDynodeTTS2theta) {
                    m_timeSpread = TTSDynode(hit->getLocalTheta()/3.1415926*180., m_pmtsimsvc->get_tts(hit_pmtid));
                } else {
                    m_timeSpread = m_pmtsimsvc->get_tts(hit_pmtid);
                }
            } else {  // NNVT
                if(m_enableMCPTTS2theta) {
                    m_timeSpread = TTSMCP(hit->getLocalTheta()/3.1415926*180., m_pmtsimsvc->get_tts(hit_pmtid));
                } else {
                    m_timeSpread = m_pmtsimsvc->get_tts(hit_timeSpread);
                }
            }

            // TT changes with theta:
            if(m_pmtsvc->isHamamatsu(hit_pmtid)) { // Hamamatsu
                if(m_enableDynodeTT2theta) {
                    m_timeOffset = ToffsetDynode(hit->getLocalTheta()/3.1415926*180., m_pmtsimsvc->get_timeOffset(hit_pmtid));
                } else{
                    m_timeOffset = m_pmtsimsvc->get_timeOffset(hit_pmtid);
                }
            } else { // NNVT
                if(m_enableMCPTT2theta) {
                    m_timeOffset = ToffsetMCP(hit->getLocalTheta()/3.1415926*180., m_pmtsimsvc->get_timeOffset(hit_pmtid));
                } else {
                    m_timeOffset = m_pmtsimsvc->get_timeOffset(hit_pmtid);
                }
            }

            // here, the hit time is not the absolute timestamp

            if(m_enablePmtTTS) hit_timeSpread =  gRandom->Gaus(0,1) * (m_timeSpread);  // PmtData file saves tts sigma now
            //if(m_enablePmtTTS) hit_timeSpread =  gRandom->Gaus(0,1) * (m_timeSpread/2.355);
            m_tof = sqrt(pow(edepX-m_pmtsvc->getPMTX(hit_pmtid),2)+pow(edepY-m_pmtsvc->getPMTY(hit_pmtid),2)+pow(edepZ-m_pmtsvc->getPMTZ(hit_pmtid),2))/(2.99792458*1.5*100); 
            //m_tof = sqrt(pow(edepX-m_elecsvc->get_pmtPosX(hit_pmtid),2)+pow(edepY-m_elecsvc->get_pmtPosY(hit_pmtid),2)+pow(edepZ-m_elecsvc->get_pmtPosZ(hit_pmtid),2))/(2.99792458*1.5*100); 
            double hit_hitTime = hit->getHitTime() + hit_timeSpread + m_timeOffset;
            if(hit_hitTime + start_time.GetSec()*1.0e9 + start_time.GetNanoSec() > stop_time.GetSec()*1.0e9 + stop_time.GetNanoSec()) continue;

            hitTimeStamp.Add(hit_hitTime*1e-9); // ns -> s  TTS + timeoffset
            double corrtime = hit_hitTime-m_tof;
            if(corrtime<0) corrtime=0;
            corrhitTimeStamp.Add((corrtime)*1e-9);
/*
            if(m_enableSNMode) {   // SN Mode pulse hitTime
                //hitTimeStamp = m_elecsvc->get_start_time();
                TimeStamp pulseTime(hit_hitTime*1e-9);
                hitTimeStamp = pulseTime;
                //hitTimeStamp.Subtract(firstSNEvtTime.GetSeconds());

                corrhitTimeStamp = hitTimeStamp;
                corrhitTimeStamp.Subtract(m_tof*1e-9);
            }
*/
            double hit_weight = hit->getNPE();
            // add QE affect.
            if(m_enableEfficiency){
                if(gRandom->Rndm() > m_efficiency){
                    continue;    //ignore SimHit due to efficiency
                }
            }

            // single PE sigmaGain   
            if ( !m_enableChargeSmearing )
                m_sigmaGain = 0;
            if(m_pmtsvc->isHamamatsu(hit_pmtid)){          
                hit_amplitude =  PulseAmp(hit_weight,m_gain,m_sigmaGain);
            }else{
                double theta = hit->getLocalTheta()*180/3.1415926;
                double exp_frac = gNNVTAmp->Eval(theta);
                hit_amplitude =  PulseAmpMCP(hit_weight,m_gain,m_sigmaGain,exp_frac);
            }
        }
        else { //SPMT
            if(!m_enableSPMT) continue;
            double hit_hitTime = hit->getHitTime();
            if(hit_hitTime + start_time.GetSec()*1.0e9 + start_time.GetNanoSec() > stop_time.GetSec()*1.0e9 + stop_time.GetNanoSec()) continue;
            hitTimeStamp.Add(hit_hitTime*1e-9); // ns -> s  TTS + timeoffset
            hit_amplitude = hit->getNPE();
        }

        IPMTTool* tool = 0;
        if (PMT::IsCD(hit_pmtid) and PMT::Is20inch(hit_pmtid)) { // LPMT
            tool = m_pmt_tool[kPMT_CD_Lpmt];
        } else if (PMT::IsCD(hit_pmtid) and PMT::Is3inch(hit_pmtid)) { // SPMT
            tool = m_pmt_tool[kPMT_CD_Spmt];
        } else {
            LogWarn << "Unknown pmtid: " << hit_pmtid << std::endl;
            continue;
        }
        Pulse pulse = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
        pulse.evtTimeStamp=curTimeStamp;  
        pulse.corrpulseHitTime = corrhitTimeStamp;
        pulse.tof = m_tof;
        if(pulse.truth)
	{
		pulse.truth->npe = hit->getNPE();
		pulse.truth->hitTime = hit->getHitTime();
		pulse.truth->timeWindow = hit->getTimeWindow();
		pulse.truth->TTS = hit_timeSpread;
		pulse.truth->timeoffset = m_timeOffset;
		pulse.truth->m_entry_pointer = entry_p;
	}

        if (PMT::IsCD(hit_pmtid) and PMT::Is20inch(hit_pmtid)) { // LPMT
            LPMT_pulse_buffer.push_back(pulse);
        } else if (PMT::IsCD(hit_pmtid) and PMT::Is3inch(hit_pmtid)) { // SPMT
            SPMT_pulse_buffer.push_back(pulse);
        } else {
            LogWarn << "Unknown pmtid: " << hit_pmtid << std::endl;
            continue;
        }
   
        //add afterPulse   
        //if(m_enableAfterPulse&&hit_pmtid<=18000){// only LPMT for now
        //    if(gRandom->Rndm() < m_afterPulseProb){
        //        double current_rand = gRandom -> Rndm();
        //        double delta_afterPulseTime = ConvertPdfRand01(current_rand,m_afterPulsePdf,m_afterPulseEdges);
        //        if(delta_afterPulseTime>10000){
        //            delta_afterPulseTime = 10000;
        //        }
        //        hitTimeStamp.Add(delta_afterPulseTime * 1e-9);

        //        if(m_elecsvc->get_isHamamatsu(hit_pmtid)==1){
        //            hit_amplitude =  PulseAmp(hit_amplitude,m_gain,m_sigmaGain);
        //        }else{
        //            //TGraph* g1 = m_elecsvc->get_graph();
        //            double exp_frac = gNNVTAmp->Eval(0);
        //            hit_amplitude =  PulseAmpMCP(hit_amplitude,m_gain,m_sigmaGain,exp_frac);
        //        }


        //        Pulse after_pulse = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude);
        //        after_pulse.npe=1;
        //        after_pulse.hitTime = hit->getHitTime() + delta_afterPulseTime;
        //        after_pulse.timeoffset = m_timeOffset;
        //        after_pulse.type = kAfterPulse;
        //        LPMT_pulse_buffer.push_back(after_pulse);
        //    }
        //}

        if(hit_pmtid<m_total_Lpmt && m_enableAfterPulse) { // only adding Lpmt afterPulses
            if(m_pmtsvc->isHamamatsu(hit_pmtid)) {  // Hamamatsu PMTs
                // AP1
                int ap1_npe = m_pmtsimsvc->dynodeAP1_amp();
                if(ap1_npe>0) {
                    hit_amplitude =  PulseAmp(ap1_npe, m_gain, m_sigmaGain);
                    double delta_afterPulseTime1 = m_pmtsimsvc->dynodeAP1_time();
                    hitTimeStamp.Add(delta_afterPulseTime1* 1e-9);
                    corrhitTimeStamp.Add(delta_afterPulseTime1* 1e-9);
		    if(hitTimeStamp > stop_time) continue;
                    Pulse after_pulse1 = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
                    after_pulse1.type = kAfterPulse;
                    after_pulse1.evtTimeStamp = curTimeStamp;
                    after_pulse1.corrpulseHitTime = corrhitTimeStamp;
                    after_pulse1.tof = m_tof;
		    if(after_pulse1.truth)
		    {
			after_pulse1.truth->npe=ap1_npe;
			after_pulse1.truth->hitTime = hit->getHitTime() + delta_afterPulseTime1;
			after_pulse1.truth->TTS = hit_timeSpread;
                        after_pulse1.truth->timeoffset = m_timeOffset;
                        after_pulse1.truth->m_entry_pointer = entry_p;

		    }
                    LPMT_pulse_buffer.push_back(after_pulse1);
                    hitTimeStamp.Subtract(delta_afterPulseTime1* 1e-9);
                    corrhitTimeStamp.Subtract(delta_afterPulseTime1* 1e-9);
                }
                // AP2
                int ap2_npe = m_pmtsimsvc->dynodeAP2_amp();
                if(ap2_npe>0) {
                    hit_amplitude =  PulseAmp(ap2_npe, m_gain, m_sigmaGain);
                    double delta_afterPulseTime2 = m_pmtsimsvc->dynodeAP2_time();
                    hitTimeStamp.Add(delta_afterPulseTime2* 1e-9);
                    corrhitTimeStamp.Add(delta_afterPulseTime2* 1e-9);
		    if(hitTimeStamp > stop_time) continue;
                    Pulse after_pulse2 = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
                    after_pulse2.type = kAfterPulse;
                    after_pulse2.evtTimeStamp = curTimeStamp;
                    after_pulse2.corrpulseHitTime = corrhitTimeStamp;
                    after_pulse2.tof = m_tof;
                    if(after_pulse2.truth)
                    {
                        after_pulse2.truth->npe=ap2_npe;
                        after_pulse2.truth->hitTime = hit->getHitTime() + delta_afterPulseTime2;
                        after_pulse2.truth->TTS = hit_timeSpread;
                        after_pulse2.truth->timeoffset = m_timeOffset;
                        after_pulse2.truth->m_entry_pointer = entry_p;

                    }

                    LPMT_pulse_buffer.push_back(after_pulse2);
                    hitTimeStamp.Subtract(delta_afterPulseTime2* 1e-9);
                    corrhitTimeStamp.Subtract(delta_afterPulseTime2* 1e-9);
                }
            } else { //MCP PMTs
                // AP1
                int ap1_npe = m_pmtsimsvc->mcpAP1_amp();
                if(ap1_npe>0) {
                    double exp_frac = gNNVTAmp->Eval(0);
                    hit_amplitude =  PulseAmpMCP(ap1_npe,m_gain,m_sigmaGain,exp_frac);
                    double delta_afterPulseTime1 = m_pmtsimsvc->mcpAP1_time();
                    hitTimeStamp.Add(delta_afterPulseTime1* 1e-9);
                    corrhitTimeStamp.Add(delta_afterPulseTime1* 1e-9);
		    if(hitTimeStamp > stop_time) continue;
                    Pulse after_pulse1 = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
                    after_pulse1.type = kAfterPulse;
                    after_pulse1.evtTimeStamp = curTimeStamp;
                    after_pulse1.corrpulseHitTime = corrhitTimeStamp;
                    after_pulse1.tof = m_tof;
                    if(after_pulse1.truth)
                    {
                        after_pulse1.truth->npe=ap1_npe;
                        after_pulse1.truth->hitTime = hit->getHitTime() + delta_afterPulseTime1;
                        after_pulse1.truth->TTS = hit_timeSpread;
                        after_pulse1.truth->timeoffset = m_timeOffset;
                        after_pulse1.truth->m_entry_pointer = entry_p;

                    }

                    LPMT_pulse_buffer.push_back(after_pulse1);
                    hitTimeStamp.Subtract(delta_afterPulseTime1* 1e-9);
                    corrhitTimeStamp.Subtract(delta_afterPulseTime1* 1e-9);
                }
                // AP2
                int ap2_npe = m_pmtsimsvc->mcpAP2_amp();
                if(ap2_npe>0) {
                    double exp_frac = gNNVTAmp->Eval(0);
                    hit_amplitude =  PulseAmpMCP(ap2_npe,m_gain,m_sigmaGain,exp_frac);
                    double delta_afterPulseTime2 = m_pmtsimsvc->mcpAP2_time();
                    hitTimeStamp.Add(delta_afterPulseTime2* 1e-9);
                    corrhitTimeStamp.Add(delta_afterPulseTime2* 1e-9);
		    if(hitTimeStamp > stop_time) continue;
                    Pulse after_pulse2 = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
                    after_pulse2.type = kAfterPulse;
                    after_pulse2.evtTimeStamp = curTimeStamp;
                    after_pulse2.corrpulseHitTime = corrhitTimeStamp;
                    after_pulse2.tof = m_tof;
                    if(after_pulse2.truth)
                    {   
                        after_pulse2.truth->npe=ap2_npe;
                        after_pulse2.truth->hitTime = hit->getHitTime() + delta_afterPulseTime2;
                        after_pulse2.truth->TTS = hit_timeSpread;
                        after_pulse2.truth->timeoffset = m_timeOffset;
                        after_pulse2.truth->m_entry_pointer = entry_p;
                    
                    }

                    LPMT_pulse_buffer.push_back(after_pulse2);
                    hitTimeStamp.Subtract(delta_afterPulseTime2* 1e-9);
                    corrhitTimeStamp.Subtract(delta_afterPulseTime2* 1e-9);
                }
                // AP3
                int ap3_npe = m_pmtsimsvc->mcpAP3_amp();
                if(ap3_npe>0) {
                    double exp_frac = gNNVTAmp->Eval(0);
                    hit_amplitude =  PulseAmpMCP(ap3_npe,m_gain,m_sigmaGain,exp_frac);
                    double delta_afterPulseTime3 = m_pmtsimsvc->mcpAP3_time();
                    hitTimeStamp.Add(delta_afterPulseTime3* 1e-9);
                    corrhitTimeStamp.Add(delta_afterPulseTime3* 1e-9);
		    if(hitTimeStamp > stop_time) continue;
                    Pulse after_pulse3 = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
                    after_pulse3.type = kAfterPulse;
                    after_pulse3.evtTimeStamp = curTimeStamp;
                    after_pulse3.corrpulseHitTime = corrhitTimeStamp;
                    after_pulse3.tof = m_tof;
                    if(after_pulse3.truth)
                    {   
                        after_pulse3.truth->npe=ap3_npe;
                        after_pulse3.truth->hitTime = hit->getHitTime() + delta_afterPulseTime3;
                        after_pulse3.truth->TTS = hit_timeSpread;
                        after_pulse3.truth->timeoffset = m_timeOffset;
                        after_pulse3.truth->m_entry_pointer = entry_p;
                    
                    }

                    LPMT_pulse_buffer.push_back(after_pulse3);
                    hitTimeStamp.Subtract(delta_afterPulseTime3* 1e-9);
                    corrhitTimeStamp.Subtract(delta_afterPulseTime3* 1e-9);
                }

            }
        }

        ++pulses_sum;
    }
    return pulses_sum;

}

int
PMTSimAlg::generate_cd_dark_pulses(TimeStamp starttime,TimeStamp endtime)
{
    std::deque<Pulse>& LPMT_pulse_buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
    std::deque<Pulse>& SPMT_pulse_buffer = m_elecsvc->get_CDSPMT_pulse_buffer();
    TimeStamp stop_time = m_elecsvc->get_stop_time();

    int pulses_sum = 0;

    if (m_doForceTrigger) {
        // in force trigger mode, add 1us windows before and after total pulse buffer
        starttime.Subtract(1e-6);
        endtime.Add(1e-5);
    } else {
        starttime.Subtract(m_pre_darkpulse*1e-9); // pre-simulate DN m_pre_darkpulse=200ns
    	if(m_elecsvc->getCDDarkPulseStartTime() > starttime) starttime = m_elecsvc->getCDDarkPulseStartTime();
    }
 
    TimeStamp delta_Time = endtime - starttime ; 

    double deltaSimTime = delta_Time.GetSeconds();


    int dark_num_total = 0;
    for (int pmtid = 0; pmtid < m_total_Lpmt; ++pmtid) {
        int Ndark = 0;
        if(m_enableAssignDarkPulse) { Ndark = PoissonRand(m_darkRate *deltaSimTime *m_darkRateScale);}                    
        else { Ndark = PoissonRand(( m_pmtsimsvc->get_dcr(pmtid))*1000*deltaSimTime*m_darkRateScale);}  // need to transfer kHz into Hz

        for (int dummy = 0; dummy < Ndark; ++dummy) {
            double amplitude = 0.0;
            double darkPulseTime = gRandom->Rndm() * (deltaSimTime*1e9);
            TimeStamp pulseHitTime = starttime;
            pulseHitTime.Add(darkPulseTime*1e-9) ;
            double m_gain = m_pmtsimsvc->get_gain(pmtid);
            double m_sigmaGain = m_pmtsimsvc->get_sigmaGain(pmtid);
            if (!m_enableChargeSmearing) 
                m_sigmaGain = 0;

            if(m_pmtsvc->isHamamatsu(pmtid)){
                amplitude =  PulseAmp(1.0,m_gain,m_sigmaGain);
            }else{
                double exp_frac = gNNVTAmp->Eval(0);
                amplitude =  PulseAmpMCP(1.0,m_gain,m_sigmaGain,exp_frac);
            }

            IPMTTool* tool = 0;
            tool = m_pmt_tool[kPMT_CD_Lpmt];
            if(pulseHitTime > stop_time) continue;
            Pulse dark_pulse = tool->generate(pmtid, pulseHitTime, amplitude, m_enableSaveDarkPulseTruths);
            dark_pulse.corrpulseHitTime = pulseHitTime;
            dark_pulse.type = kDarkPulse;
            dark_pulse.evtTimeStamp = starttime;
            dark_pulse.tof = 0;
	    if(dark_pulse.truth)
            {
		dark_pulse.truth->npe = 1;
		dark_pulse.truth->hitTime = (pulseHitTime-starttime).GetSeconds()*1e9;
                dark_pulse.truth->timeoffset = 0;
            }
            LPMT_pulse_buffer.push_back(dark_pulse);


            TimeStamp hitTimeStamp = pulseHitTime;
            if(pmtid<m_total_Lpmt && m_enableAfterPulse) { // only adding Lpmt afterPulses
                if(m_pmtsvc->isHamamatsu(pmtid)) {  // Hamamatsu PMTs
                    // AP1
                    int ap1_npe = m_pmtsimsvc->dynodeAP1_amp();
                    if(ap1_npe>0) {
                        amplitude =  PulseAmp(ap1_npe, m_gain, m_sigmaGain);
                        double delta_afterPulseTime1 = m_pmtsimsvc->dynodeAP1_time();
                        hitTimeStamp.Add(delta_afterPulseTime1* 1e-9);
                        if(hitTimeStamp > stop_time) continue;
                        Pulse after_pulse1 = tool->generate(pmtid, hitTimeStamp, amplitude, m_enableSaveDarkPulseTruths);
                        after_pulse1.type = kDNAfterPulse;
                        after_pulse1.evtTimeStamp = starttime;
                        after_pulse1.corrpulseHitTime = hitTimeStamp;
                        after_pulse1.tof = 0;
            		if(after_pulse1.truth)
            		{
                		after_pulse1.truth->npe = ap1_npe;
                		after_pulse1.truth->hitTime = (hitTimeStamp - starttime).GetSeconds()*1e9;
                		after_pulse1.truth->timeoffset = 0;
            		}

                        LPMT_pulse_buffer.push_back(after_pulse1);
                        hitTimeStamp.Subtract(delta_afterPulseTime1* 1e-9);
                    }
                    // AP2
                    int ap2_npe = m_pmtsimsvc->dynodeAP2_amp();
                    if(ap2_npe>0) {
                        amplitude =  PulseAmp(ap2_npe, m_gain, m_sigmaGain);
                        double delta_afterPulseTime2 = m_pmtsimsvc->dynodeAP2_time();
                        hitTimeStamp.Add(delta_afterPulseTime2* 1e-9);
                        if(hitTimeStamp > stop_time) continue;
                        Pulse after_pulse2 = tool->generate(pmtid, hitTimeStamp, amplitude, m_enableSaveDarkPulseTruths);
                        after_pulse2.type = kDNAfterPulse;
                        after_pulse2.evtTimeStamp = starttime;
                        after_pulse2.corrpulseHitTime = hitTimeStamp;
                        after_pulse2.tof = 0;
                        if(after_pulse2.truth)
                        {       
                                after_pulse2.truth->npe = ap2_npe;
                                after_pulse2.truth->hitTime = (hitTimeStamp - starttime).GetSeconds()*1e9;
				after_pulse2.truth->TTS = 0;
                                after_pulse2.truth->timeoffset = 0;
                        }

                        LPMT_pulse_buffer.push_back(after_pulse2);
                        hitTimeStamp.Subtract(delta_afterPulseTime2* 1e-9);
                    }
                } else { //MCP PMTs
                    // AP1
                    int ap1_npe = m_pmtsimsvc->mcpAP1_amp();
                    if(ap1_npe>0) {
                        double exp_frac = gNNVTAmp->Eval(0);
                        amplitude =  PulseAmpMCP(ap1_npe,m_gain,m_sigmaGain,exp_frac);
                        double delta_afterPulseTime1 = m_pmtsimsvc->mcpAP1_time();
                        hitTimeStamp.Add(delta_afterPulseTime1* 1e-9);
                        if(hitTimeStamp > stop_time) continue;
                        Pulse after_pulse1 = tool->generate(pmtid, hitTimeStamp, amplitude, m_enableSaveDarkPulseTruths);
                        after_pulse1.type = kDNAfterPulse;
                        after_pulse1.evtTimeStamp = starttime;
                        after_pulse1.corrpulseHitTime = hitTimeStamp;
                        after_pulse1.tof = 0;
                        if(after_pulse1.truth)
                        {       
                                after_pulse1.truth->npe = ap1_npe;
                                after_pulse1.truth->hitTime = (hitTimeStamp - starttime).GetSeconds()*1e9;
                                after_pulse1.truth->TTS = 0;
                                after_pulse1.truth->timeoffset = 0;
                        }

                        LPMT_pulse_buffer.push_back(after_pulse1);
                        hitTimeStamp.Subtract(delta_afterPulseTime1* 1e-9);
                    }
                    // AP2
                    int ap2_npe = m_pmtsimsvc->mcpAP2_amp();
                    if(ap2_npe>0) {
                        double exp_frac = gNNVTAmp->Eval(0);
                        amplitude =  PulseAmpMCP(ap2_npe,m_gain,m_sigmaGain,exp_frac);
                        double delta_afterPulseTime2 = m_pmtsimsvc->mcpAP2_time();
                        hitTimeStamp.Add(delta_afterPulseTime2* 1e-9);
                        if(hitTimeStamp > stop_time) continue;
                        Pulse after_pulse2 = tool->generate(pmtid, hitTimeStamp, amplitude, m_enableSaveDarkPulseTruths);
                        after_pulse2.type = kDNAfterPulse;
                        after_pulse2.evtTimeStamp = starttime;
                        after_pulse2.corrpulseHitTime = hitTimeStamp;
                        after_pulse2.tof = 0;
                        if(after_pulse2.truth)
                        {       
                                after_pulse2.truth->npe = ap2_npe;
                                after_pulse2.truth->hitTime = (hitTimeStamp - starttime).GetSeconds()*1e9;
                                after_pulse2.truth->TTS = 0;
                                after_pulse2.truth->timeoffset = 0;
                        }

                        LPMT_pulse_buffer.push_back(after_pulse2);
                        hitTimeStamp.Subtract(delta_afterPulseTime2* 1e-9);
                    }
                    // AP3
                    int ap3_npe = m_pmtsimsvc->mcpAP3_amp();
                    if(ap3_npe>0) {
                        double exp_frac = gNNVTAmp->Eval(0);
                        amplitude =  PulseAmpMCP(ap3_npe,m_gain,m_sigmaGain,exp_frac);
                        double delta_afterPulseTime3 = m_pmtsimsvc->mcpAP3_time();
                        hitTimeStamp.Add(delta_afterPulseTime3* 1e-9);
                        if(hitTimeStamp > stop_time) continue;
                        Pulse after_pulse3 = tool->generate(pmtid, hitTimeStamp, amplitude, m_enableSaveDarkPulseTruths);
                        after_pulse3.type = kDNAfterPulse;
                        after_pulse3.evtTimeStamp = starttime;
                        after_pulse3.corrpulseHitTime = hitTimeStamp;
                        after_pulse3.tof = 0;
                        if(after_pulse3.truth)
                        {       
                                after_pulse3.truth->npe = ap3_npe;
                                after_pulse3.truth->hitTime = (hitTimeStamp - starttime).GetSeconds()*1e9;
                                after_pulse3.truth->TTS = 0;
                                after_pulse3.truth->timeoffset = 0;
                        }

                        LPMT_pulse_buffer.push_back(after_pulse3);
                        hitTimeStamp.Subtract(delta_afterPulseTime3* 1e-9);
                    }

                }
            }
        }   

        dark_num_total += Ndark;
    }

    for (int i = 0; i < m_total_Spmt; ++i) {
        int pmtid = kOFFSET_CD_SPMT+i;
        int Ndark = PoissonRand(( m_pmtsimsvc->get_dcr(pmtid))*deltaSimTime*m_darkRateScale_SPMT); //info need to be in PmtData.root
        for(int dummy = 0; dummy < Ndark; ++dummy){
            double amplitude = 1.0; //1 p.e.
            double darkPulseTime = gRandom->Rndm() * (deltaSimTime*1e9);
            TimeStamp pulseHitTime = starttime;
            pulseHitTime.Add(darkPulseTime*1e-9);

            IPMTTool* tool = 0;
            tool = m_pmt_tool[kPMT_CD_Spmt];
            if(pulseHitTime > stop_time) continue;
            Pulse dark_pulse = tool->generate(pmtid, pulseHitTime, amplitude, m_enableSaveDarkPulseTruths);
            dark_pulse.corrpulseHitTime = pulseHitTime;
            dark_pulse.type = kDarkPulse;
            dark_pulse.evtTimeStamp = starttime;
	    if(dark_pulse.truth)
	    {
		dark_pulse.truth->npe = 1;
                dark_pulse.truth->hitTime = (pulseHitTime-starttime).GetSeconds()*1e9;
                dark_pulse.truth->timeoffset = 0;
	    }
            SPMT_pulse_buffer.push_back(dark_pulse);
        }
        dark_num_total += Ndark;
    }
    return dark_num_total;
}
int PMTSimAlg::merge_cd_LPMT_pulses(){
    int pulse_size = 0;
    std::deque<Pulse>& buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
    std::deque<Pulse> buffer_after_merge;
    buffer_after_merge.clear();
    pulse_size = buffer.size();
    LogInfo<<"pulse size before merge:"<<pulse_size<<endl;
    std::deque<Pulse>::iterator it;
    for ( it=buffer.begin(); it != buffer.end(); ++it) {
        if(buffer[1] != buffer[0]){
            buffer_after_merge.push_back(buffer[0]);
        }else{
            buffer[1] += buffer[0];
        }
        buffer.pop_front();
    }
    m_elecsvc->set_CDLPMT_pulse_buffer(buffer_after_merge);
    LogInfo<<"pulse size after merge:"<<buffer_after_merge.size()<<endl;
    pulse_size = buffer_after_merge.size();
    return pulse_size;
}
////////////////////WP pulse///////////////////////////
int PMTSimAlg::generate_wp_pulses(JM::SimEvt* evt, TimeStamp curTimeStamp)
{
    std::deque<Pulse>& WPLPMT_pulse_buffer = m_elecsvc->get_WPLPMT_pulse_buffer();
    TimeStamp stop_time = m_elecsvc->get_stop_time();
    int pulses_sum = 0; // generated pulses

    const auto& wp_hits = evt->getWPHitsVec();

    EventKeeper& keeper = EventKeeper::Instance();
    const EventKeeper::Entry& entry = keeper.current_entry();
    shared_ptr<EventKeeper::Entry> entry_p{new EventKeeper::Entry};
    if(m_savetruthsMap[entry.tag])
    {
        *entry_p = entry;
    }


    for (auto it_hit = wp_hits.begin();
         it_hit != wp_hits.end(); ++it_hit) {

        auto hit=*it_hit;
        int hit_pmtid = hit->getPMTID();
        TimeStamp hitTimeStamp = curTimeStamp;
        double hit_amplitude = 0;

        double m_efficiency = 0;
        double m_afterPulseProb = 0;
        double m_timeSpread = 0;
        double m_timeOffset = 0;
        double m_gain = 0;
        double m_sigmaGain =0;

        if(PMT::IsWP(hit_pmtid)){ // for WP Lpmts
            int param_pmt_id = hit_pmtid; // - kOFFSET_WP_PMT;
            //LPMT data service
            if(m_enableAssignGain){
                m_gain = m_Gain;
            }else{
                m_gain = m_pmtsimsvc->get_gain(param_pmt_id);
            }
            if(m_enableAssignSigmaGain){
                m_sigmaGain = m_SigmaGain;
            }else{
                m_sigmaGain = m_pmtsimsvc->get_sigmaGain(param_pmt_id);
            }

            m_efficiency = m_pmtsimsvc->get_pde(param_pmt_id);
            m_afterPulseProb = m_pmtsimsvc->get_afterPulseProb(param_pmt_id);
            m_timeSpread = m_pmtsimsvc->get_tts(param_pmt_id);
            m_timeOffset = m_pmtsimsvc->get_timeOffset(param_pmt_id);

            // here, the hit time is not the absolute timestamp
            double hit_hitTime = hit->getHitTime() + gRandom->Gaus(0,1) * (m_timeSpread/2.355) + m_timeOffset;
            hitTimeStamp.Add(hit_hitTime*1e-9); // ns -> s  TTS + timeoffset


            double hit_weight = hit->getNPE();
            // add QE affect.
            if(m_enableEfficiency){
                if(gRandom->Rndm() > m_efficiency){
                    continue;    //ignore SimHit due to efficiency
                }
            }

            m_gain = m_pmtsimsvc->get_gain(param_pmt_id);
            m_sigmaGain = m_pmtsimsvc->get_sigmaGain(param_pmt_id);

            // single PE sigmaGain
            if(!m_enableChargeSmearing)
                m_sigmaGain = 0;
            hit_amplitude =  PulseAmp(hit_weight,m_gain,m_sigmaGain);
        }

        IPMTTool* tool = 0;
        if (PMT::IsWP(hit_pmtid)) { //WP LPMT
            tool = m_pmt_tool[kPMT_WP];
        } else {
            LogWarn << "Unknown pmtid: " << hit_pmtid << std::endl;
            continue;
        }
 	if(hitTimeStamp > stop_time) continue; 
        Pulse pulse = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
        // LogWarn << "pulse entry: " << pulse.m_entry.entry << std::endl;
        pulse.evtTimeStamp=curTimeStamp;
    	if(pulse.truth)
    	{
		pulse.truth->npe = hit->getNPE();
                pulse.truth->hitTime = hit->getHitTime();
		pulse.truth->timeWindow = hit->getTimeWindow();
                pulse.truth->m_entry_pointer = entry_p;
	}

        if (PMT::IsWP(hit_pmtid)) { //WP LPMT
            WPLPMT_pulse_buffer.push_back(pulse);
        } else {
            LogWarn << "Unknown pmtid: " << hit_pmtid << std::endl;
            continue;
        }
   
        //add afterPulse   
        if(m_enableAfterPulse&&PMT::IsWP(hit_pmtid)){// WP LPMT for now
            if(gRandom->Rndm() < m_afterPulseProb){
                double current_rand = gRandom -> Rndm();
                double delta_afterPulseTime = ConvertPdfRand01(current_rand,m_afterPulsePdf,m_afterPulseEdges);
                if(delta_afterPulseTime>10000){
                    delta_afterPulseTime = 10000;
                }
                hitTimeStamp.Add(delta_afterPulseTime * 1e-9);
                hit_amplitude = PulseAmp(hit_amplitude,m_gain,m_sigmaGain) ;
 		if(hitTimeStamp > stop_time) continue; 
                Pulse after_pulse = tool->generate(hit_pmtid, hitTimeStamp, hit_amplitude, m_savetruthsMap[entry.tag]);
		after_pulse.type = kAfterPulse;
                WPLPMT_pulse_buffer.push_back(after_pulse);
            }
        }
        ++pulses_sum;
    }
    return pulses_sum;

}

int
PMTSimAlg::generate_wp_dark_pulses(TimeStamp starttime,TimeStamp endtime)
{
    std::deque<Pulse>& WPLPMT_pulse_buffer = m_elecsvc->get_WPLPMT_pulse_buffer();
    TimeStamp stop_time = m_elecsvc->get_stop_time();

    int pulses_sum = 0;

    starttime.Subtract(200*1e-9);
    endtime.Add(800*1e-9);


    TimeStamp delta_Time = endtime - starttime ; 

    double deltaSimTime = delta_Time.GetSeconds();


    int dark_num_total = 0;
    for (int pmtid = kOFFSET_WP_PMT; pmtid < kOFFSET_WP_PMT + m_total_WPLpmt; ++pmtid) {    
        int Ndark = 0;
        int param_pmt_id = pmtid; // -kOFFSET_WP_PMT; // TODO: WP PMT ID is already in SimSvc
        if(m_enableAssignDarkPulse) { Ndark = PoissonRand(m_darkRate_WP *deltaSimTime* m_darkRateScale_WP);}                    
        else { Ndark = PoissonRand(( m_pmtsimsvc->get_dcr(param_pmt_id))*deltaSimTime* m_darkRateScale_WP);}

        for (int dummy = 0; dummy < Ndark; ++dummy) {
            double amplitude = 0.0;
            double darkPulseTime = gRandom->Rndm() * (deltaSimTime*1e9);
            TimeStamp pulseHitTime = starttime;
            pulseHitTime.Add(darkPulseTime*1e-9) ;
            
            double m_gain = m_pmtsimsvc->get_gain(param_pmt_id);
            double m_sigmaGain = m_pmtsimsvc->get_sigmaGain(param_pmt_id);
            if(!m_enableChargeSmearing)
                m_sigmaGain = 0;
            amplitude = PulseAmp(1.0, m_gain, m_sigmaGain);
 
            IPMTTool* tool = 0;
            tool = m_pmt_tool[kPMT_WP];
	    if(pulseHitTime > stop_time) continue;
            Pulse dark_pulse = tool->generate(pmtid, pulseHitTime, amplitude, m_enableSaveDarkPulseTruths);
	    dark_pulse.type = kDarkPulse;
            WPLPMT_pulse_buffer.push_back(dark_pulse);
        }   
        
        dark_num_total += Ndark;
    }

    return dark_num_total;
}
int PMTSimAlg::merge_wp_LPMT_pulses(){
    int pulse_size = 0;
    std::deque<Pulse>& buffer = m_elecsvc->get_WPLPMT_pulse_buffer();
    std::deque<Pulse> buffer_after_merge;
    buffer_after_merge.clear();
    pulse_size = buffer.size();
    LogInfo<<"pulse size before merge:"<<pulse_size<<endl;
    std::deque<Pulse>::iterator it;
    for ( it=buffer.begin(); it != buffer.end(); ++it) {
        if(buffer[1] != buffer[0]){
            buffer_after_merge.push_back(buffer[0]);
        }else{
            buffer[1] += buffer[0];
        }
        buffer.pop_front();
    }
    m_elecsvc->set_WPLPMT_pulse_buffer(buffer_after_merge);
    LogInfo<<"wp pulse size after merge:"<<buffer_after_merge.size()<<endl;
    pulse_size = buffer_after_merge.size();
    return pulse_size;
}

//////////////////////////////////////

int 
PMTSimAlg::generate_tt_pulses(JM::SimEvt* evt,TimeStamp curTimeStamp)
{
    std::deque<Pulse>& TT_pulse_buffer = m_elecsvc->get_TT_pulse_buffer();

    //Clear pulse buffers, not perfect, but this helps to remove the untriggered pulses that are not cleared by Trigger Handler. 
    TT_pulse_buffer.clear();

    TimeStamp stop_time = m_elecsvc->get_stop_time();
    int pulses_sum = 0; // generated pulses

  //Integrate TTDigitization class
    bool Execute = TTDigit->initialize(evt, CTchannels);

    vector<int> hitChannel = TTDigit->get_channelID();
    vector<int> hitADC = TTDigit->get_adc();
    vector<int> hitTime = TTDigit->get_time();
    vector<float> hitNPE = TTDigit->get_npe();
    auto NChannels = TTDigit->get_TotalChannels();

    EventKeeper& keeper = EventKeeper::Instance();
    const EventKeeper::Entry& entry = keeper.current_entry();
    shared_ptr<EventKeeper::Entry> entry_p{new EventKeeper::Entry};
    if(m_savetruthsMap[entry.tag])
    {
        *entry_p = entry;
    }

    for(int ich =0; ich<NChannels; ich++)
      {
        int hit_pmtid = hitChannel[ich];
        int hit_ADC = hitADC[ich];
        int hit_Time = hitTime[ich];

        TimeStamp hitTimeStamp = curTimeStamp;

        hitTimeStamp.Add(hit_Time*1e-9); // ns -> s

        IPMTTool* tool = m_pmt_tool[kPMT_TT];

        if(hitTimeStamp > stop_time) continue;
        Pulse pulse = tool->generate(hit_pmtid, hitTimeStamp, hit_ADC, m_savetruthsMap[entry.tag]);
        pulse.evtTimeStamp=curTimeStamp;
        if(pulse.truth)
          {
            pulse.truth->npe=hitNPE[ich];
            pulse.truth->hitTime=hitTime[ich];
            pulse.truth->m_entry_pointer=entry_p;
          }
        TT_pulse_buffer.push_back(pulse);
        ++pulses_sum;
      }

    return pulses_sum;
}

std::shared_ptr<JM::EvtNavigator> PMTSimAlg::get_one_event()
{
    LogInfo << "************ getting a non-SN event!!! " << std::endl;

    // choose one type of data

    // in this prototype, we just load hits from buffer
    // without mixing any other events.

    //    string sample = "default";
    string sample;

    double ranNum = gRandom->Uniform(m_totalRate);
    double sumRate = 0;

    for(map<string,double>::iterator it=m_rateMap.begin(); it!=m_rateMap.end(); it++){
        sumRate += it->second;
        if(ranNum<sumRate){
            sample = it->first;
            break;
        }
    }
    if(m_savetruthsMap[sample]) m_current_event.tag = sample;

    if( m_firstMap[sample] ){
        int entries = m_incidentMap[sample]->getEntries();

        int num = 0;

        if (m_startIndexMap.count(sample)) {
            num = m_startIndexMap[sample];
        }
        // else {
        //     num = gRandom->Integer(entries-1);
        // }

        // if the num is too large, go back to the first entry
        if (num >= entries) {
            LogWarn << "The start index " << num << " >= " 
                    << entries << " total events for tag " << sample
                    << ". Reset to the first event."
                    << std::endl;
            num = 0;
        }


        LogInfo << "TTTTTTTTTTTTTTTTTTTTTTTTTTTT "
                << "initial selected sample: "<<sample <<", sample entries: " <<entries
                << " start index: " << num
                << std::endl;

        //LogInfo << "sample: " << sample <<" first evt starts index: "<<num<<endl;
        m_incidentMap[sample]->reset(num);//we will read data from this entry
        m_firstMap[sample] = false; //for each sample just need set one time
    }


    //LogInfo << " m_incidentMap[" << sample << "]->fire() begin. " << std::endl;

    // In order to handle the missing event, we need to catch the Sniper Incident.
    try {
        m_incidentMap[sample]->fire(*dynamic_cast<Task*>(getRoot()));
    } catch (StopRunThisEvent& e) {
        return 0;
    } catch (StopRunProcess& e) {
        return 0;
    }
    //LogInfo << " m_incidentMap[" << sample << "]->fire() end. " << std::endl;

    // get current entry
    // m_current_event.entry = m_incidentMap[sample]->getEntry();
    // LogWarn << "current entry: " << m_current_event.entry << std::endl;
    Task* task = m_taskObjMap[sample];
    if (getRoot()->Snoopy().state()==Sniper::RunState::Stopped
        || task->Snoopy().state()==Sniper::RunState::Stopped) {
        LogWarn << "task stopped." << std::endl;
        LogWarn << "top task " << " state: " << static_cast<int>(getRoot()->Snoopy().state()) << std::endl;
        LogWarn << "task " << sample << " state: " << static_cast<int>(task->Snoopy().state()) << std::endl;
        return 0;
    }
    if(m_savetruthsMap[sample]) m_current_event.filename = m_incidentMap[sample]->getFileName();

    std::string path = "/Event"; 

    LogDebug << "SniperDataPtr path: " << path << std::endl;
    SniperDataPtr<JM::NavBuffer> navBuf(task,path); 
    if (navBuf.invalid()) {
        LogError << "Can't locate data: " << path << std::endl;
        return 0;
    }
    if (!navBuf->size()) {
        LogWarn << "Can't load more events in " << sample << std::endl;
        return 0;
    }

    if (!navBuf->curEvt()) {
        LogWarn << "Can't load more events in " << sample << std::endl;
        return 0;
    }

    if(m_savetruthsMap[sample]) m_current_event.evtnav = *navBuf->current();
    JM::EvtNavigator* Nav = navBuf->curEvt();

    // FIXME
    // To get the real entry number, try to get from SmartRef.
    const std::vector<JM::SmartRef*> refs = Nav->getRef();
    const std::vector<std::string>& paths = Nav->getPath();
    if (refs.size() == 0) {
        LogWarn << "Can't find any SmartRef." << std::endl;
    } else {
        for (int i = 0; i < refs.size(); ++i) {
            const std::string& p = paths[i];
            if (p != "/Event/Sim") {
                continue;
            }

            const JM::SmartRef* ref = refs[i];
            const Long64_t& ref_entry = ref->entry();

            if(m_savetruthsMap[sample]) m_current_event.entry = ref_entry;
        }
    }

    if(m_savetruthsMap[sample]) m_current_event.header = JM::getHeaderObject<JM::SimHeader>(Nav);
    //    EventKeeper& keeper = EventKeeper::Instance();
    //    keeper.set_current_entry(m_current_event);
    vec_current_event.push_back(m_current_event); 
    //LogInfo << "CURRENT EVENT: [" << m_current_event.tag << "] "
    //        << m_current_event.filename
    //        << " "
    //        << m_current_event.entry
    //        << std::endl;


    //return Nav;
    return *navBuf->current();
}


std::shared_ptr<JM::EvtNavigator> PMTSimAlg::get_one_SN_event()
{
    LogInfo << "************ getting a SN event!!! " << std::endl;
    string sample = m_SN_sampleName;

    if(sample == "") { LogError << " No SN input file!! " << std::endl; return 0; }

    if(m_savetruthsMap[sample]) m_current_event.tag = sample;

    if( m_firstMap[sample] ){
        int entries = m_incidentMap[sample]->getEntries();
        //LogInfo<<"initial selected sample: "<<sample <<", sample entries: " <<entries<<endl;

        int num = 0;

        if (m_startIndexMap.count(sample)) {
            num = m_startIndexMap[sample];
        }
        // else {
        //     num = gRandom->Integer(entries-1);
        // }

        //LogInfo << "sample: " << sample <<" first evt starts index: "<<num<<endl;
        m_incidentMap[sample]->reset(num);//we will read data from this entry
        m_firstMap[sample] = false; //for each sample just need set one time
    }


    //LogInfo << " m_incidentMap[" << sample << "]->fire() begin. " << std::endl;

    // In order to handle the missing event, we need to catch the Sniper Incident.
    try {
        m_incidentMap[sample]->fire(*dynamic_cast<Task*>(getRoot()));
    } catch (StopRunThisEvent& e) {
        return 0;
    } catch (StopRunProcess& e) {
        return 0;
    }
    //LogInfo << " m_incidentMap[" << sample << "]->fire() end. " << std::endl;

    // get current entry
    // m_current_event.entry = m_incidentMap[sample]->getEntry();
    // LogWarn << "current entry: " << m_current_event.entry << std::endl;
    Task* task = m_taskObjMap[sample];
    if (getRoot()->Snoopy().state()==Sniper::RunState::Stopped
        || task->Snoopy().state()==Sniper::RunState::Stopped) {
        LogWarn << "task stopped." << std::endl;
        LogWarn << "top task " << " state: " << static_cast<int>(getRoot()->Snoopy().state()) << std::endl;
        LogWarn << "task " << sample << " state: " << static_cast<int>(task->Snoopy().state()) << std::endl;
        return 0;
    }
    if(m_savetruthsMap[sample]) m_current_event.filename = m_incidentMap[sample]->getFileName();

    std::string path = "/Event"; 

    LogDebug << "SniperDataPtr path: " << path << std::endl;
    SniperDataPtr<JM::NavBuffer> navBuf(task,path); 
    if (navBuf.invalid()) {
        LogError << "Can't locate data: " << path << std::endl;
        return 0;
    }
    if (!navBuf->size()) {
        LogWarn << "Can't load more events in " << sample << std::endl;
        return 0;
    }

    if (!navBuf->curEvt()) {
        LogWarn << "Can't load more events in " << sample << std::endl;
        return 0;
    }

    if(m_savetruthsMap[sample]) m_current_event.evtnav = *navBuf->current();
    JM::EvtNavigator* Nav = navBuf->curEvt();

    // FIXME
    // To get the real entry number, try to get from SmartRef.
    const std::vector<JM::SmartRef*> refs = Nav->getRef();
    const std::vector<std::string>& paths = Nav->getPath();
    if (refs.size() == 0) {
        LogWarn << "Can't find any SmartRef." << std::endl;
    } else if (refs.size() == 1) {
        const std::string& p = paths[0];
        if (p != "/Event/Sim") {
            LogWarn << "The ref path is not /Event/Sim" << std::endl;
        }

        const JM::SmartRef* ref = refs[0];
        const Long64_t& ref_entry = ref->entry();
        if(m_savetruthsMap[sample]) m_current_event.entry = ref_entry;

    } else {
        LogWarn << "There are several SmartRefs in the EvtNavigator."
                << "We will try to find the /Event/Sim."
                << std::endl;

        for (int i = 0; i < refs.size(); ++i) {
            const std::string& p = paths[i];
            if (p != "/Event/Sim") {
                continue;
            }

            const JM::SmartRef* ref = refs[i];
            const Long64_t& ref_entry = ref->entry();

            if(m_savetruthsMap[sample]) m_current_event.entry = ref_entry;
        }
    }

    if(m_savetruthsMap[sample]) m_current_event.header = JM::getHeaderObject<JM::SimHeader>(Nav);
    vec_current_SNevent.push_back(m_current_event); 


    //return Nav;
    return *navBuf->current();
}

std::shared_ptr<JM::EvtNavigator> PMTSimAlg::get_one_Laser_event()
{
    LogInfo << "************ getting a Laser event!!! " << std::endl;
    string sample = m_Laser_sampleName;

    if(sample == "") { LogError << " No Laser input file!! Put 'Laser' tag for Laser events." << std::endl; return 0; }

    if(m_savetruthsMap[sample]) m_current_event.tag = sample;

    if( m_firstMap[sample] ){
        int entries = m_incidentMap[sample]->getEntries();
        //LogInfo<<"initial selected sample: "<<sample <<", sample entries: " <<entries<<endl;

        int num = 0;

        if (m_startIndexMap.count(sample)) {
            num = m_startIndexMap[sample];
        }
        // else {
        //     num = gRandom->Integer(entries-1);
        // }

        //LogInfo << "sample: " << sample <<" first evt starts index: "<<num<<endl;
        m_incidentMap[sample]->reset(num);//we will read data from this entry
        m_firstMap[sample] = false; //for each sample just need set one time
    }


    //LogInfo << " m_incidentMap[" << sample << "]->fire() begin. " << std::endl;

    // In order to handle the missing event, we need to catch the Sniper Incident.
    try {
        m_incidentMap[sample]->fire(*dynamic_cast<Task*>(getRoot()));
    } catch (StopRunThisEvent& e) {
        return 0;
    } catch (StopRunProcess& e) {
        return 0;
    }
    //LogInfo << " m_incidentMap[" << sample << "]->fire() end. " << std::endl;

    // get current entry
    // m_current_event.entry = m_incidentMap[sample]->getEntry();
    // LogWarn << "current entry: " << m_current_event.entry << std::endl;
    Task* task = m_taskObjMap[sample];
    if (getRoot()->Snoopy().state()==Sniper::RunState::Stopped
        || task->Snoopy().state()==Sniper::RunState::Stopped) {
        LogWarn << "task stopped." << std::endl;
        LogWarn << "top task " << " state: " << static_cast<int>(getRoot()->Snoopy().state()) << std::endl;
        LogWarn << "task " << sample << " state: " << static_cast<int>(task->Snoopy().state()) << std::endl;
        return 0;
    }
    if(m_savetruthsMap[sample]) m_current_event.filename = m_incidentMap[sample]->getFileName();

    std::string path = "/Event"; 

    LogDebug << "SniperDataPtr path: " << path << std::endl;
    SniperDataPtr<JM::NavBuffer> navBuf(task,path); 
    if (navBuf.invalid()) {
        LogError << "Can't locate data: " << path << std::endl;
        return 0;
    }
    if (!navBuf->size()) {
        LogWarn << "Can't load more events in " << sample << std::endl;
        return 0;
    }

    if (!navBuf->curEvt()) {
        LogWarn << "Can't load more events in " << sample << std::endl;
        return 0;
    }

    if(m_savetruthsMap[sample]) m_current_event.evtnav = *navBuf->current();
    JM::EvtNavigator* Nav = navBuf->curEvt();

    // FIXME
    // To get the real entry number, try to get from SmartRef.
    const std::vector<JM::SmartRef*> refs = Nav->getRef();
    const std::vector<std::string>& paths = Nav->getPath();
    if (refs.size() == 0) {
        LogWarn << "Can't find any SmartRef." << std::endl;
    } else if (refs.size() == 1) {
        const std::string& p = paths[0];
        if (p != "/Event/Sim") {
            LogWarn << "The ref path is not /Event/Sim" << std::endl;
        }

        const JM::SmartRef* ref = refs[0];
        const Long64_t& ref_entry = ref->entry();
        if(m_savetruthsMap[sample]) m_current_event.entry = ref_entry;

    } else {
        LogWarn << "There are several SmartRefs in the EvtNavigator."
                << "We will try to find the /Event/Sim."
                << std::endl;

        for (int i = 0; i < refs.size(); ++i) {
            const std::string& p = paths[i];
            if (p != "/Event/Sim") {
                continue;
            }

            const JM::SmartRef* ref = refs[i];
            const Long64_t& ref_entry = ref->entry();

            if(m_savetruthsMap[sample]) m_current_event.entry = ref_entry;
        }
    }

    if(m_savetruthsMap[sample]) m_current_event.header = JM::getHeaderObject<JM::SimHeader>(Nav);
    //    EventKeeper& keeper = EventKeeper::Instance();
    //    keeper.set_current_entry(m_current_event);
    vec_current_event.push_back(m_current_event); 
    //LogInfo << "CURRENT EVENT: [" << m_current_event.tag << "] "
    //        << m_current_event.filename
    //        << " "
    //        << m_current_event.entry
    //        << std::endl;


    //return Nav;
    return *navBuf->current();
}

//cd Lpmt Algorithms
//SPE response
/*double PMTSimAlg::PulseAmp(double weight,double gain, double sigmaGain){
    double  m_speExpDecay = 1.1;
    double  amp;
    double  m_speCutoff = 0.15; 
    double randW = gRandom->Rndm();
    double m_expWeight = 0.01;
    if (randW > m_expWeight || weight >1.1){
        amp = gRandom->Gaus(0,1) * sigmaGain * TMath::Sqrt(weight) + gain * weight;     
    }   
    else {
        amp = (gRandom->Exp(m_speExpDecay) + m_speCutoff) * gain * weight;
    }   
    if(amp<0) amp = 0;
    
    return amp;
}

double PMTSimAlg::PulseAmpMCP(double weight,double gain, double sigmaGain,double exp_frac){

    double amp;
    double m_speCutoff = 0.1;
    double randW = gRandom->Rndm();
    //enable large signal.
    if (!m_enableMCPLargeSignal ){ randW = 1.01; }
    if(randW > exp_frac){
        amp = gRandom->Gaus(0,1) * sigmaGain * TMath::Sqrt(weight) + gain * weight;
    }
    else {
        amp = (gRandom->Exp(2.2) + m_speCutoff) * gain * weight;
    }
    if(amp<0) amp = 0;
    return amp;
}*/

double PMTSimAlg::PulseAmp(double weight,double gain, double sigmaGain){
    double m_speExpDecay = 1.1;
    double m_expWeight = 0.01;
    double m_speCutoff = 0.15; 
    double Amp = 0;
    for(int i = 0; i < int(weight); i++)
    {
    		double randW = gRandom->Rndm();
		double amp;
    		if (randW > m_expWeight){
        		amp = gRandom->Gaus(0,1) * sigmaGain + gain * 1;     
    		}   
    		else {
        		amp = (gRandom->Exp(m_speExpDecay) + m_speCutoff) * gain * 1;
    		}   
    		if(amp<0) amp = 0;
		Amp = Amp + amp;
    }
    return Amp;

}

double PMTSimAlg::PulseAmpMCP(double weight,double gain, double sigmaGain,double exp_frac){

    double Amp = 0;
    double m_speCutoff = 0.1;
    //enable large signal.
    if (m_enableMCPLargeSignal )
    {
	for(int i = 0; i < int(weight); i++)
    	{
		double randW = gRandom->Rndm();
		double amp;
    		if(randW > exp_frac){
        		amp = gRandom->Gaus(0,1) * sigmaGain  + gain * 1;
    		}
    		else {
        		amp = (gRandom->Exp(2.2) + m_speCutoff) * gain * 1;
    		}
    		if(amp<0) amp = 0;
		Amp = Amp + amp;
	}		
    }
    else
    {
	for(int i = 0; i < int(weight); i++)
	{
		double amp = gRandom->Gaus(0,1) * sigmaGain  + gain * 1;
		if(amp<0) amp = 0;
		Amp = Amp + amp;
	}
    }
    return Amp;
}

double PMTSimAlg::ConvertPdfRand01(double rand,vector<double> pdf, vector<double> edges){
    // Defined PDF returns random number in [0,1] distributed according to user-defined histogram.
    // It assumes even bin sizes, so accomodate uneven bin sizes for generality. 
    int current_bin = 0;
    int Nbins = pdf.size();

    for(int bin=0; bin<Nbins; bin++) {
        if(rand >= pdf[bin] && rand < pdf[bin+1]) {
            current_bin = bin;
            break;
        }
        else
            current_bin = Nbins-1;
    }


    return edges[current_bin] + (rand-pdf[current_bin])*(edges[current_bin+1]-edges[current_bin])
        /(pdf[current_bin+1]-pdf[current_bin]);

}    

int PMTSimAlg::PoissonRand(double mean) {

    int n;
    if (mean <= 0) return 0;

    double expmean = exp(-mean);
    double pir = 1;
    n = -1;
    while(1) {
        n++;
        pir *= gRandom->Rndm();
        if (pir <= expmean) break;
    }
    return n;
}


// tts&tt v.s. theta on PMT:  angular unit->degree
double PMTSimAlg::TTSDynode(double theta, double tts) {
    double theta_cut = 79; //degree
    double m_tts;
    if ( theta<= theta_cut) {
        m_tts = tts;
    } else {
        m_tts = tts + 0.79*(theta-theta_cut)/(85-theta_cut);
    }
    return m_tts;
}


double PMTSimAlg::TTSMCP(double theta, double tts) { 
    double p0 = tts; double p1 = -4.24e-03; double p2 = 3.92e-04;
    double m_tts;
    
    return p0+p1*theta+p2*theta*theta;
}


double PMTSimAlg::ToffsetDynode(double theta, double tt) {
    double theta_cut = 79; //degree
    double m_tt;
    if ( theta<=theta_cut  ) {
        m_tt = tt;
    } else {
        m_tt = tt + 2.42/(85-theta_cut)*(theta-theta_cut);
    }
    return m_tt;
}

double PMTSimAlg::ToffsetMCP(double theta, double tt) {
    return tt+gTT_MCP->Eval(theta);
}

void PMTSimAlg::check_subdet_hits(std::shared_ptr<JM::EvtNavigator> Nav)
{
	bool do_wpsim = m_enableWP;
	bool do_ttsim = m_enableTT;

	bool nonempty_wphits = false;
	bool nonempty_tthits = false;
	JM::SimHeader* a_SimHeader = JM::getHeaderObject<JM::SimHeader>(Nav.get());
	if(!a_SimHeader)
	{
		LogError << "Can't Get SimHeader in PMTSimAlg::check_subdet_hits, Can't do tt and wp simulation, Please Check... " << std::endl;
		m_do_subdet_sim = false;
		return;
	}
	JM::SimEvt* a_Event = dynamic_cast<JM::SimEvt*>(a_SimHeader->event());
	if(!a_Event)
	{
		LogError << "Can't Get SimEvt in PMTSimAlg::check_subdet_hits, Can't do tt and wp simulation, Please Check... " << std::endl;
		m_do_subdet_sim = false;
		return;
		
	}
	if((a_Event->getWPHitsVec()).size()) nonempty_wphits = true;
	if((a_Event->getTTHitsVec()).size()) nonempty_tthits = true;
	bool to_do_wp_sim = (nonempty_wphits && do_wpsim);
	bool to_do_tt_sim = (nonempty_tthits && do_ttsim);
	m_do_subdet_sim = (to_do_wp_sim || to_do_tt_sim);
}
