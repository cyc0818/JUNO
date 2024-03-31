void dr_comparison(){


    // draw comparison of dr spectra of data and MC

    //gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    TFile* Fdata = new TFile("./rootfile/P17B_accsub_final_800iso_40.root");
    TH1D* hdata = dynamic_cast<TH1D*>(Fdata->Get("can_dr_AD1"));
    TFile* FMC = new TFile("./rootfile/Efficiency_noMOcap_sq_unified.root");
    TH1D* hMC = dynamic_cast<TH1D*>(FMC->Get("dr_passEpdtEd"));
    int finalbin = hdata->FindBin(300);
    double scale = hdata->Integral(1,finalbin) / hMC->Integral(1,finalbin);
    hMC->Scale(scale);
    

    TH1D* hdata_m = new TH1D("hdata_m","EH1AD1-data;Distance (m);Entries/0.01m",500,0,50);
    TH1D* hMC_m = new TH1D("hMC_m","MC;Distance (m);Entries/0.01m",500,0,50);
    for(int i = 1; i<hdata->GetNbinsX()+1; i++){
        hdata_m->SetBinContent(i, hdata->GetBinContent(i));
        hdata_m->SetBinError(i, hdata->GetBinError(i));
        hMC_m->SetBinContent(i, hMC->GetBinContent(i));
        hMC_m->SetBinError(i, hMC->GetBinError(i));
    }

    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    //gPad->DrawFrame(0,0,50,1e5);
    hdata_m->GetXaxis()->CenterTitle(kTRUE);
    hdata_m->GetYaxis()->CenterTitle(kTRUE);
    hdata_m->SetLineWidth(3);
    hdata_m->SetMarkerStyle(20);
    hdata_m->SetMarkerSize(0.8);
    hdata_m->SetMarkerColor(4);
    hdata_m->SetLineColor(4);
    hMC_m->SetLineColor(2);
    hMC_m->SetLineWidth(6);
    hdata_m->Draw("e");
    hMC_m->Draw("same hist");
    c->BuildLegend();
    /*TLine* l = new TLine(5,65,5,7.2e4);
    l->SetLineColor(2);
    l->Draw("same");
    TText* text = new TText(5,7e4," Cut");
    text->SetTextAlign(13); // align = 10*HorizontalAlign + VerticalAlign, 
    text->SetTextColor(2);  // 1=left adjusted, 2=centered, 3=right adjusted
    text->Draw();           // 1=bottom adjusted, 2=centered, 3=top adjusted
    */


}




