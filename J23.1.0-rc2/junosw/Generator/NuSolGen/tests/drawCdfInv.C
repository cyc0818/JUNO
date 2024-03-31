#include "SolarNuEES.cc"
#include "SolarNuESpectrum.cc"
#include "PeeSun.cc"
#include "NuEESXSec.cc"
#include "TCanvas.h"
#include "TString.h"
#include "TH1.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGraph.h"
void drawCdfInv() {
  SolarNuEES dNdt;
  TString title[] = {"pp","pep","hep","be7","b8","n13","o15","f17"};
  const double flux[] = {5.98e10,1.44e8,7.98e3,4.93e9,5.46e6,2.78e8,2.05e8,5.29e6}; // cm^{-2}s^{-1}
  const double sigma0 = 2*pow(1.166e-11,2)*0.511/3.14159*pow(197.3269804e-13,2); // unit: cm^2MeV^{-1} 1.722e-44 cm^2/MeV
  double NelectronPerkton = 3.373e32; // #H per kt
  double exposure = 86400; // unit: s x kt ; = 1 day x kt
  int color[] = {1,2,3,4,6,7,8,9};
  TCanvas *cc = new TCanvas;
  auto frame = gPad->DrawFrame(-0.2,8e-4, 1.4, 80, "test string");
  frame->GetXaxis()->SetTitle("Cumulative probability");
  frame->GetYaxis()->SetTitle("Recoil electron energy (MeV)");
  gPad->SetLogy(true);
  TLegend *leg = new TLegend(0.7,0.6,0.95,0.95);
  for(int i = 0;i<SolarNu::NSolarNuType;++i) {
    enum SolarNu::SolarNuType type = static_cast<enum SolarNu::SolarNuType>(i);
    dNdt.initialize("..",type);
    std::vector<double> x,y;
    const double dpcum = 1.2/1000;
    double s = 0;
    for(int j = 0;j<1000;++j) {
      const double pcum = dpcum*(j-10);
      x.push_back(pcum);
      y.push_back(dNdt.getCdfInv(type,pcum));
    }
    TGraph *gr = new TGraph(x.size(),&x[0],&y[0]);
    gr->Draw("L");
    gr->SetLineColor(color[i]);
    leg->AddEntry(gr,title[i],"l");
  }
  leg->Draw();
  cc->Print("Cdf_Inv.pdf");
}
