#ifndef MuonWaveRec_h
#define MuonWaveRec_h

#include "SniperKernel/AlgBase.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "TTree.h"
#include "TH1D.h"
#include <vector>

namespace JM{
  class ElecWaveform;
}

class MuonWaveRec: public AlgBase
{

 public:
  MuonWaveRec(const std::string& name);

  ~MuonWaveRec();

  bool initialize();
  bool execute();
  bool finalize();

 private:

  bool waveReco(const JM::ElecWaveform &channel, std::vector<float> &charge, std::vector<float> &time, float &riseTime);
  
  IDataMemMgr* m_memMgr;

  double m_threshold;
  int m_length;
  int m_totalPMT;	

  //user data
  TTree* m_calib;
  std::vector<float> m_charge;
  std::vector<float> m_time;
  std::vector<int> m_pmtId;
  float m_totalpe;

};

#endif
