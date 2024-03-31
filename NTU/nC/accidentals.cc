void accidentals(){


    // draw the candidates and corresponding accidentals

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    TFile* infile = new TFile("./rootfile/P17B_accsub_final_800iso_40.root");
    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    gPad->SetLogy(1);
    TH1D* hcan = infile->Get<TH1D>("canD_AD1");
    TH1D* hacc = infile->Get<TH1D>("accD_AD1");
    hcan->GetXaxis()->SetTitle("Delayed Energy (MeV)");
    hcan->SetLineColor(4);
    hcan->SetLineWidth(2);
    hcan->SetMarkerColor(4);
    hcan->SetMarkerStyle(20);
    hcan->SetMarkerSize(0.7);
    hcan->SetTitle("Candidates");
    hacc->SetLineColor(2);
    hacc->SetLineWidth(2);
    hacc->SetMarkerColor(2);
    hacc->SetMarkerStyle(21);
    hacc->SetMarkerSize(0.6);
    hacc->SetTitle("Accidentals");
    hcan->Draw();
    hacc->Draw("same");
    c->BuildLegend();





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
