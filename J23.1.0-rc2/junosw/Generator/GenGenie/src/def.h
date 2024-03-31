#ifndef def_h
#define def_h
#include <map>

struct Data {

  Data()
  {

    Iev = -1;
    pPdg = 0;
    tPdg = 0;
    interType = 0;  // 1---QEL; 2---RES; 3---COH; 4---DIS; 5---2p2h(MEC)
    ctag = 0;       // 1---NC; 2---CC

    pEn = 0.;
    pPx = 0.;
    pPy = 0.;
    pPz = 0.;

    isoPdg = 0;  //----- after interaction
    isoPx = 0.;
    isoPy = 0.;
    isoPz = 0.;
    isoMass = 0.;

    Npars = 0;
    for (int i = 0; i < 250; i++) {
      pdg[i] = 0;      //---- after interaction and deex
      deextag[i] = 0;  // 0---from interaction, 1---from deex
      px[i] = 0.;
      py[i] = 0.;
      pz[i] = 0.;
      energy[i] = 0.;
      mass[i] = 0.;
    }
    weight = 0.;
  }

  int Iev;
  int pPdg;
  int tPdg;
  int interType;
  int ctag;

  double pEn;
  double pPx;
  double pPy;
  double pPz;

  int isoPdg;
  double isoPx;
  double isoPy;
  double isoPz;
  double isoMass;

  int Npars;
  int pdg[250];
  int deextag[250];
  double px[250];
  double py[250];
  double pz[250];
  double energy[250];
  double mass[250];
  double weight;
};
//----

//--- a map to find the mass of isotopes
struct MassMap {
  MassMap()
  {
    pdg2mass.clear();
    pdg2mass[22] = 0;
    pdg2mass[2112] = 0.93957;
    pdg2mass[2212] = 0.93827;
    pdg2mass[1000010020] = 1.8756;
    pdg2mass[1000010030] = 2.8089;
    pdg2mass[1000020030] = 2.8084;
    pdg2mass[1000020040] = 3.7274;
    pdg2mass[1000030060] = 5.6015;
    pdg2mass[1000030070] = 6.5335;
    pdg2mass[1000030080] = 7.4708;
    pdg2mass[1000030090] = 8.4061;
    pdg2mass[1000040070] = 6.5344;
    pdg2mass[1000040080] = 7.4548;
    pdg2mass[1000040090] = 8.3925;
    pdg2mass[1000040100] = 9.3249;
    pdg2mass[1000050080] = 7.4728;
    pdg2mass[1000050090] = 8.3935;
    pdg2mass[1000050100] = 9.3244;
    pdg2mass[1000050110] = 10.2522;
    pdg2mass[1000060090] = 8.4100;
    pdg2mass[1000060100] = 9.3280;
    pdg2mass[1000060110] = 10.2542;
    pdg2mass[1000060120] = 11.1748;
    pdg2mass[1000060130] = 12.1123;
    pdg2mass[1000080160] = 14.8984;
    pdg2mass[1000070140] = 13.0438;
    pdg2mass[1000160320] = 29.7819;
    pdg2mass[1000140280] = 26.0596;
    pdg2mass[1000130270] = 25.1328;
    pdg2mass[1000260560] = 52.1025;
    pdg2mass[1000120240] = 22.342;
    pdg2mass[1000200400] = 37.2246;
    pdg2mass[1000110230] = 21.4143;
    pdg2mass[1000190390] = 36.294;
  }

  std::map<int, double> pdg2mass;
};

#endif
