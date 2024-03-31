#ifndef userdata_h
#define userdata_h

/*
 * Description:
 *     save the GHEP (GENIE Event Record) into user TTree
 *
 *     GENIE source code src/Apps/gNtpConv.cxx provides a good example.
 *
 * Author:
 *     Jie Cheng <chengjie AT ncepu.edu.cn>
 */

#include "Framework/EventGen/EventRecord.h"
#include "HepMC/GenEvent.h"
#include "def.h"

struct FinalParInfo {

  FinalParInfo()
  {
    n_n = 0;
    n_p = 0;
    n_meson_plus = 0;
    n_meson_minus = 0;
    n_sigmaminus = 0;

    n_lep_plus = 0;
    n_lep_minus = 0;

    n_Sigma_cplusplus = 0;
  }

  int n_n;            //--- neutron or ...
  int n_p;            //--- proton or ...
  int n_meson_plus;   //--- pi+, or K+ or D+
  int n_meson_minus;  //--- pi- or K- or D-
  int n_sigmaminus;

  int n_lep_plus;
  int n_lep_minus;

  int n_Sigma_cplusplus;
};

bool ghep2userdata(genie::EventRecord &, Data &userdata);
bool hepmc2userdata(HepMC::GenEvent &event, Data &userdata);

int GetResIsoPdg(FinalParInfo &FP);

#endif
