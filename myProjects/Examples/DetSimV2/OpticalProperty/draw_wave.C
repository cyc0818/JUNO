double eV = 1.;
#include "OpticalProperty_svn.icc"
#include "OpticalProperty_lixy.icc"
#include "OpticalProperty_orig.icc"

struct Params {
    int style;
    int color;

    Params() {
        style = 7;
        color = 2;
    }
};

void draw_wave_one(int N, double energy[], double prob[], Params& param) {
    double* wavelength = new double[N];
    double* component = new double[N];
    for (int i = 0; i < N; ++i) {
        wavelength[i] = 1240./energy[i];
        component[i] = prob[i]*(energy[i]*energy[i])/1240.;
    }

    TGraph* gr_new = new TGraph(N, wavelength, component);
    gr_new->SetMarkerStyle(param.style);
    gr_new->SetMarkerSize(1.2);
    gr_new->SetMarkerColor(param.color);
    gr_new->Draw("P");

}

void draw_wave() {
    TCanvas* c = new TCanvas;
    frm = c->DrawFrame(300, 0, 550, 0.008);
    // frm = c->DrawFrame(300, 0, 550, 1);

    Params param_svn;
    param_svn.color = 2; // red
    draw_wave_one(273, energy, prob, param_svn);

    Params param_lixy;
    param_lixy.color = 4; // blue
    draw_wave_one(275, GdLSComEnergy, GdLSFastComponent, param_lixy);

    Params param_orig;
    param_orig.color = 6; // pink
    draw_wave_one(275, GdLSComEnergy_orig, GdLSFastComponent_orig, param_orig);
}
