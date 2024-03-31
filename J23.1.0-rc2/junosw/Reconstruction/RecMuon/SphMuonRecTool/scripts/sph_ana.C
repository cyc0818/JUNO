#include <fstream>
#include <iostream>

#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TSpline.h>
#include <TError.h>
#include <TVector3.h>
#include "gsl/gsl_sf_legendre.h"

using namespace std;

static const int max_events = 50;
static const int max_order = 50;
static const int max_files = 100;
static const int files_calib = 500;

TString pmtStr[2] = {"_3inch.dat", "_20inch.dat"};
TString path_data = "/data2/juno-data/calib_muons/";

TH1F *histo_cos_evt[max_order+1][max_order+1][max_events+1];

void FilterCosineCoefficients(int order);
void ReadEventsEnergy(TH1F *histo_energy[], int time_event, int pmtType);
void ReadMuonPositions(int i, double diff_xy[], TH1F *histo_slope, int time_event);

void sph_ana()
{
  gErrorIgnoreLevel = 5000;

  int order = -1;
  int time_event = 300;
  TH1F *histo_cos[max_order+1][max_order+1];
  TH1F *histo_sin[max_order+1][max_order+1];
  TH1F *histo_c_evt[max_events+1];
  TH1F *histo_s_evt[max_events+1];
  TH1F *histo_events[max_order+1][max_order+1];
  TH1F *histo_energy[max_events+1];
  TFile fileSpline(Form("results/sph_muon_spline_%d.root", time_event) , "RECREATE");

  for (int n = 0; n <= max_order; ++n)
    for (int k = 0; k <= max_order; ++k) {
      histo_cos[n][k] = new TH1F(Form("cos %d %d",n,k), "cos", 38, -0.25, 18.75);
      histo_sin[n][k] = new TH1F(Form("sin %d %d",n,k), "sin", 38, -0.25, 18.75);
      histo_events[n][k] = new TH1F(Form("events number %d %d", n, k),
				    "events number", 38, -0.25, 18.75);

      for (int i = 0; i <= max_events; ++i)
	histo_cos_evt[n][k][i] = new TH1F(Form("c %d %d %d", n, k, i), "cos coefficients", max_files, 0, max_files);
    }

  for (int i = 0; i <= max_events; ++i) {
    TString dist_str = Form(" %.1f m", i*0.5);
    histo_energy[i] = new TH1F("energy" + dist_str, "energy after filter" +
			       dist_str, max_files, 0, max_files);
    histo_c_evt[i] = new TH1F(Form("cos coefficient %.1f", i*0.5),
			      Form("cos coefficient for track %.1f m", i*0.5),
			      100, 0, 100);
    histo_s_evt[i] = new TH1F(Form("sin coefficient %.1f", i*0.5),
			      Form("sin coefficient for track %.1f m", i*0.5),
			      100, 0, 100);
  }

  for (int pmtType = 0; pmtType < 2; ++pmtType) {
    
    cout << "Reading event energies and make a filtration" << endl;
  
    ReadEventsEnergy(histo_energy, time_event, pmtType);

    // continue;
    
    TH1F *histo_slope = new TH1F("muons slope (mm)", "muons slope (mm)", 20, 0, 10);
  
    int nfiles = 0;

    cout << "Reading files with data and fill histograms" << endl;
  
    for (int i = 5; i <= files_calib; i += 5) {
    
      ++nfiles;
      double diff_xy[max_events];
    
      // read muon position from file

      ReadMuonPositions(i, diff_xy, histo_slope, time_event);
    
      TString file_name = Form("%smuons_%d/results/spherical_parameters_%d%s", path_data.Data(), i, time_event, pmtStr[pmtType].Data());
    
      ifstream input_file(file_name.Data(), ios::in);

      while ((!input_file.eof()) && (!input_file.fail())) {
	double dist, c, s, energy;
	int evt, n, k;
	input_file >> evt >> dist >> n >> k >> c >> s >> energy;

	if (!input_file.fail()) {
	  
	  if (evt > max_events) break;
	  if (n > max_order) continue;
	  histo_cos_evt[n][k][evt]->SetBinContent(nfiles, -10000);
	  if (diff_xy[evt] > 30) continue; // 30 mm
	  if (histo_energy[evt]->GetBinContent(nfiles) < 1) continue;
	
	  if (c > -9000) {
	    histo_cos[n][k]->Fill(dist, c);
	    histo_sin[n][k]->Fill(dist, s);
	    histo_cos_evt[n][k][evt]->SetBinContent(nfiles, c);
	    histo_events[n][k]->Fill(dist);
	  }
	  
	  if (order < n) order = n;
	}
      }

      input_file.close();
    }
    
    // cout << "Filter cosine coefficients" << endl;

    // FilterCosineCoefficients(order);

    // return;
  
  // for (int n = 0; n <= order; ++n)
  //   for (int k = 0; k <= n; ++k)
  //     for (int i = 0; i < max_events; ++i) {
  // 	int n_files = 0;
  // 	double sum = 0;
  // 	for (int bin = 1; bin <= histo_cos_evt[n][k][i]->GetNbinsX(); ++bin) {
  // 	  double binCont = histo_cos_evt[n][k][i]->GetBinContent(bin);
  // 	  if (binCont > -9000) {
  // 	    ++n_files;
  // 	    sum += binCont;
  // 	  }
  // 	}
  // 	if (n_files && i < histo_cos[n][k]->GetNbinsX())
  // 	  histo_cos[n][k]->SetBinContent(i+1, 1.*sum/n_files);
  //     }


    // TCanvas *canvas5 = new TCanvas("canvas 5", "canvas 5", 1600, 1200);
    // canvas5->cd(); canvas5->Divide(2, 1);
    // canvas5->cd(1); histo_cos[1][0]->Draw();
    // canvas5->cd(2); histo_events[1][0]->Draw();
    // canvas5->Print("pictures/histo_cosine.pdf");

    // return;
    
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
  
    TCanvas *canvas2 = new TCanvas("canvas 2", "canvas 2", 1600, 1200);
    // canvas2->cd(); histo_slope->Draw(); canvas2->Print("results/muons_slope.pdf");
    // delete canvas2;

    TCanvas *canvas = new TCanvas("canvas", "canvas", 1500, 500);
    canvas->cd();
    canvas->Divide(3, 1);
    canvas->Print("results/cs_distr.pdf[");

    // canvas->cd(1); histo_slope->Draw(); canvas->Print("results/cs_distr.pdf");

    TCanvas *canvas1 = new TCanvas("canvas1", "canvas1", 800, 600);
    // canvas1->cd(); histo_slope->Draw();canvas1->Print("pictures/tracks_angle.pdf");

    int bin = 0;

    fileSpline.cd();
    
    for (int n = 0; n <= order; ++n)
      for (int k = 0; k <= n; ++k) {
	histo_cos[n][k]->Smooth(); histo_sin[n][k]->Smooth();

	++bin;
      
	for (int i = 1; i <= histo_cos[n][k]->GetNbinsX(); ++i)
	  if (bin <= histo_c_evt[i]->GetNbinsX()) {
	    histo_c_evt[i-1]->SetBinContent(bin,histo_cos[n][k]->GetBinContent(i));
	    histo_s_evt[i-1]->SetBinContent(bin,histo_sin[n][k]->GetBinContent(i));
	  }

	int index = gsl_sf_legendre_array_index(n, k);
	TSpline5 spl1(histo_cos[n][k]);
	TSpline5 spl2(histo_sin[n][k]);

	spl1.SetName(Form("spline c %d %d", pmtType, index));
	spl2.SetName(Form("spline s %d %d", pmtType, index));

	spl1.Write(); spl2.Write();
	
	// canvas->cd(1); histo_cos[n][k]->Draw(); spl1.Draw("same");
	// canvas->cd(2); histo_sin[n][k]->Draw(); spl2.Draw("same");
	// canvas->cd(3); histo_events[n][k]->Draw();
	// canvas->Print("results/cs_distr.pdf");
	// canvas2->Print(Form("results/cs_distr_%d%d.pdf", n, k));
      }

    canvas->Print("results/cs_distr.pdf]");

    // canvas2->cd(); canvas2->Clear(); canvas2->Divide(1, 2);
    // canvas2->Print("results/cos_coefficient.pdf[");
  
    // for (int i = 1; i <= histo_cos[0][0]->GetNbinsX(); ++i) {
    //   canvas2->cd(1); histo_c_evt[i-1]->Draw();
    //   canvas2->cd(2); histo_s_evt[i-1]->Draw();
    //   canvas2->Print("results/cos_coefficient.pdf");
    // }

    // canvas2->Print("results/cos_coefficient.pdf]");

    // reset histograms

    for (int n = 0; n <= max_order; ++n)
      for (int k = 0; k <= max_order; ++k) {
	histo_cos[n][k]->Reset();
	histo_sin[n][k]->Reset();
	histo_events[n][k]->Reset();
	for (int i = 0; i <= max_events; ++i) histo_cos_evt[n][k][i]->Reset();
      }

    for (int i = 0; i <= max_events; ++i) {
      histo_c_evt[i]->Reset();
      histo_s_evt[i]->Reset();
      histo_energy[i]->Reset();
    }

    delete histo_slope;
  }

  fileSpline.Close();
  
  // for (int n = 0; n <= max_order; ++n)
  //   for (int k = 0; k <= max_order; ++k) {
  //     delete histo_cos[n][k];
  //     delete histo_sin[n][k];
  //     delete histo_events[n][k];
  //     for (int i = 0; i <= max_events; ++i) delete histo_cos_evt[n][k][i];
  //   }

  // for (int i = 0; i <= max_events; ++i) {
  //   delete histo_c_evt[i];
  //   delete histo_s_evt[i];
  // }

  // delete histo_slope;
  // delete canvas;
  // delete canvas1;
  // delete canvas2;

  exit(0);
}


//----------------------------------------------------------------------------
void FilterCosineCoefficients(int order)
{
  TCanvas *canvas3 = new TCanvas("canvas 3", "canvas 3", 1600, 600);
  canvas3->Print("pictures/cos_coefficents.pdf[");
  canvas3->Divide(2, 1);

  TH1F histo1("cos coefficients", "cos coefficients", max_files, 0, max_files);
  
  // keep only histograms with cosine coefficients in 15% around mean value
  
  for (int n = 0; n <= order; ++n) {
    for (int k = 0; k <= n; ++k)
      for (int i = 0; i <= max_events; ++i) {

	TH1F *histo = histo_cos_evt[n][k][i];

	histo->Copy(histo1);
	
	int n1 = 0;
	bool binContent_removed;

	do {

	  binContent_removed = false;
	  int binDev_max = -1;
	  double dev_max = 0, diff = 0;
	  
	  for (int bin_exc = 1; bin_exc <= histo->GetNbinsX(); ++bin_exc) {
	    double binContExc = histo->GetBinContent(bin_exc);

	    if (binContExc < -9000) continue;
	
	    n1 = 0;
	    double mean = 0;
    
	    for (int bin = 1; bin <= histo->GetNbinsX(); ++bin) {
	      if (bin == bin_exc) continue;
	      double binCont = histo->GetBinContent(bin);
	      if (binCont > -9000) {mean += binCont; ++n1;}
	    }

	    mean /= 1.*n1;
	    double dev = TMath::Abs((mean-binContExc)/mean);
	
	    if (dev_max < dev) {
	      dev_max = dev;
	      binDev_max = bin_exc;
	      diff = TMath::Abs(mean - binContExc);
	    }
	  }

	  if (binDev_max != -1 && dev_max > 0.20 && diff > 20) {
	    histo->SetBinContent(binDev_max, -10000);
	    binContent_removed = true;
	  }
	} while (binContent_removed && n1 > 2);
      
	// set 0 for undefined bins
  
	for (int bin = 1; bin <= histo->GetNbinsX(); ++bin) {
	  if (histo->GetBinContent(bin) < -9000)
	    histo->SetBinContent(bin, 0);
	  if (histo1.GetBinContent(bin) < -9000)
	    histo1.SetBinContent(bin, 0);
	}

	// draw cos coefficients for different events

	if (n <= 4) {
	  canvas3->cd(1); histo1.Draw();
	  canvas3->cd(2); histo->Draw();
	  canvas3->Print("pictures/cos_coefficents.pdf");
	}
      }
  }
  
  canvas3->Print("pictures/cos_coefficents.pdf]");
  delete canvas3;
}


//----------------------------------------------------------------------------
void ReadEventsEnergy(TH1F *histo_energy[], int time_event, int pmtType)
{
  int events = -1;
  int nfiles = 0;
  
  TH1F *histo_energy_rel[max_events+1];
  
  for (int i = 0; i <= max_events; ++i) {
    TString dist_str = Form(" %.1f m", i*0.5);
    histo_energy_rel[i] = new TH1F("energy released" + dist_str,
				   "energy released after filter" +
				   dist_str, max_files, 0, max_files);
  }
  
  for (int i = 5; i <= files_calib; i += 5) {
    int evt1 = 0;
    TString file_name = Form("%smuons_%d/results/muon_position_%d.dat",
			     path_data.Data(), i, time_event);
    ifstream input_file(file_name.Data(), ios::in);
    ++nfiles;
    if (input_file.fail() || input_file.eof()) cout << "There is no data for muons_" << i << endl;
    
    while ((!input_file.eof()) && (!input_file.fail())) {
      double tmp, energy, sumPE[2], dist;
      int evt;
      
      input_file >> evt >> dist >> energy >> sumPE[0] >> sumPE[1] >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp;

      if (!input_file.fail()) {
  	if (evt > max_events) break;
	if (energy > 50) continue;
	evt1 = evt;
  	histo_energy[evt]->SetBinContent(nfiles, sumPE[pmtType]);
  	histo_energy_rel[evt]->SetBinContent(nfiles, energy);
  	if (events < evt) events = evt;
      }
    }
    if (evt1 < 20) cout << "Low events number: file " << i << endl;
    input_file.close();
  }
  
  // keep only histograms with energy deposition in 15% around mean value

  for (int evt = 0; evt <= events; ++evt) {
    int n = 0;
    bool binContent_removed;

    do {

      binContent_removed = false;
      int binDev_max = -1;
      double dev_max = 0;
      
      for (int bin_exc = 1; bin_exc<=histo_energy[evt]->GetNbinsX(); ++bin_exc) {
	double binContExc = histo_energy[evt]->GetBinContent(bin_exc);

	if (binContExc < 1) {
	  histo_energy[evt]->SetBinContent(bin_exc, 0);
	  histo_energy_rel[evt]->SetBinContent(bin_exc, 0);
	  continue;
	}
      
	n = 0;
	double mean = 0;
    
	for (int bin = 1; bin <= histo_energy[evt]->GetNbinsX(); ++bin) {
	  if (bin == bin_exc) continue;
	  double binCont = histo_energy[evt]->GetBinContent(bin);
	  if (binCont > 1) {mean += binCont; ++n;}
	}

	mean /= 1.*n;
	double dev = TMath::Abs((mean-binContExc)/mean);
	
	if (dev_max < dev) {
	  dev_max = dev;
	  binDev_max = bin_exc;
	}
      }

      if (binDev_max != -1 && dev_max > 0.15) {
	histo_energy[evt]->SetBinContent(binDev_max, 0);
	histo_energy_rel[evt]->SetBinContent(binDev_max, 0);
	binContent_removed = true;
      }
    } while (binContent_removed && n > 2);
  }

  // draw histogram with energy deposition depending on distance

  TH1F *h1 = new TH1F("energy deposition", "energy deposition",30,-0.25,18.75);
  TH1F *h2 = new TH1F("energy release", "energy release",30,-0.25,18.75);
  
  for (int evt = 0; evt <= events; ++evt) {
    int nbins = 0;
    double energy = 0, sumPE = 0;;
    for (int bin = 1; bin <= histo_energy[evt]->GetNbinsX(); ++bin) {

      double pe = histo_energy[evt]->GetBinContent(bin);
      double en_rel = histo_energy_rel[evt]->GetBinContent(bin);
      if (pe < 1) continue;
      
      ++nbins;
      sumPE += pe;
      energy += en_rel;
    }
    h1->SetBinContent(evt+1, sumPE/nbins);
    h2->SetBinContent(evt+1, energy/nbins);
    // h1->SetBinContent(evt+1, energy/sumPE);
  }
  
  TCanvas *canvas1 = new TCanvas("canvas1", "canvas1", 1600, 800);
  canvas1->cd(); canvas1->Divide(2,1);
  canvas1->cd(1); h1->Draw(); h1->Fit("pol2");
  canvas1->cd(2); h2->Draw(); h2->Fit("pol2");
  canvas1->Print(Form("pictures/distance_energy_deposition_%d.pdf", pmtType));
}


//----------------------------------------------------------------------------
void ReadMuonPositions(int i, double diff_xy[], TH1F *histo_slope, int time_event)
{
  ifstream position_file(Form("%smuons_%d/results/muon_position_%d.dat",
			      path_data.Data(), i, time_event), ios::in);

  while ((!position_file.eof()) && (!position_file.fail())) {
    int eventNumber;
    double x, y, z, x1, y1, z1, tmp;
    TVector3 firstPosition(0,0,0), stopPosition(0,0,0);
    
    position_file >> eventNumber >> tmp >> tmp >> tmp >> tmp >> x >> y >> z >> x1 >> y1 >> z1;
      
    if (eventNumber > max_events) break;
      
    if (!position_file.fail()) {
      firstPosition.SetXYZ(x, y, z);
      stopPosition.SetXYZ(x1, y1, z1);
      diff_xy[eventNumber]=34000.*(firstPosition-stopPosition).Perp()/(z-z1);
      histo_slope->Fill(diff_xy[eventNumber]);
    }
  }

  position_file.close();
}
