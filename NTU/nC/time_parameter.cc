#include "./include/TimeInterval.h"
#include "./include/Uncertainty.h"



TCanvas* c[20];
int CanvasNo = 1;
bool FittingNearandFarSiteAvg(vector<TH1D*>& h, TF1** func);
bool SetComponents(const TF1* ftotal, map<string,TF1*>& fmap);
bool GetEventbyIntegral(const double total, const map<string,TF1*>& input, map<string,Uncertainty>& output);
double binwidth = 0;
map<string,TF1*> component;     //string = "GdLS", "LS", "Const"
map<string,Uncertainty> events; 
array<double,11> acc = {0, 101.693, 112.002, 108.232, 90.8603, 123.393, 106.526, 107.338, 86.4925, 0, 0};


deque<Uncertainty> v_GdLS;
deque<Uncertainty> v_LS;
deque<Uncertainty> v_ther;
deque<Uncertainty> v_alpha;
TH1D* h_GdLS = new TH1D("tau_GdLS","#tau_{GdLS};;Relative difference (%)",9,0,9);
TH1D* h_LS = new TH1D("tau_LS","#tau_{LS};;Relative difference (%)",9,0,9);
TH1D* h_ther = new TH1D("tau_ther","#tau_{ther};;Relative difference (%)",9,0,9);
TH1D* h_alpha = new TH1D("alpha","#alpha;;Relative difference (%)",9,0,9);
//map<int, pair<TH1D*,deque<Uncertainty>&>> m = {{1, {h_GdLS,v_GdLS}}, {2, {h_LS,v_LS}}, {3, {h_ther,v_ther}}, {4, {h_alpha,v_alpha}}};
map<int, pair<TH1D*,deque<Uncertainty>&>> m = {{1, {h_GdLS,v_GdLS}}, {2, {h_LS,v_LS}}, {3, {h_ther,v_ther}}};




void time_parameter(){
    
    gROOT->ForceStyle();
    gStyle->SetOptFit(1);
    gStyle->SetOptStat(0);
    gStyle->SetOptStat(0);
    bool fix = false;       //fix tau_Gd and tau_thr of AD1-8 in near and far site fitting value, respectively.


    TFile* f[4];
    vector<TH1D*> h;
    TF1* func[11];
    TGraphErrors* TimePlot[3] = {new TGraphErrors(), new TGraphErrors(), new TGraphErrors()};
    f[0] = new TFile("./rootfile/dT1000.root");
    h.push_back((TH1D*)f[0]->Get("dt_plot"));
    

    for(int EHNo = 1; EHNo<4; EHNo++){
        f[EHNo] = new TFile(TString::Format("./rootfile/P17B_EH%d_dT1000Ep4.root",EHNo));
        //cout << TString::Format("/Users/chengyuchin/Analysis/data/P17B_Ed4058cut/P17B_EH%d_Ed4058cut.root",EHNo) << endl;
        cout << EHNo << "OK" << endl;
        if(EHNo == 3){
            for(int ADNo = 1; ADNo<5; ADNo++){
                h.push_back((TH1D*)f[EHNo]->Get(TString::Format("h1dCapT_IBD_AD%d",ADNo)));
            }
        }

        else{
            for(int ADNo = 1; ADNo<3; ADNo++){
                h.push_back((TH1D*)f[EHNo]->Get(TString::Format("h1dCapT_IBD_AD%d",ADNo)));
            }
        }

    }


    component.insert(pair<string,TF1*>("GdLS",new TF1("GdLS",GdLSComponent,0,1000,4)));
    component.insert(pair<string,TF1*>("LS",new TF1("LS",LSComponent,0,1000,2)));
    component.insert(pair<string,TF1*>("Const",new TF1("Const","pol0",0,1000)));
    for(auto i : component) events.insert(pair<string,Uncertainty>(i.first,Uncertainty()));


    for(auto i : component){
        i.second->SetNpx(10000);
        i.second->SetLineStyle(5);
        i.second->SetLineWidth(3);
    }
    component.at("GdLS")->SetLineColor(6);
    component.at("LS")->SetLineColor(8);
    component.at("Const")->SetLineColor(1);


    /*if(!FittingNearandFarSiteAvg(h, &(func[9]))){
        cout << "Error!" << endl;
        return 0;
    }*/
    for(int ADNo = 0; ADNo<9; ADNo++){
        if(ADNo<9){
            const int EH = ((ADNo-1)/2+1)*2/3 + 1;
            const int AD = ADNo - 2*(EH-1);
            h[ADNo]->Rebin(5);
            if(ADNo > 0) h[ADNo]->SetNameTitle(TString::Format("CapT_AD%d",ADNo),TString::Format("CapT_EH%dAD%d;dt(#mus);Entries/1#mus",EH,AD));
            else h[ADNo]->SetNameTitle("CapT_MC","CapT_MC;dt(#mus);Entries/1#mus");
            h[ADNo]->GetXaxis()->CenterTitle(kTRUE);
            h[ADNo]->GetYaxis()->CenterTitle(kTRUE);
            h[ADNo]->GetYaxis()->SetTitleSize(0.05);
            h[ADNo]->GetYaxis()->SetTitleOffset(0.8);
            c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1760,1060);
            gPad->SetLogy(1);
            func[ADNo] = new TF1(TString::Format("f%d",ADNo),TimeIntervalFunc,0,1000,7);
            func[ADNo]->SetParNames("N_{LS}","N_{GdLS}","#alpha","#tau_{LS}","#tau_{GdLS}","#tau_{ther}","const");
            if(ADNo <= 4) func[ADNo]->SetParameters(20000,20000,0.2,220,25,3,1000);
            else func[ADNo]->SetParameters(2000,2000,0.2,220,25,3,1000);
            func[ADNo]->SetParLimits(2,0,1);
            //func[ADNo]->FixParameter(3,201.4);
            //func[ADNo]->FixParameter(4,29.55);
            //func[ADNo]->FixParameter(5,3.8);
            func[ADNo]->SetParLimits(3,150,350);
            func[ADNo]->SetParLimits(4,20,35);
            func[ADNo]->SetParLimits(5,1.5,9);
            //func[ADNo]->SetParLimits(6,0,10000000);
            //func[ADNo]->FixParameter(6, acc.at(ADNo)/200);
            func[ADNo]->SetNpx(10000);
            if(ADNo == 0) func[ADNo]->FixParameter(6,0);
            if(fix){
                if(ADNo <= 4){
                func[ADNo]->FixParameter(4,func[9]->GetParameter(4));
                func[ADNo]->FixParameter(5,func[9]->GetParameter(5));
                }
                else{
                func[ADNo]->FixParameter(4,func[10]->GetParameter(4));
                func[ADNo]->FixParameter(5,func[10]->GetParameter(5));
                }
            }
            cout << "Fitting AD" << ADNo << "..." << std::endl;
            h[ADNo]->Fit(func[ADNo],"","",1,1000);
            h[ADNo]->Fit(func[ADNo],"","",1,1000);
        }
        //c[ADNo]->SaveAs(TString::Format("/Users/chengyuchin/Analysis/Plots/P17B/dtfit_AD%d.png",ADNo));
        TimePlot[0]->SetPoint(ADNo, ADNo, func[ADNo]->GetParameter(3)); 
        TimePlot[0]->SetPointError(ADNo, 0, func[ADNo]->GetParError(3)); 
        TimePlot[1]->SetPoint(ADNo, ADNo, func[ADNo]->GetParameter(4)); 
        TimePlot[1]->SetPointError(ADNo, 0, func[ADNo]->GetParError(4)); 
        TimePlot[2]->SetPoint(ADNo, ADNo, func[ADNo]->GetParameter(5)); 
        TimePlot[2]->SetPointError(ADNo, 0, func[ADNo]->GetParError(5));
        if(ADNo > 0){
            /*
            TimePlot[0]->SetPoint(ADNo-1, ADNo, func[ADNo]->GetParameter(3)); 
            TimePlot[0]->SetPointError(ADNo-1, 0, func[ADNo]->GetParError(3)); 
            TimePlot[1]->SetPoint(ADNo-1, ADNo, func[ADNo]->GetParameter(4)); 
            TimePlot[1]->SetPointError(ADNo-1, 0, func[ADNo]->GetParError(4)); 
            TimePlot[2]->SetPoint(ADNo-1, ADNo, func[ADNo]->GetParameter(5)); 
            TimePlot[2]->SetPointError(ADNo-1, 0, func[ADNo]->GetParError(5)); */


            v_GdLS.push_back(Uncertainty(func[ADNo]->GetParameter(4), func[ADNo]->GetParError(4)));
            v_LS.push_back(Uncertainty(func[ADNo]->GetParameter(3), func[ADNo]->GetParError(3)));
            v_ther.push_back(Uncertainty(func[ADNo]->GetParameter(5), func[ADNo]->GetParError(5)));
            v_alpha.push_back(Uncertainty(func[ADNo]->GetParameter(2), func[ADNo]->GetParError(2)));
        }

        SetComponents(func[ADNo],component);
        for(auto i : component) i.second->DrawCopy("same");
        CanvasNo++;
        

    }

    //FittingNearandFarSiteAvg(h, &(func[9]));


    /*for(int ADNo = 0; ADNo<1; ADNo++){

        cout << "AD = " << ADNo << " , tau_H = " << func[ADNo]->GetParameter(3) << " , error_H = " << func[ADNo]->GetParError(3) << endl;
        cout << "AD = " << ADNo << " , tau_Gd = " << func[ADNo]->GetParameter(4) << " , error_Gd = " << func[ADNo]->GetParError(4) << endl;
        cout << "AD = " << ADNo << " , tau_thr = " << func[ADNo]->GetParameter(5) << " , error_thr = " << func[ADNo]->GetParError(5) << endl;


        SetComponents(func[ADNo],component);
        GetEventbyIntegral(h[ADNo]->Integral(2,400),component,events);
        cout << "----------- AD" << ADNo << " ------------" << endl;
        cout << "total events = " << h[ADNo]->Integral(2,400) << endl;
        cout << "IBD events in GdLS = " << events.at("GdLS") << endl;
        cout << "IBD events in LS = " << events.at("LS") << endl;
        cout << "acc events = " << events.at("Const") << endl;
    
    }*/


    TString titles[3] = {"#tau_{LS}","#tau_{GdLS}","#tau_{ther}"};
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1760,1060);
    c[CanvasNo]->Divide(1,3,0,0);
    for(int i = 0; i<3; i++){
        TimePlot[i]->SetTitle(titles[i]+";AD;"+titles[i]+"(#mus)");
        for(int ADNo = 0; ADNo < 9; ADNo++){

            const int EH = ((ADNo-1)/2+1)*2/3 + 1;
            const int AD = ADNo - 2*(EH-1);
            if(ADNo == 0) TimePlot[i]->GetXaxis()->SetBinLabel(ADNo,"MC");
            else TimePlot[i]->GetXaxis()->SetBinLabel(ADNo,TString::Format("EH%dAD%d",EH,AD));

        }
        /*TimePlot[i]->GetXaxis()->SetTitle("");
        TimePlot[i]->GetYaxis()->SetTitle(titles[i]+"(#mus)");
        TimePlot[i]->SetMarkerStyle(20);
        TimePlot[i]->SetMarkerSize(2.8);
        TimePlot[i]->SetMarkerColor(2);
        TimePlot[i]->SetLineColor(4);
        TimePlot[i]->GetYaxis()->CenterTitle(kTRUE);
        TimePlot[i]->GetXaxis()->SetLabelSize(0.16);
        TimePlot[i]->GetXaxis()->SetLabelFont(62);*/
        c[CanvasNo]->cd(i+1);
        TimePlot[i]->Draw("ap");
    }
    CanvasNo++;


    /*for(int i = 0; i<3;i++){
        cout << TimePlot[i]->GetMean(2) << endl;
    }*/


    vector<string> name = {"","tau_{GdLS}","tau_{LS}","tau_{0}","alpha"};
    map<int,int> par_position = {{1,4}, {2,3}, {3,5}, {4,2}};
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1920,1080);
    c[CanvasNo]->Divide(1,3,0,0);
    int counts = 1;
    for(auto& i : m){

        cout << "size = " << i.second.second.size() << endl;
        const Uncertainty mean = VarWeightedMean(i.second.second);     // This mean should not include MC value!
        const Uncertainty sd = VarWeightedStdDev(i.second.second);     // This sd should not include MC value!
        i.second.second.push_front(Uncertainty(func[0]->GetParameter(par_position.at(i.first)), func[0]->GetParError(par_position.at(i.first))));
        for(int ADNo = 0; ADNo < 9; ADNo++){

            const int EH = ((ADNo-1)/2+1)*2/3 + 1;
            const int AD = ADNo - 2*(EH-1);
            const Uncertainty temp = (i.second.second.at(ADNo)-mean)/mean*100.;
            //i.second.first->SetBinContent(ADNo+1,temp.GetValue());
            //i.second.first->SetBinError(ADNo+1,temp.GetError());
            i.second.first->SetBinContent(ADNo+1,i.second.second.at(ADNo).GetValue());
            i.second.first->SetBinError(ADNo+1,i.second.second.at(ADNo).GetError());
            if(ADNo == 0) i.second.first->GetXaxis()->SetBinLabel(ADNo+1,"MC");
            //else if(ADNo == 5) i.second.first->GetXaxis()->SetBinLabel(ADNo+1,"EH3");
            else i.second.first->GetXaxis()->SetBinLabel(ADNo+1,TString::Format("EH%dAD%d",EH,AD));

        }


        i.second.first->SetMarkerStyle(20);
        i.second.first->SetMarkerSize(2.8);
        i.second.first->SetMarkerColor(2);
        i.second.first->SetLineColor(4);
        i.second.first->GetYaxis()->CenterTitle(kTRUE);
        i.second.first->GetXaxis()->SetLabelSize(0.16);
        i.second.first->GetXaxis()->SetLabelFont(62);
        i.second.first->GetYaxis()->SetTitle(("#"+name.at(counts)+"(#mus)").c_str());
        c[CanvasNo]->cd(counts);
        i.second.first->Draw("e");
        //CanvasNo++;
        cout << name.at(i.first) << " : Mean = " << mean << " , Std dev = " << sd << endl;
        cout << "far = " << i.second.second.at(5) << endl;
        counts++;

    }


}






bool FittingNearandFarSiteAvg(vector<TH1D*>& h, TF1** func){

    //Near Site
    /*binwidth = (h[1]->GetXaxis()->GetXmax()-h[1]->GetXaxis()->GetXmin())/h[1]->GetNbinsX(); 
    h[9] = new TH1D("CapT_Near","CapT_Near;dt(#mus);Counts",h[1]->GetNbinsX(),h[1]->GetXaxis()->GetXmin(),h[1]->GetXaxis()->GetXmax());
    for(int ADNo = 1; ADNo<5; ADNo++){
        h[9]->Add(h[ADNo],1.0);
        acc.at(9) += acc.at(ADNo);
    }
    h[9]->Rebin(2);
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1920,1080);
    func[0] = new TF1("f9",TimeIntervalFunc,h[9]->GetXaxis()->GetXmin(),h[9]->GetXaxis()->GetXmax(),7);
    func[0]->SetParNames("N_{LS}","N_{GdLS}","#alpha","#tau_{LS}","#tau_{GdLS}","#tau_{ther}","const");
    func[0]->SetParameters(40000,40000,0.3,220,25,2,10);
    func[0]->SetParLimits(2,0,1);
    func[0]->SetParLimits(3,100,600);
    //func[0]->FixParameter(3,200);
    func[0]->SetParLimits(4,20,35);
    func[0]->SetParLimits(5,2,11);
    func[0]->FixParameter(4,29.1453);
    func[0]->FixParameter(5,4.31875);
    //func[0]->SetParLimits(6,0,10000000);
    func[0]->FixParameter(6, acc.at(9)/200);
    func[0]->SetNpx(10000);
    cout << "Fitting Near Site..." << std::endl;
    h[9]->Fit(func[0],"","",h[9]->GetXaxis()->GetXmin(),h[9]->GetXaxis()->GetXmax());
    h[9]->Fit(func[0],"","",h[9]->GetXaxis()->GetXmin(),h[9]->GetXaxis()->GetXmax());
    SetComponents(func[0],component);
    GetEventbyIntegral(h[9]->Integral(1,400),component,events);
    cout << "-----------Near Site------------" << endl;
    cout << "total events = " << h[9]->Integral(1,400) << endl;
    cout << "IBD events in GdLS = " << events.at("GdLS") << endl;
    cout << "IBD events in LS = " << events.at("LS") << endl;
    cout << "acc events = " << events.at("Const") << endl;
    CanvasNo++;*/


    //Far Site
    h[10] = new TH1D("CapT_Far","CapT_Far",h[5]->GetNbinsX(),h[5]->GetXaxis()->GetXmin(),h[5]->GetXaxis()->GetXmax());
    for(int ADNo = 5; ADNo<9; ADNo++) h[10]->Add(h[ADNo],1.0);
    h[10]->Rebin(5);
    c[CanvasNo] = new TCanvas(TString::Format("c%d",CanvasNo),TString::Format("c%d",CanvasNo),1920,1080);
    func[0] = new TF1("f10",TimeIntervalFunc,0,1000,7);
    func[0]->SetParNames("N_{LS}","N_{GdLS}","#alpha","#tau_{LS}","#tau_{GdLS}","#tau_{ther}","const");
    func[0]->SetParameters(0.2,0.2,0.3,210,25,3.5,1000);
    func[0]->SetParLimits(2,0,1);
    func[0]->SetParLimits(3,150,300);
    func[0]->SetParLimits(4,20,35);
    func[0]->SetParLimits(5,2,8);
    func[0]->SetNpx(10000);
    cout << "Fitting Far Site..." << std::endl;
    h[10]->Fit(func[0],"","",1,1000);
    h[10]->Fit(func[0],"","",1,1000);
    SetComponents(func[0],component);
    for(auto i : component) i.second->DrawCopy("same");
    CanvasNo++;
    v_GdLS.push_back(Uncertainty(func[0]->GetParameter(4), func[0]->GetParError(4)));
    v_LS.push_back(Uncertainty(func[0]->GetParameter(3), func[0]->GetParError(3)));
    v_ther.push_back(Uncertainty(func[0]->GetParameter(5), func[0]->GetParError(5)));
    v_alpha.push_back(Uncertainty(func[0]->GetParameter(2), func[0]->GetParError(2)));


    return true;



}





bool SetComponents(const TF1* ftotal, map<string,TF1*>& fmap){

    if(not ftotal) return false;
    for(auto i : fmap){
        bool check = i.second;
        if(not check) return false;
    }


    double NH = ftotal->GetParameter(0);
    double NGd = ftotal->GetParameter(1);
    double alpha = ftotal->GetParameter(2);
    double tau_H = ftotal->GetParameter(3);
    double tau_Gd = ftotal->GetParameter(4);
    double tau_thr = ftotal->GetParameter(5);
    double constant = ftotal->GetParameter(6);


    fmap.at("GdLS")->SetParameters(NGd,alpha,tau_Gd,tau_thr);
    fmap.at("LS")->SetParameters(NH,tau_H);
    fmap.at("Const")->SetParameter(0,constant/999);


    return true;



}





bool GetEventbyIntegral(const double total, const map<string,TF1*>& input, map<string,Uncertainty>& output){

    if(not total) return false;
    for(auto i : input){
        bool check = i.second;
        if(not check) return false;
    }


    double GdLS, LS, Const, Integral;
    GdLS = input.at("GdLS")->Integral(1,400);
    LS = input.at("LS")->Integral(1,400);
    Const = input.at("Const")->Integral(1,400);
    Integral = GdLS + LS + Const;


    output.at("GdLS").SetBoth(total*GdLS/Integral, sqrt(total*GdLS/Integral));
    output.at("LS").SetBoth(total*LS/Integral, sqrt(total*LS/Integral));
    output.at("Const").SetBoth(total*Const/Integral, sqrt(total*Const/Integral));


    return true;



}




