#include "TChain.h"
#include "TFile.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"

#include <TMath.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

TFile* result = 0;

Int_t nPhotons = 0;
Int_t totalPE = 0; 

Int_t pmtID[20000] = {0};
Int_t nPE[20000] = {0};
Double_t hitTime[20000] = {0};

Int_t nInitParticles = 0;
Float_t InitX[20000] = {0};
Float_t InitY[20000] = {0};
Float_t InitZ[20000] = {0};
Float_t NCStartX[8000], NCStartY[8000], NCStartZ[8000], NCStopX[8000], NCStopY[8000], NCStopZ[8000];
Float_t edepX, edepY, edepZ;

Int_t NeutronN = 0;
Double_t NeutronCaptureT[10000] = {0};

Int_t nent = 0;
TChain ch1("evt");
TChain ch2("geninfo");
TChain ch3("nCapture");

void createfile(char* filename) {
    if (!result) {
        result = new TFile(filename, "recreate");
    }
}
    
void setaddress(char* listname){
    ifstream fin(listname);
    TString fn;
    
    for(int i=0;fin>>fn;i++){
        ch1.Add(fn);
        ch2.Add(fn);
        ch3.Add(fn);
    }

    nent = ch1.GetEntries();
    cout<<"Nent = "<<nent<<endl;

    ch1.SetBranchAddress("nPhotons", &nPhotons);
    ch1.SetBranchAddress("totalPE", &totalPE);
    ch1.SetBranchAddress("pmtID", pmtID);
    ch1.SetBranchAddress("nPE", nPE);
    ch1.SetBranchAddress("hitTime", hitTime);
    ch1.SetBranchAddress("edepX", &edepX);
    ch1.SetBranchAddress("edepY", &edepY);
    ch1.SetBranchAddress("edepZ", &edepZ);

    ch2.SetBranchAddress("nInitParticles", &nInitParticles);
    ch2.SetBranchAddress("InitX", InitX);
    ch2.SetBranchAddress("InitY", InitY);
    ch2.SetBranchAddress("InitZ", InitZ);

    ch3.SetBranchAddress("NeutronN", &NeutronN);
    ch3.SetBranchAddress("NeutronCaptureT", NeutronCaptureT);
    ch3.SetBranchAddress("NCStartX", NCStartX);
    ch3.SetBranchAddress("NCStartY", NCStartY);
    ch3.SetBranchAddress("NCStartZ", NCStartZ);
    ch3.SetBranchAddress("NCStopX", NCStopX);
    ch3.SetBranchAddress("NCStopY", NCStopY);
    ch3.SetBranchAddress("NCStopZ", NCStopZ);

}

// Max PE per PMT (at different R) 

void draw_MaxPEperPMT(TString type){
    
    TCanvas *c = new TCanvas;
    
    Float_t R;
    Int_t fPE[20000];
    TH2D *h = new TH2D("MaxPEperPMT","Max PE per PMT",30,0,pow(18.5,3),110,0,110);

    for(Int_t i=0;i<nent;i++){
        ch1.GetEntry(i);
        ch2.GetEntry(i);

        Int_t MaxPE = 0;

        for(int k=0;k<20000;k++){
            fPE[k] = 0;
        }

        for(int n=0;n<nInitParticles;n++){
            R = sqrt(InitX[n]*InitX[n]+InitY[n]*InitY[n]+InitZ[n]*InitZ[n]);
        }

        for(int m=0;m<nPhotons;m++){
            if(pmtID[m]>17746)continue;
            fPE[ pmtID[m] ] += nPE[m];
        }

        for(int j=0;j<17746;j++){
            MaxPE = MaxPE>fPE[j]?MaxPE:fPE[j];
        }
        h->Fill(pow(R/1000,3),MaxPE);
    }
    
    h->GetXaxis()->SetTitle("R^{3}[m^{3}]");
    h->GetYaxis()->SetTitle("max pe per pmt");
    h->Draw("colz");

    c->Print("draw_"+type+"MaxPEperPMT.png", "png");
    c->Print("draw_"+type+"MaxPEperPMT.eps", "eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// Mean PE per PMT

void draw_MeanPEperPMT(TString type){
    Double_t NeutronCaptureTime;
    Int_t pmtCounter[17746];
    Float_t R, InitXprime, InitYprime, InitZprime;
    Int_t PMTnum;
    Float_t mean;
    Int_t test = 0;
    
    TCanvas *c = new TCanvas;
    TH1D *h = new TH1D("MeanPEperPMT","Mean PE per PMT",100,0,5);
    
    for(int i=0;i<nent;i++){
        PMTnum = 0;
        totalPE = 0;
        for(int x=0;x<20000;x++){
            hitTime[x] = 0;
        }
        for(int k=0;k<17746;k++){
            pmtCounter[k] = 0;
        }
        for(int z=0;z<10000;z++){
            NeutronCaptureT[z] = 0;
        }
        for(int a=0;a<10;a++){
            InitX[a] = 0;
            InitY[a] = 0;
            InitZ[a] = 0;
        }
        
        ch1.GetEntry(i);
        ch2.GetEntry(i);
        ch3.GetEntry(i);
        
        for(int n=0;n<nInitParticles;n++){
            InitXprime = InitX[n];
            InitYprime = InitY[n];
            InitZprime = InitZ[n];
        }

        for(int y=0;y<NeutronN;y++){
            NeutronCaptureTime = NeutronCaptureT[y];
            test++;
        }
        
        R = sqrt(pow(InitXprime,2)+pow(InitYprime,2)+pow(InitZprime,2));
        
        for(int j=0;j<nPhotons;j++){
            if(pmtID[j]>17746 || hitTime[j]<NeutronCaptureTime)continue;
            pmtCounter[ pmtID[j] ] = 1;
        }

        for(int m=0;m<17746;m++){
            if(pmtCounter[m] == 1){
                PMTnum++;
            }
        }

        mean = 1.0*totalPE/PMTnum;
        h->Fill(mean);
    }
    h->GetXaxis()->SetTitle("number of pe");
    h->Draw();
    
    c->Print("draw_"+type+"MeanPEperPMT.png","png");
    c->Print("draw_"+type+"MeanPEperPMT.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// nPE per PMT

void draw_nPEperPMT(TString type){
    TCanvas *c = new TCanvas; 
    TH1D *h = new TH1D("nPEperPMT","nPE per PMT",120,0,120);
    Int_t fPE[17746];

    for(int i=0;i<nent;i++){
        ch1.GetEntry(i);
        
        for(int m=0;m<17746;m++){
            fPE[m] = 0;
        }

        for(int j=0;j<17746;j++){
            if(fPE[j]>=0){
                h->Fill(fPE[j]);
            }
        }
    }

    h->GetXaxis()->SetTitle("nPE per PMT");
    gStyle->SetOptStat("mr");
    h->Draw();
    
    c->Print("draw_"+type+"nPEperPMT.png","png");
    c->Print("draw_"+type+"nPEperPMT.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// PMT span time: for one PMT, the time difference between the first and last PE.

void draw_PMTSpanTime(TString type){
    TCanvas *c = new TCanvas; 
    Int_t Max[17746], Min[17746];
    Int_t ResponseTime[17746];
    
    TH1D *h = new TH1D("PMTSpanTime","PMT span time",100,0,3500);

    for(int m=0;m<nent;m++){
        ch1.GetEntry(m);
        
        for(int i=0;i<17746;i++){
            ResponseTime[i] = 0;
            Max[i] = 0;
            Min[i] = 0;
        }

        for(int k=0;k<nPhotons;k++){
            if(pmtID[k]>17745)continue;
            Max[ pmtID[k] ] = Max[ pmtID[k] ]<hitTime[k]?hitTime[k]:Max[ pmtID[k] ];
            Min[ pmtID[k] ] = Min[ pmtID[k] ]<hitTime[k]?Min[ pmtID[k] ]:hitTime[k];
            ResponseTime[ pmtID[k] ] = Max[ pmtID[k] ] - Min[ pmtID[k] ];
        }

        for(int n=0;n<17746;n++){
            if(ResponseTime[n]>0){
                h->Fill(ResponseTime[n]);
            }
        }
    }

    h->GetXaxis()->SetTitle("PMT span time/ns");
    gStyle->SetPadTopMargin(0.10);
    gStyle->SetPadLeftMargin(0.17);
    gStyle->SetPadRightMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetOptStat("mr");
    h->Draw();
    
    c->Print("draw_"+type+"PMTSpanTime.png","png");
    c->Print("draw_"+type+"PMTSpanTime.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// Event span time: the time difference between the first PE and the last PE for all PMTs.

void draw_PositronEventSpanTime(){
    
    Double_t htmax, htmin, htmax1, htmax_real, htmin_real;
    Int_t counter = 0; 
    TCanvas *c = new TCanvas;
    TH1D *h = new TH1D("PositronEventSpanTime","event span time",100,0,1000);

    for(int i=0;i<nent;i++){
        htmax = -1;
        htmin = 100000000;
        ch1.GetEntry(i);
        ch3.GetEntry(i);

        for(int k=0;k<nPhotons;k++){
            for(int j=0;j<nPhotons-k-1;j++){
                if(hitTime[j]>hitTime[j+1]){
                    Double_t t = hitTime[j];
                    hitTime[j] = hitTime[j+1];
                    hitTime[j+1] = t;
                }
            }
        }

        Int_t max_num = 0.98*nPhotons;
        htmin = hitTime[0];
        htmax = hitTime[max_num];
        
        h->Fill(htmax-htmin);
    }

    h->GetXaxis()->SetTitle("event span time[ns]");
    gStyle->SetOptStat("mr");
    h->Draw();
    
    c->Print("draw_EventSpanTime.png","png");
    c->Print("draw_EventSpanTime.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

void draw_NeutronEventSpanTime(){
    Double_t htmax, htmin;
    Int_t counter = 0;
    Double_t NeutronCaptureTime;
    
    TCanvas *c = new TCanvas;
    TH1D *h = new TH1D("NeutronEventSpanTime","event span time",200,0,1000);

    for(int i=0;i<nent;i++){
        htmax = -1;
        htmin = 100000000;
        ch1.GetEntry(i);
        ch3.GetEntry(i);

        for(int k=0;k<nPhotons;k++){
            for(int j=0;j<nPhotons-k-1;j++){
                if(hitTime[j]>hitTime[j+1]){
                    Double_t t = hitTime[j];
                    hitTime[j] = hitTime[j+1];
                    hitTime[j+1] = t;
                }
            }
        }

        Int_t max_num = 0.98*nPhotons;
        htmax = hitTime[max_num];
        for(int j=0;j<NeutronN;j++){
            NeutronCaptureTime = NeutronCaptureT[j];
        }
        for(int k=0;k<nPhotons;k++){
            if(hitTime[k]<NeutronCaptureTime)continue;
            htmin = htmin<hitTime[k]?htmin:hitTime[k];
        }
        h->Fill(htmax-htmin);
    }
    h->GetXaxis()->SetTitle("neutron event span time[ns]");
    h->Draw();

    c->Print("draw_NeutronEventSpanTime.png","png");
    c->Print("draw_NeutronEventSpanTime.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// Distance of the visible energy center-of-gravity from the neutron production position.

void draw_ProductionCogDistance(TString type){
    Float_t distance;
    TCanvas *c = new TCanvas;
    TH1D *h = new TH1D("ProductionCogDistance","",250,0,2500);

    for(int i=0;i<nent;i++){
        ch1.GetEntry(i);
        ch2.GetEntry(i);
        for(int k=0;k<nInitParticles;k++){
            distance = sqrt((edepX-InitX[k])*(edepX-InitX[k])+(edepY-InitY[k])*(edepY-InitY[k])+(edepZ-InitZ[k])*(edepZ-InitZ[k]));
        }
        h->Fill(distance);
    }

    h->GetXaxis()->SetTitle("distance[mm]");
    h->Draw();
    gStyle->SetOptStat("mr");

    c->Print("draw_"+type+"ProductionCogDistance.png","png");
    c->Print("draw_"+type+"ProductionCogDistance.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// Neutron capture time.

void draw_NeutronCaptureTime(){
    Double_t capture_time;
    TH1D *h = new TH1D("NeutronCaptureTime","neutron capture time",300,0,2600);
    TCanvas *c = new TCanvas;

    for(int i=0;i<nent;i++){
        ch3.GetEntry(i);
        for(int k=0;k<NeutronN;k++){
            capture_time = NeutronCaptureT[k];
        }
        h->Fill(capture_time/1000);
    }

    h->GetXaxis()->SetTitle("neutron capture time[us]");
    h->Draw();

    c->Print("draw_NeutronCaptureTime.png","png");
    c->Print("draw_NeutronCaptureTime.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

// Neutron drift distance.

void draw_NeutronDriftDistance(){
    TCanvas *c = new TCanvas;
    TH1D *h = new TH1D("NeutronDriftDistance","neutron drift distance",200,0,600);
    
    for(int i=0;i<nent;i++){
        Float_t distance;
        ch3.GetEntry(i);
        for(int j=0;j<NeutronN;j++){
            distance = sqrt((NCStopX[j]-NCStartX[j])*(NCStopX[j]-NCStartX[j])+(NCStopY[j]-NCStartY[j])*(NCStopY[j]-NCStartY[j])+(NCStopZ[j]-NCStartZ[j])*(NCStopZ[j]-NCStartZ[j]));
            h->Fill(distance);
        }
    }
    h->GetXaxis()->SetTitle("neutron drift distance[mm]");
    h->Draw();

    c->Print("draw_NeutronDriftDistance.png","png");
    c->Print("draw_NeutronDriftDistance.eps","eps");

    if (result) {
        result->cd();
        h->Write();
    }
}

int main(int argc, char **argv){
   bool is_eplus = false;
   bool is_neutron = false;
   TString type;
   Int_t num;

   if(argc<3){
        cout<<"*************************************************************************"<<endl;
        cout<<"*                                                                       *"<<endl;
        cout<<"* You need input two parameters: -eplus or -neutron, -listname LISTNAME *"<<endl;
        cout<<"*                                                                       *"<<endl;
        cout<<"*************************************************************************"<<endl;
        return false;
   }

   for(int i=1;i<argc;i++){
       if( strcmp(argv[i],"-eplus") == 0){
           is_eplus = true;
           type = "Positron";
       }else if( strcmp(argv[i], "-neutron") == 0){
           is_neutron = true;
           type = "Neutron";
       }else if( strcmp(argv[i], "-listname") == 0){
           i++;
           num = i;

           if(argv[i] == NULL || strcmp(argv[i],"-neutron") == 0){
               cout<<"##############################################################"<<endl;
               cout<<"#                                                            #"<<endl;
               cout<<"#               Please input the filename!                   #"<<endl;
               cout<<"#                                                            #"<<endl;
               cout<<"##############################################################"<<endl;

               return false;
           }

           fstream _file;
           _file.open(argv[i],ios::in);
           if(!_file){
               cout<<"##############################################################"<<endl;
               cout<<"#                                                            #"<<endl; 
               cout<<"             error: "<<argv[i]<<" does not exist!!            "<<endl;
               cout<<"#                                                            #"<<endl; 
               cout<<"##############################################################"<<endl;
               return false;
           }
       }else{
           cout<<"*************************************************************************"<<endl;
           cout<<"*                                                                       *"<<endl;
           cout<<"* You need input two parameters: -eplus or -neutron, -listname LISTNAME *"<<endl;
           cout<<"*                                                                       *"<<endl;
           cout<<"*************************************************************************"<<endl;
           return false;
       }
   }
   
   if(is_eplus){

        createfile("eplus.root");

        setaddress(argv[num]);

        draw_MaxPEperPMT(type);
    
        draw_MeanPEperPMT(type);
    
        draw_nPEperPMT(type);
    
        draw_PMTSpanTime(type);
    
        draw_PositronEventSpanTime();
    
        draw_ProductionCogDistance(type);
   
   }else if(is_neutron){

       createfile("neutron.root");
    
       setaddress(argv[num]);
       
       draw_MaxPEperPMT(type);
       
       draw_MeanPEperPMT(type);
       
       draw_nPEperPMT(type);
       
       draw_PMTSpanTime(type);
       
       draw_NeutronEventSpanTime();
       
       draw_ProductionCogDistance(type);
       
       draw_NeutronCaptureTime();
       
   }

   if (result) {
       result->Close();
       delete result;
   }
   return 0;
}

