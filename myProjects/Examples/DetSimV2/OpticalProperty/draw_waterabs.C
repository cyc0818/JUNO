struct Params {
    int style;
    int color;

    Params() {
        style = 7;
        color = 2;
    }
};

double eV = 1.0;
double mm = 1.0;
double cm = 10.0*mm;
double m = 100.0*cm;

#include "water_abs_orig.icc"

void draw_wave_one(int N, double energy[], double prob[], Params& param) {
    double* wavelength = new double[N];
    double* component = new double[N];
    for (int i = 0; i < N; ++i) {
        wavelength[i] = 1240./energy[i];
        component[i] = prob[i];
    }

    TGraph* gr_new = new TGraph(N, wavelength, component);
    gr_new->SetMarkerStyle(param.style);
    gr_new->SetMarkerSize(1.2);
    gr_new->SetMarkerColor(param.color);
    gr_new->Draw("P");

}

void draw_waterabs() {

    TCanvas* c = new TCanvas;
    frm = c->DrawFrame(80, 0, 800, 40.*m);

    Params param_orig;
    param_orig.color = 2; // red
    draw_wave_one(316, energy, abslen, param_orig);
}
