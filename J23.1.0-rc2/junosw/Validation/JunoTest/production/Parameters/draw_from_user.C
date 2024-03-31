/*
 * This script will load ROOT file with optical parameter,
 * and draw these.
 *
 * This script can support run as a script or a compiled executable:
 * + root draw_from_user.C
 * + g++ -o draw_from_user draw_from_user.C $(root-config --cflags --libs)
 *
 * Due to there are a lot of branches, we use TTree::MakeClass to generate
 * a template, then copy it into this script.
 */

#include <iostream>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TMath.h>

struct Params {
    Params() {
        inputfile = "sample_detsim_user.root";
        outputfile = "Parameters.root";
    }

    TString inputfile, outputfile;
};

struct Loader {

    Loader(Params* p)
        : param(p), input(0), oppar(0)
    {
        init();
    }

    ~Loader() {
      if (input) input->Close();
      output->Close();
    }

    void init() {
        input = TFile::Open(param->inputfile);
        if (!input) { return; }

        output = TFile::Open(param->outputfile, "recreate");
        output->cd();

        oppar = dynamic_cast<TTree*>(input->Get("opticalparam"));
        oppar->SetBranchAddress("LS_LY_n",            &LS_LY_n);
        oppar->SetBranchAddress("LS_LY_energy",       LS_LY_energy);
        oppar->SetBranchAddress("LS_LY_ly",           LS_LY_ly);
        oppar->SetBranchAddress("LS_RI_n",            &LS_RI_n);
        oppar->SetBranchAddress("LS_RI_energy",       LS_RI_energy);
        oppar->SetBranchAddress("LS_RI_idx",          LS_RI_idx);
        oppar->SetBranchAddress("LS_Emission_n",      &LS_Emission_n);
        oppar->SetBranchAddress("LS_Emission_energy", LS_Emission_energy);
        oppar->SetBranchAddress("LS_Emission_spec",   LS_Emission_spec);
        oppar->SetBranchAddress("LS_ReEmission_n",    &LS_ReEmission_n);
        oppar->SetBranchAddress("LS_ReEmission_energy", LS_ReEmission_energy);
        oppar->SetBranchAddress("LS_ReEmission_prob", LS_ReEmission_prob);
        oppar->SetBranchAddress("LS_ABS_n",           &LS_ABS_n);
        oppar->SetBranchAddress("LS_ABS_energy",      LS_ABS_energy);
        oppar->SetBranchAddress("LS_ABS_len",         LS_ABS_len);
        oppar->SetBranchAddress("LS_Rayleigh_n",      &LS_Rayleigh_n);
        oppar->SetBranchAddress("LS_Rayleigh_energy", LS_Rayleigh_energy);
        oppar->SetBranchAddress("LS_Rayleigh_len",    LS_Rayleigh_len);
        oppar->SetBranchAddress("Acrylic_ABS_n",      &Acrylic_ABS_n);
        oppar->SetBranchAddress("Acrylic_ABS_energy", Acrylic_ABS_energy);
        oppar->SetBranchAddress("Acrylic_ABS_len",    Acrylic_ABS_len);
        oppar->SetBranchAddress("Acrylic_RI_n",       &Acrylic_RI_n);
        oppar->SetBranchAddress("Acrylic_RI_energy",  Acrylic_RI_energy);
        oppar->SetBranchAddress("Acrylic_RI_idx",     Acrylic_RI_idx);
        oppar->SetBranchAddress("Water_ABS_n",        &Water_ABS_n);
        oppar->SetBranchAddress("Water_ABS_energy",   Water_ABS_energy);
        oppar->SetBranchAddress("Water_ABS_len",      Water_ABS_len);
        oppar->SetBranchAddress("Water_RI_n",         &Water_RI_n);
        oppar->SetBranchAddress("Water_RI_energy",    Water_RI_energy);
        oppar->SetBranchAddress("Water_RI_idx",       Water_RI_idx);
        oppar->SetBranchAddress("PC_EFF_n",           &PC_EFF_n);
        oppar->SetBranchAddress("PC_EFF_energy",      PC_EFF_energy);
        oppar->SetBranchAddress("PC_EFF_eff",         PC_EFF_eff);
        oppar->SetBranchAddress("Pyrex_RI_n",         &Pyrex_RI_n);
        oppar->SetBranchAddress("Pyrex_RI_energy",    Pyrex_RI_energy);
        oppar->SetBranchAddress("Pyrex_RI_idx",       Pyrex_RI_idx);
    }

    // ======================================================================
    // Draw figures
    // ======================================================================
    void draw() {
        oppar->GetEntry(0);
        // = LS related =
        draw_LS_refractive_index();
        draw_LS_emission_spectrum();
        draw_LS_reemission_prob();
        draw_LS_absorption_length();
        draw_LS_rayleigh_length();
        // = Acrylic =
        draw_Acrylic_absorption_length();
        draw_Acrylic_refractive_index();
        // = Water =
        draw_Water_absorption_length();
        draw_Water_refractive_index();
        // = Pyrex =
        draw_Pyrex_refractive_index();
        // = QE =
        draw_QE();
    }

    void draw_LS_refractive_index() {
        TCanvas* c = new TCanvas;
        Double_t* LS_RI_wavelength = new Double_t[LS_RI_n];
        for (int i = 0; i < LS_RI_n; ++i) {
            LS_RI_wavelength[i] = 1240./(LS_RI_energy[i]*1e6);
        }
        TGraph* g = new TGraph(LS_RI_n, LS_RI_wavelength, LS_RI_idx);
        g -> SetTitle("LS Refractive Index");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Refractive Index");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_LS_refractive_index.png", "png");
        c->Print("draw_LS_refractive_index.eps", "eps");
        g->Write("LS_refractive_index");
    }

    void draw_LS_emission_spectrum() {
        TCanvas* c = new TCanvas;
        Double_t* LS_Emission_wavelength = new Double_t[LS_Emission_n];
        for (int i = 0; i < LS_Emission_n; ++i) {
            double energy = (LS_Emission_energy[i]*1e6);
            LS_Emission_wavelength[i] = 1240./energy;
            LS_Emission_spec[i] = LS_Emission_spec[i]*TMath::Power(energy, 2)/1240.;
        }
        TGraph* g = new TGraph(LS_Emission_n, LS_Emission_wavelength, LS_Emission_spec);
        g -> SetTitle("Emission Spectrum");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("a.u.");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_LS_emission_spectrum.png", "png");
        c->Print("draw_LS_emission_spectrum.eps", "eps");
        g->Write("LS_emission_spectrum");
    }

    void draw_LS_reemission_prob() {
        TCanvas* c = new TCanvas;
        Double_t* LS_ReEmission_wavelength = new Double_t[LS_ReEmission_n];
        for (int i = 0; i < LS_ReEmission_n; ++i) {
            LS_ReEmission_wavelength[i] = 1240./(LS_ReEmission_energy[i]*1e6);
        }
        TGraph* g = new TGraph(LS_ReEmission_n, LS_ReEmission_wavelength, LS_ReEmission_prob);
        g -> SetTitle("LS Reemission Prob");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Reemission Prob");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_LS_reemission_prob.png", "png");
        c->Print("draw_LS_reemission_prob.eps", "eps");
        g->Write("LS_reemission_prob");
    }

    void draw_LS_absorption_length() {
        TCanvas* c = new TCanvas;
        Double_t* LS_ABS_wavelength = new Double_t[LS_ABS_n];
        for (int i = 0; i < LS_ABS_n; ++i) {
            LS_ABS_wavelength[i] = 1240./(LS_ABS_energy[i]*1e6);
        }
        TGraph* g = new TGraph(LS_ABS_n, LS_ABS_wavelength, LS_ABS_len);
        g -> SetTitle("LS Absorption Length");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Absorption Length");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_LS_absorption_length.png", "png");
        c->Print("draw_LS_absorption_length.eps", "eps");
        g->Write("LS_absorption_length");
    }

    void draw_LS_rayleigh_length() {
        TCanvas* c = new TCanvas;
        Double_t* LS_Rayleigh_wavelength = new Double_t[LS_Rayleigh_n];
        for (int i = 0; i < LS_Rayleigh_n; ++i) {
            LS_Rayleigh_wavelength[i] = 1240./(LS_Rayleigh_energy[i]*1e6);
        }
        TGraph* g = new TGraph(LS_Rayleigh_n, LS_Rayleigh_wavelength, LS_Rayleigh_len);
        g -> SetTitle("LS Rayleigh Scattering Length");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Scattering Length");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_LS_rayleigh_length.png", "png");
        c->Print("draw_LS_rayleigh_length.eps", "eps");
        g->Write("LS_rayleigh_length");
    }

    void draw_Acrylic_absorption_length() {
        TCanvas* c = new TCanvas;
        Double_t* Acrylic_ABS_wavelength = new Double_t[Acrylic_ABS_n];
        for (int i = 0; i < Acrylic_ABS_n; ++i) {
            Acrylic_ABS_wavelength[i] = 1240./(Acrylic_ABS_energy[i]*1e6);
        }
        TGraph* g = new TGraph(Acrylic_ABS_n, Acrylic_ABS_wavelength, Acrylic_ABS_len);
        g -> SetTitle("Acrylic Absorption Length");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Absorption Length");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_Acrylic_absorption_length.png", "png");
        c->Print("draw_Acrylic_absorption_length.eps", "eps");
        g->Write("Acrylic_absorption_length");
    }

    void draw_Acrylic_refractive_index() {
        TCanvas* c = new TCanvas;
        Double_t* Acrylic_RI_wavelength = new Double_t[Acrylic_RI_n];
        for (int i = 0; i < Acrylic_RI_n; ++i) {
            Acrylic_RI_wavelength[i] = 1240./(Acrylic_RI_energy[i]*1e6);
        }
        TGraph* g = new TGraph(Acrylic_RI_n, Acrylic_RI_wavelength, Acrylic_RI_idx);
        g -> SetTitle("Acrylic Refractive Index");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Refractive Index");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_Acrylic_refractive_index.png", "png");
        c->Print("draw_Acrylic_refractive_index.eps", "eps");
        g->Write("Acrylic_refractive_index");
    }

    void draw_Water_absorption_length() {
        TCanvas* c = new TCanvas;
        Double_t* Water_ABS_wavelength = new Double_t[Water_ABS_n];
        for (int i = 0; i < Water_ABS_n; ++i) {
            Water_ABS_wavelength[i] = 1240./(Water_ABS_energy[i]*1e6);
        }
        TGraph* g = new TGraph(Water_ABS_n, Water_ABS_wavelength, Water_ABS_len);
        g -> SetTitle("Water Absorption Length");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Absorption Length");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_Water_absorption_length.png", "png");
        c->Print("draw_Water_absorption_length.eps", "eps");
        g->Write("Water_absorption_length");
    }

    void draw_Water_refractive_index() {
        TCanvas* c = new TCanvas;
        Double_t* Water_RI_wavelength = new Double_t[Water_RI_n];
        for (int i = 0; i < Water_RI_n; ++i) {
            Water_RI_wavelength[i] = 1240./(Water_RI_energy[i]*1e6);
        }
        TGraph* g = new TGraph(Water_RI_n, Water_RI_wavelength, Water_RI_idx);
        g -> SetTitle("Water Refractive Index");
        g -> SetMarkerStyle(22);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Refractive Index");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        // g -> GetYaxis()->SetRangeUser(0,0.04);
        g->Draw("APL");

        c->Print("draw_Water_refractive_index.png", "png");
        c->Print("draw_Water_refractive_index.eps", "eps");
        g->Write("Water_refractive_index");
    }

    void draw_Pyrex_refractive_index() {
        TCanvas* c = new TCanvas;
        Double_t* Pyrex_RI_wavelength = new Double_t[Pyrex_RI_n];
        for (int i = 0; i < Pyrex_RI_n; ++i) {
            Pyrex_RI_wavelength[i] = 1240./(Pyrex_RI_energy[i]*1e6);
        }
        TGraph* g = new TGraph(Pyrex_RI_n, Pyrex_RI_wavelength, Pyrex_RI_idx);
        g -> SetTitle("Pyrex Refractive Index");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("Refractive Index");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        g->Draw("APL");

        c->Print("draw_Pyrex_refractive_index.png", "png");
        c->Print("draw_Pyrex_refractive_index.eps", "eps");
        g->Write("Pyrex_refractive_index");
    }

    void draw_QE() {
        TCanvas* c = new TCanvas;
        Double_t* PC_EFF_wavelength = new Double_t[PC_EFF_n];
        for (int i = 0; i < PC_EFF_n; ++i) {
            PC_EFF_wavelength[i] = 1240./(PC_EFF_energy[i]*1e6);
        }
        TGraph* g = new TGraph(PC_EFF_n, PC_EFF_wavelength, PC_EFF_eff);
        g -> SetTitle("QE");
        g -> SetMarkerStyle(21);
        g -> GetXaxis()->SetTitle("wavelength(nm)");
        g -> GetXaxis()->SetLabelSize(0.05);
        g -> GetXaxis()->SetTitleSize(0.05);
        g -> GetYaxis()->SetTitle("QE");
        g -> GetYaxis()->SetTitleSize(0.055);
        g -> GetYaxis()->SetTitleOffset(1.0);
        g -> GetYaxis()->SetLabelSize(0.05);
        // g -> GetYaxis()->SetRangeUser(0,0.04);
        g->Draw("APL");

        c->Print("draw_QE.png", "png");
        c->Print("draw_QE.eps", "eps");
        g->Write("QE");
    }

    // ======================================================================
    Params* param;
    TFile* input, *output;
    TTree* oppar;

    // variables
    // Declaration of leaf types
    Int_t           LS_LY_n;
    Double_t        LS_LY_energy[2];   //[LS_LY_n]
    Double_t        LS_LY_ly[2];   //[LS_LY_n]
    Int_t           LS_RI_n;
    Double_t        LS_RI_energy[18];   //[LS_RI_n]
    Double_t        LS_RI_idx[18];   //[LS_RI_n]
    Int_t           LS_Emission_n;
    Double_t        LS_Emission_energy[275];   //[LS_Emission_n]
    Double_t        LS_Emission_spec[275];   //[LS_Emission_n]
    Int_t           LS_ReEmission_n;
    Double_t        LS_ReEmission_energy[28];   //[LS_ReEmission_n]
    Double_t        LS_ReEmission_prob[28];   //[LS_ReEmission_n]
    Int_t           LS_ABS_n;
    Double_t        LS_ABS_energy[502];   //[LS_ABS_n]
    Double_t        LS_ABS_len[502];   //[LS_ABS_n]
    Int_t           LS_Rayleigh_n;
    Double_t        LS_Rayleigh_energy[11];   //[LS_Rayleigh_n]
    Double_t        LS_Rayleigh_len[11];   //[LS_Rayleigh_n]
    Int_t           Acrylic_ABS_n;
    Double_t        Acrylic_ABS_energy[9];   //[Acrylic_ABS_n]
    Double_t        Acrylic_ABS_len[9];   //[Acrylic_ABS_n]
    Int_t           Acrylic_RI_n;
    Double_t        Acrylic_RI_energy[18];   //[Acrylic_RI_n]
    Double_t        Acrylic_RI_idx[18];   //[Acrylic_RI_n]
    Int_t           Water_ABS_n;
    Double_t        Water_ABS_energy[316];   //[Water_ABS_n]
    Double_t        Water_ABS_len[316];   //[Water_ABS_n]
    Int_t           Water_RI_n;
    Double_t        Water_RI_energy[36];   //[Water_RI_n]
    Double_t        Water_RI_idx[36];   //[Water_RI_n]
    Int_t           PC_EFF_n;
    Double_t        PC_EFF_energy[42];   //[PC_EFF_n]
    Double_t        PC_EFF_eff[42];   //[PC_EFF_n]
    Int_t           Pyrex_RI_n;
    Double_t        Pyrex_RI_energy[6];   //[Pyrex_RI_n]
    Double_t        Pyrex_RI_idx[6];   //[Pyrex_RI_n]
};

int draw_from_user() {
    Params* param = new Params();

    Loader* loader = new Loader(param);
    loader->draw();
}

int main() {

}
