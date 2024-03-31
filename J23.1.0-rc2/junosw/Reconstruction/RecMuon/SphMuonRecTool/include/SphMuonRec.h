/*=============================================================================
#
# Author:       Artem Chukanov - chukanov@jinr.ru
# Created:      26.01.2018
# Filename:	SphMuonRec.h
# Description:
=============================================================================*/
#ifndef SphMuonRec_h
#define SphMuonRec_h

#include <TSpline.h>

#include <fstream>
#include "gsl/gsl_sf_legendre.h"

#include "SphTrack.h"

class SphMuonRec
{
 public:
  SphMuonRec(int pmtType, int eventLength, bool muonCalibration);
  ~SphMuonRec();

  int  LoadSphFuncParameters();
  void SetMaxOrder(int maxOrder) {fMaxOrder = maxOrder;}
  void SaveSphFuncParameters(EventSimRec_t &eventSimRec, int fEvt,
			     double *cnk[2], double *snk[2], double sumPE[]);
  void FillCoeffFromSplines(int track, double radius);
  void GetSphericalFunction(int track, double cos_theta, double phi, double f[][5]);
  void FillMuonRecHisto(EventSimRec_t &eventSimRec, int evt, double *par,
			int ntracks);
  
  inline double GetSplEvalCos(int pmtType, int index, double par) const
  {return fSpline_c[pmtType][index].Eval(par);}

  inline int GetMaxOrder()   const {return fMaxOrder;}
  inline int GetPMTtype()    const {return fPMTtype;}
  inline int GetLArraySize() const {return fLArraySize;}
 
 private:

  int       fMaxOrder;
  int       fLArraySize;
  int       fPMTtype;
  int       fEventLength;
  double    pi;
  double    fDegToRad;
  double    fRadToDeg;

  double   *fSplc[2][5];
  TSpline5 *fSpline_c[2];

  std::ofstream fPositionFile;
  std::ofstream fOutputFile[2];
};



#endif // SphMuonRec_h
