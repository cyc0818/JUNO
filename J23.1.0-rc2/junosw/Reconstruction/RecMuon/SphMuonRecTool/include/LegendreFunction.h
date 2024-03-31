/*=============================================================================
#
# Author:       Artem Chukanov - chukanov@jinr.ru
# Created:      12.10.2021
# Filename:	LegendreFunction.h
# Description:
=============================================================================*/
#ifndef LegendreFunction_h
#define LegendreFunction_h

#include <TSpline.h>
#include "gsl/gsl_sf_legendre.h"

#include "SphFunc.h"

#ifdef _OPENMP
#include <omp.h>
#endif


class LegendreFunction
{
 public:

  static void SinCosCalculation(int k, double angle, double sin_array[], double cos_array[]) {
    if (k < 0) return;
    sin_array[0] = 0; cos_array[0] = 1;
    if (k > 0) {sin_array[1] = sin(angle); cos_array[1] = cos(angle);}

    for (int i = 2; i <= k; ++i) {
      sin_array[i] = sin_array[1]*cos_array[i-1] + sin_array[i-1]*cos_array[1];
      cos_array[i] = cos_array[1]*cos_array[i-1] - sin_array[i-1]*sin_array[1];
    }
  };

  // implementation of Legendre representation

  class  funcLegendre {
  public:
    funcLegendre(SphFunc *func) {fSphf = func;}
    double operator() (double *x, double *p) {
      const int maxOrder = fSphf->GetMaxOrder();
      double f = 0;
      double ctheta = cos(x[0]);
      double pmtType = p[0];

      double *legendre = new double [fSphf->GetLArraySize()];
      double *sin_phi = new double [maxOrder + 1];
      double *cos_phi = new double [maxOrder + 1];

      gsl_sf_legendre_array_e(GSL_SF_LEGENDRE_NONE, maxOrder, ctheta, -1,
			      legendre);
      SinCosCalculation(maxOrder, x[1], sin_phi, cos_phi);

#ifdef _OPENMP
#pragma omp parallel
      {
#pragma omp for reduction(+:f)
#endif
	for (int k = 0; k <= maxOrder; ++k) {
	  for (int n = k; n <= maxOrder; ++n) {
	    int index = gsl_sf_legendre_array_index(n, k);
	    f += legendre[index]*
	      (fSphf->GetCnk(pmtType, index)*cos_phi[k] +
	       fSphf->GetSnk(pmtType, index)*sin_phi[k]);
	  }
	}
#ifdef _OPENMP
      }
#endif

      if (f < 0) f = 0;

      delete [] legendre;
      delete [] sin_phi;
      delete [] cos_phi;
      
      return f;
    }
  private:
    SphFunc *fSphf;
  };
};


#endif // LegendreFunction_h
