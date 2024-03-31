#include "./include/Uncertainty.h"



Uncertainty ENDF(const char* ele, int iso, double given_err){

    // To draw nH cross section, use ENDF("H",1,0.02) 
    // To draw nGd155 cross section, use ENDF("Gd",155,5000) 
    // To draw nGd157 cross section, use ENDF("Gd",157,12000) 
    // To draw nC cross section, use ENDF("C",12,4.e-4) 


    gROOT->ForceStyle();
    string line;
    TGraphErrors* g = new TGraphErrors();
    ifstream Fin(TString::Format("./ENDF/%s%d-102.dat",ele,iso));
    int i = 0;
    int points = 0;


    while(Fin.good()){
        i++;
        getline(Fin, line);
        if(i < 6) continue;


        stringstream ss(line);
        string word;
        pair<double, double> p(0,0);    // < kinetic energy , cross section >


        for(int j = 0; j<7; j++){
            getline(ss, word, ' ');
            if(not j) continue;
            if(word.empty()) continue;


            if(j&1){
                //cout << "Energy = " << word;
                p.first = stod(word);
            }
            else{
                //cout << " , cross section = " << word << endl;
                p.second = stod(word);
            }


            if(p.first and p.second){
                g->SetPoint(points, p.first, p.second);
                p = {0,0};
                points++;
            }

        }


    }


    g->SetTitle("^{1}H(n,#gamma)^{2}H cross section;E_{n}^{kin}(eV);Counts");
    g->SetTitle(TString::Format("^{%d}%s(n,#gamma)^{%d}%s cross section;Kinetic energy (eV);Cross section (b)",iso,ele,iso+1,ele));
    g->SetMarkerStyle(20);
    g->SetMarkerColor(2);
    g->SetLineColor(2);
    g->SetMarkerSize(1.4);
    g->GetXaxis()->CenterTitle(kTRUE);
    g->GetYaxis()->CenterTitle(kTRUE);
    TCanvas* c1 = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogx(1);
    gPad->SetLogy(1);
    g->Draw();


    Uncertainty integral;
    double kine = 0;
    double total = 0;
    double error = 0;
    TFile* F = new TFile("./rootfile/Volume_flux_C.root");
    TH1D* h = dynamic_cast<TH1D*>(F->Get("nKE_Before_Cap_woMO_C")); 
    //TH1D* h = dynamic_cast<TH1D*>(F->Get("nKE_Before_Cap")); 
    TCanvas* c2 = new TCanvas("c2","c2",3840,2160);
    h->Rebin(100);
    //h->Rebin(10);
    h->Draw();


    for(int bin = 1; bin<h->GetNbinsX()+1; bin++){

        Uncertainty N(h->GetBinContent(bin), sqrt(h->GetBinContent(bin)));
        Uncertainty cs(g->Eval(h->GetBinCenter(bin)*1e6), given_err);
        //Uncertainty cs(g->Eval(h->GetBinCenter(bin)*1e6), g->Eval(h->GetBinCenter(bin)*1e6)*percent_err);
        Uncertainty temp = N*cs;


        integral += temp;
        total += h->GetBinContent(bin);
        kine += h->GetBinContent(bin) * h->GetBinCenter(bin)*1e6;

    }


    cout << "Integral = " << integral << endl;
    cout << "Total = " << total << " +/- " << sqrt(total) << endl;
    cout << "Weighted average of cross section = " << integral/total << endl;
    cout << "Kine = " << kine/total << endl;
    cout << g->Eval(0.05) << endl;


    F->Close();


    //return integral;
    return integral/total;



}


