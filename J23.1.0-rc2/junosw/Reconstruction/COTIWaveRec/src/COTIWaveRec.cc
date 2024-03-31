///////////////////////////////////////////////
//#
//#      Filename: COTIWaveRec.cc
//#
//#        Author: Xuantong Zhang - zhangxuantong@ihep.ac.cn
//#   Description: Continuous Over-Threshold Integral
//#                Waveform Reconstruction Algorithm
//#        Create: 2020-05-04 20:31:05
//# Last Modified: 2020-05-26 14:50:37
//#
///////////////////////////////////////////////

#ifndef COTIWaveRec_cc
#define COTIWaveRec_cc

#include "COTIWaveRec.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Task.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "SniperKernel/SniperPtr.h"
#include "RootWriter/RootWriter.h"

#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Identifier/CdID.h"
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include "TROOT.h"
#include <iostream>
#include <sstream>
#include <vector>

#include "PMTCalibSvc/PMTCalibSvc.h"
using namespace std;

DECLARE_ALGORITHM(COTIWaveRec);

COTIWaveRec::COTIWaveRec(const string& name) : AlgBase(name), m_memMgr(0), m_totalPMT(17612), m_c(0), m_alg(2), m_bl(32), m_nRms(1.5), m_nBPoints(5), m_nEPoints(3), debug(1)
{
  declProp("TotalPMT", m_totalPMT);
  declProp("Calib", m_c);
  declProp("Algrithm", m_alg);
  declProp("BslBufLen", m_bl);
  declProp("NTimesRMS", m_nRms);
  declProp("NHeadPoints", m_nBPoints);
  declProp("NTailPoints", m_nEPoints);
  declProp("DebugMode", debug);
}

COTIWaveRec::~COTIWaveRec()
{
}

bool COTIWaveRec::initialize()
{
  m_stat = 0;

  amp = new TH1D("amp", "amp", 50, 50, 150);
  blh0 = new TH1D("baseline", "baseline", 200, 0, 200);
  rh0 = new TH1D("rh", "rh", 500, 0, 100);
  blvsr0 = new TH2D("blvsr0", "blvsr0", 200, 0, 200, 500, 0, 100);
  tf = new TF1("tf", "gaus(0)", 50, 150);

  m_memMgr = SniperPtr<IDataMemMgr>(getParent(), "BufferMemMgr").data();
  SniperPtr<RootWriter> svc(getParent(), "RootWriter");

  if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please enalbe it in your job option file." << std::endl;
    return false;
  }

  // prepare PMT gain calib data
  SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
  if (calSvc.invalid()) {
    LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
    return false;
  }
  calSvc->setChargeAlgType(0);
  gain = calSvc->getGain();
  toffset = calSvc->getTimeOffset();
  LogInfo << "Load gain and timeoffset correction successfully" << std::endl;
  if (gain.size() != m_totalPMT) {
    LogError << "Wrong gain size! Size in svc file is " << gain.size() << std::endl;
    return false;
  }
  for (int i = 0; i < 10 && i < m_totalPMT; i++) {
    LogInfo << i << "th PMT Gain = " << gain.at(i) << std::endl;
  }
  // gain.push_back(1);

  m_calib = svc->bookTree(*m_par, "USER_OUTPUT/cotiwaverec", "simple output of calibration data");
  m_calib->Branch("Charge", &m_charge);
  m_calib->Branch("Time", &m_time);
  m_calib->Branch("Width", &m_width);
  m_calib->Branch("PMTID", &m_pmtId);
  m_calib->Branch("TotalPE", &m_totalpe, "TotalPE/F");
  if (debug == 1) {
    debugWave = new TH1D("debugWave", "debugWave", 1250, 0, 1250);
    m_debug = svc->bookTree(*m_par, "USER_OUTPUT/debug", "debug waves");
    m_debug->Branch("BugWaves", "TH1D", &debugWave);
  }

  svc->attach("USER_OUTPUT", blh0);
  svc->attach("USER_OUTPUT", rh0);
  svc->attach("USER_OUTPUT", blvsr0);
  return true;
}

bool COTIWaveRec::execute()
{
  LogDebug << "Start Integral Pmt Rec" << std::endl;
  // preparation of the user output
  m_charge.clear();
  m_time.clear();
  m_width.clear();
  m_pmtId.clear();
  m_totalpe = 0;
  // read the electronics event
  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if (navBuf.invalid()) {
    LogWarn << "can't find the buffer. " << std::endl;
    return false;
  }
  auto nav = navBuf->curEvt();
  LogInfo << "EvtNav: " << nav << std::endl;
  if (!nav) {
    LogWarn << "can't load event navigator." << std::endl;
    return dynamic_cast<Task*>(getRoot())->stop();
  }
  auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
  // no waveform found
  if (!eh) return true;
  // only use large pmts
  if (!eh->hasEvent()) return true;
  auto ee = eh->event();

  const auto& feeChannels = ee->channelData();

  list<JM::CalibPmtChannel*> cpcl;  // CalibPMTChannel list

  for (auto it = feeChannels.begin(); it != feeChannels.end(); ++it) {
    const auto& channel = *(it->second);
    if (channel.adc().size() == 0) continue;

    int pmtID = it->first;
    if (pmtID >= m_totalPMT) continue;
    unsigned int detID = CdID::id(static_cast<unsigned int>(pmtID), 0);
    vector<float> charge;
    vector<float> time;
    vector<float> width;
    double nPE = 0;
    if (m_alg == 1)
      nPE = getREC1(channel, charge, time, width);
    else if (m_alg == 2)
      nPE = getREC2(channel, charge, time, width);
    else {
      LogError << "Wrong algorithm number!" << std::endl;
      return false;
    }

    // correct charge
    if (m_c == 0) {
      nPE /= gain.at(pmtID);
      for (int i = 0; i < charge.size(); i++) {
        charge.at(i) /= gain.at(pmtID);
        time.at(i) -= toffset.at(pmtID);
      }
    }

    double firstHitTime = 0;
    if (charge.size() > 0) firstHitTime = time.at(0);

    // fill user root
    for (int i = 0; i < charge.size(); i++) {
      m_charge.push_back(charge.at(i));
      m_time.push_back(time.at(i));
      m_width.push_back(width.at(i));
      m_pmtId.push_back(detID);
    }
    m_totalpe += nPE;

    // fill data model root
    auto cpc = new JM::CalibPmtChannel;
    cpc->setNPE(nPE);
    cpc->setPmtId(detID);
    cpc->setFirstHitTime(firstHitTime);
    cpc->setTime(time);
    cpc->setCharge(charge);
    cpcl.push_back(cpc);
  }
  m_calib->Fill();

  auto ce = new JM::CdLpmtCalibEvt;
  // JM::CalibHeader* ch = new JM::CalibHeader;
  ce->setCalibPMTCol(cpcl);
  auto ch = new JM::CdLpmtCalibHeader;
  ch->setEvent(ce);

  JM::addHeaderObject(nav, ch);

  ProcInfo_t info;
  gSystem->GetProcInfo(&info);
  double m_memory = info.fMemResident / 1024;
  LogInfo << "Memory cost: " << m_memory << endl;
  LogDebug << "End Integral Pmt Rec" << std::endl;
  return true;
}

bool COTIWaveRec::finalize()
{
  LogInfo << "Total baseline errors: " << m_stat << std::endl;
  return true;
}

double COTIWaveRec::getREC2(const JM::ElecWaveform& channel, std::vector<float>& charge, std::vector<float>& time, std::vector<float>& width)
{
  charge.clear();
  time.clear();
  width.clear();
  const auto& adc = channel.adc();
  if (adc.size() == 0) return 0;
  const int length = adc.size();
  double AC[length];
  double AC2[length];
  vector<double> blBuf;
  vector<double> blBuf2;
  for (int i = 0; i < length; i++) {
    AC[i] = adc.at(i);
    AC2[i] = adc.at(i);
    if (adc.at(i) > 1e9) {
      AC[i] = 0;   // robust for unsigned int adc bug.
      AC2[i] = 0;  // robust for unsigned int adc bug.
    }
    if (i < m_bl)
      blBuf.push_back(adc.at(i));
    else if (i < 2 * m_bl)
      blBuf2.push_back(adc.at(i));
  }
  if (blBuf.size() != m_bl) {
    LogError << "Wrong baseline buffer size!" << std::endl;
    return 0;
  }

  // baseline robust
  double tmpSum1 = 0;
  double tmpSum2 = 0;
  for (int i = 0; i < m_bl; i++) {
    tmpSum1 += blBuf.at(i);
    tmpSum2 += blBuf2.at(i);
  }
  if (tmpSum1 >= tmpSum2) blBuf = blBuf2;

  std::vector<double> HitBTime;
  std::vector<double> HitETime;
  double tmpCharge = 0;

  bool isOverThreshold;
  bool isWave = false;

  double bl = 0;
  double rms = 0;
  for (int i = m_bl; i < length; i++) {
    bl = 0;
    rms = 0;
    // calculate baseline and rms
    for (int j = 0; j < m_bl; j++) bl += blBuf.at(j);
    bl /= 1. * m_bl;
    for (int j = 0; j < m_bl; j++) rms += (blBuf.at(j) - bl) * (blBuf.at(j) - bl);
    rms /= 1. * m_bl;
    rms = sqrt(rms);

    // check points
    if (AC[i] > bl + m_nRms * rms) {
      isOverThreshold = 1;
      for (int j = i - 1; j > i - m_nBPoints; j--) {
        if (AC[j] > bl + m_nRms * rms)
          isOverThreshold = isOverThreshold && 1;
        else
          isOverThreshold = isOverThreshold && 0;
      }
      if (isOverThreshold == true && isWave == false) {
        // start taking data
        isWave = true;
        for (int j = i; j > i - m_nBPoints; j--) tmpCharge += AC[j] - bl;
        HitBTime.push_back(i - m_nBPoints + 1);
      } else if (isOverThreshold == true && isWave == true)
        // continue taking data
        tmpCharge += AC[i] - bl;
      else if (isOverThreshold == false && isWave == true)
        // continue taking data
        tmpCharge += AC[i] - bl;
      else
        continue;
    } else {
      isOverThreshold = 0;
      if (isWave == true) {
        for (int j = i - 1; j > i - m_nEPoints; j--) {
          if (AC[j] <= bl + m_nRms * rms)
            isOverThreshold = isOverThreshold || 0;
          else
            isOverThreshold = isOverThreshold || 1;
        }
        if (isOverThreshold == true)
          // continue taking data
          tmpCharge += AC[i] - bl;
        else {
          // end taking data
          isWave = false;
          HitETime.push_back(i - m_nEPoints + 1);
          for (int j = i; j > i - m_nEPoints; j--) tmpCharge -= AC[j] - bl;
          charge.push_back(tmpCharge / 1000.);
        }
      } else {
        blBuf.erase(blBuf.begin());
        blBuf.push_back(AC[i]);
      }
    }

    // empty charge reg
    if (isWave == false) tmpCharge = 0;

    // robust at last point
    if (i == length - 1 && isWave == true) {
      // LogInfo << "robust at last point!" << endl;
      int tt = 0;
      for (int j = i; j >= 0; j--) {
        tt = j;
        if (AC[j] <= bl + m_nRms * rms)
          tmpCharge -= AC[j] - bl;
        else
          break;
      }
      charge.push_back(tmpCharge / 1000.);
      HitETime.push_back(tt);
    }
  }

  // size check
  if (HitBTime.size() != HitETime.size() || HitBTime.size() != charge.size()) {
    LogError << "Wrong hits size! HitBTime size = " << HitBTime.size() << ", HitETime size = " << HitETime.size() << ", Charge size = " << charge.size() << ", Last hit Time = " << HitBTime.back() << std::endl;
    if (debug == 1) {
      // LogError << "Wrong hits size! AAA" << std::endl;
      for (int i = 0; i < length; i++) debugWave->SetBinContent(i + 1, AC2[i]);
      TString debugTitle = Form("Error: Wrong hits size! HitBTime size = %lu, HitETime size = %lu, Charge size = %lu", HitBTime.size(), HitETime.size(), charge.size());
      debugWave->SetTitle(debugTitle);
      m_debug->Fill();
    }
    return 0;
  }

  double temp = 0;
  if (HitETime.size() == 0) return 0;
  for (int i = 0; i < HitETime.size(); i++) {
    time.push_back(HitBTime.at(i));
    width.push_back(HitETime.at(i) - HitBTime.at(i));
    temp += charge.at(i);
  }
  return temp;
}

double COTIWaveRec::getREC1(const JM::ElecWaveform& channel, std::vector<float>& charge, std::vector<float>& time, std::vector<float>& width)
{
  charge.clear();
  time.clear();
  width.clear();
  const auto & adc = channel.adc();
  if (adc.size() == 0) return 0;

  amp->Reset();
  double AC[adc.size()];
  for (int i = 0; i < adc.size(); i++) {
    AC[i] = adc.at(i);
    if (adc.at(i) > 1e9) AC[i] = 0;  // robust for unsigned int adc bug.
    if (i < 190) amp->Fill(AC[i]);
  }
  amp->Fit("tf", "RLQ");
  double baseline = tf->GetParameter(1);
  double rms = tf->GetParameter(2);
  blh0->Fill(baseline);
  rh0->Fill(rms);
  blvsr0->Fill(baseline, rms);

  double BL = baseline;
  if (baseline < 75) {
    double tmpbl = 0;
    for (int i = 0; i < 50; i++) tmpbl += AC[i];
    tmpbl /= 50.;
    double tmpr = 0;
    for (int i = 0; i < 50; i++) tmpr += (AC[i] - tmpbl) * (AC[i] - tmpbl);
    tmpr /= 50.;
    tmpr = sqrt(tmpr);
    if (tmpr > 10) {
      LogInfo << "Find a baseline error!" << std::endl;
      m_stat++;
    }
    BL = tmpbl;
  }
  for (int i = 0; i < adc.size(); i++) AC[i] -= BL;

  double temp = getNPE(AC, rms, adc.size(), charge, time, width);

  temp /= 1000.;
  for (int i = 0; i < charge.size(); i++) {
    charge.at(i) /= 1000.;
  }

  return temp;
}

double COTIWaveRec::getNPE(double AC[], double rms, int length, std::vector<float>& charge, std::vector<float>& time, std::vector<float>& width)
{
  charge.clear();
  time.clear();
  std::vector<double> HitBTime;
  std::vector<double> HitETime;
  bool isOverThreshold[length];
  for (int i = 0; i < length; i++) {
    if (AC[i] >= m_nRms * rms)
      isOverThreshold[i] = true;
    else
      isOverThreshold[i] = false;
  }

  bool isWave = false;
  for (int i = m_nBPoints - 1; i < length - m_nEPoints + 1; i++) {
    if (isWave == false && isOverThreshold[i] == true) {
      isWave = true;
      for (int j = i - 1; j > i - m_nBPoints; j--) {
        isWave = isWave && isOverThreshold[j];
      }
      if (isWave == true) {
        HitBTime.push_back(i);
      }
    }
    if (isWave == true && isOverThreshold[i] == false) {
      isWave = false;
      for (int j = i + 1; j < i + m_nEPoints; j++) {
        isWave = isWave || isOverThreshold[j];
      }
      if (isWave == false) {
        HitETime.push_back(i);
      }
    }
    if (i == length - m_nEPoints && isWave == true) HitETime.push_back(i);
  }

  if (HitBTime.size() != HitETime.size()) {
    LogInfo << "Errors in charge extraction algorithm! Please check it!" << std::endl;
    return 0;
  }

  double temp = 0;
  double temp2 = 0;
  for (int i = 0; i < HitBTime.size(); i++) {
    temp2 = temp;
    for (int j = HitBTime.at(i) - m_nBPoints + 1; j < HitETime.at(i) + m_nEPoints; j++) {
      temp += AC[j];
    }
    charge.push_back(temp - temp2);
    time.push_back(HitBTime.at(i) - m_nBPoints + 1);
    width.push_back(HitETime.at(i) - HitBTime.at(i) + m_nBPoints + 1);
  }
  return temp;
}

#endif
