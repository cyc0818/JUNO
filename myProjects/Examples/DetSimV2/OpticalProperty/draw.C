double eV = 1.;
#include "OpticalProperty_svn.icc"
#include "OpticalProperty_lixy.icc"
#include "OpticalProperty_orig.icc"

void draw() {
    TCanvas* c = new TCanvas;
    // frm = c->DrawFrame(0., 0., 15.5, 1.);
    // frm = c->DrawFrame(0., 0., 5., 1.);
    frm = c->DrawFrame(2., 0., 4., 1.1);

    TGraph* gr_new = new TGraph(273, energy, prob);
    gr_new->SetMarkerStyle(7);
    gr_new->SetMarkerSize(1.2);
    gr_new->SetMarkerColor(2);
    gr_new->Draw("Psame");

    TGraph* gr_lixy = new TGraph(275, GdLSComEnergy, GdLSFastComponent);
    gr_lixy->SetMarkerStyle(7);
    gr_lixy->SetMarkerSize(1.2);
    gr_lixy->SetMarkerColor(4);
    gr_lixy->Draw("Psame");

    TGraph* gr_orig = new TGraph(275, GdLSComEnergy_orig, GdLSFastComponent_orig);
    gr_orig->SetMarkerStyle(7);
    gr_orig->SetMarkerSize(1.2);
    gr_orig->SetMarkerColor(6);
    gr_orig->Draw("Psame");

}
