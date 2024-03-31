#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Double_t fitf3(Double_t *x, Double_t *par)
{
  Double_t fitval = pow(par[0]/sqrt(x[0]),2) + pow(par[1],2) + pow(par[2]/x[0],2);
  return fitval;
}

Double_t fitf2(Double_t *x, Double_t *par)
{
  Double_t fitval = par[0]/sqrt(x[0]) + par[1];
  return fitval;
}

void resolution_center()
{
  gStyle->SetOptFit(1);
  static const int lines=7;
  float vsigma[lines];
  float emean[lines];
  float esigma[lines];

  for(int i=0;i<lines;i++)
  {
    vsigma[i]=0;
    emean[i]=0;
    esigma[i]=0;
  }
  ifstream file;
  file.open("result_center.txt",ios::in);
  if(file.fail())
  {
    cout<<"file doesn't exist"<<endl;
    file.close();
  }
  else
  {
    int i=0;
    while(!file.eof())
    {
      file>>vsigma[i]>>emean[i]>>esigma[i];
      i++;
    }
  }

  for(int i=0;i<lines;i++)
  {
    cout<<vsigma[i]<<" "<<emean[i]<<" "<<esigma[i]<<endl;
  }

  TCanvas* mc_e = new TCanvas("mc_e","mc_e",10,10,700,500);
  TGraph* gr_e= new TGraph(7,emean, esigma);
  gr_e->SetLineColor(2);
  gr_e->SetLineWidth(4);
  gr_e->SetMarkerColor(4);
  gr_e->SetMarkerStyle(21);
  gr_e->SetTitle("energy resolution");
  gr_e->GetXaxis()->SetTitle("E_{vis}(MeV)");
  gr_e->GetXaxis()->SetTitleSize(0.042);
  gr_e->GetYaxis()->SetTitle("energy resolution(%)");
  gr_e->GetYaxis()->SetTitleSize(0.042);
  gr_e->Draw("AP");

  gPad->SetGrid();
  //gPad->SetGridx();
  //gPad->SetGridy();
  //TF1 *func = new TF1("fitf3",fitf3, 1, 8, 3);
  //func->SetParameters(2.9,0.6,0.0);
  //func->SetParNames("A","B","C");
  
  TF1 *func_e = new TF1("fitf2_e",fitf2, 0.5, 8, 2);
  func_e->SetParameters(2.9,0.6);
  func_e->SetParNames("A","B");
  gr_e->Fit(func_e, "r");

  TCanvas* mc_v = new TCanvas("mc_v","mc_v",10,10,700,500);
  TGraph* gr_v= new TGraph(7,emean, vsigma);
  gr_v->SetLineColor(2);
  gr_v->SetLineWidth(4);
  gr_v->SetMarkerColor(4);
  gr_v->SetMarkerStyle(21);
  gr_v->SetTitle("vertex resolution");
  gr_v->GetXaxis()->SetTitle("E_{vis}(MeV)");
  gr_v->GetXaxis()->SetTitleSize(0.042);
  gr_v->GetYaxis()->SetTitle("vertex resolution(mm)");
  gr_v->GetYaxis()->SetTitleSize(0.042);
  gr_v->Draw("AP");

  gPad->SetGrid();
  //gPad->SetGridx();
  //gPad->SetGridy();
  //TF1 *func = new TF1("fitf3",fitf3, 1, 8, 3);
  //func->SetParameters(2.9,0.6,0.0);
  //func->SetParNames("A","B","C");

  TF1 *func_v = new TF1("fitf2_v",fitf2, 0.5, 8, 2);
  func_v->SetParameters(50,5);
  func_v->SetParNames("A","B");
  gr_v->Fit(func_v, "r");
  mc_e->Update();
  mc_v->Update();
  mc_e->SaveAs("energy_resolution.eps");
  mc_e->SaveAs("energy_resolution.png");
  mc_v->SaveAs("vertex_resolution.eps");
  mc_v->SaveAs("vertex_resolution.png");

  TFile* output = new TFile("resolution.root", "recreate");
  output->cd();
  gr_v->Write("vertex_resolution");
  gr_e->Write("energy_resolution");
  output->Close();  
}




