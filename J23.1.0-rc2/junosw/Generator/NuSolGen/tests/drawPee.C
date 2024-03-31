#include "PeeSun.cc"
void drawPee() {
  PeeSun pee;
  TString title[] = {"pp","pep","hep","be7","b8","n13","o15","f17"};
  int color[] = {1,2,3,4,6,7,8,9};
  TCanvas *cc = new TCanvas;
  auto frame = gPad->DrawFrame(0.2, 0.2, 30, 0.8, "test string");
  frame->GetYaxis()->SetTitle("Survival probability");
  frame->GetXaxis()->SetTitle("Solar neutrino energy (MeV)");
  gPad->SetLogx(true);
  TLegend *leg = new TLegend(0.7,0.6,0.95,0.95);
  for(int i = 0;i<SolarNu::NSolarNuType;++i) {
    enum SolarNu::SolarNuType type = static_cast<enum SolarNu::SolarNuType>(i);
    pee.setNuType(type);
    std::vector<double> x,y;
    double q0 = log(0.1);
    double q1 = log(30);
    double dq = (q1-q0)/999;
    for(int  j = 0;j<1000;++j) {
      const double q = exp(q0+dq*j);
      x.push_back(q);
      y.push_back(pee.Pee(q));
    }
    TGraph *gr = new TGraph(x.size(),&x[0],&y[0]);
    gr->Draw("L");
    gr->SetLineColor(color[i]);
    leg->AddEntry(gr,title[i],"l");
  }
  leg->Draw();
  cc->Print("Pee.pdf");
}
