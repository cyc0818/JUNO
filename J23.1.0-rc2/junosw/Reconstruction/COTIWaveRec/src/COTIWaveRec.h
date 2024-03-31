#ifndef COTIWaveRec_h
#define COTIWaveRec_h

#include <boost/python.hpp>
#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include <map>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TTree.h>
#include <vector>
#include <fstream>

namespace JM {
class ElecWaveform;
}

class COTIWaveRec : public AlgBase {
  public:
  COTIWaveRec(const std::string& name);
  ~COTIWaveRec();

  public:
  bool initialize();
  bool execute();
  bool finalize();

  private:
  // alg1
  double getREC1(const JM::ElecWaveform& channel, std::vector<float>& charge, std::vector<float>& time, std::vector<float>& width);
  double getNPE(double AC[], double rms, int length, std::vector<float>& charge, std::vector<float>& time, std::vector<float>& width);

  // alg2
  double getREC2(const JM::ElecWaveform& channel, std::vector<float>& charge, std::vector<float>& time, std::vector<float>& width);

  private:
  IDataMemMgr* m_memMgr;

  int m_stat;
  int m_c;
  int m_alg;
  int m_nBPoints;
  int m_nEPoints;
  int m_bl;
  int m_totalPMT;
  float m_nRms;
  std::vector<double> gain;
  std::vector<double> toffset;

  TH1D* amp;
  TH1D* blh0;
  TH1D* rh0;
  TH2D* blvsr0;
  TF1* tf;

  std::vector<float> m_charge;
  std::vector<float> m_time;
  std::vector<float> m_width;
  std::vector<int> m_pmtId;
  float m_totalpe;

  TTree* m_calib;

  int debug;
  TTree* m_debug;
  TH1D* debugWave;
};

#endif

