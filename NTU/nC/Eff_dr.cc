void Eff_dr(){


    // draw dr spectra for efficiencies

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    vector<TString> iso = {"Gd", "H", "C"};
    TFile* FMC = new TFile("./rootfile/Efficiency_noMOcap_sq_unified.root");
    vector<TH1D*> hMC, hMC_m;
    for(auto i : iso){
        TString loadname = "dr_"+ i + "_passEpdtEd";
        TH1D* htemp = dynamic_cast<TH1D*>(FMC->Get(loadname));
        htemp->Scale(1./htemp->GetEntries());
        hMC.push_back(htemp);
        hMC_m.push_back(new TH1D(i,i,500,0,5));
    }
    

    for(int j = 0; j<hMC.size(); j++){
        for(int i = 1; i<hMC.at(j)->GetNbinsX()+1; i++){
            hMC_m.at(j)->SetBinContent(i, hMC.at(j)->GetBinContent(i));
            hMC_m.at(j)->SetBinError(i, hMC.at(j)->GetBinError(i));
        }
    }


    vector<int> color = {4,2,8};
    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    //gPad->DrawFrame(0,0,50,1e5);
    for(int i = 0; i<hMC_m.size(); i++){
        hMC_m.at(i)->GetXaxis()->SetTitle("Distance (m)");
        hMC_m.at(i)->GetXaxis()->SetRangeUser(0,2.2);
        hMC_m.at(i)->GetYaxis()->SetTitle("Entries/0.01m");
        hMC_m.at(i)->GetXaxis()->CenterTitle(kTRUE);
        hMC_m.at(i)->GetYaxis()->CenterTitle(kTRUE);
        hMC_m.at(i)->SetLineWidth(5);
        //hMC_m.at(i)->SetMarkerStyle(20);
        hMC_m.at(i)->SetMarkerSize(0.8);
        hMC_m.at(i)->SetMarkerColor(color.at(i));
        hMC_m.at(i)->SetLineColor(color.at(i));
        hMC_m.at(i)->Draw("hist same");
    }
    c->BuildLegend();
    TLine* l = new TLine(.5,3.5e-6,.5,0.05);
    l->SetLineColor(1);
    l->SetLineWidth(5);
    l->Draw("same");
    TText* text = new TText(5,0.05," Cut");
    text->SetTextAlign(13); // align = 10*HorizontalAlign + VerticalAlign, 
    text->SetTextColor(1);  // 1=left adjusted, 2=centered, 3=right adjusted
    text->SetTextFont(132);
    text->SetTextSize(0.07);
    //text->Draw();           // 1=bottom adjusted, 2=centered, 3=top adjusted


}




