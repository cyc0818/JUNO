#include <iostream>
#include <fstream>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TMultiGraph.h>
#include <TCanvas.h>

void PMT_position();

void muon_deviation()
{
  gErrorIgnoreLevel = 5000;

  // PMT_position();

  // reading information about muon coordinates
  
  ifstream input_file("results/track_position.dat", ios::in);
  TGraph *graphXZ[10], *graphXY[10], *graphDet;
  int point = 0;

  TMultiGraph *mgraphXZ = new TMultiGraph();
  TMultiGraph *mgraphXY = new TMultiGraph();

  for (int evt = 0; evt < 10; ++evt) {
    graphXZ[evt] = new TGraph();
    graphXZ[evt]->SetMarkerStyle(20);
    // graphXZ[evt]->SetMarkerColor(evt);
    graphXZ[evt]->SetMarkerSize(0.1);
    
    graphXY[evt] = new TGraph();
    graphXY[evt]->SetMarkerStyle(20);
    // graphXY[evt]->SetMarkerColor(evt);
    graphXY[evt]->SetMarkerSize(0.1);
  }

  graphDet = new TGraph();
  graphDet->SetMarkerStyle(21);
  graphDet->SetMarkerColor(1);
  graphDet->SetMarkerSize(0.3);

  // Set points for detector

  double r = 17000*17000;
  
  for (int i = 0; i < 1000; ++i) {
    double x = -17000 + 17000*2*i/1000.;
    double y = TMath::Sqrt(r-x*x);
    // cout << x << " " << y << endl;
    graphDet->SetPoint(2*i, x, y);
    graphDet->SetPoint(2*i+1, x, -y);
  }

  while ((!input_file.eof()) && (!input_file.fail())) {
    int event;
    double x, y, z, energy;
    input_file >> event >> x >> y >> z >> energy;
    if (!input_file.fail()) {
      if (z < -17000) continue;
      if (point > 5000000) break;
      int enr = int(energy/50) - 1;
      if (enr > 9) enr = 9;
      if (enr < 0) continue;
      graphXZ[enr]->SetPoint(graphXZ[enr]->GetN(), x/1000, z/1000);
      graphXY[enr]->SetPoint(graphXY[enr]->GetN(), x/1000, y/1000);
    }
    ++point;
  }
  input_file.close();

  TCanvas *canvas = new TCanvas("muon deviation", "muon deviation", 1600, 800);
  canvas->cd(); canvas->Divide(2, 1);

  canvas->Print("pictures/muon_position.pdf[");
  
  for (int i = 0; i < 9; ++i) {
    TString en = Form("energy %d-%d GeV", (i+1)*50, (i+2)*50);
    TString en1 = Form("energy_%d_%d", (i+1)*50, (i+2)*50);
    graphXZ[i]->SetTitle("muons XZ projection (m) " + en);
    graphXY[i]->SetTitle("muons XY projection (m) " + en);
    graphXZ[i]->GetXaxis()->SetLimits(-0.2, 0.2);
    graphXY[i]->GetXaxis()->SetLimits(-0.2, 0.2);
    graphXY[i]->GetYaxis()->SetRangeUser(-0.2, 0.2);
    canvas->cd(1); graphXZ[i]->Draw("AP");
    canvas->cd(2); graphXY[i]->Draw("AP");
    canvas->Print("pictures/muon_position.pdf");
    canvas->Print("pictures/muon_position_"+en1+".png");
  }

  canvas->Print("pictures/muon_position.pdf]");

  exit(0);
}


void PMT_position()
{
  TGraph2D graph;
  ifstream input_file("results/pmt_position.dat");
  
  while ((!input_file.eof()) && (!input_file.fail())) {
    double x, y, z;
    input_file >> x >> y >> z;
    if (!input_file.fail()) graph.SetPoint(graph.GetN(), x, y, z);
  }
  input_file.close();

  TCanvas *canvas2 = new TCanvas("pmt position", "pmt position", 1600, 1200);
  canvas2->cd(); graph.Draw("AP");
  canvas2->Print("pictures/pmt_position.pdf");
}
