/*=============================================================================
#
# Author:       Artem Chukanov - chukanov@jinr.ru
# Created:      24.12.2020
# Filename:	SphShowerRec.h
# Description: 
=============================================================================*/
#ifndef SphShowerRec_h
#define SphShowerRec_h

#include "SphTrack.h"

#include <fstream>
#include <TSpline.h>
#include "LegendreFunction.h"

class TCanvas;

class SphShowerRec
{
 public:
  
  SphShowerRec(double radius, int pmtType, bool photonCalib, int eventLength,
	       bool productionMode);
  ~SphShowerRec();

  inline double GetSplEvalCos(int pmtType, int index, double par) const
  {return fSpline_c[pmtType][index].Eval(par);}
  
  void SetMaxOrder(int maxOrder) {fMaxOrder = maxOrder;}
  void SetMuonInit(std::vector <trackSim_t> &muonSim) {fMuonSim = muonSim;}
  void LoadSphFuncParameters();
  void SaveSphFuncParameters(double **cnk, double **snk, double sumPE[]);
  void FillCoeffFromSplines(double radius);
  void GetSphericalFunction(double cos_theta, double *f);
  void DrawShower(double par[], int nTracks, EventSimRec_t &eventSimRec);
  bool DefineShower(double sumPE[], std::vector<track_t> &muonCandidate);
  
private:

  int         fPmtType;
  int         fEventLength;
  int         fMaxOrder;
  int         fLArraySize;
  bool        fPhotonCalib;
  double      fRadius;
  double      fDegToRad;
  double      fRadToDeg;
  double      pi;
  std::vector <trackSim_t> fMuonSim;

  TString    fPmtStr[2];
  TCanvas   *fCanvas;
  double    *fSplc[2];
  TSpline5  *fSpline_c[2];

  std::ofstream fOutputFile[2];
}; 

#endif // SphShowerRec_h
