/*=============================================================================
#
# Author: Artem Chukanov - chukanov@jinr.ru
# Created: 26.01.2018
# Filename: SphMuonRecTool.cxx
# Description:
=============================================================================*/
#include "SphMuonRecTool.h"

#include "RootIOSvc/RootInputSvc.h"
#include "EvtNavigator/NavBuffer.h"
#include "SniperKernel/ToolFactory.h"

#include <Math/SpecFunc.h>
#include <TMath.h>
#include <TF2.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TSystem.h>
#include <TList.h>
#include <TFile.h>
#include <TStyle.h>
#include <TTree.h>
#include <TKey.h>

#include <fstream>

using namespace std;

DECLARE_TOOL(SphMuonRecTool);

std::ostream& operator << (std::ostream& s, const TVector3& v){
  s << "(" << v.x() <<  "," << v.y() << "," << v.z() << ")";
  return s;
}

SphMuonRecTool *rotateFCN::fSphf = nullptr;

SphMuonRecTool::SphMuonRecTool(const std::string& name) : ToolBase(name) {

#ifdef _OPENMP
  omp_set_num_threads(4);
#endif

  gErrorIgnoreLevel = 5000;
  fMaxOrderInit = fMaxOrder = 15;
  fMuonCalibration = false;
  fMakePmtMap = false;
  fSetPmtDistr = false;
  fPhotonResponseCalib = false; // calibration of photon response
  fUseMCtrue = true;
  fDrawShower = false;
  fProductionMode = true; // set UseMCtrue == false
  fPMTtype = 0; // 0 - using both pmt types, 1 - using only 20'' pmt
  pi = TMath::Pi();
  fEvt = -1;
  fEventLength = 300;     // ns
  fAngleStep = 5;         // degree

  // we will correct all signals from PMTs to this radius

  fRadius = 19.434;       // m
  fRadius2 = fRadius*fRadius;

  fDegToRad = TMath::DegToRad();
  fRadToDeg = TMath::RadToDeg();
  fEventSimRec.clear();
  fTracksRec.clear();
  fTracksNotConnected.clear();
  fDistMuonSimList.clear();
  fDistMuonRecList.clear();

  if (fProductionMode) fUseMCtrue = false;
  if (fMuonCalibration) fUseMCtrue = true;
  if (fPhotonResponseCalib) fUseMCtrue = true;
  
  bool mcalib = fMuonCalibration;

  if (fMakePmtMap) {
    mcalib = false;
    fMuonCalibration = true;
    fEventLength = 1000;
    fMaxOrderInit = fMaxOrder = 50;
  }

  fLArraySize = gsl_sf_legendre_array_n(fMaxOrder);
  
  fMuonCandidates = new MuonCandidates(fRadius, fPMTtype, fMuonCalibration);
  fMuonCandidates->SetProductionMode(fProductionMode);
  fSphMuonRec = new SphMuonRec(fPMTtype, fEventLength, mcalib);
  fSphMuonRec->SetMaxOrder(fMaxOrder);
  fSphShowerRec = new SphShowerRec(fRadius, fPMTtype, fPhotonResponseCalib, fEventLength, fProductionMode);
  fSphShowerRec->SetMaxOrder(fMaxOrder);

  // Cnk and Snk vectors initialization
  // "0" - 3 inch PMT, "1" - 20 inch PMT

  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
    fCnk[pmtType] = new double [fLArraySize];
    fSnk[pmtType] = new double [fLArraySize];
    fCnkPmt[pmtType] = new double [fLArraySize];
    fSnkPmt[pmtType] = new double [fLArraySize];
  }

  // calculate legendre functions

  if (!fMuonCalibration) {
    for (int theta = 0; theta < 180; ++theta) {
      fLegendre[theta] = new double [fLArraySize];
      double cos_theta = cos(fDegToRad*theta);
      gsl_sf_legendre_array_e(GSL_SF_LEGENDRE_NONE, fMaxOrder, cos_theta, -1,
			      fLegendre[theta]);
    }
  }

  // system("rm -f results/error_message.txt");

  if (!fProductionMode) {
    system("test -d results || mkdir results");
    system("test -d pictures || mkdir pictures");
  }
  
  TString pmtStr[2] = {"_3inch.dat", "_20inch.dat"};
  TString SPHFUNC_DATA = (TString)getenv("RECMU_SPHFUNC_DATA") + "/";

  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
    fFileParNamePmt[pmtType] = fMakePmtMap ? "results/" : SPHFUNC_DATA;
    fFileParNamePmt[pmtType] += "pmt_distribution" + pmtStr[pmtType];
  }
}


//----------------------------------------------------------------------------
SphMuonRecTool::~SphMuonRecTool()
{
  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
    delete [] fCnk[pmtType];
    delete [] fSnk[pmtType];
    delete [] fCnkPmt[pmtType];
    delete [] fSnkPmt[pmtType];
  }

  delete fMuonCandidates;
  delete fSphMuonRec;
  delete fSphShowerRec;
  
  if (!fMuonCalibration)
    for (int theta = 1; theta < 180; ++theta)
      delete [] fLegendre[theta];
}


//----------------------------------------------------------------------------
bool SphMuonRecTool::configure(const Params*, const PmtTable *ptab)
{
  gStyle->SetOptStat("");

  fPmtPtr = ptab;

  LogDebug  << "configure the reconstruct tool [SphMuonRecTool]!" << endl; 

  // loading pmt distribution spherical function parameters

  if (!fMakePmtMap) LoadPmtDistr();
  
  // loading spherical function parameters for muon reconstruction
  
  if (!fMuonCalibration) fMaxOrder = fSphMuonRec->LoadSphFuncParameters(); 

  // loading spherical function pareameters for shower reconstruction
  
  if (!fPhotonResponseCalib) fSphShowerRec->LoadSphFuncParameters(); 
  
  return true; 
}


void outputpmtinfo(const PmtTable &oriptab, const PmtTable &ptab,
		   std::ofstream &f)
{
  for (unsigned int i = 0; i < ptab.size(); ++i) {
    if (ptab[i].used) {
      f << i << " " << ptab[i].q << " " << oriptab[i].fht << " "
	<< ptab[i].fht << std::endl;
    }
  }
}


//----------------------------------------------------------------------------
bool SphMuonRecTool::reconstruct(RecTrks* trks)
{
  double par[35], x[2];
  fIncludeShower = false;
  fSumPE[0] = fSumPE[1] = 0;
  fBeginEventTime = -1;
  fPmtTable = *fPmtPtr;
  
  ++fEvt;

  LogDebug << "---------------------------------------" << endl;
  LogDebug << "Processing event " << fEvt << endl;

  if (fMakePmtMap && fEvt > 0) return true;

  if (fUseMCtrue) GetTrueMuonDirection(); // get true muon positions

  // if (fEvt > 1) return true;

  EventSimRec_t &eventSimRec = fEventSimRec[fEvt];
  if (!fUseMCtrue) eventSimRec.muonSim.clear();

  // set data for detector response calibration

  if (fPhotonResponseCalib)
    fSphShowerRec->SetMuonInit(eventSimRec.muonSim);

  // histograms initialization for sin and cos coefficients

  if (!fProductionMode)
    eventSimRec.hEventRaw = new TH2F(Form("event, %d", fEvt), "Raw event",
				     45, -180, 180, 30, -90, 90);

  // spherical function coefficient arrays initialization

  for (int n = 0; n <= fMaxOrder; ++n)
    for (int k = 0; k <= n; ++k) {
      int index = gsl_sf_legendre_array_index(n, k);
      for (int pmtType = fPMTtype; pmtType < 2; ++pmtType)
      fCnk[pmtType][index] = fSnk[pmtType][index] = 0;
    }

  double minTime = GetMinTime(); // get minimum time

  double *legendre = new double [fLArraySize];
  double *sin_lam = new double [fMaxOrder+1];
  double *cos_lam = new double [fMaxOrder+1];

  // function for Pmt responce

  fSetPmtDistr = true;
  LegendreFunction::funcLegendre *funcF = new LegendreFunction::funcLegendre(this);
  TF2 *func = new TF2("func", funcF, -7, 7, -7, 7, 0);
  
  fShowerStartPoint.SetXYZ(0,0,0);

  for (auto &itPmt : fPmtTable) {
    if (!(itPmt.used && itPmt.loc == 1)) continue;

    int pmtType = (itPmt.type == _PMTINCH20) ? 1 : 0;

    double pmtTheta = itPmt.pos.Theta();
    double pmtThetaDeg = itPmt.pos.Theta()*fRadToDeg;
    double ctheta = cos(pmtTheta);
    double lam = itPmt.pos.Phi();
    double pmtDistCorr = 1000000*fRadius2/itPmt.pos.Mag2();
    double square = (itPmt.type == _PMTINCH20) ? 0.202683 : 0.004653; // m2
    square *= pmtDistCorr;

    // lam is in the range 0 .. 2pi

    if (lam < 0) lam += 2*pi;

    double nPE = 0;

    for (size_t i = 0; i < itPmt.hittime.size(); ++i) {
      if (itPmt.hittime[i] < fEventLength + minTime) nPE += itPmt.hitq[i];
      else break;
    }

    // correction for the Pmt distribution

    if (!fMakePmtMap) {
      x[0] = pmtTheta; x[1] = lam; par[0] = pmtType;
      square *= nPE/func->EvalPar(x, par);
    }

    fShowerStartPoint += itPmt.pos*nPE;

    fSumPE[pmtType] += nPE;

    if (lam > pi) lam -= 2*pi;

    if (pmtType == 1 && sin(pmtTheta) && eventSimRec.hEventRaw) eventSimRec.hEventRaw->Fill(lam*fRadToDeg, 90-pmtThetaDeg, nPE/sin(pmtTheta));

    if (lam < 0) lam += 2*pi;

    // get Legendre array

    gsl_sf_legendre_array_e(GSL_SF_LEGENDRE_NONE,fMaxOrder,ctheta,-1,legendre);
    LegendreFunction::SinCosCalculation(fMaxOrder, lam, sin_lam, cos_lam);

#ifdef _OPENMP
#pragma omp parallel for
#endif
    for (int k = 0; k <= fMaxOrder; ++k) {
      for (int n = k; n <= fMaxOrder; ++n) {
        int index = gsl_sf_legendre_array_index(n, k);
        fCnk[pmtType][index] += square*legendre[index]*cos_lam[k];
        fSnk[pmtType][index] += square*legendre[index]*sin_lam[k];
      }
    }
  }

  if (fUseMCtrue) {
    for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
      if (fSumPE[pmtType] < 1000) {
	if (!fOutputFileError.is_open())
	  fOutputFileError.open("results/error_message.txt");
	fOutputFileError << fEvt << " " << fSumPE[pmtType] << endl;

	// sometimes we have empty extra events. We should correctly skip their
	// to have a right muon track position

	if (eventSimRec.muonSim.front().initMuonPosition.z() != eventSimRec.muonSim.front().endMuonPosition.z()) {
	  --fEvt;
	  SafeDelete(eventSimRec.hEventRaw);
	}
	return true;
      }
    }
  }

  fShowerStartPoint *= 0.001/(fSumPE[0] + fSumPE[1]);

#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int n = 0; n <= fMaxOrder; ++n){
    double k1 = (2.*n+1.)/(2.*pi*fRadius2);
    for (int k = 0; k <= n; ++k) {
      double delta_k = (k == 0) ? 2. : 1.;
      double k2 = k1*TMath::Factorial(n-k)/TMath::Factorial(n+k)/delta_k;
      int index = gsl_sf_legendre_array_index(n, k);
      for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
        fCnk[pmtType][index] *= k2; fSnk[pmtType][index] *= k2;
      }
    }
  }

  fSetPmtDistr = false;

  if (fPhotonResponseCalib)
    fSphShowerRec->SaveSphFuncParameters(fCnk, fSnk, fSumPE);
  
  // draw function

  // filling histogram with PMT responce

  if (!fProductionMode) {
    eventSimRec.hEventLegendre = new TH2F(Form("legendre, %d", fEvt),
					  "Event in spherical functions",
					  45, -180, 180, 45, -90, 90);

    eventSimRec.hEventLegendre->GetXaxis()->SetTitle("#phi^{#circ}");
    eventSimRec.hEventLegendre->GetYaxis()->SetTitle("#theta^{#circ}");

    for (int binx = 1; binx <= eventSimRec.hEventLegendre->GetNbinsX(); ++binx) {
      x[1]=eventSimRec.hEventLegendre->GetXaxis()->GetBinCenter(binx)*fDegToRad;
      for (int biny=1; biny <= eventSimRec.hEventLegendre->GetNbinsY(); ++biny){
	double theta=eventSimRec.hEventLegendre->GetYaxis()->GetBinCenter(biny);
	x[0] = (90-theta)*fDegToRad;
	double cont = 0;
	for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
	  par[0] = pmtType; cont += func->EvalPar(x, par);
	}
	eventSimRec.hEventLegendre->SetBinContent(binx, biny, cont);
      }
    }
  }
  
  if (fPhotonResponseCalib) {
    TCanvas *canvas = new TCanvas("c1", "c1", 3600, 1200);
    canvas->cd(); eventSimRec.hEventLegendre->Draw("COLZ");
    canvas->Print(Form("pictures/shower_legendre_%d.png", fEvt));
    delete canvas;
    return true;
  }

  // writing event energy to file

  if (!fMuonCalibration && fUseMCtrue) {
    if (!fOutputFileEnergy.is_open())
      fOutputFileEnergy.open(Form("results/event_energy_%d.dat", fEventLength));
    fOutputFileEnergy << fSumPE[0] << " " << fSumPE[1] << endl;
  }

  if (fMuonCalibration) {
    // writing parameters of spherical functions for event and PMT distributions
    if (fMakePmtMap)
      SaveSphFuncParameters();
    else 
      fSphMuonRec->SaveSphFuncParameters(eventSimRec, fEvt, fCnk, fSnk, fSumPE);
  }
  else {

    // Giving information about WP PMTs

    fMuonCandidates->SetWPsignal(fPmtTable);
    fMuonCandidates->SetEventNumber(fEvt);

    // define roughly initial and outgoing points for muon track

    fMuonCandidates->DefineMuonCandidates(func, eventSimRec.muonCandidates,
					  eventSimRec.hEventLegendre);

    double chi2 = MuonTrackReconstruction(par);
    
    if (!fProductionMode) {
      fSphMuonRec->FillMuonRecHisto(eventSimRec, fEvt, par, fNtracksMn);
      if (fIncludeShower) fSphShowerRec->DrawShower(par,fNtracksMn,eventSimRec);
    }

    // compare results with simulated muons

    CompareResults(par, chi2, trks, minTime);

  }

  delete func;
  delete funcF;
  delete [] legendre;
  delete [] sin_lam;
  delete [] cos_lam;
  
  return true;
}


//----------------------------------------------------------------------------
bool SphMuonRecTool::finalize()
{
  if (fProductionMode) return true;

  if (!fOutputFileError.is_open()) fOutputFileError.close();
  if (!fOutputFileEnergy.is_open()) fOutputFileEnergy.close();

  TCanvas *canvas;
  TPad *pad1 = NULL, *pad2 = NULL, *pad3 = NULL;

  if (fDrawShower && !fMuonCalibration) {
    canvas = new TCanvas("c1", "c1", 3600, 1200);
    canvas->cd();
    pad1 = new TPad("pad1", "pad1", 0, 0, 0.3, 1);
    pad2 = new TPad("pad2", "pad2", 0.33, 0, 0.66, 1);
    pad3 = new TPad("pad3", "pad3", 0.69, 0, 1, 1);
    pad1->Draw(); pad2->Draw(); pad3->Draw();
  }
  else if (!fMuonCalibration) {
    canvas = new TCanvas("c1", "c1", 2400, 1200);
    canvas->cd();
    pad1 = new TPad("pad1", "pad1", 0, 0, 0.48, 1);
    pad2 = new TPad("pad2", "pad2", 0.5, 0, 0.98, 1);
    pad1->Draw(); pad2->Draw();
  }
  else {
    canvas = new TCanvas("c1", "c1", 1200, 1200);
    canvas->cd();
    pad1 = new TPad("pad1", "pad1", 0, 0, 1, 1);
    pad1->Draw();
  }

  canvas->Print("results/function.pdf[");

  for (auto iEvt : fEventSimRec) {

    EventSimRec_t &eventSimRec = iEvt.second;

    if (!eventSimRec.hEventRaw || !eventSimRec.hEventLegendre) continue;

    // TList *functions = eventSimRec.hEventLegendre->GetListOfFunctions();
    // TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");

    // if (eventSimRec.hEventRecMuon) {
    //   TList *functions1 = eventSimRec.hEventRecMuon->GetListOfFunctions();
    //   TPolyMarker *pm1 = (TPolyMarker*)functions1->FindObject("TPolyMarker");
    //   if (pm1) pm1->Draw("same");
    // }
    
    eventSimRec.hEventLegendre->GetXaxis()->SetTitle("#phi, deg");
    eventSimRec.hEventLegendre->GetYaxis()->SetTitle("#theta, deg");
    pad1->cd(); eventSimRec.hEventLegendre->Draw("colz");
    
    // if (fMakePmtMap) {
    //   TCanvas *canvas1 = new TCanvas("c2", "c2", 1800, 1200);
    //   eventSimRec.hEventLegendre->GetZaxis()->SetRangeUser(0, 3);
    //   canvas1->cd(1); eventSimRec.hEventLegendre->Draw("aitoff");
    //   canvas1->Print("pictures/pmt_distribution.pdf");
    //   delete canvas1;
    // }

    for (auto &iMuon : eventSimRec.muonSim) iMuon.markerMuonPosition->Draw("same");
    // if (pm) pm->Draw("same");
    
    if (pad2 && !fPhotonResponseCalib) {
      eventSimRec.hEventRecMuon->GetXaxis()->SetTitle("#phi, deg");
      eventSimRec.hEventRecMuon->GetYaxis()->SetTitle("#theta, deg");
      pad2->cd(); eventSimRec.hEventRecMuon->Draw("colz");

      for (auto &iMuon : eventSimRec.muonSim) iMuon.markerMuonPosition->Draw("same");
      // if (pm) pm->Draw("same");

      TList *functions1 = eventSimRec.hEventRecMuon->GetListOfFunctions();
      TPolyMarker *pm1 = (TPolyMarker*)functions1->FindObject("TPolyMarker");
      if (pm1) pm1->Draw("same");
    }

    if (pad3 && eventSimRec.hEventRecShower && fIncludeShower) {
      pad3->cd(); eventSimRec.hEventRecShower->Draw("COLZ");
      for (auto &iMuon : eventSimRec.muonSim) iMuon.markerMuonPosition->Draw("same");
      TList *functions1 = eventSimRec.hEventRecMuon->GetListOfFunctions();
      TPolyMarker *pm1 = (TPolyMarker*)functions1->FindObject("TPolyMarker");
      if (pm1) pm1->Draw("same");
    }
    canvas->Print("results/function.pdf");
    canvas->Print(Form("results/function_%d.pdf", iEvt.first));

    if (pad1) pad1->Clear();
    if (pad2) pad2->Clear();
    if (pad3) pad3->Clear();
  }

  canvas->Print("results/function.pdf]");

  // write muon reconstruction resolution

  if (fUseMCtrue) {
  
    if (!fMuonCalibration && !fPhotonResponseCalib) {

      ofstream output_file(Form("results/muon_resolution_%d.dat", fEventLength), ios::out);

      for (auto &iMuon : fTracksRec) {
	output_file << iMuon.eventNumber << " " << iMuon.recDistCenter << " "
		    << iMuon.recDistMuon << " " << iMuon.chi2 << " "
		    << iMuon.scaleEn << " " << iMuon.sumPE[0] << " "
		    << iMuon.sumPE[1] << " " << iMuon.intEnergyTrue << " "
		    << iMuon.intEnergyRec << " " << iMuon.energyLossTrue << " "
		    << iMuon.energyLossRec << endl;
      }

      output_file.close();

      output_file.open(Form("results/double_muon_resolution_separated_%d.dat", fEventLength), ios::out);
      // output_file.open(Form("results/double_muon_resolution_%d.dat", fEventLength), ios::out);

      for (auto &iMuon : fDistMuonSimList) output_file<< "sim " << iMuon << endl;
      for (auto &iMuon : fDistMuonRecList) output_file<< "rec " << iMuon << endl;

      output_file.close();

      // writing not connected muons

      output_file.open(Form("results/muons_not_connected_%d.dat", fEventLength), ios::out);

      for (auto &iMuon : fTracksNotConnected)
	output_file << iMuon.eventNumber << " " << iMuon.scaleEn << endl;

      output_file.close();
    }
  }

  delete canvas;

  return true;
}


//----------------------------------------------------------------------------
double SphMuonRecTool::MuonTrackReconstruction(double par[])
{
  // searching for the sin and cos coefficients

  int n_tracks = 0, n_prev_tracks=0, n_fits = 0, Ntracks = 0, trackToRefit = -1;
  double x_pos[5], phi1[5], phi2[5], k1[5], alpha[5];
  double sum, chi2 = 1000;
  double chi2min = 10000;

  // initialization of parameters

  for (int i = 0; i < 5; ++i) {
    x_pos[i] = 5; phi1[i] = phi2[i] = alpha[i] = 0.; k1[i] = 1;
  }

  LogDebug << "number of muon combinations: "
	   << fEventSimRec[fEvt].muonCandidates.size() << endl;

  for (auto &muonCandidate : fEventSimRec[fEvt].muonCandidates) {

    trackToRefit = -1;

    // get initial starting values

    n_prev_tracks = n_tracks;

    n_tracks = GetInitialValues(x_pos, phi1, phi2, alpha, muonCandidate);
    
    bool new_track;

    fIncludeShower = fSphShowerRec->DefineShower(fSumPE, muonCandidate);

    if (fIncludeShower) LogDebug << "detecting electromagnetic shower" << endl;
    
    do { // loop for the muon tracks number

      new_track = false;

      ++n_fits;

      // Minimizer initalization

      fNtracksMn = n_tracks;

      ROOT::Math::Minimizer *minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
      minimizer->SetMaxFunctionCalls(5000);
      minimizer->SetMaxIterations(5000);
      minimizer->SetTolerance(0.001);

      int showerVar = fIncludeShower ? 4 : 0;
      rotateFCN::SetFunc(this);
      ROOT::Math::Functor rotateFunc(&(rotateFCN::RotateFCN), n_tracks*5 + showerVar);
      minimizer->SetFunction(rotateFunc);

      // Set starting values and step sizes for parameters

      for (int i = 0; i < n_tracks; ++i) {
	k1[i] = 1;
        minimizer->SetLimitedVariable(i*5 + 0, Form("position %d", i), x_pos[i], 0.1, 0, 18);
        minimizer->SetLimitedVariable(i*5 + 1, Form("phi1 %d", i),     phi1[i],  0.001, -10000., 10000.);
        minimizer->SetLimitedVariable(i*5 + 2, Form("phi2 %d", i),     phi2[i],  0.001, -10000., 10000.);
        minimizer->SetLimitedVariable(i*5 + 3, Form("alpha %d", i),    alpha[i], 0.001, -10000., 10000.);
        minimizer->SetLowerLimitedVariable(i*5 + 4, Form("k1 %d", i), k1[i], 0.001, 0);
	// minimizer->FixVariable(i*5+0);
	// minimizer->FixVariable(i*5+1);
	// minimizer->FixVariable(i*5+2);
	// minimizer->FixVariable(i*5+3);
	// minimizer->FixVariable(i*5+4);
      }

      if (fIncludeShower) {
	minimizer->SetLimitedVariable(n_tracks*5 + 0, "shower position", fShowerStartPoint.Mag(), 0.1, 0, 18);
        minimizer->SetLimitedVariable(n_tracks*5 + 1, "theta", fShowerStartPoint.Theta(), 0.001, -10000, 10000);
        minimizer->SetLimitedVariable(n_tracks*5 + 2, "phi",   fShowerStartPoint.Phi(),   0.001, -10000, 10000);
	minimizer->SetLowerLimitedVariable(n_tracks*5 + 3, "shower energy", 30, 0.001, 0);
      }

      minimizer->Minimize();
      const double *par1 = minimizer->X();
      chi2 = minimizer->MinValue();
      
      LogDebug << "chi2 " << chi2 << endl;

      // get parameters

      sum = 0;
      double angleAdd = 0;

      if (trackToRefit >= 0) {

	// if one more muon track was added, it should be parallel to found one
      
	TVector3 prevMuon(0, 0, -1); // initial direction
	TVector3 addedMuon(0, 0, -1); // initial direction

	// first muon track
	prevMuon.RotateZ(-par[trackToRefit*5+2]); // around Oz
	prevMuon.RotateX(-par[trackToRefit*5+3]); // around Ox'
	prevMuon.RotateZ(-par[trackToRefit*5+1]); // around Oz'

	// added muon track
	addedMuon.RotateZ(-par1[(n_tracks-1)*5+2]); // around Oz
	addedMuon.RotateX(-par1[(n_tracks-1)*5+3]); // around Ox'
	addedMuon.RotateZ(-par1[(n_tracks-1)*5+1]); // around Oz'
	angleAdd = prevMuon.Angle(addedMuon);
	
	LogDebug << "angle between two tracks: " << angleAdd << " rad" << endl;
      }

      // check the scale factor and if one of them is very low
      // then we will discard this combination
      
      for (int i = 0; i < n_tracks; ++i)
       	if (par1[i*5+4] < 0.6) {chi2 = 1000; break;}
      
      // in a case of adding new track chi2 should be 2 times less than previuos

      if ((0.6*chi2min > chi2 && trackToRefit >= 0 && angleAdd < 0.03) ||
	  (0.8*chi2min > chi2 && trackToRefit < 0) ||
	  (chi2min > chi2 && n_tracks == n_prev_tracks)) {

	chi2min = chi2;
	Ntracks = n_tracks;

	for (int i = 0; i < n_tracks; ++i) {
	  par[i*5+0] = x_pos[i] = par1[i*5+0];
	  par[i*5+1] = phi1[i]  = par1[i*5+1];
	  par[i*5+2] = phi2[i]  = par1[i*5+2];
	  par[i*5+3] = alpha[i] = par1[i*5+3];
	  par[i*5+4] = k1[i]    = par1[i*5+4];
	  sum += k1[i];
	}

	if (fIncludeShower) {
	  par[n_tracks*5+0] = par1[n_tracks*5+0];
	  par[n_tracks*5+1] = par1[n_tracks*5+1];
	  par[n_tracks*5+2] = par1[n_tracks*5+2];
	  par[n_tracks*5+3] = par1[n_tracks*5+3];
	}
	
	new_track = false;

	// try to add new track if the sum is to high

	trackToRefit = fMuonCandidates->DefineNumberOfMuons(par, n_tracks);

	if (trackToRefit > -1) {
	  new_track = true;
	  x_pos[n_tracks] = par1[trackToRefit*5+0];
	  phi1[n_tracks]  = par1[trackToRefit*5+1];
	  phi2[n_tracks]  = par1[trackToRefit*5+2];
	  alpha[n_tracks] = par1[trackToRefit*5+3];
	  k1[n_tracks]    = par1[trackToRefit*5+4];

	  if (x_pos[n_tracks] > 2 && x_pos[n_tracks] < fRadius - 2) {
	    x_pos[n_tracks] += 1; x_pos[trackToRefit] -= 1;
	  }
	  else if (x_pos[n_tracks] <= 2) x_pos[n_tracks] += 2;
	  else x_pos[n_tracks] -= 2;

	  LogDebug << "adding new track" << endl;
	  ++n_tracks;
	}
      }
      else if (trackToRefit >= 0) LogDebug << "New track was rejected" << endl;

      // if (!fIncludeShower) {fIncludeShower = true; continue;}

    } while (n_fits < 10 && new_track);
  }
  
  // get scale factor for all muons

  if (fUseMCtrue) {

    // double muonPE = 0;
    trackRec_t trackRec;

    for (int i = 0; i < Ntracks; ++i) {
      trackRec.eventNumber = fEvt;
      trackRec.scaleEn = par[i*5+4];
      fTracksNotConnected.push_back(trackRec);

      // get energy released by muon tracks

      // muonPE+=(-72335.6*par[i*5]*par[i*5]+810109*par[i*5]+11882500)*par[i*5+4];
    }

    // event energy

  //   double energyTrue = 0;
    
  //   for (auto &iMuon : fEventSimRec[fEvt].muonSim)
  //     energyTrue += iMuon.initMuonMomentum.Mag() - iMuon.endMuonMomentum.Mag();
    
  //   double energy = 0.70664*0.000001*fSumPE[1];

  //   cout << "event energy: " << energy << "\n"
  // 	 << "energy of muon: " << 0.70664*0.000001*muonPE << "\n";
    
  //   // energy of electromagnetic shower

  //   if (fIncludeShower) {
  //     cout << "shower energy: " << energy - 0.70664*0.000001*muonPE << "\n";

  //     for (auto &iMuon : fEventSimRec[fEvt].muonSim) {
  //     	if (iMuon.intEnergy > 0)
  // 	  cout << "simulated shower energy: " << iMuon.intEnergy << "\n";
  //     }
  //   }

  //   cout << "simulated energy: " << energyTrue << "\n"; 

  }
  
  fNtracksMn = Ntracks;
  return chi2;
}


//----------------------------------------------------------------------------
void SphMuonRecTool::CompareResults(double par[], double chi2, RecTrks *trks,
				   double startTime)
{
  double dist_sim, dist_rec, dist_rec2, scaleEn, showerEnergyRec = 0;
  EventSimRec_t &eventSimRec = fEventSimRec[fEvt];
  trackRec_t trackRec;

  // shower energy

  if (fIncludeShower) {
    double muonPE = 0;
    for (int j = 0; j < fNtracksMn; ++j)
      muonPE+=(-72335.6*par[j*5]*par[j*5]+810109*par[j*5]+11882500)*par[j*5+4]; 
    showerEnergyRec = (fSumPE[1]-muonPE)*0.7066*0.000001;
  }
    
  // compare closest obtained results with simulated muons

  for (auto &iMuon : eventSimRec.muonSim) {

    dist_sim = dist_rec = scaleEn = -1; dist_rec2 = 10;

    // check if muon stops in detector

    // if (eventSimRec.endMuonPosition[i].Mag() < 17.3) {
    //   dist_rec[i] = 0.9;
    //   cout << "Stopping point r = "<<eventSimRec.endMuonPosition[i].Mag()<<endl;
    // }

    // check that the muon enter to the LS

    if (iMuon.initMuonPosition == iMuon.endMuonPosition) continue;

    // TVector3 m1 = -eventSimRec.beginMuonPosition[i];
    // TVector3 m2 = eventSimRec.endMuonPosition[i];
    // TVector3 initMuon = m2 + m1;

    // TVector3 m1H = initMuon*(m1.Dot(initMuon)/initMuon.Mag2()) + eventSimRec.beginMuonPosition[i];
    // double dist = m1H.Mag();

    // for (int j = 0; j < fNtracksMn; ++j) {

    //   // // reconstructed muon

    //   // TVector3 recMuon(0, 0, -1); // initial direction

    //   // // rotate initial vector

    //   // recMuon.RotateZ(-par[j*5+2]); // around Oz
    //   // recMuon.RotateX(-par[j*5+3]); // around Ox'
    //   // recMuon.RotateZ(-par[j*5+1]); // around Oz'

    //   // // middle point in reconstructed muon track

    //   // TVector3 middleRec(par[j*5], 0, 0);

    //   // middleRec.RotateZ(-par[j*5+2]); // around Oz
    //   // middleRec.RotateX(-par[j*5+3]); // around Ox'
    //   // middleRec.RotateZ(-par[j*5+1]); // around Oz'

    //   // // distance from reconstructed muon track to input and output points
    //   // // of simulated track

    //   // double dist1 = ((m1-middleRec).Cross(recMuon)).Mag(); // |recMuon| = 1
    //   // double dist2 = ((m2-middleRec).Cross(recMuon)).Mag(); // |recMuon| = 1

    //   // double distMuons = TMath::Max(dist1, dist2);

    //   // get distance from rotated calibration muon to the detector center
    //   // rotate middle point of calibration muon

    //   TVector3 middleCalib(par[j*5], 0, 0);

    //   middleCalib.RotateZ(-par[j*5+2]); // around Oz
    //   middleCalib.RotateX(-par[j*5+3]); // around Ox'
    //   middleCalib.RotateZ(-par[j*5+1]); // around Oz'

    //   // distance between center of initial muon and calibrated muon

    //   double distMuons = (m1H-middleCalib).Mag();
    //   double distMuons2 = dist - par[j*5];

    //   TVector3 calibMuon(0, 0, -1); // vector of calibrated muons

    //   // rotate calibration muon

    //   calibMuon.RotateZ(-par[j*5+2]); // around Oz
    //   calibMuon.RotateX(-par[j*5+3]); // around Ox'
    //   calibMuon.RotateZ(-par[j*5+1]); // around Oz'

    //   double angle = TMath::Abs(initMuon.Angle(calibMuon)*fRadToDeg);
    //   if (angle > 90) angle = 180-angle;

    //   if (distMuons < dist_rec[i]) {
    // 	dist_sim[i]  = dist;
    // 	dist_rec[i]  = distMuons;
    // 	dist_rec2[i] = distMuons2;
    // 	angle_rec[i] = angle;
    //   }
    //   else if (dist_sim[i] < 0) dist_sim[i] = dist;
    // }

    // check the maximum radius for excluding background events

    // get distance from inital muon to the detector center

    TVector3 m1 = iMuon.initMuonPosition;
    TVector3 m2 = iMuon.endMuonPosition;
    TVector3 simMuon = m2 - m1;
    double   distSim = ((0.5*(m1+m2)).Cross(simMuon)).Mag()/simMuon.Mag();

    for (int j = 0; j < fNtracksMn; ++j) {

      // reconstructed muon

      TVector3 recMuon(0, 0, -1); // initial direction

      // rotate initial vector

      recMuon.RotateZ(-par[j*5+2]); // around Oz
      recMuon.RotateX(-par[j*5+3]); // around Ox'
      recMuon.RotateZ(-par[j*5+1]); // around Oz'

      // middle point in reconstructed muon track

      TVector3 middleRec(par[j*5], 0, 0);

      middleRec.RotateZ(-par[j*5+2]); // around Oz
      middleRec.RotateX(-par[j*5+3]); // around Ox'
      middleRec.RotateZ(-par[j*5+1]); // around Oz'

      // distance from reconstructed muon track to input and output points
      // of simulated track

      double dist1 = ((m1-middleRec).Cross(recMuon)).Mag(); // |recMuon| = 1
      double dist2 = ((m2-middleRec).Cross(recMuon)).Mag(); // |recMuon| = 1

      double distMuons = TMath::Max(dist1, dist2);
      
      if (distMuons < dist_rec2) {
	dist_sim  = distSim;
	dist_rec  = par[j*5];
	dist_rec2 = distMuons;
	scaleEn = par[j*5+4];
      }
      else if (dist_sim < 0) dist_sim = distSim;
    }

    trackRec.eventNumber = fEvt;
    trackRec.recDistCenter = dist_rec;
    trackRec.recDistMuon = dist_rec2;
    trackRec.chi2 = chi2;
    trackRec.scaleEn = scaleEn;
    trackRec.sumPE[0] = fSumPE[0]; // p.e.
    trackRec.sumPE[1] = fSumPE[1]; // p.e.
    trackRec.intEnergyTrue = iMuon.intEnergy; // GeV
    trackRec.intEnergyRec = showerEnergyRec; // GeV
    trackRec.energyLossTrue = iMuon.initMuonMomentum.Mag()-iMuon.endMuonMomentum.Mag(); // GeV
    trackRec.energyLossRec = (-18843.7*dist_rec*dist_rec + 101989*dist_rec + 13101100)*scaleEn*0.70664*0.000001; // GeV
    
    fTracksRec.push_back(trackRec);
  }

  // removing connected muons from not connected list

  for (auto &iMuonRec : fTracksRec) {
    auto iMuonRec1 = std::begin(fTracksNotConnected);
    for ( ; iMuonRec1 != std::end(fTracksNotConnected); ++iMuonRec1) {
      if (iMuonRec.eventNumber == iMuonRec1->eventNumber &&
	  iMuonRec.scaleEn == iMuonRec1->scaleEn) {
	fTracksNotConnected.erase(iMuonRec1);
	break;
      }
    }
  }

  // reconstructed input and output points

  TPolyMarker *pm = NULL;

  if (!fProductionMode) {
    pm = (TPolyMarker*)eventSimRec.hEventRecMuon->GetListOfFunctions()->FindObject("TPolyMarker");
    if (pm) {
      eventSimRec.hEventRecMuon->GetListOfFunctions()->Remove(pm);
      delete pm;
    }

    pm = new TPolyMarker();
  }

  vector <track_t> muonTrack;
  muonTrack.clear();

  for (int j = 0; j < fNtracksMn; ++j) {

    // get incoming and outgoing points

    double z = TMath::Sqrt(fRadius2 - par[j*5]*par[j*5]);
    TVector3 inPoint(par[j*5], 0, z);
    TVector3 outPoint(par[j*5], 0, -z);

    inPoint.RotateZ(-par[j*5+2]); // around Oz
    inPoint.RotateX(-par[j*5+3]); // around Ox'
    inPoint.RotateZ(-par[j*5+1]); // around Oz'

    outPoint.RotateZ(-par[j*5+2]); // around Oz
    outPoint.RotateX(-par[j*5+3]); // around Ox'
    outPoint.RotateZ(-par[j*5+1]); // around Oz'
    
    // fill reconstructed tracks info

    trks->addTrk(inPoint, outPoint, startTime, -1, fSumPE[0]+fSumPE[1], chi2, 0);

    if (fProductionMode) continue;
    
    muonTrack.push_back(std::make_pair(inPoint, outPoint));

    double x_in  = inPoint.Phi()*fRadToDeg;
    double x_out = outPoint.Phi()*fRadToDeg;

    double y_in  = 90 - inPoint.Theta()*fRadToDeg;
    double y_out = 90 - outPoint.Theta()*fRadToDeg;
    pm->SetNextPoint(x_in, y_in);
    pm->SetNextPoint(x_out, y_out);
  }

  if (fProductionMode) return;
  
  // set polymarkers for found incoming and outgoing points

  eventSimRec.hEventRecMuon->GetListOfFunctions()->Add(pm);
  pm->SetMarkerStyle(29);
  pm->SetMarkerSize(3);
  pm->SetMarkerColor(6);

  // calculate energy deposition in incoming and outgoing points

  if (fUseMCtrue) fMuonCandidates->EnergyDepositionMuonTrue(eventSimRec.muonSim);

  // calculate distance between muons
  // simulated muons

  auto iMuon = std::begin(eventSimRec.muonSim);

  for ( ; iMuon != std::end(eventSimRec.muonSim); ++iMuon) {

    // check that the muon enter to the LS

    if (iMuon->initMuonPosition == iMuon->endMuonPosition) continue;

    TVector3 m1 = -iMuon->initMuonPosition;
    TVector3 m2 = iMuon->endMuonPosition;
    TVector3 initMuon = m2 + m1;

    // coordinate of the muon center

    TVector3 m1H = initMuon*(m1.Dot(initMuon)/initMuon.Mag2()) + iMuon->initMuonPosition;
    auto iMuon1 = std::next(iMuon);

    for ( ; iMuon1 != std::end(eventSimRec.muonSim); ++iMuon1) {

      // check that the muon enter to the LS

      if (iMuon1->initMuonPosition == iMuon1->endMuonPosition) continue;

      TVector3 ma1 = -iMuon1->initMuonPosition;
      TVector3 ma2 = iMuon1->endMuonPosition;
      TVector3 initMuon2 = ma2 + ma1;

      // coordinate of the muon center

      TVector3 ma1H = initMuon2*(ma1.Dot(initMuon2)/initMuon2.Mag2()) + iMuon1->initMuonPosition;

      fDistMuonSimList.push_back((ma1H-m1H).Mag());
    }
  }
  
  // reconstructed muons

  for (int i = 0; i < fNtracksMn; ++i) {

    // rotate middle point of calibration muon

    TVector3 middleCalib(par[i*5], 0, 0);

    middleCalib.RotateZ(-par[i*5+2]); // around Oz
    middleCalib.RotateX(-par[i*5+3]); // around Ox'
    middleCalib.RotateZ(-par[i*5+1]); // around Oz'

    for (int j = i+1; j < fNtracksMn; ++j) {

      // rotate middle point of calibration muon

      TVector3 middleCalib2(par[j*5], 0, 0);

      middleCalib2.RotateZ(-par[j*5+2]); // around Oz
      middleCalib2.RotateX(-par[j*5+3]); // around Ox'
      middleCalib2.RotateZ(-par[j*5+1]); // around Oz'

      // distance between two reconstructed tracks

      fDistMuonRecList.push_back((middleCalib-middleCalib2).Mag());
    }
  }
}


//----------------------------------------------------------------------------
double SphMuonRecTool::fcnRotation(const double u[])
{
  static const double square[2] = {1., 1.};
  double chi2 = 0;
  double cos_alpha[5], sin_alpha[5];
  double delta_phi1[5], delta_phi2[5];
  double cos_alpha_shower = 0, sin_alpha_shower = 0, cos_phi_shower = 0, sin_phi_shower = 0;

  for (int i = 0; i < fNtracksMn; ++i) {
    delta_phi1[i] = u[i*5+1];
    delta_phi2[i] = u[i*5+2];
    cos_alpha[i]  = cos(u[i*5+3]);
    sin_alpha[i]  = sin(u[i*5+3]);

    // calculate splines for muon track and write their to the array

    fSphMuonRec->FillCoeffFromSplines(i, u[i*5]);
  }

  if (fIncludeShower) {
    cos_alpha_shower =  cos(u[fNtracksMn*5+1]);
    sin_alpha_shower = -sin(u[fNtracksMn*5+1]);
    cos_phi_shower   =  cos(u[fNtracksMn*5+2]);
    sin_phi_shower   = -sin(u[fNtracksMn*5+2]);
    
    // calculate splines for shower and write their to the array

    fSphShowerRec->FillCoeffFromSplines(u[fNtracksMn*5]);
  }

#ifdef _OPENMP
#pragma omp parallel
  {
#pragma omp for reduction(+:chi2)
#endif
    for (int theta = fAngleStep; theta < 180; theta += fAngleStep) {
      double cos_theta = cos(fDegToRad*theta);
      double sin_theta = sin(fDegToRad*theta);
      double z = cos_theta, step = fAngleStep/sin_theta*fDegToRad;

      double *sin_phi = new double [fMaxOrder+1];
      double *cos_phi = new double [fMaxOrder+1];

      double f1[2], f2[2][5];
      double f3[2] = {0, 0};
      
      for (double phi = -pi; phi < pi; phi += step) {

	f1[0] = f1[1] = 0;
	
	LegendreFunction::SinCosCalculation(fMaxOrder, phi, sin_phi, cos_phi);

	for (int k = 0; k <= fMaxOrder; ++k) {
	  for (int n = k; n <= fMaxOrder; ++n) {
	    int index = gsl_sf_legendre_array_index(n, k);
	    for (int pmtType = fPMTtype; pmtType < 2; ++pmtType)
	      f1[pmtType] += fLegendre[theta][index]*
		(fCnk[pmtType][index]*cos_phi[k] + fSnk[pmtType][index]*sin_phi[k]);
	  }
	}

	for (int i = 0; i < fNtracksMn; ++i) {

	  f2[0][i] = f2[1][i] = 0;

	  // first rotation

	  double x = sin_theta*cos(phi + delta_phi1[i]);
	  double y = sin_theta*sin(phi + delta_phi1[i]);

	  // second rotation

	  double y1 = y*cos_alpha[i] - z*sin_alpha[i];
	  double cos_th1 = y*sin_alpha[i] + z*cos_alpha[i];

	  // third rotation

	  double phi2 = TMath::ATan2(y1, x) + delta_phi2[i];
	    
	  fSphMuonRec->GetSphericalFunction(i, cos_th1, phi2, f2);
	}

	// calculate functions for shower reconstruction

	if (fIncludeShower) {

	  f3[0] = f3[1] = 0;
	  
	  double x_sh = sin_theta*cos(phi);
	  double y_sh = sin_theta*sin(phi);
	  double z_sh = cos_theta;
	  
	  // reverse rotation
      
	  // first rotation around OZ axis

	  double x1_sh = x_sh*cos_phi_shower - y_sh*sin_phi_shower;
      
	  // second rotation around OY axis

	  double cos_th1 = -x1_sh*sin_alpha_shower + z_sh*cos_alpha_shower; // z2_sh
	  fSphShowerRec->GetSphericalFunction(cos_th1, f3);
	}
	
	double sum[2] = {0, 0};

	for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
	  if (f3[pmtType] > 0) sum[pmtType] += u[fNtracksMn*5+3]*f3[pmtType];

	  for (int i = 0; i < fNtracksMn; ++i)
	    if (f2[pmtType][i] > 0) sum[pmtType] += u[i*5+4]*f2[pmtType][i];

	  if (f1[pmtType] <= 0) chi2 += sum[pmtType]*sum[pmtType];
	  else chi2 += square[pmtType]*(sum[pmtType]-f1[pmtType])*(sum[pmtType]-f1[pmtType])/f1[pmtType];
	}
      }
      delete [] sin_phi;
      delete [] cos_phi;
    }
#ifdef _OPENMP
  }
#endif

  return chi2/12000.;
}


//----------------------------------------------------------------------------
void SphMuonRecTool::GetTrueMuonDirection()
{
  // get muon parameters from root file

  static TString filename = "";
  
  // check if we have the same file or new file

  SniperPtr <RootInputSvc> reSvc(getParent(), "InputSvc");
  TString fn = reSvc->getInputStream("EvtNavigator")->streamname();

  if (fn == filename) return; // we have the same file

  filename = fn;
  
  // get file name

  TString baseName = gSystem->BaseName(filename);
  int fnLength = baseName.Length()-18; // sample_calib_N.root
  TString fileNumber = baseName(13, fnLength);

  // read muon position from file

  TString position_name = Form("%s/results/muon_sim_%s.root", gSystem->DirName(filename), fileNumber.Data());

  TFile inputFile(position_name, "READ");
  TTree *tree = (TTree*)inputFile.Get("mu_tracking");

  int eventNumber, muTrackId;
  vector <float> *x = 0, *y = 0, *z = 0;
  vector <float> *px = 0, *py = 0, *pz = 0;

  tree->SetBranchAddress("evtID", &eventNumber);
  tree->SetBranchAddress("MuTrackID", &muTrackId);
  tree->SetBranchAddress("Mu_Posx", &x);
  tree->SetBranchAddress("Mu_Posy", &y);
  tree->SetBranchAddress("Mu_Posz", &z);
  tree->SetBranchAddress("Mu_Px", &px);
  tree->SetBranchAddress("Mu_Py", &py);
  tree->SetBranchAddress("Mu_Pz", &pz);

  int nentries = tree->GetEntries();
  int evt = -1, muonNumber = -1;
  TVector3 v1_mod, v2_mod, vp1_mod, vp2_mod;
  trackSim_t muon_sim;

  // map <int, int> nMuons;
  // nMuons.clear();
  
  for (int event = 0; event < nentries; ++event) {
    tree->GetEntry(event);

    EventSimRec_t &eventSimRec = fEventSimRec[eventNumber];

    if (evt != eventNumber) {
      evt = eventNumber; muonNumber = -1;
      eventSimRec.muonSim.clear();
    }

    bool inputPoint = false;

    muon_sim.intEnergy = 0;
    muon_sim.intPoint = TVector3(0, 0, 0);
      
    for (size_t i = 0; i < x->size(); ++i) {

      if (z->at(i)/1000 > fRadius+0.2) continue;

      TVector3 v1(x->at(i)/1000, y->at(i)/1000, z->at(i)/1000);
      TVector3 vp1(px->at(i)/1000, py->at(i)/1000, pz->at(i)/1000);

      if (!inputPoint) { // input point
	if (v1.Mag2() < fRadius2) {
	  ++muonNumber;
	  v1_mod = v2_mod = v1;
	  vp1_mod = vp2_mod = vp1;
	  inputPoint = true;
	}
      }
      else {
	// check interaction point

	double energy = vp2_mod.Mag() - vp1.Mag();
	
	if (energy > 5) {

	  // if (nMuons.count(eventNumber)) nMuons[eventNumber]++;
	  // else nMuons[eventNumber] = 1;
	  
	  // cout << "interaction point " << v1.x() << ", " << v1.y() << ", "
	  //      << v1.z() << "\nevent " << eventNumber << ", energy "
	  //      << energy << endl;

	  if (muon_sim.intEnergy < energy) {
	    muon_sim.intEnergy = energy;
	    muon_sim.intPoint = v1;
	  }
	}

	v2_mod = v1; vp2_mod = vp1;
	if (v1.Mag2() > fRadius2) break; // output point
      }
    }
            
    if (muonNumber == -1) continue;

    muon_sim.initMuonPosition = v1_mod;
    muon_sim.endMuonPosition = v2_mod;
    muon_sim.initMuonMomentum = vp1_mod;
    muon_sim.endMuonMomentum = vp2_mod;

    // skip dublicated events or very short muon tracks

    if ((v2_mod-v1_mod).Mag() < 0.1 || 
	(!fPhotonResponseCalib && !eventSimRec.muonSim.empty() &&
	 eventSimRec.muonSim.back().initMuonPosition == v1_mod &&
	 eventSimRec.muonSim.back().endMuonPosition == v2_mod)) continue;

    eventSimRec.muonSim.push_back(muon_sim);
  }
  
  inputFile.Close();
 
  if (fPhotonResponseCalib) return;
  
  // compound two muon tracks for generated shower

  for (auto &iEvents : fEventSimRec) {
    bool compound_muons;

    do {
      compound_muons = false;
      auto iMuon = std::begin(iEvents.second.muonSim);
      for ( ; iMuon != std::end(iEvents.second.muonSim); ++iMuon) {
	auto iMuon1 = std::begin(iEvents.second.muonSim);
	for ( ; iMuon1 != std::end(iEvents.second.muonSim); ++iMuon1) {
	  
          if (iMuon == iMuon1) continue;

          double dist = (iMuon->endMuonPosition - iMuon1->initMuonPosition).Mag();
          if (dist < 0.2) { // found muon interaction
            TVector3 &v1 = iMuon->endMuonPosition;
	    double energy = iMuon->endMuonMomentum.Mag() - iMuon1->initMuonMomentum.Mag();
	    if (energy > 5) {
	      // cout << "interaction point (compound muons) "
	      // 	   << v1.x() << ", " << v1.y() << ", " << v1.z()
	      // 	   << "\nevent " << iEvents.first << ", energy "
	      // 	   << energy << endl;
	      
	      if (iMuon->intEnergy < energy) {
		iMuon->intEnergy = energy;
		iMuon->intPoint = v1;
	      }
	    }
	    
	    // get maximum energy deposition as interaction vertex in a
	    // case of multiple interactions

	    if (iMuon1->intEnergy > iMuon->intEnergy) {
	      iMuon->intEnergy = iMuon1->intEnergy;
	      iMuon->intPoint = iMuon1->intPoint;
	    }
	    
            iMuon->endMuonPosition = iMuon1->endMuonPosition;
            iMuon->endMuonMomentum = iMuon1->endMuonMomentum;
            iEvents.second.muonSim.erase(iMuon1);
            compound_muons = true;
            break;
          }
        }
        if (compound_muons) break;
      }
    } while (compound_muons);
  }

  for (auto &iEvents : fEventSimRec) {
    int n_muon = 0;

    // set markers for muon track in and out points
    // and remove muons produced inside LS

    auto iMuon = std::begin(iEvents.second.muonSim);
    
    while (iMuon != std::end(iEvents.second.muonSim)) {

      if (iMuon->initMuonPosition.Mag2() < fRadius2 - 100) {
	iMuon = iEvents.second.muonSim.erase(iMuon);
	continue;
      }

      double theta[2], phi[2];

      theta[0] = 90-iMuon->initMuonPosition.Theta()*fRadToDeg;
      theta[1] = 90-iMuon->endMuonPosition.Theta()*fRadToDeg;
      phi[0] = iMuon->initMuonPosition.Phi()*fRadToDeg;
      phi[1] = iMuon->endMuonPosition.Phi()*fRadToDeg;
      
      iMuon->markerMuonPosition = new TPolyMarker(2, phi, theta);
      iMuon->markerMuonPosition->SetMarkerStyle(21+n_muon);
      iMuon->markerMuonPosition->SetMarkerSize(3);
      iMuon->markerMuonPosition->SetMarkerColor(1);

      ++n_muon;
      ++iMuon;
    }
  }

  // print showers number

  // for (auto &muon : nMuons) cerr << filename << "\n" << muon.first
  // 				 << " " << muon.second << endl;
 
}


//----------------------------------------------------------------------------
int SphMuonRecTool::GetInitialValues(double pos[], double phi1[],
				    double phi2[], double alpha1[],
				    std::vector<track_t> &muonCandidate)
{
  int j = 0;

  for (auto &track : muonCandidate) {

    double x1s = track.first.x();
    double y1s = track.first.y();
    double z1s = track.first.z();

    double x2s = track.second.x();
    double z2s = track.second.z();

    double z1 = 0.5*(track.first - track.second).Mag();
    double x1 = sqrt((track.first).Mag2() - z1*z1);

    pos[j] = x1;

    alpha1[j] = TMath::ACos(0.5*(z1s-z2s)/z1);

    if (alpha1[j] == 0.) {
      phi1[j] = phi2[j] = 0;
      if (x1) phi1[j] = TMath::ACos(x1s/x1);
    }
    else {
      // asin(1) returns "nan" value, so we are doing small correction

      double sin1 = 0.5*(x1s-x2s)/(z1*sin(alpha1[j]));
      double sin2 = 0.5*(z1s+z2s)/(x1*sin(alpha1[j]));

      if (sin1 >= 1) phi1[j] = 0.5*pi;
      else if (sin1 <= -1) phi1[j] = -0.5*pi;
      else phi1[j] = TMath::ASin(sin1);

      if (sin2 >= 1) phi2[j] = 0.5*pi;
      else if (sin2 <= -1) phi2[j] = -0.5*pi;
      else phi2[j] = TMath::ASin(sin2);
    }

    // now we are searching for the correct angles sign

    int n_try = 0;

    while (1) {

      if (++n_try > 5) break;

      TVector3 p1(x1, 0, z1);

      p1.RotateZ(phi2[j]);
      p1.RotateX(alpha1[j]);
      p1.RotateZ(phi1[j]);

      if ((p1-track.first).Mag() < 0.001) break; // vectors are equal

      // check sign of alpha angle
      // correction for the alpha angle

      if (TMath::Abs(p1.y() + y1s) < 0.001) {
       	alpha1[j] *= -1;
       	phi1[j] *= -1;
       	phi2[j] *= -1;
       	continue;
      }

      // check sign of phi2 angle
      // correction for the phi2 angle

      if (TMath::Abs(p1.z() - z1s) < 0.001) {
	phi2[j] = pi - phi2[j];
	continue;
      }

      // correction for the phi1 angle

      phi1[j] = pi - phi1[j];
    }

    // set angles for the direction reconstruction

    phi1[j] *= -1;
    alpha1[j] *= -1;
    phi2[j] *= -1;

    ++j;
  }


  return j;
}


//----------------------------------------------------------------------------
void SphMuonRecTool::SaveSphFuncParameters()
{
  // save PMT parameters

  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {

    ofstream output_file(fFileParNamePmt[pmtType], ios::out);
    
    for (int n = 0; n <= fMaxOrder; ++n)
      for (int k = 0; k <= n; ++k) {
	int index = gsl_sf_legendre_array_index(n, k);
	output_file << n << " " << k << " " << fCnk[pmtType][index] << " "
		    << fSnk[pmtType][index] << "\n";
      }
    output_file.close();
  }
}


//----------------------------------------------------------------------------
void SphMuonRecTool::LoadPmtDistr()
{
  for (int pmtType = fPMTtype; pmtType < 2; ++pmtType) {
    int order = -1;
    ifstream input_file(fFileParNamePmt[pmtType], ios::in);

    while ((!input_file.eof()) && (!input_file.fail())) {
      double c, s;
      int n, k;
      input_file >> n >> k >> c >> s;
      if (!input_file.fail()) {
	if (n > fMaxOrder) break;
	if (order < n) order = n;
	int index = gsl_sf_legendre_array_index(n, k);
	fCnkPmt[pmtType][index] = c;
	fSnkPmt[pmtType][index] = s;
      }
    }

    if (order < fMaxOrder) {
      fMaxOrder = order;
      fLArraySize = gsl_sf_legendre_array_n(fMaxOrder);
      cout << "Spherical function order for PMT description is less then required \nSet order to " << fMaxOrder << endl;
    }
    input_file.close();
  }
}


//----------------------------------------------------------------------------
double SphMuonRecTool::GetMinTime()
{
  double minTime = 1000000;
  TH1F histoNpe("energy deposition", "energy deposition", 250, 0, 1000);

  // define first event time

  for (auto &itPmt : fPmtTable) {
    if (!(itPmt.used && itPmt.loc == 1)) continue;
    if (fBeginEventTime < -0.001 || fBeginEventTime > itPmt.fht)
      fBeginEventTime = itPmt.fht;
  }
  
  // fill histo for defining initial time
  // only for the CD pmt's
  
  for (auto &itPmt : fPmtTable) {
    if (!(itPmt.used && itPmt.loc == 1)) continue;
    for (size_t i = 0; i < itPmt.hittime.size(); ++i) {
      histoNpe.Fill(itPmt.hittime[i]-fBeginEventTime, itPmt.hitq[i]);
    }
  }
  
  // define time for event beginning

  for (int bin = 1; bin <= histoNpe.GetNbinsX(); ++bin) {
    if (histoNpe.GetBinContent(bin) > 5000 || (fPhotonResponseCalib && histoNpe.GetBinContent(bin) > 200)) {
      minTime = histoNpe.GetBinCenter(bin) - 0.5*histoNpe.GetBinWidth(bin);
      break;
    }
  }

  if (!fProductionMode) {
    TCanvas *canvas = new TCanvas("npe", "npe", 1600, 1200);
    histoNpe.GetXaxis()->SetTitle("time, ns");
    histoNpe.GetYaxis()->SetTitle("p.e.");
    canvas->cd(); histoNpe.Draw(); canvas->Print("pictures/npe_muon.pdf");
    delete canvas;
  }

  return minTime+fBeginEventTime;
}

