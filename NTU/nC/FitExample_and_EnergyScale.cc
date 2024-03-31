#include "./include/EdSpectrum.h"
#include "./include/Uncertainty.h"


using namespace std;
deque<Uncertainty> v_H;
deque<Uncertainty> v_C;
deque<Uncertainty> v_Gd155;
deque<Uncertainty> v_Gd157;
deque<Uncertainty> v_Gd;
TH1D* h_H = new TH1D("mu_H","#mu_{nH};;Relative difference (%)",9,0,9);
TH1D* h_C = new TH1D("mu_C","#mu_{nC};;Relative difference (%)",9,0,9);
TH1D* h_Gd155 = new TH1D("mu_Gd155","#mu_{nGd155};;Relative difference (%)",9,0,9);
TH1D* h_Gd157 = new TH1D("mu_Gd157","#mu_{nGd157};;Relative difference (%)",9,0,9);
TH1D* h_Gd = new TH1D("mu_Gd","#mu_{nGd};;Relative difference (%)",9,0,9);
//map<int, pair<TH1D*,deque<Uncertainty>&>> m = {{1, {h_H,v_H}}, {2, {h_C,v_C}}, {3, {h_Gd155,v_Gd155}}, {4, {h_Gd157,v_Gd157}}, {5, {h_Gd,v_Gd}}};
map<int, pair<TH1D*,deque<Uncertainty>&>> m = {{1, {h_H,v_H}}, {2, {h_C,v_C}}, {4, {h_Gd157,v_Gd157}}};
Uncertainty MC_muGd(0,0);

void FitExample_and_EnergyScale(){

    gStyle->SetOptFit(0);
    gStyle->SetOptStat(0);
    TVirtualFitter::SetMaxIterations(100000);
    TString datapath = "./rootfile/P17B_accsub_by_dis_1515cut.root";
    TString MCpath = "./rootfile/Efficiency.root";
    TCanvas* c[50];
    int CanvasNo = 0;
    TGraphErrors* gmu157 = new TGraphErrors();
    gmu157->SetTitle("peak position Gd157;AD;#mu_{Gd157}(MeV)");
    TGraphErrors* gmu155 = new TGraphErrors();
    gmu155->SetTitle("peak position Gd155;AD;#mu_{Gd155}(MeV)");
    TGraphErrors* gmuH = new TGraphErrors();
    gmuH->SetTitle("peak position H;AD;#mu_{H}(MeV)");
    TGraphErrors* gmuC = new TGraphErrors();
    gmuC->SetTitle("peak position C;AD;#mu_{C}(MeV)");
    TGraphErrors* gsigma157 = new TGraphErrors();
    gsigma157->SetTitle("sigma Gd157;AD;#sigma_{Gd157}(MeV)");
    TGraphErrors* gsigma155 = new TGraphErrors();
    gsigma155->SetTitle("sigma Gd155;AD;#sigma_{Gd155}(MeV)");
    TGraphErrors* gsigmaH = new TGraphErrors();
    gsigmaH->SetTitle("sigma H;AD;#sigma_{H}(MeV)");
    TGraphErrors* gsigmaC = new TGraphErrors();
    gsigmaC->SetTitle("sigma C;AD;#sigma_{C}(MeV)");
    TGraphErrors* RHGd = new TGraphErrors();
    RHGd->SetTitle("observed nH/nGd;AD;nH/nGd");
    TGraphErrors* RHGdt = new TGraphErrors();
    RHGdt->SetTitle("true nH/nGd;AD;nH/nGd");
    TGraphErrors* RCH = new TGraphErrors();
    RCH->SetTitle("nC/nH ratio;AD;nC/nH");
    TGraphErrors* RCGd = new TGraphErrors();
    RCGd->SetTitle("nC/nGd ratio;AD;nC/nGd");
    TGraphErrors* RGdbelowH = new TGraphErrors();
    RGdbelowH->SetTitle("nGd in [1.9, 2.7]/obeserved nH;AD;nGd_{[1.9,2.7]}/obeserved nH(%)");

    TMultiGraph* mg = new TMultiGraph();
    mg->SetTitle("nH/nGd;AD;nH/nGd");


    vector<Uncertainty> below = {Uncertainty(14907), Uncertainty(2769.06), Uncertainty(3108.7), Uncertainty(2816.68), Uncertainty(2557.9),
                              Uncertainty(534.706), Uncertainty(526.057), Uncertainty(567.585), Uncertainty(495.353)};



    vector<EdSpectrum> AD;
    for(int i = 0; i<9; i++){
        if(i == 0) AD.push_back(EdSpectrum());
        else AD.push_back(EdSpectrum(datapath,i));
    }
    AD[0].Rename("MC");
    AD[0].Load(MCpath, "Ed_passEpdrdt", 0);

    /*EdSpectrum AD;
    AD.Rename("MC");
    AD.Load(MCpath, "Ed_passEpdrdt", 0);
    AD.Load(MCpath, "Ed_H_passEpdrdt", 1);
    AD.Load(MCpath, "Ed_Gd_passEpdrdt", 64);*/
    //AD.isMC = true;
    //AD.Load(datapath, "subD_AD1", 0);


    TF1* fGd = new TF1("fGd",nGdFitter3,0,12,10);
    fGd->SetLineStyle(5);
    fGd->SetLineColor(8);
    fGd->SetLineWidth(3);
    TF1* fH = new TF1("fH",OldDYBFunc,0,12,5);
    fH->SetLineStyle(5);
    fH->SetLineColor(6);
    fH->SetLineWidth(3);
    TF1* fC = new TF1("fC","[0]*TMath::Gaus(x,[1],[2],1)",0,12);
    fC->SetLineStyle(5);
    fC->SetLineColor(9);
    fC->SetLineWidth(3);




    for(int j = 0; j < 9; j++){
        //AD.at(j).Data->Rebin();
        c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1760,1060);
        AD.at(j).DYBFit();
        gPad->SetLogy(1);
        CanvasNo++;
        for(int i = 0; i<5; i++){
            fGd->SetParameter(i,AD.at(j).fGd->GetParameter(i));
            fGd->SetParameter(i+5,AD.at(j).fGd->GetParameter(i+5));
            fH->SetParameter(i,AD.at(j).fGd->GetParameter(i+10));
            fC->SetParameter(i,AD.at(j).fGd->GetParameter(i+15));
        }


        fGd->DrawCopy("same");
        fH->DrawCopy("same");
        fC->DrawCopy("same");


        gmu157->SetPoint(j,j,AD.at(j).fGd->GetParameter(1));
        gmu157->SetPointError(j,0,AD.at(j).fGd->GetParError(1));
        gmu155->SetPoint(j,j,AD.at(j).fGd->GetParameter(8));
        gmu155->SetPointError(j,0,AD.at(j).fGd->GetParError(8));
        gmuH->SetPoint(j,j,AD.at(j).fGd->GetParameter(11));
        gmuH->SetPointError(j,0,AD.at(j).fGd->GetParError(11));
        gmuC->SetPoint(j,j,AD.at(j).fGd->GetParameter(16));
        gmuC->SetPointError(j,0,AD.at(j).fGd->GetParError(16));


        gsigma157->SetPoint(j,j,AD.at(j).fGd->GetParameter(2));
        gsigma157->SetPointError(j,0,AD.at(j).fGd->GetParError(2));
        gsigma155->SetPoint(j,j,AD.at(j).fGd->GetParameter(9));
        gsigma155->SetPointError(j,0,AD.at(j).fGd->GetParError(9));
        gsigmaH->SetPoint(j,j,AD.at(j).fGd->GetParameter(12));
        gsigmaH->SetPointError(j,0,AD.at(j).fGd->GetParError(12));
        gsigmaC->SetPoint(j,j,AD.at(j).fGd->GetParameter(17));
        gsigmaC->SetPointError(j,0,AD.at(j).fGd->GetParError(17));


        if(j > 0){
            v_H.push_back(Uncertainty(AD.at(j).fGd->GetParameter(11), AD.at(j).fGd->GetParError(11)));
            //v_C.push_back(Uncertainty(AD.at(j).fGd->GetParameter(17), AD.at(j).fGd->GetParError(17)));
            v_C.push_back(Uncertainty(AD.at(j).fGd->GetParameter(16), AD.at(j).fGd->GetParError(16)));
            v_Gd155.push_back(Uncertainty(AD.at(j).fGd->GetParameter(8), AD.at(j).fGd->GetParError(8)));
            v_Gd157.push_back(Uncertainty(AD.at(j).fGd->GetParameter(1), AD.at(j).fGd->GetParError(1)));


            //const Uncertainty N155(AD.at(j).fGd->GetParameter(7), AD.at(j).fGd->GetParError(7));
            const Uncertainty N155(AD.at(j).fGd->GetParameter(7), 0);
            const Uncertainty mu155(AD.at(j).fGd->GetParameter(8), AD.at(j).fGd->GetParError(8));
            const Uncertainty N157(AD.at(j).fGd->GetParameter(0), AD.at(j).fGd->GetParError(0));
            const Uncertainty mu157(AD.at(j).fGd->GetParameter(1), AD.at(j).fGd->GetParError(1));
            //const Uncertainty alpha(AD.at(j).fGd->GetParameter(5), AD.at(j).fGd->GetParError(5));
            const Uncertainty alpha(AD.at(j).fGd->GetParameter(5), 0);


            const Uncertainty mu = (N155*mu155+alpha*N157*mu157)/(N155+alpha*N157);
            v_Gd.push_back(mu);
        }


        if(j == 0){
            const Uncertainty N155(AD.at(j).fGd->GetParameter(7), AD.at(j).fGd->GetParError(7));
            const Uncertainty mu155(AD.at(j).fGd->GetParameter(8), AD.at(j).fGd->GetParError(8));
            const Uncertainty N157(AD.at(j).fGd->GetParameter(0), AD.at(j).fGd->GetParError(0));
            const Uncertainty mu157(AD.at(j).fGd->GetParameter(1), AD.at(j).fGd->GetParError(1));
            const Uncertainty alpha(AD.at(j).fGd->GetParameter(5), AD.at(j).fGd->GetParError(5));


            const Uncertainty mu = (N155*mu155+alpha*N157*mu157)/(N155+alpha*N157);
            MC_muGd = mu;
        }


        /*below.at(j).SetError(sqrt(below.at(j).GetValue()));
        cout << below.at(j).GetError() << endl;


        Uncertainty nH(AD.at(j).fGd->Integral(1.9,2.7)*100, 10*sqrt(AD.at(j).fGd->Integral(1.9,2.7)));
        Uncertainty nGd(AD.at(j).fGd->Integral(6,9.2)*100, 10*sqrt(AD.at(j).fGd->Integral(6,9.2)));
        Uncertainty nC(fC->Integral(4,5.8)*100, 10*sqrt(fC->Integral(4,5.8)));
        RHGd->SetPoint(j,j,(nH/nGd).GetValue());
        RHGd->SetPointError(j,0,(nH/nGd).GetError());
        RCH->SetPoint(j,j,(nC/nH).GetValue());
        RCH->SetPointError(j,0,(nC/nH).GetError());
        RCGd->SetPoint(j,j,(nC/nGd).GetValue());
        RCGd->SetPointError(j,0,(nC/nGd).GetError());


        Uncertainty nHt = nH - below.at(j);
        RHGdt->SetPoint(j,j,(nHt/nGd).GetValue());
        RHGdt->SetPointError(j,0,(nHt/nGd).GetError());


        Uncertainty perc = below.at(j) / nH;
        RGdbelowH->SetPoint(j,j,100*perc.GetValue());
        RGdbelowH->SetPointError(j,0,100*perc.GetError());

        cout << "AD" << j << endl;
        cout << "nGd in nH range = " << below.at(j) << endl;
        cout << "observed nH = " << nH << endl;
        cout << "ratio = " << 100*perc << endl; */


    }


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gmu157->SetMarkerStyle(8);
    gmu157->GetXaxis()->SetRangeUser(-1,9);
    gmu157->Draw("ap");
    cout << gmu157->GetTitle() << " : " << gmu157->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gmu155->SetMarkerStyle(8);
    gmu155->GetXaxis()->SetRangeUser(-1,9);
    gmu155->Draw("ap");
    cout << gmu155->GetTitle() << " : " << gmu155->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gmuH->SetMarkerStyle(8);
    gmuH->GetXaxis()->SetRangeUser(-1,9);
    gmuH->Draw("ap");
    cout << gmuH->GetTitle() << " : " << gmuH->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gmuC->SetMarkerStyle(8);
    gmuC->GetXaxis()->SetRangeUser(-1,9);
    gmuC->Draw("ap");
    cout << gmuC->GetTitle() << " : " << gmuC->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gsigma157->SetMarkerStyle(8);
    gsigma157->GetXaxis()->SetRangeUser(-1,9);
    gsigma157->Draw("ap");
    cout << gsigma157->GetTitle() << " : " << gsigma157->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gsigma155->SetMarkerStyle(8);
    gsigma155->GetXaxis()->SetRangeUser(-1,9);
    gsigma155->Draw("ap");
    cout << gsigma155->GetTitle() << " : " << gsigma155->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gsigmaH->SetMarkerStyle(8);
    gsigmaH->GetXaxis()->SetRangeUser(-1,9);
    gsigmaH->Draw("ap");
    cout << gsigmaH->GetTitle() << " : " << gsigmaH->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    gsigmaC->SetMarkerStyle(8);
    gsigmaC->GetXaxis()->SetRangeUser(-1,9);
    gsigmaC->Draw("ap");
    cout << gsigmaC->GetTitle() << " : " << gsigmaC->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    RHGd->SetMarkerStyle(8);
    RHGdt->SetMarkerStyle(8);
    RHGdt->SetMarkerColor(2);
    mg->Add(RHGd);
    mg->Add(RHGdt);
    mg->GetXaxis()->SetRangeUser(-1,9);
    mg->Draw("ap");
    c[CanvasNo]->BuildLegend();
    cout << "nH/nGd" << " : " << RHGd->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    RCH->SetMarkerStyle(8);
    RCH->GetXaxis()->SetRangeUser(-1,9);
    RCH->Draw("ap");
    cout << "nC/nH" << " : " << RCH->GetMean(2) << endl;
    CanvasNo++;


    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    RCGd->SetMarkerStyle(8);
    RCGd->GetXaxis()->SetRangeUser(-1,9);
    RCGd->Draw("ap");
    cout << "nC/nGd" << " : " << RCGd->GetMean(2) << endl;
    CanvasNo++;


    /*TH1D* MCchi = new TH1D("MCChi2",";E_d(MeV);Counts",1200,0,12);
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    *(MCchi) = GetFitPerformance(AD[0].Data,AD[0].fGd,"MCChi2");
    MCchi->SetTitle(";E_{d}(MeV);#chi^{2}/bin");
    MCchi->GetXaxis()->SetRangeUser(1.5,9.4);
    MCchi->Draw();
    CanvasNo++;


    TH1D* MCdiff = new TH1D("MCDiff",";E_d(MeV);Counts",1200,0,12);
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    *(MCdiff) = GetRatioHistogram(AD[0].Data,AD[0].fGd,"MCDiff");
    MCdiff->SetTitle(";E_{d}(MeV);Diff(%)");
    MCdiff->GetXaxis()->SetRangeUser(1.5,9.4);
    MCdiff->Draw();
    CanvasNo++;*/


    TH1D* AD1chi = new TH1D("AD1_Chi2",";E_d(MeV);Counts",1200,0,12);
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    *(AD1chi) = GetFitPerformance(AD[1].Data,AD[1].fGd,"AD1_Chi2");
    AD1chi->SetTitle(";E_{d}(MeV);#chi^{2}");
    AD1chi->GetXaxis()->SetRangeUser(1.5,9.4);
    AD1chi->Draw();
    CanvasNo++;


    TH1D* AD1diff = new TH1D("AD1_Diff",";E_d(MeV);Counts",1200,0,12);
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    *(AD1diff) = GetRatioHistogram(AD[1].Data,AD[1].fGd,"AD1_Diff");
    AD1diff->SetTitle(";E_{d}(MeV);Diff(%)");
    AD1diff->GetXaxis()->SetRangeUser(1.5,9.4);
    AD1diff->Draw();
    CanvasNo++;



    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1600,900);
    RGdbelowH->SetMarkerStyle(8);
    RGdbelowH->GetXaxis()->SetRangeUser(-1,9);
    RGdbelowH->Draw("ap");
    CanvasNo++;


    vector<string> name = {"","mu_H","mu_C","mu_Gd155","mu_Gd157","mu_Gd"};
    map<int,int> par_position = {{1,11}, {2,16}, {3,8}, {4,1}};
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1920,1080);
    c[CanvasNo]->Divide(1,3,0,0);
    int counts = 1;
    for(auto& i : m){

        cout << "size = " << i.second.second.size() << endl;
        const Uncertainty mean = VarWeightedMean(i.second.second);     // This mean should not include MC value!
        const Uncertainty sd = VarWeightedStdDev(i.second.second);     // This sd should not include MC value!
        auto it = par_position.find(i.first);
        if(it == par_position.end()) i.second.second.push_front(MC_muGd); 
        else i.second.second.push_front(Uncertainty(AD.at(0).fGd->GetParameter(par_position.at(i.first)), AD.at(0).fGd->GetParError(par_position.at(i.first))));
        for(int ADNo = 0; ADNo < 9; ADNo++){

            const int EH = ((ADNo-1)/2+1)*2/3 + 1;
            const int AD = ADNo - 2*(EH-1);
            const Uncertainty temp = (i.second.second.at(ADNo)-mean)/mean*100.;
            i.second.first->SetBinContent(ADNo+1,temp.GetValue());
            i.second.first->SetBinError(ADNo+1,temp.GetError());
            if(ADNo == 0) i.second.first->GetXaxis()->SetBinLabel(ADNo+1,"MC");
            //else if(ADNo == 5) i.second.first->GetXaxis()->SetBinLabel(ADNo+1,"EH3");
            else i.second.first->GetXaxis()->SetBinLabel(ADNo+1,TString::Format("EH%dAD%d",EH,AD));

        }


        //i.second.first->SetMarkerStyle(20);
        i.second.first->SetMarkerStyle(20);
        i.second.first->SetMarkerSize(2.8);
        i.second.first->SetMarkerColor(2);
        i.second.first->SetLineColor(4);
        i.second.first->GetYaxis()->CenterTitle(kTRUE);
        i.second.first->GetXaxis()->SetLabelSize(0.16);
        i.second.first->GetXaxis()->SetLabelFont(62);
        i.second.first->GetYaxis()->SetTitle("");
        //c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1920,1080);
        c[CanvasNo]->cd(counts);
        i.second.first->Draw("e");
        //CanvasNo++;
        cout << name.at(i.first) << " : Mean = " << mean << " , Std dev = " << sd << endl;
        counts++;

    }



}
