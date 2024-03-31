#include "./include/Uncertainty.h"

void dr_syst(){

    gROOT->ForceStyle();
    gStyle->SetOptStat(0);
    gStyle->SetLegendTextSize(0.06);
    Uncertainty sum_wd(0,0);
    double sum_w = 0;
    const double SF = 0.99951956;
    TH1D* hall = new TH1D("all","all",500,0,5000);
    TH1D* hRD = new TH1D("hRD","#epsilon_{dr};;Relative difference (%)",9,0,9);
    TGraphErrors* gall = new TGraphErrors();
    TMultiGraph* mg = new TMultiGraph();
    TMultiGraph* mg2 = new TMultiGraph();
    vector<TGraphErrors*> g;
    vector<TGraphErrors*> g2;   // relative diff.
    deque<Uncertainty> eff500;
    Uncertainty effMean(0,0);
    for(int i = 0; i < 9; i++){
        g.push_back(new TGraphErrors());
        g2.push_back(new TGraphErrors());
    }
    //eff500.push_back(Double_e(0,0));
    TFile* f = new TFile("./rootfile/P17B_accsub_Dist_perrun_800iso.root");
    TFile* fmc = new TFile("./rootfile/Efficiency_noMOcap_sq_unified.root");


    for(int i = 1; i < 9; i++){
        TH1D* htemp = f->Get<TH1D>(TString::Format("h1dDpd_sub_AD%d",i));
        hall->Add(htemp);
    }


    for(int j = 1; j < 201; j++){

        Uncertainty numer(0,0);
        Uncertainty denom(0,0);
        double temp_err;


        numer.SetValue(hall->IntegralAndError(1,j,temp_err));
        numer.SetError(temp_err);
        denom.SetValue(hall->IntegralAndError(1,200,temp_err));
        denom.SetError(temp_err);


        const Uncertainty eff = numer/denom;
        gall->SetPoint(j-1,j*10,eff.GetValue());
        gall->SetPointError(j-1,0,eff.GetError());


        if(j == 50){
            effMean = eff;
            cout << "Mean dr eff = " << effMean << endl;
        }

    }



    for(int i = 0; i < 9; i++){
        
        const int EH = ((i-1)/2+1)*2/3 + 1;
        const int AD = i - 2*(EH-1);
        g.at(i)->SetMarkerStyle(8);
        g.at(i)->SetMarkerColor(i+1);
        g.at(i)->SetLineColor(i+1);
        g2.at(i)->SetMarkerStyle(8);
        g2.at(i)->SetMarkerColor(i+1);
        g2.at(i)->SetLineColor(i+1);
        TH1D* htemp;
        if(i == 0){
            g.at(i)->SetNameTitle("MC","MC");
            g2.at(i)->SetNameTitle("MC_diff","MC");
            htemp = fmc->Get<TH1D>("dr_passEpdtEd");
        }
        else{
            g.at(i)->SetNameTitle(TString::Format("EH%dAD%d",EH,AD), TString::Format("EH%dAD%d",EH,AD));
            g2.at(i)->SetNameTitle(TString::Format("EH%dAD%d_diff",EH,AD), TString::Format("EH%dAD%d",EH,AD));
            htemp = f->Get<TH1D>(TString::Format("h1dDpd_sub_AD%d",i));
        }


        for(int j = 1; j < 201; j++){

            Uncertainty numer(0,0);
            Uncertainty denom(0,0);
            double temp_err;


            numer.SetValue(htemp->IntegralAndError(1,j,temp_err));
            numer.SetError(temp_err);
            denom.SetValue(htemp->IntegralAndError(1,200,temp_err));
            denom.SetError(temp_err);


            const Uncertainty eff = numer/denom*SF;
            g.at(i)->SetPoint(j-1,j*10,eff.GetValue());
            g.at(i)->SetPointError(j-1,0,eff.GetError());


            // relative diff. part
            numer.SetValue(hall->IntegralAndError(1,j,temp_err));
            numer.SetError(temp_err);
            denom.SetValue(hall->IntegralAndError(1,200,temp_err));
            denom.SetError(temp_err);
            const Uncertainty effall = numer/denom*SF;
            const Uncertainty RD = (eff-effall)/(eff+effall)*100.;
            g2.at(i)->SetPoint(j-1,j*10,RD.GetValue());
            //g2.at(i)->SetPointError(j-1,0,RD.GetError());


            if(j == 50){
                eff500.push_back(eff);
                cout << "AD" << i << " dr eff = " << eff << endl;
                cout << "AD" << i << " dr RD = " << RD << endl;


                hRD->SetBinContent(i+1, RD.GetValue());
                hRD->SetBinError(i+1, RD.GetError());
                if(i == 0) hRD->GetXaxis()->SetBinLabel(i+1,"MC");
                else{
                    hRD->GetXaxis()->SetBinLabel(i+1,TString::Format("EH%dAD%d",EH,AD));
                    double w = 1/eff.GetError()/eff.GetError();
                    sum_wd += (w*(eff-effall)*(eff-effall));
                    sum_w += w;
                }
            }

        }

        mg->Add(g.at(i));
        mg2->Add(g2.at(i));



    }


    const Uncertainty Uncorr = sqrt(sum_wd/(sum_w*7./8.)); 
    cout << "Uncorrlated systematic uncertainty = " << Uncorr*100. << endl;
    hRD->SetMarkerStyle(20);
    hRD->SetMarkerSize(3);
    hRD->SetLineColor(4);
    hRD->GetYaxis()->CenterTitle(kTRUE);
    hRD->GetXaxis()->SetLabelSize(0.06);
    hRD->GetXaxis()->SetLabelFont(62);
    TCanvas* c1 = new TCanvas("c1","c1",1760,1060);
    hRD->Draw("e");
    TCanvas* c2 = new TCanvas("c2","c2",1760,1060);
    mg->SetTitle(";dr(mm);#epsilon_{dr}");
    mg->GetXaxis()->CenterTitle(kTRUE);
    mg->GetYaxis()->CenterTitle(kTRUE);
    mg->Draw("ap");
    c2->BuildLegend();
    TCanvas* c3 = new TCanvas("c3","c3",1760,1060);
    mg2->SetTitle(";dr(mm);Relative difference (%)");
    mg2->GetXaxis()->CenterTitle(kTRUE);
    mg2->GetYaxis()->CenterTitle(kTRUE);
    mg2->Draw("ap");
    c3->BuildLegend();




}

