/*=============================================================================
#
# Author:       Artem Chukanov - chukanov@jinr.ru
# Created:      26.01.2018
# Filename:	SphMuonRecTool.h
# Description:
=============================================================================*/
#ifndef SphMuonRecTool_h
#define SphMuonRecTool_h

#include "SniperKernel/AlgBase.h"
#include "RecTools/IRecMuonTool.h"

#include <fstream>
#include <map>

#include <TSpline.h>
#include "Math/Minimizer.h"
#include "Math/Functor.h"
#include "Math/Factory.h"
#include "Minuit2/FCNBase.h"
#include "gsl/gsl_sf_legendre.h"

#include "SphFunc.h"
#include "LegendreFunction.h"
#include "MuonCandidates.h"
#include "SphMuonRec.h"
#include "SphShowerRec.h"
#include "SphTrack.h"

#include "SniperKernel/ToolBase.h"

#ifdef _OPENMP
#include <omp.h>
#endif

class TF2;

class SphMuonRecTool: public IRecMuonTool, public ToolBase, public SphFunc
{
 public:
  SphMuonRecTool(const std::string& name);
  virtual ~SphMuonRecTool();

  bool reconstruct(RecTrks* trks);
  bool configure(const Params*, const PmtTable *ptab);
  bool finalize();
  
  virtual inline int    GetMaxOrder()        const {return fMaxOrder;}
  virtual inline int    GetLArraySize()      const {return fLArraySize;}
  virtual inline double GetCnk(int pmtType, int index) const {return fSetPmtDistr ? fCnkPmt[pmtType][index] : fCnk[pmtType][index];}
  virtual inline double GetSnk(int pmtType, int index) const {return fSetPmtDistr ? fSnkPmt[pmtType][index] : fSnk[pmtType][index];}

  inline int    GetNtracksMn()       const {return fNtracksMn;}
  inline int    GetPMTtype()         const {return fPMTtype;}
  double fcnRotation(const double par[]);

 private:

  int            fEvt;
  int            fNtracksMn;
  int            fMaxOrder;
  int            fMaxOrderInit;
  int            fLArraySize;
  int            fAngleStep;
  int            fPMTtype;
  int            fEventLength;
  double         pi;
  double         fRadius;
  double         fRadius2;
  double         fDegToRad;
  double         fRadToDeg;
  double         fSumPE[2];
  double         fBeginEventTime;
  bool           fMuonCalibration;
  bool           fSetPmtDistr;
  bool           fMakePmtMap;
  bool           fPhotonResponseCalib; // calibration detector response
  bool           fIncludeShower;
  bool           fUseMCtrue;
  bool           fDrawShower;
  bool           fProductionMode;
  TString        fFileParNamePmt[2];
  TVector3       fShowerStartPoint;

  // 0 - 3 inch PMT, 1 - 20 inch PMT

  double   *fCnk[2];
  double   *fSnk[2];
  double   *fCnkPmt[2];
  double   *fSnkPmt[2];
  std::vector <double> fDistMuonSimList;
  std::vector <double> fDistMuonRecList;

  double *fLegendre[180];

  std::map <int, EventSimRec_t> fEventSimRec;
  std::vector <trackRec_t>      fTracksRec;
  std::vector <trackRec_t>      fTracksNotConnected;

  std::ofstream fOutputFileError;
  std::ofstream fOutputFileEnergy;

  MuonCandidates *fMuonCandidates;
  SphMuonRec     *fSphMuonRec;
  SphShowerRec   *fSphShowerRec;

  unsigned int fTotPmtNum;
  
  PmtTable     fPmtTable;
  const PmtTable *fPmtPtr;
  
  friend class rotateFCN;

  void   LoadPmtDistr();
  void   SaveSphFuncParameters();
  double MuonTrackReconstruction(double par[]); // return chi2
  void   CompareResults(double par[], double chi2, RecTrks *trks, double startTime);
  void   GetTrueMuonDirection();
  int    GetInitialValues(double pos[], double phi1[], double phi2[], double alpha[], std::vector<track_t> &muonCandidate);
  double GetMinTime();
  void   FillCoeffFromSplines(int track, double radius);
};

// implementation of FCN rotation

class rotateFCN {
public:
  static void SetFunc(SphMuonRecTool *sphfunc) {fSphf = sphfunc;}
  static double RotateFCN(const double par[]) {
    return fSphf->fcnRotation(par);
  };
private:
  static SphMuonRecTool *fSphf;
};

#endif // SphMuonRecTool_h
