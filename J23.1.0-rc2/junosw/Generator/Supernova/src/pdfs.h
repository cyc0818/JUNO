#pragma once

#include "TRandom3.h"

namespace PDFs{


  //--IBD-------------------------------------
  double IBDRate(double *x, double *par);
  double IBDEe(double eNu, double costheta);

  double IBDRateFlavor(double *x, double *par);

  //--PES-------------------------------------
  double PESTotalRate(double *x, double *par);
  double PESRate(double *x, double *par);
  double PESEp(double eNu, double costheta);
  double dEpdCos(double eNu, double costheta);

  int PESNuID(const double t, const double eNu, const double costheta, TRandom *const rand);
  double PESNue(const double &t, const double &e, const double &costheta);
  double PESNueBar(const double &t, const double &e, const double &costheta);
  double PESNux(const double &t, const double &e, const double &costheta);
  double PESNuxBar(const double &t, const double &e, const double &costheta);

  double PESRateFlavor(double *x, double *par);
  double PESTotalRateFlavor(double *x, double *par);

  //--EES-------------------------------------
  double EESTotalRate(double *x, double *par);
  double EESRate(double *x, double *par);
  double EESEe(double eNu, double costheta);
  double dEedCos(double eNu, double costheta);

  int EESNuID(const double t, const double eNu, const double costheta, TRandom *const rand);
  double EESNue(const double &t, const double &e, const double &costheta);
  double EESNueBar(const double &t, const double &e, const double &costheta);
  double EESNux(const double &t, const double &e, const double &costheta);
  double EESNuxBar(const double &t, const double &e, const double &costheta);

  double EESRateFlavor(double *x, double *par);
  double EESTotalRateFlavor(double *x, double *par);

  //--N12-------------------------------------
  double N12Rate(double *x, double *par);
  double N12DecayRate(double *x, double *par);

  double N12RateFlavor(double *x, double *par);

  //--B12-------------------------------------
  double B12Rate(double *x, double *par);
  double B12DecayRate(double *x, double *par);

  double B12RateFlavor(double *x, double *par);

  //--C12-------------------------------------
  double C12Rate(double *x, double *par);

  int C12NuID(const double t, const double eNu, TRandom *const rand);
  double C12Nue(const double &t, const double &e);
  double C12NueBar(const double &t, const double &e);
  double C12Nux(const double &t, const double &e);
  double C12NuxBar(const double &t, const double &e);

  double C12RateFlavor(double *x, double *par);


}
