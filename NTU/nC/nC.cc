void nC(){


    // draw the candidates and corresponding accidentals

    gROOT->ForceStyle();
    gStyle->SetLegendTextSize(0.06);
    gStyle->SetOptStat(0);
    TFile* infile = new TFile("./rootfile/nC_spectrum.root");
    TCanvas* c = new TCanvas("c1","c1",1760,1060);
    TH1D* hcan = infile->Get<TH1D>("nC_EH2AD1");
    hcan->GetXaxis()->SetTitle("Delayed Energy (MeV)");
    hcan->GetYaxis()->SetTitle("Entries/0.1MeV");
    hcan->GetXaxis()->CenterTitle(kTRUE);
    hcan->GetYaxis()->CenterTitle(kTRUE);
    hcan->SetLineColor(4);
    hcan->SetLineWidth(5);
    hcan->SetMarkerColor(2);
    hcan->SetMarkerStyle(20);
    hcan->SetMarkerSize(2.);
    hcan->Draw();





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
