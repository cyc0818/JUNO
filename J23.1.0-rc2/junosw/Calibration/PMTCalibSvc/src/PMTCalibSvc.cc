#include "PMTCalibSvc/PMTCalibSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SharedElemFactory.h"
#include "SniperKernel/SniperException.h"
#include "TH1.h"
#include "TFile.h"
#include "TSystem.h"
#include "Identifier/Identifier.h"
#include "Identifier/CdID.h"
#include <boost/filesystem.hpp>

#include <fstream>

DECLARE_SERVICE(PMTCalibSvc);
DECLARE_CAN_BE_SHARED(PMTCalibSvc);

PMTCalibSvc::PMTCalibSvc(const std::string& name) : SvcBase(name), m_readDB(0), m_cur(20201218)
{
  // hx_DirPath = "/junofs/users/huangx/HXProject/MyProject/PMTCalibSvc/share";
  // hx_DirPath = "/junofs/users/zhangxt/20inch/rec/deconvolution/runs/pars20200504";
  hx_file = NULL;
  m_algtype = deconvolution;
  m_totalLPMT = 17612;
  declProp("DirPath", hx_DirPath);
  declProp("ReadDB",m_readDB);
  declProp("DBcur",m_cur);
  declProp("Filter", m_FilterFile);
  declProp("CalibFile", m_CalibFile);
}

PMTCalibSvc::~PMTCalibSvc()
{
  if (hx_file != NULL) hx_file->Close();
}

bool PMTCalibSvc::initialize()
{
  LogInfo << "PMTCalibSvc is initializing!" << std::endl;
  // if DirPath is not set, using the default one.
  if (hx_DirPath.empty()) {
      if (getenv("JUNOTOP")) {
          std::string s = getenv("JUNOTOP");
          s += "/data/Calibration/PMTCalibSvc/data";
          if (!gSystem->AccessPathName(s.c_str())) {
              hx_DirPath = s;
          }
      }
      if (getenv("WORKTOP")) {
          std::string s = getenv("WORKTOP");
          s += "/data/Calibration/PMTCalibSvc/data";
          if (!gSystem->AccessPathName(s.c_str())) {
              hx_DirPath = s;
          }
      }
      LogInfo << "The default PMTCalibSvc data path : " << hx_DirPath << std::endl;
  }

    if(getenv("JUNOTOP")) {
        std::string base = getenv("JUNOTOP");
        base += "/data/Reconstruction/OMILREC/RecMap";
        if(!gSystem->AccessPathName(base.c_str())) {
            RecMapPath = base;
        }
    }
    if(getenv("WORKTOP")) {
        std::string base = getenv("WORKTOP");
        base += "/data/Reconstruction/OMILREC/RecMap";
        if(!gSystem->AccessPathName(base.c_str())) {
            RecMapPath = base;
        }
    }


  if (m_readDB==0) {

  } else if (m_readDB==1) {
    LogInfo << "Loading PMTCalibSvc Files from DataBase with timeStamp:"<< m_cur << std::endl;
    SniperPtr<CondDB::ICondDBSvc> conddb(getParent(), "CondDBSvc");
    if (conddb.invalid()) {
        LogError << "Calib parameters Failed to get CondDBSvc!" << std::endl;
    }
    m_conddb_svc = conddb.data();
    if (m_algtype == deconvolution)
    {
      m_conddb_svc->declCondObj("Calib.Deconv.Para", m_Calib_Deconv_Para);
      m_conddb_svc->declCondObj("Calib.Deconv.SPEs", m_Calib_Deconv_SPEs);
      m_conddb_svc->declCondObj("Calib.Deconv.Filter", m_Calib_Deconv_Filter);
      m_conddb_svc->declCondObj("Calib.Deconv.AvgSpeWave", m_Calib_Deconv_AvgSpeWave);
      m_conddb_svc->setCurrent(m_cur);
      LogInfo << "Prepare to read deconv calibration parameter file : " <<  m_Calib_Deconv_Para.path() << std::endl;
      LogInfo << "Prepare to read deconv SPE spectra file : " <<  m_Calib_Deconv_SPEs.path() << std::endl;
    }
    else if(m_algtype == simpleIntegral)
    {
      m_conddb_svc->declCondObj("Calib.Coti.Para", m_Calib_Coti_Para);
      m_conddb_svc->declCondObj("Calib.Coti.SPEs", m_Calib_Coti_SPEs);
      m_conddb_svc->setCurrent(m_cur);
      LogInfo << "Prepare to read Coti calibration parameter file : " <<  m_Calib_Coti_Para.path() << std::endl;
      LogInfo << "Prepare to read Coti SPE spectra file : " <<  m_Calib_Coti_SPEs.path() << std::endl;
    }
  } else {
    LogInfo << "Loading PMTCalibSvc Files from: " << hx_DirPath << std::endl;
  }

  //
  ReadTxt();
  ReadRoot();
  ReadNPEMap();
  rewriteFlag = false;
  return true;
}

bool PMTCalibSvc::finalize()
{
  LogInfo << "PMTCalibSvc is finalizing!" << std::endl;
  // LogDebug << "DE size is:" << hx_RelativeDEV.size() << std::endl;
  // if (hx_RelativeDEV.size() != 0) {
  if (rewriteFlag == true) {
    WriteTxt();
    LogInfo << "Writing TXT successfully!" << std::endl;
    WriteRoot();
    LogInfo << "Writing ROOT successfully!" << std::endl;
  }
  return true;
}

void PMTCalibSvc::ReadTxt()
{
  if (hx_RelativeDEV.size() != 0) {
    hx_RelativeDEV.clear();
    hx_GainV.clear();
    hx_TimeOffsetV.clear();
    hx_DarkRateV.clear();
    hx_MeanGainV.clear();

    SPMT_RelativeDEV.clear();
    SPMT_GainV.clear();
    SPMT_TimeOffsetV.clear();
    SPMT_DarkRateV.clear();
  }
  std::ifstream myinstream;
  std::string fn; // file path to be read

  if(m_readDB==0){
      if (m_algtype == deconvolution) {
          fn = hx_DirPath + "/PmtPrtData_deconv.txt";
      } else if (m_algtype == simpleIntegral) {
          fn = hx_DirPath + "/PmtPrtData_inte.txt";
      }
  } else if(m_readDB==1){
      if (m_algtype == deconvolution) {
          fn = m_Calib_Deconv_Para.path();
      } else if(m_algtype == simpleIntegral) {
          fn = m_Calib_Coti_Para.path();
      }
  }

  myinstream.open(fn.c_str(), std::ifstream::in);


  if (myinstream.is_open()) {
    std::string temps;
    // for (int i=0;i<5;i++) myinstream>>temps;
    while (myinstream >> hx_id) {
      myinstream >> hx_RelativeDE;
      myinstream >> hx_Gain;
      myinstream >> hx_TimeOffset;
      myinstream >> hx_DarkRate;
      myinstream >> hx_MeanGain;
      hx_RelativeDEV.push_back(hx_RelativeDE);
      hx_GainV.push_back(hx_Gain);
      hx_TimeOffsetV.push_back(hx_TimeOffset);
      hx_DarkRateV.push_back(hx_DarkRate);
      hx_MeanGainV.push_back(hx_MeanGain);
    }
    myinstream.close();
  } else {
    LogError << "read file fail!" << std::endl;
  }

  // read SPMT
  fn = hx_DirPath + "/SPmtPrtData.txt";
  myinstream.open(fn.c_str(), std::ifstream::in);
  if (myinstream.is_open()){
    while (myinstream >> hx_id){
      myinstream >> hx_RelativeDE;
      myinstream >> hx_Gain;
      myinstream >> hx_TimeOffset;
      myinstream >> hx_DarkRate;
      SPMT_RelativeDEV.push_back(hx_RelativeDE);
      SPMT_GainV.push_back(hx_Gain);
      SPMT_TimeOffsetV.push_back(hx_TimeOffset);
      SPMT_DarkRateV.push_back(hx_DarkRate);
    }
    myinstream.close();
  } else {
    LogError << "read SPMT file fail!" << std::endl;
  }
}

void PMTCalibSvc::ReadRoot()
{
  if (hx_ChargeSpecV.size() != 0) {
    for (int i = 0; i < hx_ChargeSpecV.size(); i++) {
      hx_ChargeSpecV.at(i)->Delete();
      hx_ChargeSpecV.at(i) = NULL;
    }
    hx_ChargeSpecV.clear();
  }
  if (hx_file == NULL) {
      std::string fn;
      if(m_readDB==0) {
          if (m_algtype == deconvolution) {
              fn = hx_DirPath + "/output_deconv.root";
          } else if (m_algtype == simpleIntegral) {
              fn = hx_DirPath + "/output_inte.root";
          }
      } else if(m_readDB==1){
          if (m_algtype == deconvolution) {
              fn = m_Calib_Deconv_SPEs.path();
          } else if(m_algtype == simpleIntegral) {
              fn = m_Calib_Coti_SPEs.path();
          }
      }
      hx_file = TFile::Open(fn.c_str());
  }

  TH1D* hx_ChargeSpec;
  for (int i = 0; i < m_totalLPMT; i++) {
    hx_ChargeSpec = (TH1D*)hx_file->Get(Form("ch%d_charge_spec", i));
    hx_ChargeSpec->SetDirectory(0);
    hx_ChargeSpecV.push_back(hx_ChargeSpec);
  }
  hx_file->Close();
  hx_file = NULL;
  // LogInfo << hx_ChargeSpecV.at(17000)->GetNbinsX() << std::endl;

  LPMT_SPEre.clear();
  LPMT_SPEim.clear();
  if (m_algtype == deconvolution){
    std::string sw_env = getenv("JUNOTOP");
    std::string SPE_path = sw_env + "/data/Reconstruction/Deconvolution/share/SPE_v20.root";
    if(m_readDB==1) SPE_path = m_Calib_Deconv_AvgSpeWave.path();
    else if(!m_CalibFile.empty()) SPE_path = m_CalibFile;
    hx_file = TFile::Open(SPE_path.c_str());
    if(!hx_file) LogError << "read " << SPE_path << " fail!" << std::endl;
    else{
      LPMT_SPEAdcSumV.clear();
      for(int i=0;i<m_totalLPMT;i++){
          LPMT_SPEAdcSumV.emplace_back(((TH1D*)hx_file->Get( Form("PMTID_%d_MeanSpec",i) ))->Integral());
      }
      TH1D *freq0re = (TH1D*)hx_file->Get( Form("PMTID_%d_SPERE",0) );
      TH1D *freq0im = (TH1D*)hx_file->Get( Form("PMTID_%d_SPEIM",0) );
      const int N_freq = freq0re->GetNbinsX();
      auto re0_arr = freq0re->GetArray();
      auto im0_arr = freq0im->GetArray();
      for(int i=0;i<N_freq;i++){
          LPMT_SPEre.emplace_back(re0_arr[i+1]);
          LPMT_SPEim.emplace_back(im0_arr[i+1]);
      }
      for(int i=1;i<m_totalLPMT;i++){
          TH1D *re = (TH1D*)hx_file->Get( Form("PMTID_%d_SPERE",i) );
          TH1D *im = (TH1D*)hx_file->Get( Form("PMTID_%d_SPEIM",i) );
          auto re_arr = re->GetArray();
          auto im_arr = im->GetArray();
          for(int j=0;j<N_freq;j++){
              LPMT_SPEre.emplace_back(re_arr[j+1]);
              LPMT_SPEim.emplace_back(im_arr[j+1]);
          }
      }
    }
    hx_file->Close();

    LPMT_filterV.clear();
    std::string filter_path = sw_env + "/data/Reconstruction/Deconvolution/share/filter3_m.root";
    if(m_readDB==1) filter_path = m_Calib_Deconv_Filter.path();
    else if(!m_FilterFile.empty()) filter_path = m_FilterFile;
    hx_file = TFile::Open(filter_path.c_str());
    if(!hx_file) LogError << "read " << filter_path << " fail!" << std::endl;
    else{
      TH1D *filter_nnvt = (TH1D*)hx_file->Get("fn0");
      TH1D *filter_hmmt = (TH1D*)hx_file->Get("fh0");
      int N_filter = filter_nnvt->GetNbinsX();
      auto filter_arr_nnvt = filter_nnvt->GetArray();
      auto filter_arr_hmmt = filter_hmmt->GetArray();
      for(int i=0;i<N_filter;i++) LPMT_filterV.emplace_back(filter_arr_nnvt[i+1]);
      for(int i=0;i<N_filter;i++) LPMT_filterV.emplace_back(filter_arr_hmmt[i+1]);
    }

    hx_file->Close();
    hx_file = NULL;
  }
}


void PMTCalibSvc::ReadNPEMap()
{

    double MapThetaStep = 180./double(ThetaPMTNum); //degree

    TH2F* LMu2D[ThetaPMTNum];
    std::string nPEMapPath = RecMapPath + "/nPEMap";
    LogDebug<< nPEMapPath << std::endl;


    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        throw SniperException("Failed to get RecGeomSvc instance");
    }

    if (rgSvc->getCdGeom() == nullptr) {
        LogError << "Please initialize RecGeomSvc instance before PMTCalibSvc!" << std::endl;
        throw SniperException("Wrong initialization order. Need to initialize RecGeomSvc before. ");
    }

    int TotalLPMT = rgSvc->getCdGeom()->findPmt20inchNum();
    CdGeom* m_cdGeom = rgSvc->getCdGeom();
    std::vector<TVector3> ALL_LPMT_pos;
    for(int i = 0; i<TotalLPMT; i++){
        unsigned int all_pmt_id = (unsigned int)i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_LPMT_pos.push_back(all_pmtCenter);
    }

    LogDebug << "Load the 3-D nPEMap, the residual time pdfs and the charge response pdfs" << std::endl;
    bool UseTtuthFile = false;
    std::string LnPEMapFileName = "/LnPEMapFile.root";

    TFile* LnPEMapFile = new TFile((nPEMapPath + LnPEMapFileName).c_str(), "READ");
    if(!LnPEMapFile) LogInfo<< nPEMapPath + LnPEMapFileName  + "is not found!" << std::endl;
    for(int i=0;i<ThetaPMTNum;i++) {
        LMu2D[i] = (TH2F*)LnPEMapFile->Get(Form("hLMu2D_%d", i));
        LMu2D[i]->SetDirectory(nullptr);
    }
    LnPEMapFile->Close();
    delete LnPEMapFile;
    LogDebug<< "LMu2D[ThetaPMTNum-1]->Interpolate(rrange[1], Pi/2) "
            << LMu2D[ThetaPMTNum-1]->Interpolate(179, TMath::Pi()/2.) << std::endl;


    NormNPEMap.reset(new TH2F("", "NormNPEMap", 300,  0, 180., 18, 0, TMath::Pi()));
    NormNPEMap->SetDirectory(0);
    double Ge68ES = 0.;
    for(int ri = 1;ri<=300;ri++) {
        for(int ti = 1;ti<=18;ti++) {

            double RVar = NormNPEMap->GetXaxis()->GetBinCenter(ri);
            if(RVar>180.-1.e-8) RVar=180.-1.e-8;

            double Theta = NormNPEMap->GetYaxis()->GetBinCenter(ti);
            Ge68ES = 0.;
            for(int i = 0; i< TotalLPMT; i++){
                double pmt_pos_x = ALL_LPMT_pos.at(i).X();
                double pmt_pos_y = ALL_LPMT_pos.at(i).Y();
                double pmt_pos_z = ALL_LPMT_pos.at(i).Z();

                double PMTR = TMath::Sqrt(pmt_pos_x*pmt_pos_x + pmt_pos_y*pmt_pos_y + pmt_pos_z*pmt_pos_z);
                double cos_theta = pmt_pos_z/PMTR;
                double theta = TMath::ACos(cos_theta)*180/TMath::Pi();

                int GrMapIdA = theta/MapThetaStep;
                int GrMapIdB = GrMapIdA + 1;

                double m_average_PEA = LMu2D[GrMapIdA]->Interpolate(RVar, Theta);
                double m_average_PEB = LMu2D[GrMapIdB]->Interpolate(RVar, Theta);
                double ThetaAFrac =  theta - double(GrMapIdA)*MapThetaStep;
                double Weight = ThetaAFrac/MapThetaStep;
                double m_average_PE = (1.-Weight)*m_average_PEA + Weight*m_average_PEB;

                Ge68ES +=  m_average_PE;
            }
            NormNPEMap->SetBinContent(ri, ti, Ge68ES);
        }
    }
    Ge68ES = NormNPEMap->Interpolate(1.e-8, 1.e-8);
    LogInfo<< "Ge68ES: " << Ge68ES <<  std::endl;

    NormNPEMap->Scale(1./Ge68ES);

    /*
    TFile* mapcheck = new TFile("NPEmap.root", "RECREATE");
    NormNPEMap->Write();
    mapcheck->Close();
    */

    for(int i=0;i<ThetaPMTNum;i++) {
        delete LMu2D[i];
    }
}


void PMTCalibSvc::WriteTxt()
{
  LogInfo << "write txt.." << std::endl;
  LogDebug << "writing text..." << std::endl;
  std::ofstream myoutstream;
  if (m_algtype == deconvolution)
    myoutstream.open((hx_DirPath + "/PmtPrtData_deconv.txt").c_str(), std::ofstream::out);
  else if (m_algtype == simpleIntegral)
    myoutstream.open((hx_DirPath + "/PmtPrtData_inte.txt").c_str(), std::ofstream::out);
  // myoutstream.open((hx_DirPath + "/PmtPrtData_testTime.txt").c_str(),
  //                 std::ofstream::out);
  // int vsize=hx_RelativeDEV.size();
  int vsize = hx_TimeOffsetV.size();
  for (int i = 0; i < vsize; i++) {
    myoutstream << i << "\t" << hx_RelativeDEV.at(i) << "\t" << hx_GainV.at(i) << "\t" << hx_TimeOffsetV.at(i) << "\t" << hx_DarkRateV.at(i) << "\t" << hx_MeanGainV.at(i) << std::endl;
    // myoutstream << i << hx_TimeOffsetV.at(i) << "    \t" << std::endl;
  }
  myoutstream.close();
}

void PMTCalibSvc::WriteRoot()
{
  // if (hx_file != NULL) hx_file->Close();
  if (m_algtype == deconvolution)
    hx_file = TFile::Open(TString(hx_DirPath + "/output_deconv.root"), "RECREATE");
  else if (m_algtype == simpleIntegral)
    hx_file = TFile::Open(TString(hx_DirPath + "/output_inte.root"), "RECREATE");
  hx_file->cd();
  for (int i = 0; i < m_totalLPMT; i++) {
    hx_ChargeSpecV.at(i)->Write();
  }
  hx_file->Close();
  hx_file = NULL;
}

const std::vector<double>& PMTCalibSvc::getRelativeDE()
{
  return hx_RelativeDEV;
}

const std::vector<double>& PMTCalibSvc::getGain()
{
  return hx_GainV;
}

const std::vector<double>& PMTCalibSvc::getTimeOffset()
{
  return hx_TimeOffsetV;
}

const std::vector<double>& PMTCalibSvc::getDarkRate()
{
  return hx_DarkRateV;
}

const std::vector<double>& PMTCalibSvc::getMeanGain()
{
  return hx_MeanGainV;
}

const std::vector<TH1D*>& PMTCalibSvc::getChargeSpec()
{
  return hx_ChargeSpecV;
}

const std::vector<double>& PMTCalibSvc::getSPMTRelativeDE()
{
  return SPMT_RelativeDEV;
}

const std::vector<double>& PMTCalibSvc::getSPMTGain()
{
  return SPMT_GainV;
}

const std::vector<double>& PMTCalibSvc::getSPMTTimeOffset()
{
  return SPMT_TimeOffsetV;
}

const std::vector<double>& PMTCalibSvc::getSPMTDarkRate()
{
  return SPMT_DarkRateV;
}

const std::vector<double>& PMTCalibSvc::getLPMTfilter(){
  return LPMT_filterV;
}
const std::vector<double>& PMTCalibSvc::getLPMTre(){
  return LPMT_SPEre;
}
const std::vector<double>& PMTCalibSvc::getLPMTim(){
  return LPMT_SPEim;
}
const std::vector<double>& PMTCalibSvc::getLPMTSPEAdcSum(){
  return LPMT_SPEAdcSumV;
}

const TH2F* PMTCalibSvc::getNormNPEMap(){
  return NormNPEMap.get();
}


bool PMTCalibSvc::setRelativeDE(unsigned int id, double relativede)
{
  hx_RelativeDEV.at(id) = relativede;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setGain(unsigned int id, double gain)
{
  hx_GainV.at(id) = gain;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setTimeOffset(unsigned int id, double offset)
{
  hx_TimeOffsetV.at(id) = offset;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setDarkRate(unsigned int id, double darkrate)
{
  hx_DarkRateV.at(id) = darkrate;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setSPMTRelativeDE(unsigned int id, double relativede)
{
  SPMT_RelativeDEV.at(id) = relativede;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setSPMTGain(unsigned int id, double gain)
{
  SPMT_GainV.at(id) = gain;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setSPMTTimeOffset(unsigned int id, double offset)
{
  SPMT_TimeOffsetV.at(id) = offset;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setSPMTDarkRate(unsigned int id, double darkrate)
{
  SPMT_DarkRateV.at(id) = darkrate;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setMeanGain(unsigned int id, double meangain)
{
  hx_MeanGainV.at(id) = meangain;
  rewriteFlag = true;
  return true;
}

bool PMTCalibSvc::setChargeSpec(unsigned int id, TH1D* spec)
{
  hx_ChargeSpecV.at(id) = spec;
  TString name = Form("ch%d_charge_spec", id);
  hx_ChargeSpecV.at(id)->SetName(name);
  rewriteFlag = true;
  return true;
}

void PMTCalibSvc::setChargeAlgType(int temptype)
{  // 0:simple integral  other:deconvolution
  if (temptype == 0) m_algtype = simpleIntegral;
  ReadTxt();
  ReadRoot();
  rewriteFlag = false;
}
