//////////////////////////////////
/*
Package for PMT waveform calirabtion, it averages single p.e. waveform, and generate the frequency response to all CD PMTs.
This version is specified for J16v2 and spe samples generated by Xiao.
Each channel has a hit fixed at 300ns, no TTS.
The algorithm is still simple. It only averages spe spectrum triggered at the same time.
Author: yuzy@ihep.ac.cn
Detals in JUNO DocDB-1943
Oct. 13, 2016
*/
//////////////////////////////////
#ifndef PMTCalib_cc
#define PMTCalib_cc
#include "PMTCalib.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperDataPtr.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "Event/CdWaveformHeader.h"

#include <TFile.h>
#include <sstream>
#include <TF1.h>
#include "TVirtualFFT.h"
#include "math.h"
#include "RootWriter/RootWriter.h"
#include <fstream>
using namespace std;

DECLARE_ALGORITHM(PMTCalib);

PMTCalib::PMTCalib(const string& name) : AlgBase(name), m_memMgr(0), m_length(1250), m_step(1), m_totalPMT(17613), m_threshold(20), m_tempH(0), m_store(1)
{
  declProp("TotalPMT", m_totalPMT);
  declProp("CalibFile", m_CalibFile);
  declProp("Threshold", m_threshold);
  declProp("Step", m_step);
  declProp("Length", m_length);
  declProp("Store", m_store);
}

PMTCalib::~PMTCalib()
{
}

bool PMTCalib::initialize()
{
  for (int i = 0; i < 20000; i++) m_stat[i] = 0;
  SniperPtr<RootWriter> m_rw(*getParent(), "RootWriter");
  if (!m_rw.valid()) {
    LogError << "Failed to get RootWriter instance!" << std::endl;
    return false;
  }
  if (m_totalPMT > 17613) {
    LogError << "Only CD 20inch PMT could be processed." << endl;
    return false;
  }
  if (m_step == 2) {
    LogInfo << "The PMT waveform calibration is running. SPE waveform will be "
               "averaged and converted to the frequency domain." << endl;
    for (int i = 0; i < m_totalPMT; i++) {
      // ostringstream out1;
      // out1 << "PMTID_" << i << "_SPERE";
      // m_SPERE[i] = new TH1D(out1.str().c_str(), out1.str().c_str(), 400, 0,
      //                      400);  // specified for J16v2
      // ostringstream out2;
      // out2 << "PMTID_" << i << "_SPEIM";
      // m_SPEIM[i] = new TH1D(out2.str().c_str(), out2.str().c_str(), 400, 0,
      //                      400);  // specified for J16v2
      ostringstream out3;
      out3 << "PMTID_" << i << "_MeanSpec";
      m_meanWaveform[i] = new TH1D(out3.str().c_str(), out3.str().c_str(), m_length, 0, m_length);
      // m_rw->attach("FILE1", m_SPERE[i]);
      // m_rw->attach("FILE1", m_SPEIM[i]);
      m_rw->attach("FILE1", m_meanWaveform[i]);
    }
    ostringstream out4;
    out4 << "PMT_Counter";
    m_Counter = new TH1D(out4.str().c_str(), out4.str().c_str(), m_totalPMT, 0, m_totalPMT);
    m_rw->attach("FILE1", m_Counter);

    ifstream in1(m_CalibFile.c_str());
    for (int i = 0; i < m_totalPMT; i++) {
      if (!in1) {
        LogInfo << "Integral file is not completed. Please check it." << endl;
        LogInfo << "Only " << i + 1 << " PMTs information are there." << endl;
      }
      int id;
      double inte, intesigma;
      in1 >> id >> inte >> intesigma;
      m_inte.insert(pair<int, double>(id, inte));
      m_intesigma.insert(pair<int, double>(id, intesigma));
    }
    in1.close();
  } else if (m_step == 1) {
    LogInfo << "The PMT waveform calibration is running. SPE waveforms will be "
               "integrated and fit, to get a proper cut for step 2" << endl;
    output1.open(m_CalibFile.c_str());
    for (int i = 0; i < m_totalPMT; i++) {
      ostringstream out1;
      out1 << "PMTID_" << i << "_Inte";
      m_Integral[i] = new TH1D(out1.str().c_str(), out1.str().c_str(), 400, 0, 5000);  // specified for J16v2
      m_rw->attach("FILE1", m_Integral[i]);
    }
  } else {
    LogError << "Unknown calibration configuration. Please check it." << endl;
    LogError << "Step 1 means to calibrate the mean waveform integral, for the "
                "s.p.e. selection." << endl;
    LogError << "Step 2 means to average the single p.e. waveform with the "
                "integral cut." << endl;
    return false;
  }
  return true;
}

double PMTCalib::integral(const JM::ElecWaveform& channel)
{  // specified for J16v2
  const auto& adc = channel.adc();
  double baseline = 0;
  double rms = 0;
  for (int i = 0; i < 50 && i < m_length; i++) baseline += adc[i];
  baseline /= 50.;
  // for (int i = 0; i < 50 && i < m_length; i++) {
  //  rms += (adc[i] - baseline) * (adc[i] - baseline);
  //}
  // rms = sqrt(rms);
  double inte = 0;
  for (int i = 50; i < m_length; i++) {
    if (i > 350 && (adc[i] - baseline) < 0) break;
    inte += (adc[i] - baseline);
  }
  return inte;
}

bool PMTCalib::execute()
{
  LogDebug << "start PMT waveform unfolding" << endl;
  SniperDataPtr<JM::NavBuffer> navBuf(*getParent(), "/Event");
  auto nav = navBuf->curEvt();
  LogInfo << "EvtNav: " << nav << std::endl;
  auto eh = JM::getHeaderObject<JM::CdWaveformHeader>(nav);
  // only use large pmts
  if (!eh->hasEvent()) return true;
  auto ee = dynamic_cast<JM::CdWaveformEvt*>(eh->event());

  const auto& feeChannels = ee->channelData();

  for (auto it = feeChannels.begin(); it != feeChannels.end(); ++it) {
    auto& channel = *(it->second);
    if (channel.adc().size() == 0) {
      continue;
    }
    int pmtID = it->first;
    if (m_step == 1) {
      double inte = integral(channel);
      m_Integral[pmtID]->Fill(inte);
    } else {
      double inte = integral(channel);
      double gain = (m_inte.find(pmtID))->second;
      double gainsigma = (m_intesigma.find(pmtID))->second;
      if (inte > gain - 1.5 * gainsigma && inte < gain + 1.5 * gainsigma) {  // specified for J16v2
        double baseline = 0;
        for (int i = 0; i < 50; i++) baseline += (channel.adc())[i];
        baseline /= 50.;
        // get maxbin
        int maxbin = -1;
        double maxvalue = -1e20;
        for (int i = 0; i < m_length && i < channel.adc().size(); i++) {
          if (channel.adc()[i] > maxvalue) {
            maxbin = i;
            maxvalue = channel.adc()[i];
          }
        }
        // constant ratio timing
        double ratio = baseline + (maxvalue - baseline) / 6.;
        int start = 0;
        for (int i = maxbin; i > maxbin - 50 && i >= 1; i--) {
          if (channel.adc()[i - 1] <= ratio && channel.adc()[i] >= ratio) {
            start = i - 1;
            break;
          } else
            start = 0;
        }
        // LogInfo << start << "\t" << maxbin << endl;
        if (start > 0) {
          m_stat[pmtID] += 1.;
          m_Counter->SetBinContent(pmtID + 1, m_stat[pmtID]);
          double tmp = 0;

          // timing 1
          // for (int i = start - 100; i < start - 100 + m_length; i++) {
          //  if (i >= 0 && i < m_length)
          //    tmp = channel.adc()[i] - baseline;
          //  else
          //    tmp = 0;
          //  m_meanWaveform[pmtID]->SetBinContent(
          //      i - start + 100 + 1,
          //      m_meanWaveform[pmtID]->GetBinContent(i - start + 100 + 1) +
          // tmp);
          //}

          // timing 2
          for (int i = 0; i < m_length; i++) {
            if (start - 100 + i >= 0 && start - 100 + i < m_length)
              tmp = channel.adc()[start - 100 + i] - baseline;
            else
              tmp = 0;
            m_meanWaveform[pmtID]->SetBinContent(i + 1, m_meanWaveform[pmtID]->GetBinContent(i + 1) + tmp);
          }

          // no timing
          // for (int i = 0; i < m_length && i < channel.adc().size(); i++)
          //  m_meanWaveform[pmtID]->SetBinContent(
          //      i + 1, m_meanWaveform[pmtID]->GetBinContent(i + 1) +
          //                 (channel.adc())[i] - baseline);

          // test part
          // for (int i = 0; i < m_length && i < channel.adc().size(); i++)
          //  m_meanWaveform[pmtID]->SetBinContent(
          //      i + 1, /*m_meanWaveform[pmtID]->GetBinContent(i + 1) +*/
          //      (channel.adc())[i] - baseline);
          // TString titlename = Form("maxbin = %d, start = %d", maxbin, start);
          // m_meanWaveform[pmtID]->SetTitle(titlename);
        }
      }
    }
  }
  return true;
}
bool PMTCalib::finalize()
{
  // if (m_step == 1) {
  //  for (int i = 0; i < m_totalPMT; i++) {
  //    if (m_Integral[i]->GetEntries() > 0) {
  //      m_Integral[i]->Fit("gaus", "Q");  // specified for J16v2
  //      TF1* fun1 = m_Integral[i]->GetFunction("gaus");
  //      output1 << i << ' ' << fun1->GetParameter(1) << ' '
  //              << fun1->GetParameter(2) << endl;
  //      delete fun1;
  //    } else {
  //      output1 << i << ' ' << 0 << ' ' << 0 << endl;
  //    }
  //  }
  //  output1.close();
  //} else {
  //  for (int i = 0; i < m_totalPMT; i++) {
  //    m_meanWaveform[i]->Scale(1. / m_stat[i]);
  //    delete TVirtualFFT::GetCurrentTransform();
  //    TVirtualFFT::SetTransform(0);
  //    m_tempH = m_meanWaveform[i]->FFT(m_tempH, "MAG");
  //    std::vector<double> re_full_vec(m_length);
  //    std::vector<double> im_full_vec(m_length);
  //    double* re_full = &re_full_vec.front();
  //    double* im_full = &im_full_vec.front();
  //    for (int j = 0; j < m_length; j++) {
  //      re_full[j] = 0;
  //      im_full[j] = 0;
  //    }
  //    TVirtualFFT* fft = TVirtualFFT::GetCurrentTransform();
  //    fft->GetPointsComplex(re_full, im_full);
  //    for (int j = 0; j < 400 && j < m_length; j++) {
  //      m_SPERE[i]->SetBinContent(j + 1, re_full[j]);
  //      m_SPEIM[i]->SetBinContent(j + 1, im_full[j]);
  //    }
  //  }
  //}

  return true;
}
#endif
