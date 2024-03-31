
void Eff_Ep_vol(){


    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    TH1D* h0 = new TH1D("h0","MO;Prompt Energy (MeV);Entries/0.05MeV",240,0,12);
    TH1D* h1 = new TH1D("h1","LS;Prompt Energy (MeV);Entries/0.05MeV",240,0,12);
    TH1D* h2 = new TH1D("h2","GdLS;Prompt Energy (MeV);Entries/0.05MeV",240,0,12);
    TH1D* h3 = new TH1D("h3","Acrylic;Prompt Energy (MeV);Entries/0.05MeV",240,0,12);
    vector<TH1D*> v = {h0, h1, h2, h3};


    TString filepath = "./rootfile/prompt_eff";
    TChain* ch = new TChain("stats/tree/IBD");
    ch->Add(filepath + "/*");
    int vol;
    double ep;
    ch->SetBranchAddress("genType",&vol);
    ch->SetBranchAddress("ep",&ep);
    for(int i = 0; i<ch->GetEntries(); i++){
        ch->GetEntry(i);
        v.at(vol)->Fill(ep);
    }


    vector<int> color = {8,2,4,6};
    for(int i = 0; i<v.size(); i++){
        v.at(i)->GetXaxis()->CenterTitle(kTRUE);
        v.at(i)->GetYaxis()->CenterTitle(kTRUE);
        v.at(i)->GetYaxis()->SetRangeUser(1.e-6,1);
        v.at(i)->SetLineWidth(7);
        v.at(i)->SetMarkerSize(0.8);
        v.at(i)->SetMarkerColor(color.at(i));
        v.at(i)->SetLineColor(color.at(i));
        v.at(i)->Scale(1./v.at(i)->GetEntries());
    }

    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    h2->Draw("hist");
    h1->Draw("hist same");
    h3->Draw("hist same");
    h0->Draw("hist same");
    c->BuildLegend();


}





