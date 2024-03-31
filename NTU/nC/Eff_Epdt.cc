#include "./include/EffDraw.h"


void Eff_Epdt(){


    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    TFile* infile1 = new TFile("./rootfile/Efficiency_noMOcap.root");
    TFile* infile2 = new TFile("./rootfile/Efficiency_noMOcap_sq.root");
    pair<TString,TString> name1 = {"Ep_",""};
    vector<pair<TString,TString>> name2 = {{"dt_","_passEp"}, {"dr_","_passEpdtEd"}};
    //vector<pair<TString,TString>> name = {{"Ep_",""}, {"dt_","_passEp"}, {"Ed_","_passEpdt"}, {"dr_","_passEpdtEd"}};
    vector<TString> iso = {"Gd", "H", "C"};
    vector<SpectrumCut> spt;    // {Ep, dt, Ed, dr}
    spt.push_back(SpectrumCut(infile1, name1.first, iso, name1.second));
    for(auto i : name2) spt.push_back(SpectrumCut(infile2, i.first, iso, i.second));


    vector<int> color = {4,2,8};
    vector<double> cut = {1.5, 400, 500};
    vector<TCanvas*> c;
    vector<TString> x_title = {"Prompt Energy (MeV)", "Coincidence Time (#mus)", "Distance (m)"};
    vector<TString> y_title = {"Entries/0.05MeV", "Entries/5#mus", "Entries/0.01m"};
    c.resize(spt.size());
    spt.at(0).Rebin(5);
    spt.at(1).Rebin(5);
    for(int i = 0; i < spt.size(); i++){

        spt.at(i).SetColor(color);
        spt.at(i).SetNames(iso);
        spt.at(i).SetCut(cut.at(i));
        spt.at(i).SetXTitle(x_title.at(i));
        spt.at(i).SetYTitle(y_title.at(i));
        spt.at(i).Normalize();
        c.at(i) = new TCanvas(TString::Format("c%d",i), TString::Format("c%d",i), 1760, 1060);
        if(i == 1) spt.at(i).Draw(c.at(i), true, 0, 1000);
        else spt.at(i).Draw(c.at(i), true);

    }



}


