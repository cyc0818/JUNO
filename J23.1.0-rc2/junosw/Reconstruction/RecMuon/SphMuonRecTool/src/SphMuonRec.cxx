/*=============================================================================
#
# Author: Artem Chukanov - chukanov@jinr.ru
# Created: 26.01.2018
# Filename: SphMuonRec.cxx
# Description:
=============================================================================*/
#include "SphMuonRec.h"
#include "LegendreFunction.h"

#include <TFile.h>
#include <TKey.h>

#include <iostream>


using namespace std;

SphMuonRec::SphMuonRec(int pmtType, int eventLength, bool muonCalibration) {

  fPMTtype = pmtType;
  fEventLength = eventLength;
  pi = TMath::Pi();
  fDegToRad = TMath::DegToRad();
  fRadToDeg = TMath::RadToDeg();

  if (muonCalibration) {
    TString pmtStr[2] = {"_3inch.dat", "_20inch.dat"};
    fPositionFile.open(Form("results/muon_position_%d.dat", fEventLength));
    for (int pmt = fPMTtype; pmt < 2; ++pmt) {
      TString fileParName = Form("results/spherical_parameters_%d%s", fEventLength, pmtStr[pmt].Data());
      fOutputFile[pmt].open(fileParName);
    }
  }
}


//----------------------------------------------------------------------------
SphMuonRec::~SphMuonRec()
{
  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
    delete [] fSpline_c[pmtType];

    if (fPositionFile.is_open()) fPositionFile.close();
    if (fOutputFile[pmtType].is_open()) fOutputFile[pmtType].close();
 
    for (int i = 0; i < 5; ++i) delete [] fSplc[pmtType][i];
  }
}


//----------------------------------------------------------------------------
void SphMuonRec::FillMuonRecHisto(EventSimRec_t &eventSimRec, int evt,
				  double *par, int ntracks)
{
  eventSimRec.hEventRecMuon=new TH2F(Form("legendre muon, %d", evt),
				     "reconstructed muon",
				     90, -180, 180, 90, -90, 90);
  
  double f2[2][5];
  double delta_phi1[5], delta_phi2[5], cos_alpha[5], sin_alpha[5];

  for (int i = 0; i < ntracks; ++i) {
    delta_phi1[i] = par[i*5+1];
    delta_phi2[i] = par[i*5+2];
    cos_alpha[i] = cos(par[i*5+3]);
    sin_alpha[i] = sin(par[i*5+3]);
    FillCoeffFromSplines(i, par[i*5]);
  }

  for (int biny = 1; biny <= eventSimRec.hEventRecMuon->GetNbinsY(); ++biny){
    double theta = eventSimRec.hEventRecMuon->GetYaxis()->GetBinCenter(biny);
    double cos_theta = cos(fDegToRad*(90-theta));
    double sin_theta = sin(fDegToRad*(90-theta));
    
    for (int binx = 1; binx <= eventSimRec.hEventRecMuon->GetNbinsX(); ++binx) {
      double phi = eventSimRec.hEventRecMuon->GetXaxis()->GetBinCenter(binx)*fDegToRad;
      double f = 0;
      
      for (int i = 0; i < ntracks; ++i) {

	f2[0][i] = f2[1][i] = 0;
	
	// first rotation

	double phi1 = phi + delta_phi1[i];

	// second rotation

	double x = sin_theta*cos(phi1);
	double y = sin_theta*sin(phi1);
	double z = cos_theta;

	double y1 = y*cos_alpha[i] - z*sin_alpha[i];
	double z1 = y*sin_alpha[i] + z*cos_alpha[i];

	double cos_th1 = z1;

	// third rotation

	double phi2 = TMath::ATan2(y1, x) + delta_phi2[i];

	GetSphericalFunction(i, cos_th1, phi2, f2);
	f += f2[1][i];
      }
      
      eventSimRec.hEventRecMuon->SetBinContent(binx, biny, f);
    }
  }
}


//----------------------------------------------------------------------------
void SphMuonRec::SaveSphFuncParameters(EventSimRec_t &eventSimRec, int evt,
				       double *cnk[2], double *snk[2],
				       double sumPE[])
{
  // save calibration information
  // for event calibration we are using only events with one muon

  if (eventSimRec.muonSim.size() == 1) {

    trackSim_t &muonSim = eventSimRec.muonSim.front();
    TVector3 middlePoint = 0.5*(muonSim.initMuonPosition +
				muonSim.endMuonPosition);
    double dist = middlePoint.Perp();
    
    for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {

      bool goodMuon = true;

      if (dist > 18 || sumPE[pmtType] < 1000 ||
	  muonSim.endMuonPosition.Mag() < 17.3) goodMuon = false;

      for (int n = 0; n <= fMaxOrder; ++n)
	for (int k = 0; k <= n; ++k) {
	  int index = gsl_sf_legendre_array_index(n, k);
	  if (goodMuon) fOutputFile[pmtType] << evt << " " << dist << " "
					     << n << " " << k << " "
					     << cnk[pmtType][index] << " "
					     << snk[pmtType][index] << " "
					     << sumPE[pmtType] << "\n";
	  else fOutputFile[pmtType] << evt << " " << dist << " " << n << " "
				    << k << " " << -10000 << " "
				    << -10000 << " " << 0 << "\n";
	}
    }

    double energy = (muonSim.initMuonMomentum - muonSim.endMuonMomentum).Mag();
    
    // save incoming and outgoing points

    fPositionFile << evt << " " << dist << " " << energy << " "
		  << sumPE[0] << " " << sumPE[1] << " "
		  << muonSim.initMuonPosition.x() << " "
		  << muonSim.initMuonPosition.y() << " "
		  << muonSim.initMuonPosition.z() << " "
		  << muonSim.endMuonPosition.x() << " "
		  << muonSim.endMuonPosition.y() << " "
		  << muonSim.endMuonPosition.z() << endl;
  }
}


//----------------------------------------------------------------------------
int SphMuonRec::LoadSphFuncParameters()
{
  fLArraySize = gsl_sf_legendre_array_n(fMaxOrder);

  // fLegendre = new double [fLArraySize];
  
  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
    fSpline_c[pmtType] = new TSpline5 [fLArraySize];

    // initialization arrays for the FCN function

    for (int i = 0; i < 5; ++i)
      fSplc[pmtType][i] = new double [fLArraySize];
  }
  
  // read splines from file

  TString SPHFUNC_DATA = (TString)getenv("RECMU_SPHFUNC_DATA");
  TFile fileSpline(SPHFUNC_DATA + Form("/sph_muon_spline_%d.root", fEventLength), "read");
  
  fileSpline.cd();
  
  TIter nextTopLevelKey(fileSpline.GetListOfKeys());
  TKey *keyTopLevel;
  int pmtType, maxIndex = 0;
  int maxOrder = gsl_sf_legendre_array_index(fMaxOrder, fMaxOrder);
  
  // loop on keys, and search for the TSpline named

  while ((keyTopLevel = (TKey*)nextTopLevelKey())) {
    TString splineName(keyTopLevel->GetClassName());
    
    if (splineName.Contains("TSpline5")) splineName = keyTopLevel->GetName();
    else continue;

    pmtType = (TString(splineName(9, 1))).Atoi();
    if (pmtType != 0 && pmtType != 1) continue;

    int length = splineName.Length()-11;
    int index = (TString(splineName(11, length))).Atoi();
    if (index > maxOrder) continue;
    if (index > maxIndex) maxIndex = index;

    TSpline5 *spline = (TSpline5*)gDirectory->Get(splineName);
    
    if (TString(splineName(7, 1)) == "c")
      fSpline_c[pmtType][index] = TSpline5(*spline);
    else continue;
  }

  // calculate max order
  
  if (maxIndex < maxOrder) fMaxOrder = int(0.5*(sqrt(9+8*maxIndex)-3));

  fileSpline.Close();

  return fMaxOrder;
}


//----------------------------------------------------------------------------
void SphMuonRec::FillCoeffFromSplines(int track, double radius)
{
  
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int n = 0; n <= fMaxOrder; ++n)
    for (int k = 0; k <= n; ++k) {
      if (TMath::Odd(k+n)) continue;
      int index = gsl_sf_legendre_array_index(n, k);
       for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
	 fSplc[pmtType][track][index] = GetSplEvalCos(pmtType, index, radius);
       }
    }
}


//----------------------------------------------------------------------------
void SphMuonRec::GetSphericalFunction(int track, double cos_theta, double phi, double f[][5])
{
  double *legendre = new double [fLArraySize];
  double *sin_phi = new double [fMaxOrder+1];
  double *cos_phi = new double [fMaxOrder+1];

  gsl_sf_legendre_array_e(GSL_SF_LEGENDRE_NONE, fMaxOrder, cos_theta, -1,
			  legendre);

  while (phi > pi) {phi -= 2*pi;}
  while (phi < -pi) {phi += 2*pi;}
  
  LegendreFunction::SinCosCalculation(fMaxOrder, phi, sin_phi, cos_phi);
  
  for (int k = 0; k <= fMaxOrder; ++k) {
    for (int n = k; n <= fMaxOrder; ++n) {
      if (TMath::Odd(k+n)) continue;
      int index = gsl_sf_legendre_array_index(n, k);
      for (int pmtType = fPMTtype; pmtType < 2; ++pmtType)
	f[pmtType][track] += legendre[index]*fSplc[pmtType][track][index]*cos_phi[k];
    }
  }

  delete [] legendre;
  delete [] sin_phi;
  delete [] cos_phi;
}
