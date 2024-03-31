#include <iostream>
#include <fstream>
#include <TH2F.h>
#include <TTree.h>
#include <TChain.h>
#include <math.h>
#include "TFile.h"
#include "TH2.h"
#include "TF1.h"
#include "TH1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TPad.h"

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

void resolution(TString inputpath="lists_resolution.txt",TString outputpath="rec_ana/resolution.root")
{
//  gStyle->SetOptFit(1);
  static const int lines=7;
  float vsigma[lines];
  float emean[lines];
  float esigma[lines];
  int nlines = 0;
  TFile* f[lines];
  TH1F* h_X[lines];
  TH1F* h_E[lines];
  TF1* fitE[lines];
  TF1* fitX[lines];


    std::ifstream fp(inputpath);
    if (!fp.is_open()) {
        std::cerr << "ERROR: input lists does not exists." << std::endl;
        return;
    }
    std::string tmp_line;
    while(fp.good()) {
        std::getline(fp, tmp_line);
        if ( tmp_line.size() ==0 ) {
            continue;
        }
        f[nlines]     = new TFile(tmp_line.c_str());
        h_E[nlines]   = (TH1F*)f[nlines]->Get("h_E");
        h_X[nlines]   = (TH1F*)f[nlines]->Get("h_X");
        fitE[nlines]  = (TF1*)h_E[nlines]->GetFunction("gaus");
        emean[nlines] = fitE[nlines] ->GetParameter(1);
        esigma[nlines]= fitE[nlines]->GetParameter(2)/fitE[nlines]->GetParameter(1);
        fitX[nlines]  = (TF1*)h_X[nlines]->GetFunction("gaus");
      //  vsigma[nlines] = fitX[nlines]->GetParameter(2);
        //std::cout<<tmp_line.c_str()<<",mean:"<<emean[nlines]<<",esigma:"<<esigma[nlines]<<",vsigma:"<<vsigma[nlines]<<std::endl;
      //  h_E[nlines]->Draw();
        nlines++;
        std::cout<<"nlines:"<<nlines<<std::endl;
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
  /*mc_e->SaveAs("energy_resolution.eps");
  mc_e->SaveAs("energy_resolution.png");
  mc_v->SaveAs("vertex_resolution.eps");
  mc_v->SaveAs("vertex_resolution.png");*/


  TFile *f2= TFile::Open(outputpath,"recreate");
    if (!f2) {
        std::cerr << "Can't open file " << outputpath << std::endl;
        return;
    }
  gr_e->Write();
  //gr_v->Write();


}




