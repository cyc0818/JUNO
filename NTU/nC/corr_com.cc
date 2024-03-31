double myfunc(double* x, double* par){

    if(x[0]>4 and x[0]<6){
    //if(x[0]>3.5 and x[0]<6){
        TF1::RejectPoint();
        return 0;
    }

    return par[0];

}






void corr_com(){

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.11);
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);


    TFile* fdata = new TFile("./rootfile/P17B_accsub_final_800iso_mergefar_40.root");
    TH1D* hdata = dynamic_cast<TH1D*>(fdata->Get("subD_AD3"));
    TFile* fMC = new TFile("./rootfile/AfterCorrection.root");
    TH1D* h1 = dynamic_cast<TH1D*>(fMC->Get("firststep_corr"));
    TH1D* h2 = dynamic_cast<TH1D*>(fMC->Get("twosteps_corr"));


    hdata->Rebin(5);
    hdata->SetTitle("EH2AD1;Delayed Energy (MeV);Entries/0.05MeV");
    hdata->SetLineColor(4);
    hdata->SetLineWidth(5);
    hdata->SetMarkerColor(4);
    hdata->SetMarkerStyle(20);
    hdata->SetMarkerSize(1.2);
    hdata->GetXaxis()->CenterTitle(kTRUE);
    hdata->GetYaxis()->CenterTitle(kTRUE);
    hdata->GetXaxis()->SetRangeUser(2.9,7.1);
    hdata->GetXaxis()->SetTickSize(0.08);
    hdata->GetXaxis()->SetLabelSize(0.11);
    hdata->GetXaxis()->SetTitleSize(0.13);
    hdata->GetXaxis()->SetTitleOffset(0.79);
    hdata->GetYaxis()->SetRangeUser(150.,4.5e3);
    hdata->GetYaxis()->SetLabelSize(0.14);
    hdata->GetYaxis()->SetTitleSize(0.15);
    hdata->GetYaxis()->SetTitleOffset(0.39);

    h1->Rebin(5);
    h1->SetTitle("Corrected_MC;Delayed Energy (MeV);Entries/0.05MeV");
    h1->SetLineColor(2);
    h1->SetLineWidth(9);
    h1->SetMarkerColor(2);
    h1->SetMarkerStyle(20);
    h1->SetMarkerSize(0.7);
    h1->GetXaxis()->CenterTitle(kTRUE);
    h1->GetYaxis()->CenterTitle(kTRUE);
    h1->GetXaxis()->SetRangeUser(2.9,7.1);
    h1->GetYaxis()->SetRangeUser(150.,4.5e3);

    h2->Rebin(5);
    h2->SetTitle("Corrected_MC;Delayed Energy (MeV);Entries/0.05MeV");
    h2->SetLineColor(8);
    h2->SetLineWidth(9);
    h2->SetMarkerColor(8);
    h2->SetMarkerStyle(20);
    h2->SetMarkerSize(0.7);
    h2->GetXaxis()->CenterTitle(kTRUE);
    h2->GetYaxis()->CenterTitle(kTRUE);
    h2->GetXaxis()->SetRangeUser(2.9,7.1);
    h2->GetYaxis()->SetRangeUser(150.,4.5e3);

    TH1D* r1 = new TH1D();
    *r1 = *hdata;
    r1->SetName("r1");
    r1->SetTitle(";Delayed Energy (MeV);EH2AD1/CorrMC   ");
    r1->SetLineColor(2);
    r1->SetMarkerColor(2);
    r1->Divide(h1);
    r1->GetXaxis()->SetRangeUser(2.9,7.1);
    r1->GetXaxis()->SetTickSize(0.08);
    r1->GetYaxis()->SetRangeUser(0.55,1.45);
    r1->GetYaxis()->SetLabelSize(0.11);
    r1->GetYaxis()->SetTitleSize(0.12);
    r1->GetYaxis()->SetTitleOffset(0.48);

    TH1D* r2 = new TH1D();
    *r2 = *hdata;
    r2->SetName("r2");
    r2->SetTitle(";Delayed Energy (MeV);EH2AD1/CorrMC   ");
    r2->SetLineColor(8);
    r2->SetMarkerColor(8);
    h2->SetMarkerStyle(24);
    r2->Divide(h2);
    r2->GetXaxis()->SetRangeUser(2.9,7.1);
    r2->GetXaxis()->SetTickSize(0.08);
    r2->GetYaxis()->SetRangeUser(0.55,1.45);
    r2->GetYaxis()->SetLabelSize(0.11);
    r2->GetYaxis()->SetTitleSize(0.12);
    r2->GetYaxis()->SetTitleOffset(0.48);

    TLine* l = new TLine(2.9,1,7.1,1);
    l->SetLineColor(1);
    l->SetLineWidth(5);


    TCanvas* c1 = new TCanvas("c1","c1",1100,1160);
    c1->Divide(1,3,0,0);
    //c1->Divide(1,4,0,0);
    c1->cd(1);
    gPad->SetLogy(1);
    hdata->Draw();
    h1->Draw("hist same");
    TLegend* lg1 = new TLegend(0.52,0.08,0.999,0.46);
    lg1->AddEntry(hdata->GetName(), hdata->GetTitle(), "lep");
    lg1->AddEntry(h1->GetName(), "1-step corrected MC", "l");
    lg1->Draw("same");

    c1->cd(3);
    r1->Draw();
    l->Draw("same");

    c1->cd(2);
    gPad->SetLogy(1);
    hdata->Draw();
    h2->Draw("hist same");
    TLegend* lg2 = new TLegend(0.52,0.08,0.999,0.46);
    lg2->AddEntry(hdata->GetName(), hdata->GetTitle(), "lep");
    lg2->AddEntry(h2->GetName(), "2-step corrected MC", "l");
    lg2->Draw("same");

    c1->cd(3);
    r2->Draw("same");
    l->Draw("same");
    TLegend* lg3 = new TLegend(0.845,0.1,0.999,0.30);
    lg3->AddEntry(r1->GetName(), "1-step", "lep");
    lg3->AddEntry(r2->GetName(), "2-step", "lep");
    lg3->Draw("same");
    //c1->BuildLegend();



    /*TF1* func = new TF1("func","myfunc",3,7,1);
    r1->Fit(func,"","",3,7);    // chi2/NDF = 182.4/39
    cout << "Old : " <<  func->GetParameter(0) << " +/- " << func->GetParError(0) << endl;
    r2->Fit(func,"","",3,7);    // chi2/NDF = 70.3/39
    cout << "New : " <<  func->GetParameter(0) << " +/- " << func->GetParError(0) << endl;*/





}
