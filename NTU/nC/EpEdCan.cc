void EpEdCan(){


    // draw prompt and delayed spectrum of 8ADs (candidates)

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.045);
    gStyle->SetOptStat(0);
    TFile* infile = new TFile("./rootfile/P17B_accsub_final_800iso_40.root");
    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    for(int i = 1; i<9; i++){
        int EH = ((i-1)/2+1)*2/3 + 1; 
        int AD = i - 2*(EH-1);
        //TH1D* hcan = infile->Get<TH1D>(TString::Format("canP_AD%d",i));
        TH1D* hcan = infile->Get<TH1D>(TString::Format("canD_AD%d",i));
        //hcan->GetXaxis()->SetTitle("Prompt Energy (MeV)");
        hcan->GetXaxis()->SetTitle("Delayed Energy (MeV)");
        hcan->SetLineColor(i);
        /*hcan->SetMarkerColor(i);
        hcan->SetLineWidth(2);
        hcan->SetMarkerStyle(20);
        hcan->SetMarkerSize(0.6);*/
        hcan->SetTitle(TString::Format("EH%d-AD%d",EH,AD));
        hcan->Draw("hist same");
    }
    c->BuildLegend();


    TCanvas* c2 = new TCanvas("c2","c2",1760,1060);
    gPad->SetLogy(1);
    for(int i = 1; i<9; i++){
        int EH = ((i-1)/2+1)*2/3 + 1; 
        int AD = i - 2*(EH-1);
        TH1D* hcan = infile->Get<TH1D>(TString::Format("canP_AD%d",i));
        hcan->GetXaxis()->SetTitle("Prompt Energy (MeV)");
        hcan->SetLineColor(i);
        hcan->SetTitle(TString::Format("EH%d-AD%d",EH,AD));
        hcan->Draw("hist same");
    }
    c2->BuildLegend();





    //hcan->GetXaxis()->SetTitleSize(0.06);
    //hcan->GetXaxis()->SetTitleOffset(0.75);
    //hcan->GetYaxis()->SetTitleSize(0.06);
    //hcan->GetYaxis()->SetTitleOffset(0.75);


    //TCanvas* c = new TCanvas("c1","c1",1920,1080);
    //p->cd();
    //TCanvas* c = new TCanvas("c1","c1",1000,900);
    //TFrame* fr = new TFrame(0,0,1920,1080);
    //fr->Draw();
    //gPad->SetLogy(1);
    //c->UseCurrentStyle();
    //TFrame* t = new TFrame(.3,.3,.9,.9);
    //t->Draw();
    //hcan->GetXaxis()->SetTitle("Delayed Energy (MeV)");
    //hcan->Draw("hist");
    //cout << gPad->GetX1() << " , " << gPad->GetX2() << endl;
    //cout << gPad->GetY1() << " , " << gPad->GetY2() << endl;
    //c->SaveAs("./123.png");

}
