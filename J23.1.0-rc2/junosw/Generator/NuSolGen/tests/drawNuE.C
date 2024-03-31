#include "SolarNuESpectrum.cc"
void drawNuE() {
  SolarNuESpectrum dNdq;
  dNdq.load("..");
  TString title[] = {"pp","pep","hep","be7","b8","n13","o15","f17"};
  double flux[] = {5.98e10,1.44e8,7.98e3,4.93e9,5.46e6,2.78e8,2.05e8,5.29e6};
  int color[] = {1,0,2,0,3,4,6,7};
  TCanvas *cc = new TCanvas;
  auto frame = gPad->DrawFrame(0.08, 1, 30, 1e13, "test string");
  frame->GetYaxis()->SetTitle("Solar neutrino flux (cm^{-2}s^{-1}MeV^{-1})");
  frame->GetXaxis()->SetTitle("Solar neutrino energy (MeV)");
  gPad->SetLogx(true);
  gPad->SetLogy(true);
  TLegend *leg = new TLegend(0.7,0.6,0.95,0.95);
  for(int i = 0;i<SolarNu::NSolarNuType;++i) {
    enum SolarNu::SolarNuType type = static_cast<enum SolarNu::SolarNuType>(i);
    if(color[i]==0) continue;
    const double dq = dNdq.getDq(type);
    std::vector<double> x,y;
    int j = 1;
    while(dq*(j-2)/100<dNdq.getQMax(type)) {
      const double q = dq*j/100;
      x.push_back(q);
      y.push_back(dNdq.dNdq(type,q)*flux[i]/dq);
      ++j;
    }
    TGraph *gr = new TGraph(x.size(),&x[0],&y[0]);
    gr->Draw("L");
    gr->SetLineColor(color[i]);
    leg->AddEntry(gr,title[i],"l");
  }
  leg->Draw();
  cc->Print("SolarNuE.pdf");
}
