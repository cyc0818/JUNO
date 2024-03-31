#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Double_t fitf3(Double_t *x, Double_t *par)
{
  Double_t fitval = sqrt(pow(par[0]/sqrt(x[0]),2) + pow(par[1],2) + pow(par[2]/x[0],2));
  return fitval;
}

Double_t fitf2(Double_t *x, Double_t *par)
{
  Double_t fitval = par[0]/sqrt(x[0]) + par[1];
  return fitval;
}

void resolution()
{
  gStyle->SetOptFit(1);
  static const int lines=5;
  float vsigma_wo[lines];
  float emean_wo[lines];
  float esigma_wo[lines];
  float vsigma[lines];
  float emean[lines];
  float esigma[lines];
  for(int i=0;i<lines;i++)
  {
    vsigma_wo[i]=0;
    emean_wo[i]=0;
    esigma_wo[i]=0;
    vsigma[i]=0;
    emean[i]=0;
    esigma[i]=0; 
 }
  ifstream file;
  file.open("result.txt",ios::in);
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
      file>>vsigma_wo[i]>>emean_wo[i]>>esigma_wo[i]>>vsigma[i]>>emean[i]>>esigma[i];
      i++;
    }
  }

  for(int i=0;i<lines;i++)
  {
    cout<<vsigma_wo[i]<<" "<<emean_wo[i]<<" "<<esigma_wo[i]<<" "<<vsigma[i]<<" "<<emean[i]<<" "<<esigma[i]<<endl;
  }
  TCanvas* mc_e = new TCanvas("mc_e","mc_e",10,10,700,500);
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
        pad1->Draw();
        pad1->cd();
  TMultiGraph *mg1= new TMultiGraph(); 
 
 TGraph* gr_e_wo= new TGraph(5,emean_wo, esigma_wo);
  gr_e_wo->SetLineColor(3);
  gr_e_wo->SetLineWidth(4);
  gr_e_wo->SetMarkerColor(1);
  gr_e_wo->SetMarkerStyle(21);
  gr_e_wo->SetTitle("energy resolution");
  gr_e_wo->GetXaxis()->SetTitle("E_{vis}(MeV)");
  gr_e_wo->GetXaxis()->SetTitleSize(0.042);
  gr_e_wo->GetYaxis()->SetTitle("energy resolution(%)");
  gr_e_wo->GetYaxis()->SetTitleSize(0.042);

  gPad->SetGrid();
 TF1 *func_e_wo = new TF1("fitf3",fitf3, 0.5, 8, 3);
  func_e_wo->SetParameters(2.9,0.6,0);
  func_e_wo->SetParNames("A","B","C");
  func_e_wo->FixParameter(3,0);
  gr_e_wo->Fit(func_e_wo, "r");
     TF1  *fe_wo =  (TF1*)gr_e_wo->GetFunction("fitf3");
     fe_wo->SetLineColor(3);

  gr_e_wo->Draw("AP");

  pad1->Update();
        TPaveStats *ps1 = (TPaveStats*)gr_e_wo->GetListOfFunctions()->FindObject("stats");
           ps1->SetX1NDC(0.6);
           ps1->SetX2NDC(0.8);
           ps1->SetTextColor(3);
           pad1->Modified();
     mc_e->cd();
        pad1->Draw();
        pad1->cd();

  TGraph* gr_e= new TGraph(5,emean, esigma);
  gr_e->SetLineColor(2);
  gr_e->SetLineWidth(4);
  gr_e->SetMarkerColor(4);
  gr_e->SetMarkerStyle(21);
  gr_e->SetTitle("energy resolution");
  gr_e->GetXaxis()->SetTitle("E_{vis}(MeV)");
  gr_e->GetXaxis()->SetTitleSize(0.042);
  gr_e->GetYaxis()->SetTitle("energy resolution(%)");
  gr_e->GetYaxis()->SetTitleSize(0.042);
  gPad->SetGrid();
  
  TF1 *func_e = new TF1("fitf3",fitf3, 0.5, 8, 3);
  func_e->SetParameters(2.9,0.6,0);
  func_e->SetParNames("A","B","C");
  func_e->FixParameter(3,0);
  gr_e->Fit(func_e, "r");

  gr_e->Draw("P");
   pad1->Update();
        TPaveStats *ps2 = (TPaveStats*)gr_e->GetListOfFunctions()->FindObject("stats");
           ps2->SetX1NDC(0.8);
           ps2->SetX2NDC(1.0);
           ps2->SetTextColor(2);
   mg1->Add(gr_e_wo);
   mg1->Add(gr_e);
   mg1->Draw("AP");

  TLegend *legend=new TLegend(0.3,0.7,0.58,0.9);
    legend->SetTextFont(72);
    legend->SetTextSize(0.04);
     legend->AddEntry(gr_e,"elec date fit","l");
     legend->AddEntry(gr_e_wo,"woelec date fit","l");
     legend->Draw();

 TCanvas* mc_v = new TCanvas("mc_v","mc_v",10,10,700,500);
   TPad *pad2 = new TPad("pad2","",0,0,1,1);
        pad2->Draw();
        pad2->cd();
  TMultiGraph *mg = new TMultiGraph();

  TGraph* gr_v_wo= new TGraph(5,emean_wo, vsigma_wo);
  gr_v_wo->SetLineColor(3);
  gr_v_wo->SetLineWidth(4);
  gr_v_wo->SetMarkerColor(1);
  gr_v_wo->SetMarkerStyle(21);
  gr_v_wo->SetTitle("vertex resolution");
  gr_v_wo->GetXaxis()->SetTitle("E_{vis}(MeV)");
  gr_v_wo->GetXaxis()->SetTitleSize(0.042);
  gr_v_wo->GetYaxis()->SetTitle("vertex resolution(R_mm)");
  gr_v_wo->GetYaxis()->SetTitleSize(0.042);

  gPad->SetGrid();

  TF1 *func_v_wo = new TF1("fitf3",fitf3, 0.5, 8, 3);
  func_v_wo->SetParameters(2.9,0.6,0);
  func_v_wo->SetParNames("A","B","C");
  func_v_wo->FixParameter(3,0);
  gr_v_wo->Fit(func_v_wo, "r");
   TF1  *fv_wo =  (TF1*)gr_v_wo->GetFunction("fitf3");
     fv_wo->SetLineColor(3);
  gr_v_wo->Draw("AP");
    pad2->Update();
        TPaveStats *psv1 = (TPaveStats*)gr_v_wo->GetListOfFunctions()->FindObject("stats");
           psv1->SetX1NDC(0.6);
           psv1->SetX2NDC(0.8);
           psv1->SetTextColor(3);
           pad2->Modified();
     mc_v->cd();
        pad2->Draw();
        pad2->cd();


  TGraph* gr_v= new TGraph(5,emean, vsigma);
  gr_v->SetLineColor(2);
  gr_v->SetLineWidth(4);
  gr_v->SetMarkerColor(4);
  gr_v->SetMarkerStyle(21);
  gr_v->SetTitle("vertex resolution");
  gr_v->GetXaxis()->SetTitle("E_{vis}(MeV)");
  gr_v->GetXaxis()->SetTitleSize(0.042);
  gr_v->GetYaxis()->SetTitle("vertex resolution(R_mm)");
  gr_v->GetYaxis()->SetTitleSize(0.042);
  gPad->SetGrid();

  TF1 *func_v = new TF1("fitf3",fitf3, 0.5, 8, 3);
  func_v->SetParameters(2.9,0.6,0);
  func_v->SetParNames("A","B","C");
  func_v->FixParameter(3,0);
  gr_v->Fit(func_v, "r");
  gr_v->Draw("P");

   pad2->Update();
        TPaveStats *psv2 = (TPaveStats*)gr_v->GetListOfFunctions()->FindObject("stats");
           psv2->SetX1NDC(0.8);
           psv2->SetX2NDC(1.0);
           psv2->SetTextColor(2);
  mg->Add(gr_v_wo);
  mg->Add(gr_v);
  mg->Draw("AP");
 TLegend *legend1=new TLegend(0.3,0.7,0.58,0.9);
    legend1->SetTextFont(72);
    legend1->SetTextSize(0.04);
     legend1->AddEntry(gr_v,"elec date fit","l");
     legend1->AddEntry(gr_v_wo,"woelec date fit","l");
     legend1->Draw();  

  mc_e->Update();
  mc_v->Update();
  mc_e->SaveAs("energy_resolution.png");
  mc_v->SaveAs("vertex_resolution.png");
}




