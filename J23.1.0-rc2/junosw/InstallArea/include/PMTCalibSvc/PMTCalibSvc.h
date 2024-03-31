#ifndef PMTCALIBSVC_H
#define PMTCALIBSVC_H

#include "SniperKernel/SvcBase.h"
#include "SniperKernel/SniperPtr.h"
#include "Geometry/RecGeomSvc.h"
#include "TH2.h"

#include <CondDB/ICondDBSvc.h>
#include <CondDB/IRepo.h>
#include <CondDB/ICnvFromPayload.h>
#include <CondObj/CommonCondObj.h>
#include <memory>

class TH1D;
class TFile;

class PMTCalibSvc : public SvcBase {
  public:
  PMTCalibSvc(const std::string& name);
  ~PMTCalibSvc();

  enum chargeAlgType {
    deconvolution,
    simpleIntegral
  };

  bool initialize();
  bool finalize();

  // get function
  /*
  std::map<unsigned int,double>& getRelativeDE();
  std::map<unsigned int,double>& getGain();
  std::map<unsigned int,double>& getTimeOffset();
  std::map<unsigned int,double>& getDarkRate();
  TH1D* getChargeSpec(unsigned int id);
  */
  const std::vector<double>& getRelativeDE();
  const std::vector<double>& getGain();
  const std::vector<double>& getTimeOffset();
  const std::vector<double>& getDarkRate();
  const std::vector<double>& getMeanGain();
  const std::vector<TH1D*>& getChargeSpec();
  const std::vector<double>& getSPMTRelativeDE();
  const std::vector<double>& getSPMTGain();
  const std::vector<double>& getSPMTTimeOffset();
  const std::vector<double>& getSPMTDarkRate();

  const std::vector<double>& getLPMTfilter();
  const std::vector<double>& getLPMTre();
  const std::vector<double>& getLPMTim();
  const std::vector<double>& getLPMTSPEAdcSum();

  const TH2F* getNormNPEMap();

  // set function
  bool setRelativeDE(unsigned int id, double relativede);
  bool setGain(unsigned int id, double gain);
  bool setTimeOffset(unsigned int id, double offset);
  bool setDarkRate(unsigned int id, double darkrate);
  bool setMeanGain(unsigned int id, double meangain);
  bool setChargeSpec(unsigned int id, TH1D* spec);  // get first

  bool setSPMTRelativeDE(unsigned int id, double relativede);
  bool setSPMTGain(unsigned int id, double gain);
  bool setSPMTTimeOffset(unsigned int id, double offset);
  bool setSPMTDarkRate(unsigned int id, double darkrate);

  void setChargeAlgType(int);  // 0:simple integral  other:deconvolution
  
  private:
    CondDB::ICondDBSvc* m_conddb_svc;
    CondObj::Common::CommonCondObj m_Calib_Deconv_Para;
    CondObj::Common::CommonCondObj m_Calib_Coti_Para;
    CondObj::Common::CommonCondObj m_Calib_Deconv_SPEs;
    CondObj::Common::CommonCondObj m_Calib_Coti_SPEs;
    CondObj::Common::CommonCondObj m_Calib_Deconv_AvgSpeWave;
    CondObj::Common::CommonCondObj m_Calib_Deconv_Filter;

  int m_totalLPMT;
  int m_readDB;
  int64_t m_cur;
  static const int ThetaPMTNum = 1440;

  bool rewriteFlag;
  std::string hx_DirPath;
  std::string RecMapPath;
  std::string m_CalibFile;
  std::string m_FilterFile;
  chargeAlgType m_algtype;

  /*
  std::map<unsigned int,double> hx_RelativeDEMap;
  std::map<unsigned int,double> hx_GainMap;
  std::map<unsigned int,double> hx_TimeOffsetMap;
  std::map<unsigned int,double> hx_DarkRateMap;
  */
  std::vector<double> hx_RelativeDEV;
  std::vector<double> hx_GainV;
  std::vector<double> hx_TimeOffsetV;
  std::vector<double> hx_DarkRateV;
  std::vector<double> hx_MeanGainV;
  std::vector<TH1D*> hx_ChargeSpecV;

  std::vector<double> SPMT_RelativeDEV;
  std::vector<double> SPMT_GainV;
  std::vector<double> SPMT_TimeOffsetV;
  std::vector<double> SPMT_DarkRateV;

  std::vector<double> LPMT_filterV;
  std::vector<double> LPMT_SPEAdcSumV;
  std::vector<double> LPMT_SPEre;
  std::vector<double> LPMT_SPEim;

  std::unique_ptr<TH2F> NormNPEMap;

  TFile* hx_file;
  // TH1D* hx_ChargeSpec;

  unsigned int hx_id;
  double hx_RelativeDE;
  double hx_Gain;
  double hx_TimeOffset;
  double hx_DarkRate;
  double hx_MeanGain;

  // self defined function
  void ReadTxt();
  void ReadRoot();
  void ReadNPEMap();
  void WriteTxt();
  void WriteRoot();
};
#endif
