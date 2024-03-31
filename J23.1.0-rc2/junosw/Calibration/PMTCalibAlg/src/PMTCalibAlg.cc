#include "PMTCalibAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "Event/CdLpmtCalibHeader.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/CdID.h"
#include "RootWriter/RootWriter.h"

#include "PMTCalibSvc/PMTCalibSvc.h"
using namespace std;

DECLARE_ALGORITHM(PMTCalibAlg);

PMTCalibAlg::PMTCalibAlg(const std::string& name) : AlgBase(name), m_totalPMT(17612), m_spe(0), Ball_R(19.316), PMT_R(19.5), m_PECut(100000), m_DNStart(20), m_DNEnd(80)
{
  declProp("TotalPMT", m_totalPMT);
  declProp("CalibFile", m_CalibFile);
  declProp("CalibMode", m_CalibMode);
  declProp("SPE", m_spe);
  declProp("Alg", m_alg);
  declProp("PECut", m_PECut);
  declProp("DNStart", m_DNStart);
  declProp("DNEnd", m_DNEnd);
}

PMTCalibAlg::~PMTCalibAlg()
{
}

bool PMTCalibAlg::initialize()
{
  LogInfo << "Iinitializing..." << std::endl;
  isCounter = 0;

  // loading calib mode and data file
  if (m_CalibMode == "calib") {
    LogInfo << "Calib Mode: " << m_CalibMode << std::endl;
    LogInfo << "integral time interval: " << m_DNStart << "ns ~ " << m_DNEnd << "ns" << std::endl;
  } else {
    specfile = new TFile(m_CalibFile.c_str(), "READ");
    f.open(m_CalibMode.c_str());

    if (!specfile) {
      LogError << "cannot get spe spectra file!" << std::endl;
    }
    if (!f) {
      LogError << "cannot get calibration pars file!" << std::endl;
    }

    if (!specfile || !f) {
      return false;
    } else
      LogInfo << "Calib Mode: saving parameters, pars file: " << m_CalibMode << ", spe file: " << m_CalibFile << std::endl;
  }

  // loading buffer
  SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
  if (navBuf.invalid()) {
    LogError << "cannot get the NavBuffer @ /Event" << std::endl;
    return false;
  }
  m_buf = navBuf.data();

  // user data definitions
  SniperPtr<RootWriter> svc(*getRoot(), "RootWriter");
  if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please enalbe it in your job option file." << std::endl;
    return false;
  }

  // reconstruction geometry service
  SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
  if (rgSvc.invalid()) {
    LogError << "Failed to get RecGeomSvc instance!" << std::endl;
    return false;
  }
  m_cdGeom = rgSvc->getCdGeom();

  // Get the geometry service for all PMT;
  // num_PMT = m_cdGeom->findPmtNum();
  num_PMT = m_cdGeom->findPmt20inchNum();
  if (num_PMT == m_totalPMT)
    LogInfo << "Total PMT:" << num_PMT << std::endl;
  else
    LogError << "pmt number initialization is wrong! " << std::endl;
  for (int i = 0; i < num_PMT; i++) {
    unsigned int all_pmt_id = (unsigned int)i;
    Identifier all_id = Identifier(CdID::id(all_pmt_id, 0));
    PmtGeom* all_pmt = m_cdGeom->getPmt(all_id);
    if (!all_pmt) {
      LogError << "Wrong Pmt Id " << i << std::endl;
    }
    TVector3 all_pmtCenter = all_pmt->getCenter();
    ALL_PMT_pos.push_back(Ball_R / PMT_R * all_pmtCenter);
  }
  LogInfo << "Ball_R: " << Ball_R << ", PMT_R: " << PMT_R << std::endl;

  // LED & darkrate calib outputfile
  if (m_CalibMode == "calib") {
    for (int i = 0; i < m_totalPMT; i++) {
      TString chName = Form("ch%d_charge_spec", i);
      chargeSpec[i] = new TH1F(chName, chName, 600, -5, 25);
      svc->attach("FILE2", chargeSpec[i]);
    }
    totalCharge = new TH1F("totalCharge", "totalCharge", m_totalPMT, 0, m_totalPMT);
    darkCharge = new TH1F("darkCharge", "darkCharge", m_totalPMT, 0, m_totalPMT);
    windowCharge = new TH1F("windowCharge", "windowCharge", m_totalPMT, 0, m_totalPMT);
    // windowCharge_700 = new TH1F("windowCharge_700", "windowCharge_700", m_totalPMT, 0, m_totalPMT);
    // windowCharge_600 = new TH1F("windowCharge_600", "windowCharge_600", m_totalPMT, 0, m_totalPMT);
    // windowCharge_500 = new TH1F("windowCharge_500", "windowCharge_500", m_totalPMT, 0, m_totalPMT);
    // windowCharge_400 = new TH1F("windowCharge_400", "windowCharge_400", m_totalPMT, 0, m_totalPMT);
    totalWaveCount = new TH1F("totalWaveCount", "totalWaveCount", 1, 0, 1);
    pmtWaveCount = new TH1F("pmtWaveCount", "pmtWaveCount", m_totalPMT, 0, m_totalPMT);
    darkWaveCount = new TH1F("darkWaveCount", "darkWaveCount", m_totalPMT, 0, m_totalPMT);
    vertexX = new TH1F("vertexX", "vertexX", 200, -20000, 20000);
    vertexY = new TH1F("vertexY", "vertexY", 200, -20000, 20000);
    vertexZ = new TH1F("vertexZ", "vertexZ", 200, -20000, 20000);
    vertexR = new TH1F("vertexR", "vertexR", 100, 0, 20000);
    svc->attach("FILE2", totalCharge);
    svc->attach("FILE2", darkCharge);
    svc->attach("FILE2", windowCharge);
    // svc->attach("FILE2", windowCharge_700);
    // svc->attach("FILE2", windowCharge_600);
    // svc->attach("FILE2", windowCharge_500);
    // svc->attach("FILE2", windowCharge_400);
    svc->attach("FILE2", totalWaveCount);
    svc->attach("FILE2", pmtWaveCount);
    svc->attach("FILE2", darkWaveCount);
    svc->attach("FILE2", vertexX);
    svc->attach("FILE2", vertexY);
    svc->attach("FILE2", vertexZ);
    svc->attach("FILE2", vertexR);
  }

  // reconstruct pmtcalib svc
  SniperPtr<PMTCalibSvc> calSvc(getParent(), "PMTCalibSvc");
  if (calSvc.invalid()) {
    LogError << "Failed to get PMTCalibSvc instance!" << std::endl;
    return false;
  }
  if (m_alg == "deconv")
    calSvc->setChargeAlgType(1);
  else if (m_alg == "simpleinte")
    calSvc->setChargeAlgType(0);
  else {
    LogError << "Wrong Algorithm Setting! Please Check!" << std::endl;
    return false;
  }

  // initialize channel correction data
  relaDE = calSvc->getRelativeDE();
  gain = calSvc->getGain();
  toffset = calSvc->getTimeOffset();
  darkrate = calSvc->getDarkRate();
  qspe = calSvc->getChargeSpec();

  // save merged parameters
  if (m_CalibMode != "calib") {
    unsigned int i = 0;
    double tmp1 = 0;
    double tmp2 = 0;
    double tmp3 = 0;
    while (f >> i >> tmp1 >> tmp2 >> tmp3) {
      calSvc->setRelativeDE(i, tmp1);
      calSvc->setGain(i, tmp2);
      calSvc->setDarkRate(i, tmp3);
      calSvc->setChargeSpec(i, (TH1D*)specfile->Get(Form("ch%d_charge_spec", i)));
    }
    f.close();
  }

  LogInfo << "initialized successfully!" << std::endl;
  return true;
}

bool PMTCalibAlg::execute()
{

  if (m_CalibMode != "calib") {
    LogInfo << "Saving merged parameters and skiping all events" << std::endl;
    return true;
  }

  auto nav = m_buf->curEvt();

  // read CalibHit data
  auto chcol = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);
  const auto& chhlist = chcol->event()->calibPMTCol();

  // int m_channel = 0;
  auto chit = chhlist.begin();

  // fast reconstruction and selection
  // double m_nPE_700[20000] = {0};
  // double m_nPE_600[20000] = {0};
  // double m_nPE_500[20000] = {0};
  // double m_nPE_400[20000] = {0};
  for (int i = 0; i < m_totalPMT; i++) m_nPE[i] = 0;
  m_pe_sum = 0;
  while (chit != chhlist.end()) {
    auto calib = *chit++;
    pmtId = calib->pmtId();
    Identifier id = Identifier(pmtId);
    unsigned int detID = CdID::id(static_cast<unsigned int>(pmtId));
    if (not CdID::is20inch(id)) continue;
    PmtGeom* pmt = m_cdGeom->getPmt(id);
    TVector3 pmtCenter = pmt->getCenter();

    time = calib->time();
    charge = calib->charge();
    nPE = calib->nPE();
    firstHitTime = calib->firstHitTime();
    m_pe_sum += nPE;

    if (nPE > 0) FirstHitTime_NPE_PMTPos.push_back(make_tuple(firstHitTime, nPE, Ball_R / PMT_R * pmtCenter.x(), Ball_R / PMT_R * pmtCenter.y(), Ball_R / PMT_R * pmtCenter.z()));

    for (int j = 0; j < charge.size(); j++) {
      if (time.at(j) > 180 && time.at(j) < 800) m_nPE[CdID::module(id)] += charge.at(j);
      // if (time.at(j) > 180 && time.at(j) < 700) m_nPE_700[CdID::module(id)] += charge.at(j);
      // if (time.at(j) > 180 && time.at(j) < 600) m_nPE_600[CdID::module(id)] += charge.at(j);
      // if (time.at(j) > 180 && time.at(j) < 500) m_nPE_500[CdID::module(id)] += charge.at(j);
      // if (time.at(j) > 180 && time.at(j) < 400) m_nPE_400[CdID::module(id)] += charge.at(j);
    }
  }
  t_windowCharge = 0;
  // double t_windowCharge_700 = 0;
  // double t_windowCharge_600 = 0;
  // double t_windowCharge_500 = 0;
  // double t_windowCharge_400 = 0;
  for (int i = 0; i < m_totalPMT; i++) {
    t_windowCharge += m_nPE[i];
    // t_windowCharge_700 += m_nPE_700[i];
    // t_windowCharge_600 += m_nPE_600[i];
    // t_windowCharge_500 += m_nPE_500[i];
    // t_windowCharge_400 += m_nPE_400[i];
  }
  windowCharge->Fill(t_windowCharge);
  // windowCharge_700->Fill(t_windowCharge_700);
  // windowCharge_600->Fill(t_windowCharge_600);
  // windowCharge_500->Fill(t_windowCharge_500);
  // windowCharge_400->Fill(t_windowCharge_400);

  // selection
  if (isCalibEvt() == false) return true;
  // total evt count
  totalWaveCount->Fill(0.5);

  // fill calibration file
  chit = chhlist.begin();
  FirstHitTime_NPE_PMTPos.clear();
  while (chit != chhlist.end()) {
    auto calib = *chit++;
    pmtId = calib->pmtId();
    Identifier id = Identifier(pmtId);

    unsigned int detID = CdID::id(static_cast<unsigned int>(pmtId));
    if (not CdID::is20inch(id)) continue;

    time = calib->time();
    charge = calib->charge();

    nPE = calib->nPE();
    dnPE = 0;

    // LED & darkrate calib, filling pe to get spe charge spectrum, get each
    // pmt's counts and total counts
    if (m_CalibMode == "calib") {
      pmtWaveCount->Fill(CdID::module(id) + 0.5);
      if (m_spe == 1) {
        for (int i = 0; i < charge.size(); i++) {
          chargeSpec[CdID::module(id)]->Fill(charge.at(i));
          if (time.at(i) > m_DNStart && time.at(i) < m_DNEnd) dnPE += charge.at(i);
        }
        if (dnPE != 0) darkWaveCount->Fill(CdID::module(id) + 0.5);
      } else {
        chargeSpec[CdID::module(id)]->Fill(nPE);
        for (int i = 0; i < charge.size(); i++) {
          if (time.at(i) > m_DNStart && time.at(i) < m_DNEnd) dnPE += charge.at(i);
        }
        if (dnPE != 0) darkWaveCount->Fill(CdID::module(id) + 0.5);
      }
    }
    totalCharge->SetBinContent(CdID::module(id) + 1, totalCharge->GetBinContent(CdID::module(id) + 1) + nPE);
    darkCharge->SetBinContent(CdID::module(id) + 1, darkCharge->GetBinContent(CdID::module(id) + 1) + dnPE);
  }

  LogDebug << "End of the PMT channel correction" << std::endl;

  LogInfo << "Done to read CalibPMT " << std::endl;

  return true;
}

bool PMTCalibAlg::finalize()
{
  return true;
}

bool PMTCalibAlg::isCalibEvt()
{
  ChargeCenterRecCorr();
  double rr = ChaCenRec_x * ChaCenRec_x + ChaCenRec_y * ChaCenRec_y + ChaCenRec_z * ChaCenRec_z;
  rr = sqrt(rr);
  LogInfo << ChaCenRec_x << ", " << ChaCenRec_y << ", " << ChaCenRec_z << ", " << rr << std::endl;
  vertexX->Fill(ChaCenRec_x);
  vertexY->Fill(ChaCenRec_y);
  vertexZ->Fill(ChaCenRec_z);
  vertexR->Fill(rr);
  // if (rr < 625) {
  if (rr < m_PECut) {
    isCounter++;
    LogInfo << "isCounter: " << isCounter << std::endl;
    return true;
  } else
    return false;
}

bool PMTCalibAlg::ChargeCenterRec()
{
  double x_sum = 0;
  double y_sum = 0;
  double z_sum = 0;
  double PE_sum = 0;

  for (int i = 0; i < num_PMT; i++) {
    x_sum = x_sum + ALL_PMT_pos.at(i).X() * m_nPE[i];
    y_sum = y_sum + ALL_PMT_pos.at(i).Y() * m_nPE[i];
    z_sum = z_sum + ALL_PMT_pos.at(i).Z() * m_nPE[i];
    PE_sum += m_nPE[i];
  }

  if (PE_sum == 0) {
    ChaCenRec_x = 0;
    ChaCenRec_y = 0;
    ChaCenRec_z = 0;
  } else {
    ChaCenRec_x = x_sum / PE_sum;
    ChaCenRec_y = y_sum / PE_sum;
    ChaCenRec_z = z_sum / PE_sum;
  }
  return true;
}

bool PMTCalibAlg::ChargeCenterRecCorr()
{
  ChargeCenterRec();

  double ChaCenRec_r;
  ChaCenRec_r = sqrt(ChaCenRec_x * ChaCenRec_x + ChaCenRec_y * ChaCenRec_y + ChaCenRec_z * ChaCenRec_z);
  TVector3 vr_ChaCenRec;
  double ChaCenRec_theta;
  double ChaCenRec_phi;
  if (ChaCenRec_r <= 10000) {
    ChaCenRec_r *= 1.2;
    vr_ChaCenRec.SetXYZ(ChaCenRec_x, ChaCenRec_y, ChaCenRec_z);
    ChaCenRec_theta = vr_ChaCenRec.Theta();
    ChaCenRec_phi = vr_ChaCenRec.Phi();
  } else {
    PMTCalibAlg::ChargeCenterFitter(0.3);
    ChaCenRec_r = sqrt(ChaCenRec_x * ChaCenRec_x + ChaCenRec_y * ChaCenRec_y + ChaCenRec_z * ChaCenRec_z);
    vr_ChaCenRec.SetXYZ(ChaCenRec_x, ChaCenRec_y, ChaCenRec_z);
    ChaCenRec_theta = vr_ChaCenRec.Theta();
    ChaCenRec_phi = vr_ChaCenRec.Phi();
    ChaCenRec_r = (1.24042e+04 - 5.90834e+03 * log((2.01546e+04 - ChaCenRec_r) / 1.70063e+03));  // 10000 - 19500
    ChaCenRec_r *= 1.07143;
  }
  ChaCenRec_x = ChaCenRec_r * TMath::Sin(ChaCenRec_theta) * TMath::Cos(ChaCenRec_phi);
  ChaCenRec_y = ChaCenRec_r * TMath::Sin(ChaCenRec_theta) * TMath::Sin(ChaCenRec_phi);
  ChaCenRec_z = ChaCenRec_r * TMath::Cos(ChaCenRec_theta);

  return true;
}

bool PMTCalibAlg::ChargeCenterFitter(double percent = 0.3)
{
  double x_sum = 0;
  double y_sum = 0;
  double z_sum = 0;
  double PE_sum = 0;

  sort(FirstHitTime_NPE_PMTPos.begin(), FirstHitTime_NPE_PMTPos.end());

  for (int i = 0; i < num_PMT; i++) {
    if (PE_sum > m_pe_sum * percent) break;
    x_sum = x_sum + get<2>(FirstHitTime_NPE_PMTPos.at(i)) * get<1>(FirstHitTime_NPE_PMTPos.at(i));
    y_sum = y_sum + get<3>(FirstHitTime_NPE_PMTPos.at(i)) * get<1>(FirstHitTime_NPE_PMTPos.at(i));
    z_sum = z_sum + get<4>(FirstHitTime_NPE_PMTPos.at(i)) * get<1>(FirstHitTime_NPE_PMTPos.at(i));
    PE_sum = PE_sum + get<1>(FirstHitTime_NPE_PMTPos.at(i));
  }
  ChaCenRec_x = x_sum / PE_sum;
  ChaCenRec_y = y_sum / PE_sum;
  ChaCenRec_z = z_sum / PE_sum;

  return true;
}
