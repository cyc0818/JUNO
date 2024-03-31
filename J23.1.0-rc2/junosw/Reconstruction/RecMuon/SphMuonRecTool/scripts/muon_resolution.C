#include <iostream>
#include <fstream>
#include <TCanvas.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TMath.h>

void ReadEnergyDeposition(TString &fn, TH2F *histo);
void ReadEnergyDeposition(TString &fn, TH1F *histo3inch, TH1F *histo20inch);
void ReadErrorMessage(const TString &fn);
void ScaleEnergy(const TString &fn, int eventLength, TString &file_out,
		 TH1F *histo);

void muon_resolution()
{
  int n_muons = 1; // 1, 2, 3, 4 - shower
  TString file_in[5] = {"calib_muons", "1_muon", "2_muons", "3_muons", "shower"};
  TString file_out[5] = {"calib", "single", "double", "triple", "shower"};
  
  gErrorIgnoreLevel = 5000;
  
  double radius = 17.7; // m
  
  int eventLength = 300;
  TString fn = Form("results/muon_resolution_%d.dat", eventLength);
  TString fn2 = Form("results/double_muon_resolution_%d.dat", eventLength);
  TString fn3 = Form("results/double_muon_resolution_separated_%d.dat", eventLength);
  TString fn4 = Form("results/event_energy_%d.dat", eventLength);
  
  TH2F *histoUncrtVol = new TH2F("volume of uncertainty","volume of uncertainty",
				17, 0, 17, 60, 0, 3);
  TH2F *histoEnDist2_3 = new TH2F("energy distance resolution 3 inch",
				  "energy distance resolution 3 inch",
				  60, 0, 5, 50, 0, 4000000);
  TH2F *histoEnDist2_20 = new TH2F("energy distance resolution 20 inch",
				   "energy distance resolution 20 inch",
				   60, 0, 5, 50, 2000000, 200000000);
  TH2F *histoDist2 = new TH2F("distance resolution rec", "distance resolution rec",
			      17, 0, 17, 60, -1, 1);
  TH2F *histoEnergy_3 = new TH2F("distance energy 3 inch",
  				 "distance energy 3 inch",
  				 17, 0, 17, 50, 0, 4000000);
  TH2F *histoEnergy_20 = new TH2F("distance energy 20 inch",
  				  "distance energy 20 inch",
  				  17, 0, 17, 50, 2000000, 200000000);
  TH2F *histoIntEnergy_3 = new TH2F("interaction energy 3 inch",
				    "interaction energy 3 inch",
				    20, 0, 40, 50, 0, 4000000);
  TH2F *histoIntEnergy_20 = new TH2F("interaction energy 20 inch",
				     "interaction energy 20 inch",
				     20, 0, 40, 50, 2000000, 200000000);
  TH2F *histoChi2 = new TH2F("chi2 distribution", "chi2 distribution",
			     60, 0, 5, 50, 0, 100);
  TH1F *histoEnScale = new TH1F("scale factor connected",
				"scale factor connected",
				100, 0, 10);
  TH1F *histoEnScaleNotCon = new TH1F("scale factor not connected",
				      "scale factor not connected",
				      100, 0, 10);
  TH1F *histoDistMuonSim = new TH1F("dist sim", "distance between muons",
				    34, 0, 34);
  TH1F *histoDistMuonRec = new TH1F("dist rec", "distance between muons",
				    34, 0, 34);
  TH1F *histoDistDoubleMuonRec = new TH1F("dist double rec",
					  "distance between muons",
					  34, 0, 34);
  TH1F *histoEnergyDep3inch = new TH1F("energy deposition 3 inch",
				  "energy deposition 3 inch",
				  50, 0, 2000000);
  TH1F *histoEnergyDep20inch = new TH1F("energy deposition 20 inch",
					"energy deposition 20 inch",
					50, 2000000, 30000000);
  TH2F *histoEnergyDeposition = new TH2F("energy deposition",
					 "energy deposition",
					 50, 0, 270, 70, 1000, 15000);
  TH1F *histoEnergyLossRes = new TH1F("energy loss resolution",
				      "energy loss resolution", 50, -1, 1);
  TH1F *histoEnergyShowerRes = new TH1F("energy shower resolution",
					"energy shower resolution", 50, -1, 1);

  // reading information about reconstructed muon resolution

  double evt_prev = -1;
  double sumPE = 0, energyLossSum = 0;
  
  for (int i = 5; i <= 500; ++i) {

    TString fn1 = Form("/data2/juno-data/%s/muons_%d/", file_in[n_muons].Data(), i);
    TString file_name = fn1 + fn;
    TString file_name2 = fn1 + fn2;
    TString file_name3 = fn1 + fn3;
    TString file_name4 = fn1 + fn4;

    ReadErrorMessage(fn1);

    // reading muon resolution
    
    ifstream input_file(file_name.Data(), ios::in);

    double PEshift = 15000000;
    
    if (n_muons == 2) PEshift = 25000000;
    else if (n_muons == 3) PEshift = 45000000;
    
    while ((!input_file.eof()) && (!input_file.fail())) {
      int i;
      double x, y, chi2, enScale, sumPE_3, sumPE_20, intEnergyTrue;
      double intEnergyRec, energyLossTrue, energyLossRec;
      input_file >> i >> x >> y >> chi2 >> enScale >> sumPE_3 >> sumPE_20
		 >> intEnergyTrue >> intEnergyRec >> energyLossTrue
		 >> energyLossRec;
      if (!input_file.fail()) {
	histoEnScale->Fill(enScale);
	
	if (y < 10) {

	  // calculate volume of uncertainty

	  double volume = 2.*y*y*sqrt(radius*radius-x*x);
	  double volUnc = 400.*volume/(3.*radius*radius*radius);
	  
	  histoUncrtVol->Fill(x, volUnc);

	  histoChi2->Fill(y, chi2);

	  histoEnergy_3->Fill(x, sumPE_3);
	  histoEnergy_20->Fill(x, sumPE_20);
	  histoEnDist2_3->Fill(y, sumPE_3);
	  histoEnDist2_20->Fill(y, sumPE_20);

	  if (i != evt_prev) {
	    if (energyLossSum > 0) {
	      histoEnergyLossRes->Fill((sumPE*0.72*0.000001-energyLossSum)/energyLossSum);
	    }
	    energyLossSum = 0; evt_prev = i;
	  }
	  energyLossSum += energyLossTrue;
	  sumPE = sumPE_20;
	  
	  if (intEnergyTrue > 0) {
	    histoIntEnergy_3->Fill(intEnergyTrue, sumPE_3);
	    histoIntEnergy_20->Fill(intEnergyTrue, sumPE_20);
	    if (intEnergyRec) histoEnergyShowerRes->Fill((intEnergyRec-intEnergyTrue)/intEnergyTrue);
	  }
	}
      }
    }

    if (energyLossSum > 0) histoEnergyLossRes->Fill((sumPE*0.72*0.000001-energyLossSum)/energyLossSum);
    
    input_file.close();

    // reading distance between muons
    input_file.open(file_name2.Data(), ios::in);

    while ((!input_file.eof()) && (!input_file.fail())) {
      double x;
      string type;
      input_file >> type >> x;
      if (!input_file.fail()) {
	if (type == "sim") histoDistMuonSim->Fill(x);
	else if (type == "rec") histoDistMuonRec->Fill(x);
      }
    }
    input_file.close();

    // reading distance between muons with improvemets for closest muons

    input_file.open(file_name3.Data(), ios::in);

    while ((!input_file.eof()) && (!input_file.fail())) {
      double x;
      string type;
      input_file >> type >> x;
      if (!input_file.fail()) {
	if (type == "rec") histoDistDoubleMuonRec->Fill(x);
      }
    }
    input_file.close();

    ReadEnergyDeposition(fn1, histoEnergyDeposition);
    ReadEnergyDeposition(file_name4, histoEnergyDep3inch, histoEnergyDep20inch);
    ScaleEnergy(fn1, eventLength, file_out[n_muons], histoEnScaleNotCon);
  }

  TProfile *profileDist2 = histoDist2->ProfileX("profile distance 2", 1, -1,"e");
  TProfile *profileUncrtVol = histoUncrtVol->ProfileX("profile uncertainty volume", 1, -1, "e");
  profileDist2->SetMarkerStyle(20);
  profileDist2->SetMarkerSize(2.5);
  profileUncrtVol->SetMarkerStyle(20);
  profileUncrtVol->SetMarkerSize(2.5);

  histoUncrtVol->GetXaxis()->SetTitle("rec R, m");
  histoUncrtVol->GetYaxis()->SetTitle("volume, %");
  histoChi2->GetXaxis()->SetTitle("distance, m");
  histoChi2->GetYaxis()->SetTitle("chi2");

  TCanvas *canvas = new TCanvas("reconstruction resolution",
				"reconstruction resolution", 1200, 1600);

  canvas->cd(); histoUncrtVol->Draw("COLZ"); profileUncrtVol->Draw("same");
  canvas->Print(Form("pictures/muon_resolution_%s_%d.pdf", file_out[n_muons].Data(), eventLength));
  
  canvas->cd(); canvas->Clear();
  histoChi2->Draw("COLZ");
  canvas->Print(Form("pictures/chi2_distribution_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  TProfile *profileEnergy = histoEnergyDeposition->ProfileX("profile energy deposition", 1, -1, "e");
  profileEnergy->SetMarkerStyle(20);
  profileEnergy->SetMarkerSize(2.5);
  
  canvas->Clear(); canvas->cd();
  histoEnergyDeposition->Draw("COLZ");
  profileEnergy->Draw("same"); profileEnergy->Fit("pol2");
  canvas->Print("pictures/energy_deposition_"+file_out[n_muons]+".pdf");

  delete canvas;

  // return;
  
  histoDistMuonRec->SetMarkerStyle(20);
  histoDistMuonRec->SetMarkerSize(3);
  histoDistMuonRec->SetMarkerColor(1);
  histoDistDoubleMuonRec->SetMarkerStyle(21);
  histoDistDoubleMuonRec->SetMarkerSize(3);
  histoDistDoubleMuonRec->SetMarkerColor(2);
  histoDistMuonSim->GetXaxis()->SetTitle("R, m");
  histoDistMuonSim->SetLineWidth(4);
  
  TCanvas *canvas2 = new TCanvas("distance between muons",
				 "distance between muons", 1600, 1200);
    
  canvas2->cd();
  histoDistMuonSim->Draw("hist"); histoDistMuonRec->Draw("p same");
  histoDistDoubleMuonRec->Draw("p same");

  auto legend = new TLegend(0.60,0.55,0.90,0.75);
  legend->SetHeader("distance","C"); // option "C" allows to center the header
  legend->AddEntry(histoDistMuonSim, "true distance","l");
  legend->AddEntry(histoDistMuonRec, "rec distance","p");
  legend->AddEntry(histoDistDoubleMuonRec, "rec dist separated","p");
  legend->Draw();

  canvas2->Print(Form("pictures/muon_distance_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  delete canvas2;

  TCanvas *canvas3 = new TCanvas("energy deposition", "energy deposition", 1600, 800);
  canvas3->cd();  canvas3->Divide(2, 1);
  canvas3->cd(1); histoEnergyDep3inch->Draw();
  canvas3->cd(2); histoEnergyDep20inch->Draw();
  canvas3->Print(Form("pictures/energy_deposition_pmt_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  canvas3->cd(1); histoEnScale->Draw();
  canvas3->cd(2); histoEnScaleNotCon->Draw();
  canvas3->Print(Form("pictures/energy_scale_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  TProfile *profileEnergy_3 = histoEnergy_3->ProfileX("profile energy deposition 3 inch", 1, -1, "e");
  TProfile *profileEnergy_20 = histoEnergy_20->ProfileX("profile energy deposition 20 inch", 1, -1, "e");
  profileEnergy_3->SetMarkerStyle(20); profileEnergy_3->SetMarkerSize(2.5);
  profileEnergy_20->SetMarkerStyle(20); profileEnergy_20->SetMarkerSize(2.5);

  canvas3->cd(1); histoEnergy_3->Draw("COLZ");
  profileEnergy_3->Draw("same"); profileEnergy_3->Fit("pol2");
  canvas3->cd(2); histoEnergy_20->Draw("COLZ");
  profileEnergy_20->Draw("same"); profileEnergy_20->Fit("pol2");
  canvas3->Print(Form("pictures/distance_energy_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  canvas3->cd(1); histoIntEnergy_3->Draw("COLZ");
  canvas3->cd(2); histoIntEnergy_20->Draw("COLZ");
  canvas3->Print(Form("pictures/interaction_energy_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  canvas3->cd(1); histoEnDist2_3->Draw("COLZ");
  canvas3->cd(2); histoEnDist2_20->Draw("COLZ");
  canvas3->Print(Form("pictures/energy_distance_%s_%d.pdf", file_out[n_muons].Data(), eventLength));

  // draw energy resolution

  canvas3->cd(1); histoEnergyLossRes->Draw();
  canvas3->cd(2); histoEnergyShowerRes->Draw();
  canvas3->Print(Form("pictures/energy_resolution_%s_%d.pdf", file_out[n_muons].Data(), eventLength));
    
  delete canvas3;
  
  exit(0);
}


void ReadEnergyDeposition(TString &fn1, TH2F *histo)
{
  // reading energy deposition

  TString fn = "results/energy_point.txt";
  TString file_name = fn1 + fn;
  
  ifstream input_file(file_name.Data(), ios::in);

  while ((!input_file.eof()) && (!input_file.fail())) {
    int evt;
    double x, dist2, io_point;
    input_file >> dist2 >> x >> io_point;
    // if (x < 3000) cout << fn1 << " " << evt << endl;

    // if (!input_file.fail()) histo->Fill(dist2, x);
    if (!input_file.fail() && io_point < 6) histo->Fill(dist2, x);
  }
  input_file.close();
}


void ReadEnergyDeposition(TString &file_name, TH1F *histo3inch, TH1F *histo20inch)
{
  // reading energy deposition

  ifstream input_file(file_name.Data(), ios::in);

  while ((!input_file.eof()) && (!input_file.fail())) {
    double en3inch, en20inch;
    input_file >> en3inch >> en20inch;
    if (!input_file.fail()) {
      histo3inch->Fill(en3inch); histo20inch->Fill(en20inch);
    }
  }
  input_file.close();
}


void ReadErrorMessage(const TString &fn)
{
  char line[1024];
  TString file_name = fn + "results/error_message.txt";
  
  ifstream input_file(file_name.Data(), ios::in);

  if (input_file.good()) {
    while ((!input_file.eof()) && (!input_file.fail())) {
      input_file.getline(line, sizeof(line));
      if (!input_file.fail()) {
	cout << fn << " " << line << endl;
      }
    }
    input_file.close();
  }
}


void ScaleEnergy(const TString &fn, int eventLength, TString &file_out,
		 TH1F *histo)
{
  histo->SetLineColor(2);
  
  TString file_name = Form("%sresults/muons_not_connected_%d.dat", fn.Data(), eventLength);
  ifstream input_file(file_name.Data(), ios::in);

  while ((!input_file.eof()) && (!input_file.fail())) {
    int evt;
    double scaleEn;
    input_file >> evt >> scaleEn;
    if (!input_file.fail()) histo->Fill(scaleEn);
  }
  input_file.close();
}

