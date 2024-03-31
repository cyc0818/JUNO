struct Params {
    int style;
    int color;
    float size;

    Params() {
        style = 21;
        color = 2;
        size = 1.;
    }
};

double eV = 1.0;
double mm = 1.0;
double cm = 10.0*mm;
double m = 100.0*cm;

#include "pmt_qe_1inch_20140620.icc"

void draw_wave_one(int N, double energy[], double prob[], Params& param) {
    double* wavelength = new double[N];
    double* component = new double[N];
    for (int i = 0; i < N; ++i) {
        wavelength[i] = 1240./energy[i];
        component[i] = prob[i];
    }

    TGraph* gr_new = new TGraph(N, wavelength, component);
    gr_new->SetMarkerStyle(param.style);
    gr_new->SetMarkerSize(param.size);
    gr_new->SetMarkerColor(param.color);
    gr_new->Draw("LP");

}

void draw_pmt_qe() {

    TCanvas* c = new TCanvas;
    frm = c->DrawFrame(80, 0, 800, 0.35);

    TLine* l = new TLine(430., gPad->GetUymin(), 430., gPad->GetUymax());
    l->Draw();

    Params param_orig;
    param_orig.color = 2; // red
    draw_wave_one(42, energy, qe, param_orig);
}
