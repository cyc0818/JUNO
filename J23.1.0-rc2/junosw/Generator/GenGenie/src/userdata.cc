#include "userdata.h"
#include "CLHEP/Units/SystemOfUnits.h"

// BELOW is from GENIE gNtpConv.cxx
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>

#include "libxml/parser.h"
#include "libxml/xmlmemory.h"

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TFolder.h>
#include <TBits.h>
#include <TObjString.h>
#include <TMath.h>
#include "Framework/ParticleData/BaryonResonance.h"
#include "Framework/ParticleData/BaryonResUtils.h"
#include "Framework/Conventions/GBuild.h"
#include "Framework/Conventions/Constants.h"
#include "Framework/Conventions/Units.h"
#include "Framework/Conventions/GVersion.h"
#include "Framework/EventGen/EventRecord.h"
#include "Framework/GHEP/GHepStatus.h"
#include "Framework/GHEP/GHepParticle.h"
#include "Framework/GHEP/GHepUtils.h"
#include "Framework/Ntuple/NtpMCFormat.h"
#include "Framework/Ntuple/NtpMCTreeHeader.h"
#include "Framework/Ntuple/NtpMCEventRecord.h"
#include "Framework/Ntuple/NtpWriter.h"
#include "Framework/Numerical/RandomGen.h"
#include "Framework/Messenger/Messenger.h"
#include "Framework/ParticleData/PDGCodes.h"
#include "Framework/ParticleData/PDGUtils.h"
#include "Framework/ParticleData/PDGLibrary.h"
#include "Framework/Utils/AppInit.h"
#include "Framework/Utils/RunOpt.h"
#include "Framework/Utils/CmdLnArgParser.h"
#include "Framework/Utils/SystemUtils.h"
#include "Framework/Utils/T2KEvGenMetaData.h"

bool ghep2userdata(genie::EventRecord& event, Data& userdata)
{
  // based on ConvertToUserTTree

  // input particles
  genie::GHepParticle* neutrino = event.Probe();
  genie::GHepParticle* target = event.Particle(1);
  assert(target);
  genie::GHepParticle* fsl = event.FinalStatePrimaryLepton();
  genie::GHepParticle* hitnucl = event.HitNucleon();

  int tgtZ = 0;
  int tgtA = 0;
  if (genie::pdg::IsIon(target->Pdg())) {
    tgtZ = genie::pdg::IonPdgCodeToZ(target->Pdg());
    tgtA = genie::pdg::IonPdgCodeToA(target->Pdg());
  }
  if (target->Pdg() == genie::kPdgProton) {
    tgtZ = 1;
    tgtA = 1;
  }
  if (target->Pdg() == genie::kPdgNeutron) {
    tgtZ = 0;
    tgtA = 1;
  }

  userdata.pPdg = neutrino->Pdg();
  userdata.pEn = neutrino->Energy();
  userdata.tPdg = target->Pdg();

  const genie::Interaction* interaction = event.Summary();
  const genie::InitialState& init_state = interaction->InitState();
  const genie::ProcessInfo& proc_info = interaction->ProcInfo();
  const genie::Kinematics& kine = interaction->Kine();
  const genie::XclsTag& xcls = interaction->ExclTag();
  const genie::Target& tgt = init_state.Tgt();

  // Vertex in detector coord system
  TLorentzVector* vtx = event.Vertex();

  // Process id
  bool is_qel = proc_info.IsQuasiElastic();
  bool is_res = proc_info.IsResonant();
  bool is_dis = proc_info.IsDeepInelastic();
#if __GENIE_RELEASE_CODE__ > GRELCODE(3,0,6)
  bool is_coh = proc_info.IsCoherentProduction();
#else
  bool is_coh = proc_info.IsCoherent();
#endif
  bool is_dfr = proc_info.IsDiffractive();
  bool is_imd = proc_info.IsInverseMuDecay();
  bool is_imdanh = proc_info.IsIMDAnnihilation();
  bool is_singlek = proc_info.IsSingleKaon();
  bool is_nuel = proc_info.IsNuElectronElastic();
  bool is_em = proc_info.IsEM();
  bool is_weakcc = proc_info.IsWeakCC();
  bool is_weaknc = proc_info.IsWeakNC();
  bool is_mec = proc_info.IsMEC();
  bool is_amnugamma = proc_info.IsAMNuGamma();

  if (is_weaknc) userdata.ctag = 1;
  if (is_weakcc) userdata.ctag = 2;

  if (is_qel) userdata.interType = 1;
  if (is_res) userdata.interType = 2;
  if (is_coh || is_dfr) userdata.interType = 3;
  if (is_dis) userdata.interType = 4;
  if (is_mec) userdata.interType = 5;

  // Get event weight
  double weight = event.Weight();
  userdata.weight = weight;

  FinalParInfo FPinfo;
  int brFSPrimLept = (fsl) ? fsl->Pdg() : 0;

  if (brFSPrimLept == 11 || brFSPrimLept == 13 || brFSPrimLept == 15) {
    FPinfo.n_lep_minus++;
  }
  if (brFSPrimLept == -11 || brFSPrimLept == -13 || brFSPrimLept == -15) {
    FPinfo.n_lep_plus++;
  }

    bool study_hadsyst = (is_qel || is_res || is_dis || is_coh || is_dfr || is_mec || is_singlek);
  // =======================================================================
  // BELOW will loop all the particles
  // =======================================================================
  TObjArrayIter piter(&event);
  genie::GHepParticle* p = 0;
  int ip = -1;

  std::vector<int> final_had_syst;
  while ((p = (genie::GHepParticle*)piter.Next()) && study_hadsyst) {
    ip++;

    // don't count final state lepton as part hadronic system
    // if(!is_coh && event.Particle(ip)->FirstMother()==0) continue;
    if (event.Particle(ip)->FirstMother() == 0) continue;
    if (genie::pdg::IsPseudoParticle(p->Pdg())) continue;
    int pdgc = p->Pdg();
    int ist = p->Status();
    if (ist == genie::kIStStableFinalState) {
      if (pdgc == genie::kPdgGamma || pdgc == genie::kPdgElectron || pdgc == genie::kPdgPositron) {
        int igmom = p->FirstMother();
        if (igmom != -1) {
          // only count e+'s e-'s or gammas not from decay of pi0
          if (event.Particle(igmom)->Pdg() != genie::kPdgPi0) {
            final_had_syst.push_back(ip);
          }
        }
      } else {
        final_had_syst.push_back(ip);
      }
    }
    // now add pi0's that were decayed as short lived particles
    else if (pdgc == genie::kPdgPi0) {
      int ifd = p->FirstDaughter();
      int fd_pdgc = event.Particle(ifd)->Pdg();
      // just require that first daughter is one of gamma, e+ or e-
      if (fd_pdgc == genie::kPdgGamma || fd_pdgc == genie::kPdgElectron || fd_pdgc == genie::kPdgPositron) {
        final_had_syst.push_back(ip);
      }
    }
  }  // particle-loop

  if (count(final_had_syst.begin(), final_had_syst.end(), -1) > 0) {
    return false;
  }

  // Final state (visible) hadronic system
  auto brNf = final_had_syst.size();
  for (int j = 0; j < brNf; j++) {
    p = event.Particle(final_had_syst[j]);

    int hpdg = p->Pdg();
    if (hpdg == 211 || hpdg == 321 || hpdg == 411) FPinfo.n_meson_plus++;
    if (hpdg == -211 || hpdg == -321 || hpdg == -411) FPinfo.n_meson_minus++;
    if (hpdg == 2112 || hpdg == 3122 || hpdg == 3212) FPinfo.n_n++;
    if (hpdg == 2212 || hpdg == 3222 || hpdg == 4212 || hpdg == 4122) FPinfo.n_p++;
    if (hpdg == 4222) FPinfo.n_Sigma_cplusplus++;
    if (hpdg == 3112) FPinfo.n_sigmaminus++;
  }

  if (target->Pdg() == 1000060120) {
    std::cout << GetResIsoPdg(FPinfo) << std::endl;
    int res_iso_pdg = GetResIsoPdg(FPinfo);
    if (res_iso_pdg > 0 && res_iso_pdg != 1000060120) {
      userdata.isoPdg = res_iso_pdg;
    }
  }

  return true;
}

bool hepmc2userdata(HepMC::GenEvent& event, Data& userdata)
{

  double ut = 1e-3;
  int nf = 0;
  for (HepMC::GenEvent::particle_const_iterator p = event.particles_begin(); p != event.particles_end(); ++p) {
    (*p)->print();
    int pdgid = (*p)->pdg_id();
    bool nutag = false;
    if (pdgid == 12 || pdgid == -12 || pdgid == 14 || pdgid == -14 || pdgid == 16 || pdgid == -16) {
      nutag = true;
    }

    HepMC::FourVector mom = (*p)->momentum();

    if ((*p)->status() == 0 && nutag) {
      userdata.pPx = mom.px() * ut;
      userdata.pPy = mom.py() * ut;
      userdata.pPz = mom.pz() * ut;
    } else if ((*p)->status() == 1) {
      userdata.pdg[nf] = (*p)->pdg_id();
      userdata.deextag[nf] = 0;
      userdata.px[nf] = mom.px() * ut;
      userdata.py[nf] = mom.py() * ut;
      userdata.pz[nf] = mom.pz() * ut;
      userdata.energy[nf] = mom.e() * ut;

      if (!nutag) {
        userdata.mass[nf] = TMath::Sqrt(mom.e() * mom.e() - mom.px() * mom.px() - mom.py() * mom.py() - mom.pz() * mom.pz()) * ut;
      }
      // std::cout << "Jie test  px py pz " << ppx << "\t" << ppy << "\t" << ppz << std::endl;
      nf++;
    }
  }

  userdata.Npars = nf;
  //std::cout << "------> Jie Check number of particles = " << nf << std::endl;

  return true;
}

int GetResIsoPdg(FinalParInfo &FP)
{

  int nChargeplus = FP.n_meson_plus + FP.n_lep_plus;
  int nChargeminus = FP.n_meson_minus + FP.n_lep_minus;
  int nChargeplus2 = 0;
  int nChargeminus2 = 0;
  int nN = 0;
  int nP = 0;
  nN = FP.n_n + FP.n_sigmaminus;
  nChargeplus2 = nChargeplus - FP.n_sigmaminus;

  nP = FP.n_p + 2 * FP.n_Sigma_cplusplus;
  nChargeminus2 = nChargeminus;
  int nCharge = nChargeplus2 - nChargeminus2;
  int resN = 0;
  int resP = 0;

  resN = 6 - (nN - nCharge);
  resP = 6 - (nP + nCharge);

  //std::cout << "resP = " << resP << "|||"
            //<< "resN = " << resN << std::endl;

  int isopdg = 0;
  if (resN >= 3 && resP >= 3) {
    isopdg = 1000000000;
    isopdg += resP * 10000;
    isopdg += (resP + resN) * 10;
  }

  return isopdg;
}

