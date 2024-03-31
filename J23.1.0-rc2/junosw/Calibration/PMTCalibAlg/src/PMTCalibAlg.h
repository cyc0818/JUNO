#ifndef PMTCalibAlg_h
#define PMTCalibAlg_h

#include <algorithm>
#include <boost/python.hpp>
#include <SniperKernel/AlgBase.h>
#include <BufferMemMgr/IDataMemMgr.h>
#include "Geometry/RecGeomSvc.h"
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TVector3.h"
#include <fstream>
#include <string>
#include "EvtNavigator/NavBuffer.h"

class DataBufSvcV2;
class RecGeomSvc;
class CdGeom;

namespace JM {
class CalibHeader;
class CalibEvent;
class CalibPMTChannel;
}

class PMTCalibAlg : public AlgBase {
  public:
  PMTCalibAlg(const std::string& name);
  ~PMTCalibAlg();

  public:
  bool initialize();
  bool execute();
  bool finalize();

  private:
  JM::NavBuffer* m_buf;

  int m_totalPMT;
  std::string m_CalibFile;
  std::string m_CalibMode;
  int m_spe;
  std::string m_alg;
  double m_PECut;
  double m_DNStart;
  double m_DNEnd;

  // user's definition
  std::ifstream f;
  TFile* specfile;
  TH1F* chargeSpec[20000];
  TH1F* totalCharge;
  TH1F* darkCharge;
  TH1F* totalWaveCount;
  TH1F* pmtWaveCount;
  TH1F* darkWaveCount;

  unsigned int pmtId;
  std::vector<float> time;
  std::vector<float> charge;
  double nPE;
  double firstHitTime;
  double dnPE;
  bool isCalibEvt();

  private:
  std::vector<TVector3> ALL_PMT_pos;
  double Ball_R;
  double PMT_R;
  int num_PMT;
  double ChaCenRec_x;
  double ChaCenRec_y;
  double ChaCenRec_z;
  TH1F* vertexX;
  TH1F* vertexY;
  TH1F* vertexZ;
  TH1F* vertexR;
  TH1F* windowCharge;
  // TH1F* windowCharge_700;
  // TH1F* windowCharge_600;
  // TH1F* windowCharge_500;
  // TH1F* windowCharge_400;
  CdGeom* m_cdGeom;
  double eRec();
  double vRec();
  std::vector<std::tuple<double, double, double, double, double>> FirstHitTime_NPE_PMTPos;
  bool ChargeCenterRec();
  bool ChargeCenterRecCorr();
  bool ChargeCenterFitter(double);
  double m_nPE[20000];
  double m_pe_sum;
  double t_windowCharge;
  int isCounter;

  private:
  std::vector<double> relaDE;
  std::vector<double> gain;
  std::vector<double> toffset;
  std::vector<double> darkrate;
  std::vector<TH1D*> qspe;
};
#endif

