void nKE(){

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    TFile* infile = new TFile("./rootfile/nKE_200k.root");
    // nKE_Before_Cap_GdLS_Gd157
    //

    const TString base = "nKE_Before_Cap_";
    vector<TString> name1 = {"GdLS_H", "GdLS_C", "GdLS_Gd155", "GdLS_Gd157"};
    vector<TString> name2 = {"GdLS_H", "LS_H", "Acrylic_H"};
    vector<TH1D*> h1, h2;


    for(auto& i : name1) h1.push_back(dynamic_cast<TH1D*>(infile->Get(base+i)));
    for(auto& i : name2) h2.push_back(dynamic_cast<TH1D*>(infile->Get(base+i)));
    //for(auto& i : h1) if(i->GetBinWidth(1) != 2.e-3) i->Rebin(100);
    //for(auto& i : h2) if(i->GetBinWidth(1) != 2.e-3) i->Rebin(100);


    vector<int> color = {4,2,8,6};
    TCanvas* c1 = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    TLegend* lg1 = new TLegend(0.65,0.63,0.93,0.95);
    for(int i = 0; i<h1.size(); i++){
        h1.at(i)->Rebin(100);
        h1.at(i)->Scale(1./h1.at(i)->Integral(1, h1.at(i)->GetNbinsX()));
        h1.at(i)->GetXaxis()->SetRangeUser(0, 0.3);
        h1.at(i)->GetXaxis()->SetTitle("Kinetic energy (eV)");
        h1.at(i)->GetXaxis()->SetTitleSize(0.09);
        h1.at(i)->GetXaxis()->SetTitleOffset(0.74);
        h1.at(i)->GetYaxis()->SetTitle("Entries/2meV");
        h1.at(i)->GetYaxis()->SetTitleSize(0.09);
        h1.at(i)->GetYaxis()->SetTitleOffset(0.69);
        h1.at(i)->GetXaxis()->CenterTitle(kTRUE);
        h1.at(i)->GetYaxis()->CenterTitle(kTRUE);
        h1.at(i)->SetLineWidth(6);
        h1.at(i)->SetMarkerSize(0.8);
        h1.at(i)->SetMarkerColor(color.at(i));
        h1.at(i)->SetLineColor(color.at(i));
        h1.at(i)->Draw("hist same");
        lg1->AddEntry(h1.at(i)->GetName(), name1.at(i), "lep");
    }
    lg1->Draw("same");


    TCanvas* c2 = new TCanvas("c2","c2",1760,1060);
    gPad->SetLogy(1);
    TLegend* lg2 = new TLegend(0.65,0.63,0.93,0.95);
    for(int i = 0; i<h2.size(); i++){
        if(i != 0){
            h2.at(i)->Rebin(100);
            h2.at(i)->Scale(1./h2.at(i)->Integral(1, h2.at(i)->GetNbinsX()));
        }
        h2.at(i)->GetXaxis()->SetRangeUser(0, 0.3);
        h2.at(i)->GetXaxis()->SetTitle("Kinetic energy (eV)");
        h2.at(i)->GetXaxis()->SetTitleSize(0.09);
        h2.at(i)->GetXaxis()->SetTitleOffset(0.74);
        h2.at(i)->GetYaxis()->SetTitle("Entries/2meV");
        h2.at(i)->GetYaxis()->SetTitleSize(0.09);
        h2.at(i)->GetYaxis()->SetTitleOffset(0.69);
        h2.at(i)->GetXaxis()->CenterTitle(kTRUE);
        h2.at(i)->GetYaxis()->CenterTitle(kTRUE);
        h2.at(i)->SetLineWidth(6);
        h2.at(i)->SetMarkerSize(0.8);
        h2.at(i)->SetMarkerColor(color.at(i));
        h2.at(i)->SetLineColor(color.at(i));
        h2.at(i)->Draw("hist same");
        lg2->AddEntry(h2.at(i)->GetName(), name2.at(i), "lep");
    }
    lg2->Draw("same");



}
