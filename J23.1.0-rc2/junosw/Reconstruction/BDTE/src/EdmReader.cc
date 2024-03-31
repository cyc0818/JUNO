#include "EdmReader.h"

#include <Event/CdLpmtCalibHeader.h>
#include <Event/CdSpmtCalibHeader.h>
#include <Event/CdVertexRecEvt.h>
#include <Event/CdVertexRecHeader.h>
#include <Event/HeaderObject.h>
#include <Event/RecVertex.h>
#include <Event/SimEvt.h>
#include <Event/SimHeader.h>
#include <Event/SimTrack.h>
#include <EvtNavigator/EvtNavHelper.h>
#include <Identifier/CdID.h>
#include <Identifier/Identifier.h>
#include <Math/Vector3D.h>
#include <SniperKernel/AlgFactory.h>
#include <TH1F.h>

#include "Utils.h"

namespace BDTE {
DECLARE_ALGORITHM(EdmReader);

namespace p = boost::python;
namespace np = boost::python::numpy;

EdmReader::EdmReader(const std::string& name) : AlgBase(name) { LogDebug << "Building" << std::endl; }

bool EdmReader::initialize() {
  LogDebug << "Initializing" << std::endl;

  _rec_geo_svc = new SniperPtr<RecGeomSvc>(getParent(), "RecGeomSvc");
  _nav_buffer = new SniperDataPtr<JM::NavBuffer>(getParent(), "/Event");
  _pystore = new SniperDataPtr<PyDataStore>(getParent(), "DataStore");

  if (_rec_geo_svc == nullptr || _rec_geo_svc->invalid()) {
    LogError << "Failed to recover a RecGeoSvc instance ! - " << _rec_geo_svc << std::endl;
    return false;
  }

  if (_nav_buffer == nullptr || _nav_buffer->invalid()) {
    LogError << "Failed to recover a NavBuffer instance ! - " << _nav_buffer << std::endl;
    return false;
  }

  if (_pystore == nullptr || _pystore->invalid()) {
    LogError << "Failed to recover a PyDataStore instance ! - " << _pystore << std::endl;
    return false;
  }

  return true;
}

bool EdmReader::execute() {
  LogDebug << "Executing" << std::endl;

  JM::EvtNavigator* evt_nav = (*_nav_buffer)->curEvt();
  PyDataStore* store = _pystore->data();

  LogDebug << "Processing event " << evt_nav->EventID() << std::endl;

  // Fetch data from calibration
  auto calibLpmtHeader = JM::getHeaderObject<JM::CdLpmtCalibHeader>(evt_nav);
  auto calibSpmtHeader = JM::getHeaderObject<JM::CdSpmtCalibHeader>(evt_nav);
  auto simHeader = JM::getHeaderObject<JM::SimHeader>(evt_nav);

  if (calibLpmtHeader == nullptr || calibSpmtHeader == nullptr) {
    LogError << "Cannot retrieve the headers : (LPMT) " << calibLpmtHeader << " | (SPMT) " << calibSpmtHeader
             << std::endl;
    return false;
  }

  if (simHeader == nullptr) {
    LogWarn << "Sim header cannot be retrieved :" << simHeader << " true vertex and energy will not be stored"
            << std::endl;
  }

  auto lpmt_hits = calibLpmtHeader->event()->calibPMTCol();
  auto spmt_hits = calibSpmtHeader->event()->calibPMTCol();
  auto sim_evt = ((JM::SimEvt*)simHeader->event());

  double edep = -1.0;
  double qedep = -1.0;

  if (simHeader != nullptr) {
    edep = 0.;
    qedep = 0.;
    auto tracks_info = sim_evt->getTracksVec();

    LogDebug << tracks_info.size() << " tracks available" << std::endl;

    for (auto& track : tracks_info) {
      edep += track->getEdep();
      qedep += track->getQEdep();
    }
  }

  LogDebug << "Event retrieved" << std::endl;

  // Declaration of computation variable

  std::vector<double> fhts;
  fhts.reserve(lpmt_hits.size());
  std::vector<double> qs;
  qs.reserve(lpmt_hits.size());

  ROOT::Math::XYZVector cc(0., 0., 0.);   //< Charge barycenter
  ROOT::Math::XYZVector cht(0., 0., 0.);  //< First hit time barycenter

  double sum_charge = 0;
  double sum_time = 0;

  double n_pmt = 0;

  /**
   * Version 1: Working only with LPMT for now
   **/
  for (const auto& channel : lpmt_hits) {
    if ((double)channel->firstHitTime() <= 0) continue;

    fhts.push_back((double)channel->firstHitTime());
    qs.push_back((double)channel->nPE());

    sum_charge += channel->nPE();
    sum_time += (1. / (channel->firstHitTime() + EPSILON));

    Identifier ID = Identifier(channel->pmtId());

    ++n_pmt;

    auto pmt = _rec_geo_svc->data()->getCdGeom()->getPmt(ID);
    auto pmt_position = ROOT::Math::XYZVector(pmt->getX() / 1000., pmt->getY() / 1000., pmt->getZ() / 1000.);

    cc += channel->nPE() * pmt_position;
    cht += (1. / (channel->firstHitTime() + EPSILON)) * pmt_position;
  }

  cc *= (1. / sum_charge);
  cht *= (1. / sum_time);

  LogDebug << "Storing" << std::endl;

  if (simHeader != nullptr) {
    store->set("edep", edep);
    store->set("qedep", qedep);
  }

  store->set("AccumCharge", sum_charge);
  store->set("nPMTs", n_pmt);

  auto x_cc = cc.X();
  auto y_cc = cc.Y();
  auto z_cc = cc.Z();
  store->set("R_cc", cc.R());
  store->set("rho_cc", sqrt(x_cc * x_cc + y_cc * y_cc));
  store->set("x_cc", x_cc);
  store->set("y_cc", y_cc);
  store->set("z_cc", z_cc);

  store->set("gamma_z_cc", z_cc / sqrt(x_cc * x_cc + y_cc * y_cc));
  store->set("gamma_y_cc", y_cc / sqrt(x_cc * x_cc + z_cc * z_cc));
  store->set("gamma_x_cc", x_cc / sqrt(y_cc * y_cc + z_cc * z_cc));
  store->set("theta_cc", cc.Theta());
  store->set("phi_cc", cc.Phi());
  store->set("jacob_cc", pow(cc.R(), 2) * sin(cc.Theta()));

  auto x_cht = cht.X();
  auto y_cht = cht.Y();
  auto z_cht = cht.Z();
  store->set("R_cht", cht.R());
  store->set("rho_cht", sqrt(x_cht * x_cht + y_cht * y_cht));
  store->set("x_cht", x_cht);
  store->set("y_cht", y_cht);
  store->set("z_cht", z_cht);

  store->set("gamma_z_cht", z_cht / sqrt(x_cht * x_cht + y_cht * y_cht));
  store->set("gamma_y_cht", y_cht / sqrt(x_cht * x_cht + z_cht * z_cht));
  store->set("gamma_x_cht", x_cht / sqrt(y_cht * y_cht + z_cht * z_cht));
  store->set("theta_cht", cht.Theta());
  store->set("phi_cht", cht.Phi());
  store->set("jacob_cht", pow(cht.R(), 2) * sin(cht.Theta()));

  auto ht_mean = BDTE::mean(fhts);
  store->set("ht_std", BDTE::std_dev(fhts, ht_mean));
  store->set("ht_mean", ht_mean);
  store->set("ht_skew", BDTE::skewness(fhts, ht_mean));
  store->set("ht_kurtosis", BDTE::kurtosis(fhts, ht_mean));

  auto pe_mean = BDTE::mean(qs);
  store->set("pe_std", BDTE::std_dev(qs, pe_mean));
  store->set("pe_mean", sum_charge / n_pmt /*pe_mean*/);
  store->set("pe_skew", BDTE::skewness(qs, pe_mean));
  store->set("pe_kurtosis", BDTE::kurtosis(qs, pe_mean));

  std::vector<double> quantiles = {0.02, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45,
                                   0.5,  0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95};

  auto ht_ps = BDTE::Quantile(fhts, quantiles);
  auto pe_ps = BDTE::Quantile(qs, quantiles);

  store->set("ht_2p", ht_ps[0]);
  store->set("ht_5p", ht_ps[1]);
  store->set("ht_10p", ht_ps[2]);
  store->set("ht_15p", ht_ps[3]);
  store->set("ht_20p", ht_ps[4]);
  store->set("ht_25p", ht_ps[5]);
  store->set("ht_30p", ht_ps[6]);
  store->set("ht_35p", ht_ps[7]);
  store->set("ht_40p", ht_ps[8]);
  store->set("ht_45p", ht_ps[9]);
  store->set("ht_50p", ht_ps[10]);
  store->set("ht_55p", ht_ps[11]);
  store->set("ht_60p", ht_ps[12]);
  store->set("ht_65p", ht_ps[13]);
  store->set("ht_70p", ht_ps[14]);
  store->set("ht_75p", ht_ps[15]);
  store->set("ht_80p", ht_ps[16]);
  store->set("ht_85p", ht_ps[17]);
  store->set("ht_90p", ht_ps[18]);
  store->set("ht_95p", ht_ps[19]);

  store->set("ht_5-2p", ht_ps[1] - ht_ps[0]);
  store->set("ht_10-5p", ht_ps[2] - ht_ps[1]);
  store->set("ht_15-10p", ht_ps[3] - ht_ps[2]);
  store->set("ht_20-15p", ht_ps[4] - ht_ps[3]);
  store->set("ht_25-20p", ht_ps[5] - ht_ps[4]);
  store->set("ht_30-25p", ht_ps[6] - ht_ps[5]);
  store->set("ht_35-30p", ht_ps[7] - ht_ps[6]);
  store->set("ht_40-35p", ht_ps[8] - ht_ps[7]);
  store->set("ht_45-40p", ht_ps[9] - ht_ps[8]);
  store->set("ht_50-45p", ht_ps[10] - ht_ps[9]);
  store->set("ht_55-50p", ht_ps[11] - ht_ps[10]);
  store->set("ht_60-55p", ht_ps[12] - ht_ps[11]);
  store->set("ht_65-60p", ht_ps[13] - ht_ps[12]);
  store->set("ht_70-65p", ht_ps[14] - ht_ps[13]);
  store->set("ht_75-70p", ht_ps[15] - ht_ps[14]);
  store->set("ht_80-75p", ht_ps[16] - ht_ps[15]);
  store->set("ht_85-80p", ht_ps[17] - ht_ps[16]);
  store->set("ht_90-85p", ht_ps[18] - ht_ps[17]);
  store->set("ht_95-90p", ht_ps[19] - ht_ps[18]);

  store->set("pe_2p", pe_ps[0]);
  store->set("pe_5p", pe_ps[1]);
  store->set("pe_10p", pe_ps[2]);
  store->set("pe_15p", pe_ps[3]);
  store->set("pe_20p", pe_ps[4]);
  store->set("pe_25p", pe_ps[5]);
  store->set("pe_30p", pe_ps[6]);
  store->set("pe_35p", pe_ps[7]);
  store->set("pe_40p", pe_ps[8]);
  store->set("pe_45p", pe_ps[9]);
  store->set("pe_50p", pe_ps[10]);
  store->set("pe_55p", pe_ps[11]);
  store->set("pe_60p", pe_ps[12]);
  store->set("pe_65p", pe_ps[13]);
  store->set("pe_70p", pe_ps[14]);
  store->set("pe_75p", pe_ps[15]);
  store->set("pe_80p", pe_ps[16]);
  store->set("pe_85p", pe_ps[17]);
  store->set("pe_90p", pe_ps[18]);
  store->set("pe_95p", pe_ps[19]);

  LogDebug << "End of event " << evt_nav->EventID() << std::endl;
  return true;
}

bool EdmReader::finalize() {
  LogDebug << "Finalizing" << std::endl;

  delete _rec_geo_svc;
  delete _nav_buffer;
  delete _pystore;

  return true;
}
}  // namespace BDTE
