/*=============================================================================
#
# Author: Artem Chukanov - chukanov@jinr.ru
# Created: 24.12.2020
# Filename: SphShowerRec.cxx
# Description:
=============================================================================*/

#include "SphShowerRec.h"
#include <TMath.h>
#include <TSpline.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TKey.h>
#include "gsl/gsl_sf_legendre.h"

#include <iostream>

using namespace std;

SphShowerRec::SphShowerRec(double radius, int pmtType, bool photonCalib, int eventLength, bool productionMode)
{
  fRadius = radius;
  fPhotonCalib = photonCalib;
  fEventLength = eventLength;
  fDegToRad = TMath::DegToRad();
  fRadToDeg = TMath::RadToDeg();
  fPmtType = pmtType;
  fCanvas = NULL;
  pi = TMath::Pi();

  fPmtStr[0] = "_3inch"; fPmtStr[1] = "_20inch";

  if (!productionMode) {
    fCanvas = new TCanvas("shower", "shower", 1600, 500);
    fCanvas->Print("pictures/shower_rec.pdf[");
  }
  
  if (fPhotonCalib) {
    for (int pmt = fPmtType; pmt < 2; ++pmt) {
      TString fileName = Form("results/photon_calib_parameters_%d%s.dat", fEventLength, fPmtStr[pmt].Data());
      fOutputFile[pmt].open(fileName);
    }
  } 
}


SphShowerRec::~SphShowerRec()
{
  if (!fPhotonCalib) {

    for (int pmtType = fPmtType; pmtType < 2; ++pmtType) {
      delete [] fSplc[pmtType];
      delete [] fSpline_c[pmtType];
    }
  }
  
  if (fCanvas) {
    fCanvas->cd();
    fCanvas->Print("pictures/shower_rec.pdf]");
    delete fCanvas;
  }

  if (fPhotonCalib) {fOutputFile[0].close(); fOutputFile[1].close();}
}


//----------------------------------------------------------------------------
bool SphShowerRec::DefineShower(double sumPE[], vector<track_t> &muonCandidate)
{
  // define shower

  // define minimum distance to the muon track(s)

  int n_muons = muonCandidate.size();
  
  double PEshift = 15000000;
    
  if (n_muons == 2) PEshift = 25000000;
  else if (n_muons == 3) PEshift = 45000000;

  double dist2 = 400;
  
  for (auto &track : muonCandidate) {
    TVector3 initMuon = track.second - track.first;
    TVector3 m1H = track.first-initMuon*(track.first.Dot(initMuon)/initMuon.Mag2());
    dist2 = TMath::Min(dist2, m1H.Mag2());
  }

  return (sumPE[1] > (-57998.5*dist2 + 629692*sqrt(dist2) + PEshift));
}


//----------------------------------------------------------------------------
void SphShowerRec::FillCoeffFromSplines(double radius)
{
  int k = 0;

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int n = 0; n <= fMaxOrder; ++n) {
    int index = gsl_sf_legendre_array_index(n, k);
    for (int pmtType = fPmtType; pmtType < 2; ++pmtType)
      fSplc[pmtType][index] = GetSplEvalCos(pmtType, index, radius);
  }
}


//----------------------------------------------------------------------------
void SphShowerRec::GetSphericalFunction(double cos_theta, double *f)
{
  double *legendre = new double [fLArraySize];
  gsl_sf_legendre_array_e(GSL_SF_LEGENDRE_NONE, fMaxOrder, cos_theta, -1,
			  legendre);

  int k = 0;
  for (int n = 0; n <= fMaxOrder; ++n) {
    int index = gsl_sf_legendre_array_index(n, k);
    for (int pmtType = fPmtType; pmtType < 2; ++pmtType)
      f[pmtType] += legendre[index]*fSplc[pmtType][index];
  }

  delete [] legendre;
}


//----------------------------------------------------------------------------
void SphShowerRec::SaveSphFuncParameters(double **cnk, double **snk,
					 double sumPE[])
{
  double dist = fMuonSim.front().endMuonPosition.z();
  
  if (dist < -0.12) return;

  // get released energy

  double energy = 0;
  
  for (auto &iMuon : fMuonSim)
    energy += (iMuon.initMuonMomentum-iMuon.endMuonMomentum).Mag();
  
  // writing spherical function parameters
  
  for (int pmtType = fPmtType; pmtType < 2; ++pmtType) {

    for (int n = 0; n <= fMaxOrder; ++n)
      for (int k = 0; k <= n; ++k) {
	int index = gsl_sf_legendre_array_index(n, k);
	if (sumPE[pmtType]) fOutputFile[pmtType] << dist << " "
	 					 << n << " " << k << " "
	 					 << cnk[pmtType][index] << " "
	 					 << snk[pmtType][index] << " "
	 					 << sumPE[pmtType] << " "
						 << energy << "\n";
	else fOutputFile[pmtType] << dist << " " << n << " " << k << " "
				  << -10000 << " " << -10000 << " " << 0 << " "
				  << 0 << "\n";
      }
  }
}


//----------------------------------------------------------------------------
void SphShowerRec::LoadSphFuncParameters()
{
  fLArraySize = gsl_sf_legendre_array_n(fMaxOrder);

  for (int pmtType = fPmtType; pmtType < 2; ++pmtType) {
    fSplc[pmtType] = new double [fLArraySize];
    fSpline_c[pmtType] = new TSpline5 [fLArraySize];
  }
  
  // read splines from file

  TString SPHFUNC_DATA = (TString)getenv("RECMU_SPHFUNC_DATA");
  TFile fileSpline(SPHFUNC_DATA + Form("/photons_spline_%d.root", fEventLength), "read");
  
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

    pmtType = (TString(splineName(17, 1))).Atoi();
    if (pmtType != 0 && pmtType != 1) continue;

    int length = splineName.Length()-19;
    int index = (TString(splineName(19, length))).Atoi();
    if (index > maxOrder) continue;
    if (index > maxIndex) maxIndex = index;

    TSpline5 *spline = (TSpline5*)gDirectory->Get(splineName);
    
    if (TString(splineName(15, 1)) == "c")
      fSpline_c[pmtType][index] = TSpline5(*spline);
    else continue;
  }

  // calculate max order

  if (maxIndex < maxOrder) fMaxOrder = int(0.5*(sqrt(9+8*maxIndex)-3));

  fileSpline.Close();
}


//----------------------------------------------------------------------------
void SphShowerRec::DrawShower(double par[], int nTracks, EventSimRec_t &eventSimRec)
{
  eventSimRec.hEventRecShower = new TH2F("reconstructed muon with shower",
					 "reconstructed muon with shower",
					 90, -180, 180, 90, -90, 90);

  FillCoeffFromSplines(par[nTracks*5]);

  double cos_alpha =  cos(par[nTracks*5+1]);
  double sin_alpha = -sin(par[nTracks*5+1]);
  double cos_phi   =  cos(par[nTracks*5+2]);
  double sin_phi   = -sin(par[nTracks*5+2]);
  
  eventSimRec.hEventRecShower->GetXaxis()->SetTitle("#phi^{#circ}");
  eventSimRec.hEventRecShower->GetYaxis()->SetTitle("#theta^{#circ}");

  for (int binx = 1; binx <= eventSimRec.hEventRecShower->GetNbinsX(); ++binx)
    for (int biny = 1; biny <= eventSimRec.hEventRecShower->GetNbinsY(); ++biny){
      double f3[2] = {0, 0};
      double phi = eventSimRec.hEventRecShower->GetXaxis()->GetBinCenter(binx);
      double theta = eventSimRec.hEventRecShower->GetYaxis()->GetBinCenter(biny);

      double cos_theta = cos(fDegToRad*(90-theta));
      double sin_theta = sin(fDegToRad*(90-theta));
      
      double x = sin_theta*cos(fDegToRad*phi);
      double y = sin_theta*sin(fDegToRad*phi);
      double z = cos_theta;

      // reverse rotation
      
      // first rotation around OZ axis

      double x1 = x*cos_phi - y*sin_phi;
      
      // second rotation around OY axis

      double cos_th1 = -x1*sin_alpha + z*cos_alpha; // z2

      GetSphericalFunction(cos_th1, f3);
      double muonSignal = eventSimRec.hEventRecMuon->GetBinContent(binx, biny);
      // eventSimRec.hEventRecShower->SetBinContent(binx, biny, (f3[0] + f3[1])*par[nTracks*5+3]);
      eventSimRec.hEventRecShower->SetBinContent(binx, biny, (f3[0] + f3[1])*par[nTracks*5+3] + muonSignal);
    }

  if (fCanvas) {
    fCanvas->cd();
    eventSimRec.hEventRecShower->Draw("COLZ");
    fCanvas->Print("pictures/shower_rec.pdf");
  }
}
