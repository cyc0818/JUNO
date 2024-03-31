#include <fstream>
#include <iostream>

#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TF1.h>
#include <TSpline.h>
#include <TError.h>
#include <TVector3.h>
#include "gsl/gsl_sf_legendre.h"

using namespace std;

static const int max_events = 100;
static const int max_order = 50;
static const int max_files = 100;

static const int files_calib = 500;
static const int n_files = files_calib/5;
static const double step_dist = 0.25;
static const int n_dist = 18./step_dist+1;

TString pmtStr[2] = {"_3inch", "_20inch"};
TString path_data = "/data2/juno-data/calib_photons/";

TH2F *histo_cos_evt[max_order+1][max_order+1];
TH2F *histo_dist_rel;
TH3F *histo_energy_dist;


void ReadEventsEnergy(TH2F *histo_energy, int time_event, int pmtType);

void photons()
{
  gErrorIgnoreLevel = 5000;

  int order = -1;
  int time_event = 300;
  TH1F *histo_cos[max_order+1][max_order+1];
  TH1F *histo_sin[max_order+1][max_order+1];
  TH1F *histo_c_evt[max_events+1];
  TH1F *histo_s_evt[max_events+1];
  TH1F *histo_events[max_order+1][max_order+1];
  TH2F *histo_energy;
  TFile fileSpline(Form("results/photons_spline_%d.root", time_event) , "RECREATE");

  for (int n = 0; n <= max_order; ++n)
    for (int k = 0; k <= max_order; ++k) {
      histo_cos[n][k] = new TH1F(Form("cos %d %d",n,k), "cos", n_dist,
				 -step_dist/2, 18+step_dist/2);
      histo_sin[n][k] = new TH1F(Form("sin %d %d",n,k), "sin", n_dist,
				 -step_dist/2, 18+step_dist/2);
      histo_events[n][k] = new TH1F(Form("events number %d %d", n, k),
   				    "events number", n_dist, -step_dist/2,
				    18+step_dist/2);

      histo_cos_evt[n][k] = new TH2F(Form("c %d %d", n, k), "cos coefficients",
				     n_dist, -step_dist/2, 18+step_dist/2,
				     n_files, 5, files_calib+5);
    }

  histo_energy = new TH2F("energy photons", "energy after filter", n_dist,
			  -step_dist/2, 18+step_dist/2, n_files, 5,
			  files_calib+5);
  histo_dist_rel = new TH2F("energy coefficient", "energy coefficient", n_dist,
			    -step_dist/2, 18+step_dist/2,
			    50, 0, 20);
  histo_energy_dist = new TH3F("detector responce", "detector responce", n_dist,
			       -step_dist/2, 18+step_dist/2,
			       50, 300000, 1500000, 50, 0, 1);
  
  for (int pmtType = 0; pmtType < 2; ++pmtType) {
    
    cout << "Reading event energies and make a filtration" << endl;
  
    ReadEventsEnergy(histo_energy, time_event, pmtType);

    // continue;
    
    int nfiles = 0;

    cout << "Reading files with data and fill histograms" << endl;
  
    for (int i = 5; i <= files_calib; i += 5) {
    
      TString file_name = Form("%smuons_%d/results/photon_calib_parameters_%d%s.dat", path_data.Data(), i, time_event, pmtStr[pmtType].Data());
    
      ifstream input_file(file_name.Data(), ios::in);

      while ((!input_file.eof()) && (!input_file.fail())) {
    	int n, k;
    	double dist, c, s, tmp;
	input_file >> dist >> n >> k >> c >> s >> tmp >> tmp;

    	if (!input_file.fail()) {
    	  if (n > max_order) continue;

	  int binx = histo_cos_evt[n][k]->GetXaxis()->FindBin(dist);
	  int biny = histo_cos_evt[n][k]->GetYaxis()->FindBin(i);
    	  histo_cos_evt[n][k]->SetBinContent(binx, nfiles, -10000);

    	  if (histo_energy->GetBinContent(binx, biny) < 1) continue;
	
    	  if (c > -9000) {
    	    histo_cos[n][k]->Fill(dist, c);
    	    histo_sin[n][k]->Fill(dist, s);
    	    histo_cos_evt[n][k]->SetBinContent(binx, i, c);
    	    histo_events[n][k]->Fill(dist);
    	  }
	  
    	  if (order < n) order = n;
    	}
      }

      input_file.close();
    }

    // scale histograms

    for (int n = 0; n <= order; ++n)
      for (int k = 0; k <= n; ++k) {
   	for (int bin = 1; bin <= histo_events[n][k]->GetNbinsX(); ++bin)
   	  if (!histo_events[n][k]->GetBinContent(bin))
   	    histo_events[n][k]->SetBinContent(bin, 1); // set 1 to scale for zero bin
   	histo_cos[n][k]->Divide(histo_events[n][k]);
   	histo_sin[n][k]->Divide(histo_events[n][k]);
      }

    cout << "Draw histograms" << endl;

    TCanvas *canvas = new TCanvas("canvas", "canvas", 1500, 500);
    canvas->cd(); canvas->Divide(2, 1);
    TString csFileName = Form("pictures/photons_cs_%d.pdf", pmtType);
    canvas->Print(csFileName+"[");

    for (int n = 0; n <= order; ++n)
      for (int k = 0; k <= n; ++k) {
	
	int index = gsl_sf_legendre_array_index(n, k);
	histo_cos[n][k]->Smooth(); histo_sin[n][k]->Smooth();

	TSpline5 spl1(histo_cos[n][k]);
	TSpline5 spl2(histo_sin[n][k]);

	spl1.SetName(Form("spline photons c %d %d", pmtType, index));
	spl2.SetName(Form("spline photons s %d %d", pmtType, index));

	spl1.Write(); spl2.Write();
	
	canvas->cd(1); histo_cos[n][k]->Draw(); spl1.Draw("same");
	canvas->cd(2); histo_sin[n][k]->Draw(); spl2.Draw("same");
	canvas->Print(csFileName);
      }

    canvas->Print(csFileName+"]");

    for (int n = 0; n <= order; ++n)
      for (int k = 0; k <= n; ++k) {
   	histo_cos[n][k]->Reset(); histo_sin[n][k]->Reset();
      }

    delete canvas;
  }

  // draw energy dependence

  TCanvas *canvas = new TCanvas("canvas", "canvas", 1500, 500);

  // fill 2d histogram

  TH2F histo2d_energy("energy deposition", "energy deposition",
		      50, 300000, 1500000, 50, 0, 1);

  for (int binx = 1; binx <= histo_energy_dist->GetNbinsX(); ++binx)
  {
    // int binx = 1;
    for (int biny = 1; biny <= histo_energy_dist->GetNbinsY(); ++biny) {
      for (int binz = 1; binz <= histo_energy_dist->GetNbinsZ(); ++binz) {
	double en = histo_energy_dist->GetBinContent(binx, biny, binz);
	if (en) histo2d_energy.SetBinContent(biny, binz, en);
      }
    }
  }

  TProfile *profile_energy = histo2d_energy.ProfileX("profile energy", 1, -1, "e");
  profile_energy->SetMarkerStyle(20);
  profile_energy->SetMarkerSize(2.5);
  
  canvas->cd();
  histo2d_energy.Draw("COLZ");
  profile_energy->Draw("same"); profile_energy->Fit("pol1");
  canvas->Print("pictures/energy_dependence.pdf");

  canvas->cd();
  histo_dist_rel->Draw("COLZ");
  canvas->Print("pictures/energy_coefficient.pdf");
  
  fileSpline.Close();
  exit(0);
}


//----------------------------------------------------------------------------
void ReadEventsEnergy(TH2F *histo_energy, int time_event, int pmtType)
{
  TH2F *histo_cos = new TH2F("sph cos", "sph cos", n_dist,
			     -step_dist/2, 18+step_dist/2, n_files, 5,
			     files_calib+5);
  TH2F *histo_energy_shower = new TH2F("energy shower", "energy shower", n_dist,
				       -step_dist/2, 18+step_dist/2, n_files, 5,
				       files_calib+5);

  
  for (int i = 5; i <= files_calib; i += 5) {
    TString file_name = Form("%smuons_%d/results/photon_calib_parameters_%d%s.dat",
     			     path_data.Data(), i, time_event,
     			     pmtStr[pmtType].Data());
    ifstream input_file(file_name.Data(), ios::in);
    
    while ((!input_file.eof()) && (!input_file.fail())) {
      double tmp, energy, dist, c, sumPE;
      int n, k;
      
      input_file >> dist >> n >> k >> c >> tmp >> sumPE >> energy;

      if (!input_file.fail()) {
	if (!n && !k) {
	  histo_energy->Fill(dist, i, sumPE);
	  histo_energy_shower->Fill(dist, i, energy);
	  histo_cos->Fill(dist, i, c);
	  if (pmtType==1) {
	    histo_energy_dist->Fill(dist, sumPE, energy);
	    histo_dist_rel->Fill(dist, energy/sumPE*10000000);
	    // cout << energy/sumPE << endl;
	  }
	}
      }
    }
    input_file.close();
  }

  // keep only histograms with energy deposition in 15% around mean value

  int n;
  bool binContent_removed;

  for (int binx = 1; binx <= histo_energy->GetNbinsX(); ++binx) {

    do {

      binContent_removed = false;
      int binDevEng_max = -1, binDevCos_max = -1;
      double devEng_max = 0, devCos_max = 0;
      
      for (int biny_exc = 1; biny_exc<=histo_energy->GetNbinsY(); ++biny_exc) {
	  
	double binContEngExc = histo_energy->GetBinContent(binx, biny_exc);
	double binContCosExc = histo_cos->GetBinContent(binx, biny_exc);

	if (binContEngExc < 1) {
	  histo_energy->SetBinContent(binx, biny_exc, 0);
	  histo_energy_shower->SetBinContent(binx, biny_exc, 0);
	  continue;
	}

  	n = 0;
	double mean_eng = 0, mean_cos = 0;

	for (int biny = 1; biny <= histo_energy->GetNbinsY(); ++biny) {
	  if (biny == biny_exc) continue;
	  double binContEng = histo_energy->GetBinContent(binx, biny);
	  double binContCos = histo_cos->GetBinContent(binx, biny);
	  if (binContEng > 1) {
	    mean_eng += binContEng; mean_cos += binContCos; ++n;
	  }
	}
	
	mean_eng /= 1.*n; mean_cos /= 1.*n;

	// cout << mean_eng << endl;
	
	double dev_eng = abs((mean_eng-binContEngExc)/mean_eng);
	double dev_cos = abs((mean_cos-binContCosExc)/mean_cos);

	if (devEng_max < dev_eng) {
	  devEng_max = dev_eng;
	  binDevEng_max = biny_exc;
	}
	if (devCos_max < dev_cos) {
	  devCos_max = dev_cos;
	  binDevCos_max = biny_exc;
	}
      }
      
      if (devEng_max > 0.20) {
     	histo_energy->SetBinContent(binx, binDevEng_max, 0);
     	histo_energy_shower->SetBinContent(binx, binDevEng_max, 0);
	binContent_removed = true;
      }

      if (devCos_max > 1.0) {
     	histo_energy->SetBinContent(binx, binDevCos_max, 0);
     	histo_energy_shower->SetBinContent(binx, binDevCos_max, 0);
	binContent_removed = true;
      }
      
    } while (binContent_removed && n > 2);
  }

  // calculate mean realised energy

  int nEvt = 0;
  double meanEnergy = 0;
  
  for (int binx = 1; binx <= histo_energy_shower->GetNbinsX(); ++binx) {
    for (int biny = 1; biny <= histo_energy_shower->GetNbinsY(); ++biny) {
      double energy = histo_energy_shower->GetBinContent(binx, biny);
      if (energy > 0) {meanEnergy += energy; ++nEvt;}
    }
  }

  if (nEvt)
    cout << "mean released energy " << meanEnergy/nEvt << endl;
  
  // draw histogram with energy deposition depending on distance

  // TCanvas *canvas1 = new TCanvas("canvas1", "canvas1", 1600, 1200);
  // canvas1->cd(); histo_energy->Draw("COLZ");
  // canvas1->Print("pictures/photons_distance_energy_deposition"+pmtStr[pmtType]+".pdf");
}
