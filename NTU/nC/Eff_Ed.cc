void Eff_Ed(){


    // draw dr spectra for efficiencies

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    vector<TString> iso = {"Gd", "H", "C"};
    TFile* FMC = new TFile("./rootfile/Efficiency_noMOcap_sq_unified.root");
    vector<TH1D*> hMC, hMC_m;
    for(auto i : iso){
        TString loadname = "Ed_"+ i + "_passEpdt";
        TH1D* htemp = dynamic_cast<TH1D*>(FMC->Get(loadname));
        if (i == "C"){
            for(int j = htemp->FindBin(6.02); j<htemp->GetNbinsX()+1; j++) htemp->SetBinContent(j,0);
        }
        htemp->Scale(1./htemp->GetEntries());
        hMC.push_back(htemp);
        //hMC_m.push_back(new TH1D(i,i,500,0,50));
    }
    

    /*for(int j = 0; j<hMC.size(); j++){
        for(int i = 1; i<hMC.at(j)->GetNbinsX()+1; i++){
            hMC_m.at(j)->SetBinContent(i, hMC.at(j)->GetBinContent(i));
            hMC_m.at(j)->SetBinError(i, hMC.at(j)->GetBinError(i));
        }
    }*/


    vector<int> color = {4,2,8};
    vector<double> lowedge = {6, 1.9, 4};
    vector<double> highedge = {12, 2.7, 5.8};
    vector<TLine*> line1;
    vector<TLine*> line2;
    line1.resize(hMC.size());
    line2.resize(hMC.size());
    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    //gPad->DrawFrame(0,0,50,1e5);
    for(int i = 0; i<hMC.size(); i++){
        hMC.at(i)->GetXaxis()->SetTitle("Delayed Energy (MeV)");
        hMC.at(i)->GetYaxis()->SetTitle("Entries/0.01MeV");
        hMC.at(i)->GetYaxis()->SetRangeUser(2.2e-6,0.03);
        hMC.at(i)->GetXaxis()->CenterTitle(kTRUE);
        hMC.at(i)->GetYaxis()->CenterTitle(kTRUE);
        hMC.at(i)->SetLineWidth(5);
        hMC.at(i)->SetTitle(iso.at(i));
        //h_m.at(i)->SetMarkerStyle(20);
        hMC.at(i)->SetMarkerSize(0.8);
        hMC.at(i)->SetMarkerColor(color.at(i));
        hMC.at(i)->SetLineColor(color.at(i));
        hMC.at(i)->Draw("hist same");
    }
    c->BuildLegend();
    


    for(int i = 0; i<hMC.size(); i++){
        line1.at(i) = new TLine(lowedge.at(i), 2.2e-6, lowedge.at(i), 0.03);
        line1.at(i)->SetLineColor(color.at(i));
        line1.at(i)->SetLineWidth(5);
        line1.at(i)->SetLineStyle(7);
        line2.at(i) = new TLine(highedge.at(i), 2.2e-6, highedge.at(i), 0.03);
        line2.at(i)->SetLineColor(color.at(i));
        line2.at(i)->SetLineWidth(5);
        line2.at(i)->SetLineStyle(7);
        //line1.at(i)->Draw("same");
        //line2.at(i)->Draw("same");
    }
    TText* text = new TText(5,0.05," Cut");
    text->SetTextAlign(13); // align = 10*HorizontalAlign + VerticalAlign, 
    text->SetTextColor(1);  // 1=left adjusted, 2=centered, 3=right adjusted
    text->SetTextFont(132);
    text->SetTextSize(0.07);
    //text->Draw();           // 1=bottom adjusted, 2=centered, 3=top adjusted


}




