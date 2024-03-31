#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <string.h>
#include <string>
#include <TStyle.h>
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <stdlib.h>
#include <stdio.h>
#include <TMath.h>
#include "Event/RecHeader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <TSystem.h>
#include <TStyle.h>
#include <math.h>
#include <TCanvas.h>

using namespace std;

#define PMTSUM 17612
// #define PMTSUM 17739



int DrawHisto_cd(string recVfile, string recEfile, string histfile){

        TH1D* h_V_x = new TH1D("h_V_x","X_{rec} (TLH)",100,-18000,18000);
        TH1D* h_V_y = new TH1D("h_V_y","Y_{rec} (TLH)",100,-18000,18000);
        TH1D* h_V_z = new TH1D("h_V_z","Z_{rec} (TLH)",100,-18000,18000);
        TH1D* h_V_e = new TH1D("h_V_e","E_{rec} (TLH)",100,0,10);
        TH1D* h_V_r = new TH1D("h_V_r","R_{rec}^{3} (TLH)",100,0,6000);
        TH1D* h_E_x = new TH1D("h_E_x","X_{rec} (QMLE)",100,-18000,18000);
        TH1D* h_E_y = new TH1D("h_E_y","Y_{rec} (QMLE)",100,-18000,18000);
        TH1D* h_E_z = new TH1D("h_E_z","Z_{rec} (QMLE)",100,-18000,18000);
        TH1D* h_E_e = new TH1D("h_E_e","E_{rec} (QMLE)",100,0,10);
        TH1D* h_E_r = new TH1D("h_E_r","R_{rec}^{3} (QMLE)",100,0,6000);
        TH1D* h_firedpmts = new TH1D("h_firedpmts","Fired PMTs",100,0,18000);
        TH1D* h_pesum = new TH1D("h_pesum","PEsum",100,0,10000);
        TH1D* h_pmtnpe = new TH1D("h_pmtnpe","pmt_npe",100,0,18000);
        TH1D* h_hittime = new TH1D("h_hittime","Hit time",100,0,10000);

        cout << "rec V file name : " << recVfile << endl;
        cout << "rec E file name : " << recEfile << endl;
        TFile *Vfile = new TFile(recVfile.c_str());
        TFile *Efile = new TFile(recEfile.c_str());
        TTree *V_tr = (TTree*)Vfile->Get("data");
        TTree* E_tr = (TTree*)Efile->Get("TRec");
        if ( V_tr==NULL || E_tr==NULL ) {
            cout << "Can not find rec data" << endl;
            return -1;
        }

        int pmtnum,firedpmtnum,pesum;
        double recVx,recVy,recVz,recVe,recVr,recEr;
        float recEx,recEy,recEz,recEe;
        double hittime_max,firedpmtmax,firedpmtmin;
        double npe[PMTSUM],hittime[PMTSUM];
        V_tr->SetBranchAddress("nTotalPMT",&pmtnum);
        V_tr->SetBranchAddress("nFiredPMT",&firedpmtnum);
        V_tr->SetBranchAddress("pmt_npe",&npe);
        V_tr->SetBranchAddress("pmt_hittime",&hittime);
        V_tr->SetBranchAddress("rec_x",&recVx);
        V_tr->SetBranchAddress("rec_y",&recVy);
        V_tr->SetBranchAddress("rec_z",&recVz);
        V_tr->SetBranchAddress("rec_E",&recVe);
        E_tr->SetBranchAddress("recQx",&recEx);
        E_tr->SetBranchAddress("recQy",&recEy);
        E_tr->SetBranchAddress("recQz",&recEz);
        E_tr->SetBranchAddress("m_QEn",&recEe);
        hittime_max = 0;
        firedpmtmax = 0;
        firedpmtmin = 20000;

        V_tr->GetEntry(0);
        if (pmtnum != PMTSUM)
        {
            cout << "PMT in rec file : "<<pmtnum<<endl;
            cout << "PMT in code : "<<PMTSUM<<endl;
            cout<<"ERROR: the total number of PMT is wrong!!!!!"<<endl;
            return -1; 
        }
        
        for(int i = 0; i < V_tr->GetEntries(); i++){
            V_tr->GetEntry(i);
            recVr = sqrt(recVx*recVx + recVy*recVy + recVz*recVz)/1000;
            h_V_x->Fill(recVx);
            h_V_y->Fill(recVy);
            h_V_z->Fill(recVz);
            h_V_e->Fill(recVe);
            h_V_r->Fill(recVr*recVr*recVr);
            h_firedpmts->Fill(firedpmtnum);
            // cout<<firedpmtnum<<endl;
            if(firedpmtnum > firedpmtmax) firedpmtmax = firedpmtnum;
            if(firedpmtnum < firedpmtmin) firedpmtmin = firedpmtnum;

            pesum = 0;
            for(int j = 0; j < pmtnum; j++){
                
                if(npe[j] == 0) continue;
                pesum = pesum + npe[j];
                h_hittime->Fill(hittime[j]);
                for (int k = 0; k < npe[j]; k++){
                    h_pmtnpe->Fill(j);
                }
                if(hittime_max < hittime[j]) hittime_max = hittime[j];
                
            }
            h_pesum->Fill(pesum);
        }
        Vfile->Close();

        for (int i = 0; i < E_tr->GetEntries(); i++)
        {
            E_tr->GetEntry(i);
            recEr = sqrt(recEx*recEx + recEy*recEy + recEz*recEz)/1000;
            h_E_x->Fill(recEx);
            h_E_y->Fill(recEy);
            h_E_z->Fill(recEz);
            h_E_e->Fill(recEe);
            h_E_r->Fill(recEr*recEr*recEr);

            cout<<recEx<<" "<<recEy<<" "<<recEz<<" "<<recEr<<endl;
        }
        Efile->Close();

        // h_firedpmts->SetBins(floor((firedpmtmax-firedpmtmin)/20),firedpmtmin-10,firedpmtmax+10);
        // h_hittime->SetBins(floor(hittime_max/10),0,hittime_max*1.1);
        // h_npe->SetBins(100,0,pmtnum+200);

        TCanvas* c1 = new TCanvas("c1","c1",1000,800);
        c1->Divide(4,5);

        c1->cd(1);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_V_x->GetXaxis()->SetTitle("X_{rec} [mm]");
        h_V_x->GetXaxis()->SetTitleSize(0.042);
        h_V_x->Draw();

        c1->cd(2);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_V_y->GetXaxis()->SetTitle("Y_{rec} [mm]");
        h_V_y->GetXaxis()->SetTitleSize(0.042);
        h_V_y->Draw();

        c1->cd(3);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_V_z->GetXaxis()->SetTitle("Z_{rec} [mm]");
        h_V_z->GetXaxis()->SetTitleSize(0.042);
        h_V_z->Draw();

        c1->cd(4);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_V_r->GetXaxis()->SetTitle("R_{rec}^{3} [m]");
        h_V_r->GetXaxis()->SetTitleSize(0.042);
        h_V_r->Draw();

        c1->cd(5);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_E_x->GetXaxis()->SetTitle("X_{rec} [mm]");
        h_E_x->GetXaxis()->SetTitleSize(0.042);
        h_E_x->Draw();

        c1->cd(6);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_E_y->GetXaxis()->SetTitle("Y_{rec} [mm]");
        h_E_y->GetXaxis()->SetTitleSize(0.042);
        h_E_y->Draw();

        c1->cd(7);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_E_z->GetXaxis()->SetTitle("Z_{rec} [mm]");
        h_E_z->GetXaxis()->SetTitleSize(0.042);
        h_E_z->Draw();

        c1->cd(8);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_E_r->GetXaxis()->SetTitle("R_{rec}^{3} [m]");
        h_E_r->GetXaxis()->SetTitleSize(0.042);
        h_E_r->Draw();

        c1->cd(9);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_V_e->GetXaxis()->SetTitle("E_{rec} [MeV]");
        h_V_e->GetXaxis()->SetTitleSize(0.042);
        h_V_e->Draw();

        c1->cd(10);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_E_e->GetXaxis()->SetTitle("E_{rec} [MeV]");
        h_E_e->GetXaxis()->SetTitleSize(0.042);
        h_E_e->Draw();

        c1->cd(11);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_hittime->GetXaxis()->SetTitle("Hit time [ns]");
        h_hittime->GetXaxis()->SetTitleSize(0.042);
        h_hittime->Draw();

        c1->cd(12);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_firedpmts->GetXaxis()->SetTitle("Fired PMTs");
        h_firedpmts->GetXaxis()->SetTitleSize(0.042);
        h_firedpmts->Draw();

        c1->cd(13);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_pesum->GetXaxis()->SetTitle("PESum");
        h_pesum->GetXaxis()->SetTitleSize(0.042);
        h_pesum->Draw();

        c1->cd(14);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        h_pmtnpe->GetXaxis()->SetTitle("PMT ID");
        h_pmtnpe->GetXaxis()->SetTitleSize(0.042);
        h_pmtnpe->Draw();

        c1->SaveAs(Form("%s.pdf",histfile.c_str()));

        TFile* file = new TFile(histfile.c_str(),"recreate");
        h_E_x->Write();
        h_E_z->Write();
        h_E_y->Write();
        h_E_e->Write();
        h_E_r->Write();
        h_V_x->Write();
        h_V_z->Write();
        h_V_y->Write();
        h_V_e->Write();
        h_V_r->Write();
        h_firedpmts->Write();
        h_hittime->Write();
        h_pmtnpe->Write();
        h_pesum->Write();
        file->Close();

        cout<<"Draw histo successful"<<endl;
        // cout<<firedpmtmax<<" "<<firedpmtmin<<" "<<hittime_max<<endl;
        return 0;

}

int MergeHisto_cd(string mergefile, string histlistfile, string logpath){

        TH1D* merge_V_x = new TH1D("merge_V_x","X_{rec} (TLH)",5000,-18000,18000);
        TH1D* merge_V_y = new TH1D("merge_V_y","Y_{rec} (TLH)",5000,-18000,18000);
        TH1D* merge_V_z = new TH1D("merge_V_z","Z_{rec} (TLH)",5000,-18000,18000);
        TH1D* merge_V_e = new TH1D("merge_V_e","E_{rec} (TLH)",100,0,10);
        TH1D* merge_V_r = new TH1D("merge_V_r","R_{rec}^{3} (TLH)",100,0,6000);
        TH1D* merge_E_x = new TH1D("merge_E_x","X_{rec} (QMLE)",5000,-18000,18000);
        TH1D* merge_E_y = new TH1D("merge_E_y","Y_{rec} (QMLE)",5000,-18000,18000);
        TH1D* merge_E_z = new TH1D("merge_E_z","Z_{rec} (QMLE)",5000,-18000,18000);
        TH1D* merge_E_e = new TH1D("merge_E_e","E_{rec} (QMLE)",100,0,10);
        TH1D* merge_E_r = new TH1D("merge_r","R_{rec}^{3} (QMLE)",100,0,6000);
        // TH1D* h_eventrate = new TH1D("event_rate","EventRate",20,0,10);
        TH1D* merge_firedpmts = new TH1D("h_firedpmts","Fired PMTs",100,0,18000);
        TH1D* merge_pmtnpe = new TH1D("h_npe","pmt_npe",100,0,18000);
        TH1D* merge_hittime = new TH1D("h_hittime","hit time",100,0,10000);
        TH1D* merge_pesum = new TH1D("h_pesum","PEsum",100,0,10000);
        ifstream logfile;
        // string eventratelogname = logpath + "/eventrate.log";
        
        // logfile.open(eventratelogname,ios::in);
        // string singlerate;
        // int timenum = 0;
        // int weight;
        // while(getline(logfile,singlerate)){
        //     cout<<"s="<<singlerate<<endl;
        //     sscanf(singlerate.c_str(),"%d",&weight);
        //     cout<<"w="<<weight<<endl;
        //     for(int i = 0; i < weight; i++){
        //         h_eventrate->Fill(timenum*0.5);
        //     }
        //     timenum++;
        // }
        // logfile.close();


        logfile.open(histlistfile,ios::in);
        string singleHist;
        TFile *histrootfile;
        const TH1D *hist;
        while (getline(logfile,singleHist)) {
            cout<<"histrootfile="<<singleHist<<endl;
            histrootfile = new TFile(singleHist.c_str());
            hist = (TH1D*)histrootfile->Get("h_V_x");
            merge_V_x->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_V_y");
            merge_V_y->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_V_z");
            merge_V_z->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_V_e");
            merge_V_e->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_V_r");
            merge_V_r->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_E_x");
            merge_E_x->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_E_y");
            merge_E_y->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_E_z");
            merge_E_z->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_E_e");
            merge_E_e->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_E_r");
            merge_E_r->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_firedpmts");
            merge_firedpmts->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_pmtnpe");
            merge_pmtnpe->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_hittime");
            merge_hittime->Add(hist,1);
            hist = (TH1D*)histrootfile->Get("h_pesum");
            merge_pesum->Add(hist,1);
            histrootfile->Close();
        }
        logfile.close();

        TFile* ofile = new TFile(mergefile.c_str(),"recreate");

        TCanvas* c1 = new TCanvas("c1","c1",1000,800);
        c1->Divide(4,5);

        c1->cd(1);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_V_x->GetXaxis()->SetTitle("X_{rec} [mm]");
        merge_V_x->GetXaxis()->SetTitleSize(0.042);
        merge_V_x->Draw();

        c1->cd(2);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_V_y->GetXaxis()->SetTitle("Y_{rec} [mm]");
        merge_V_y->GetXaxis()->SetTitleSize(0.042);
        merge_V_y->Draw();

        c1->cd(3);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_V_z->GetXaxis()->SetTitle("Z_{rec} [mm]");
        merge_V_z->GetXaxis()->SetTitleSize(0.042);
        merge_V_z->Draw();

        c1->cd(4);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_V_r->GetXaxis()->SetTitle("R_{rec}^{3} [m]");
        merge_V_r->GetXaxis()->SetTitleSize(0.042);
        merge_V_r->Draw();

        c1->cd(5);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_E_x->GetXaxis()->SetTitle("X_{rec} [mm]");
        merge_E_x->GetXaxis()->SetTitleSize(0.042);
        merge_E_x->Draw();

        c1->cd(6);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_E_y->GetXaxis()->SetTitle("Y_{rec} [mm]");
        merge_E_y->GetXaxis()->SetTitleSize(0.042);
        merge_E_y->Draw();

        c1->cd(7);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_E_z->GetXaxis()->SetTitle("Z_{rec} [mm]");
        merge_E_z->GetXaxis()->SetTitleSize(0.042);
        merge_E_z->Draw();

        c1->cd(8);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_E_r->GetXaxis()->SetTitle("R_{rec}^{3} [m]");
        merge_E_r->GetXaxis()->SetTitleSize(0.042);
        merge_E_r->Draw();

        c1->cd(9);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_V_e->GetXaxis()->SetTitle("E_{rec} [MeV]");
        merge_V_e->GetXaxis()->SetTitleSize(0.042);
        merge_V_e->Draw();

        c1->cd(10);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_E_e->GetXaxis()->SetTitle("E_{rec} [MeV]");
        merge_E_e->GetXaxis()->SetTitleSize(0.042);
        merge_E_e->Draw();

        c1->cd(11);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_hittime->GetXaxis()->SetTitle("Hit time [ns]");
        merge_hittime->GetXaxis()->SetTitleSize(0.042);
        merge_hittime->Draw();

        c1->cd(12);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_firedpmts->GetXaxis()->SetTitle("Fired PMTs");
        merge_firedpmts->GetXaxis()->SetTitleSize(0.042);
        merge_firedpmts->Draw();

        c1->cd(13);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_pesum->GetXaxis()->SetTitle("PESum");
        merge_pesum->GetXaxis()->SetTitleSize(0.042);
        merge_pesum->Draw();

        c1->cd(14);
        // gPad->SetGrid();
        gStyle->SetNdivisions(510);
        merge_pmtnpe->GetXaxis()->SetTitle("PMT ID");
        merge_pmtnpe->GetXaxis()->SetTitleSize(0.042);
        merge_pmtnpe->Draw();

        c1->SaveAs(Form("%s.pdf",mergefile.c_str()));

        
        merge_V_x->Write();
        merge_V_y->Write();
        merge_V_z->Write();
        merge_V_e->Write();
        merge_V_r->Write();
        merge_E_x->Write();
        merge_E_y->Write();
        merge_E_z->Write();
        merge_E_e->Write();
        merge_E_r->Write();
        merge_firedpmts->Write();
        merge_pmtnpe->Write();
        merge_hittime->Write();
        merge_pesum->Write();
        ofile->Close();

        cout<<"Merge histo successful"<<endl;
        return 0;
    

}

int DrawHisto_tt(string recfile, string histfile){

    TH1I *h_NTotPoints = new TH1I("h_NTotPoints","NTotPoints",10,0,10);
    TH1F *h_PointX = new TH1F("h_PointX","PointX",100,-11000,11000);
    TH1F *h_PointY = new TH1F("h_PointY","PointY",100,-25000,25000);
    TH1F *h_PointZ = new TH1F("h_PointZ","PointZ",100,-5000,5000);
    TH1I *h_NTracks = new TH1I("h_NTracks","NTracks",10,0,10);
    TH1I *h_NPoints = new TH1I("h_NPoints","NPoints",10,0,10);

    cout <<"rec file name : " << recfile <<endl;
    TFile *file = new TFile(recfile.c_str());
    file->ls();
    TTree *tr = (TTree*)file->Get("TT");
    if ( tr == NULL ) {
        cout << "DrawHisto_tt: Can not find rec data" << endl;
        return -1;
    }

    int NTotPoints,NPoints,NTracks;
    float PointX[10],PointY[10],PointZ[10];

    tr->SetBranchAddress("NTotPoints",&NTotPoints);
    tr->SetBranchAddress("NPoints",&NPoints);
    tr->SetBranchAddress("NTracks",&NTracks);
    tr->SetBranchAddress("PointX",&PointX);
    tr->SetBranchAddress("PointY",&PointY);
    tr->SetBranchAddress("PointZ",&PointZ);

    for(int i = 0; i < tr->GetEntries(); i++)
    {
        tr->GetEntry(i);
        if (NTracks > 10)
        {
            cout << "！！！！！！！！！！！" <<endl;
            break;
        }

        h_NTotPoints->Fill(NTotPoints);
        h_NPoints->Fill(NPoints);
        h_NTracks->Fill(NTracks);

        for (int j = 0; j < NTotPoints; j++)
        {
            h_PointX->Fill(PointX[j]);
            h_PointY->Fill(PointY[j]);
            h_PointZ->Fill(PointZ[j]);
        }
    }
    file->Close();

    TCanvas* c1 = new TCanvas("c1","c1",1300,800);
    c1->Divide(3,2);

    c1->cd(1);
    gStyle->SetNdivisions(510);
    h_NTotPoints->GetXaxis()->SetTitle("NTotPoints");
    h_NTotPoints->GetXaxis()->SetTitleSize(0.042);
    h_NTotPoints->Draw();

    c1->cd(2);
    gStyle->SetNdivisions(510);
    h_NPoints->GetXaxis()->SetTitle("NPoints");
    h_NPoints->GetXaxis()->SetTitleSize(0.042);
    h_NPoints->Draw();

    c1->cd(3);
    gStyle->SetNdivisions(510);
    h_NTracks->GetXaxis()->SetTitle("NTracks");
    h_NTracks->GetXaxis()->SetTitleSize(0.042);
    h_NTracks->Draw();

    c1->cd(4);
    gStyle->SetNdivisions(510);
    h_PointX->GetXaxis()->SetTitle("PointX [mm]");
    h_PointX->GetXaxis()->SetTitleSize(0.042);
    h_PointX->Draw();

    c1->cd(5);
    gStyle->SetNdivisions(510);
    h_PointY->GetXaxis()->SetTitle("PointY [mm]");
    h_PointY->GetXaxis()->SetTitleSize(0.042);
    h_PointY->Draw();

    c1->cd(6);
    gStyle->SetNdivisions(510);
    h_PointZ->GetXaxis()->SetTitle("PointZ [mm]");
    h_PointZ->GetXaxis()->SetTitleSize(0.042);
    h_PointZ->Draw();

    c1->SaveAs(Form("%s.pdf",histfile.c_str()));

    file = new TFile(histfile.c_str(),"recreate");
    h_PointZ->Write();
    h_PointY->Write();
    h_PointX->Write();
    h_NTotPoints->Write();
    h_NTracks->Write();
    h_NPoints->Write();

    file->Close();
    cout<<"Draw histo successful"<<endl;
    return 0;

}

int MergeHisto_tt(string mergefile, string histlistfile, string logpath){

    TH1I *merge_NTotPoints = new TH1I("h_NTotPoints","NTotPoints",10,0,10);
    TH1F *merge_PointX = new TH1F("h_PointX","PointX",1000,-11000,11000);
    TH1F *merge_PointY = new TH1F("h_PointY","PointY",1000,-25000,25000);
    TH1F *merge_PointZ = new TH1F("h_PointZ","PointZ",1000,-5000,5000);
    TH1I *merge_NTracks = new TH1I("h_NTracks","NTracks",10,0,10);
    TH1I *merge_NPoints = new TH1I("h_NPoints","NPoints",10,0,10);
    ifstream logfile;
    logfile.open(histlistfile,ios::in);
    string singleHist;
    TFile *histrootfile;
    const TH1I *hist1;
    const TH1F *hist2;
    while (getline(logfile,singleHist)) {
        cout<<"histrootfile="<<singleHist<<endl;
        histrootfile = new TFile(singleHist.c_str());
        hist1 = (TH1I*)histrootfile->Get("h_NTotPoints");
        merge_NTotPoints->Add(hist1,1);
        hist1 = (TH1I*)histrootfile->Get("h_NTracks");
        merge_NTracks->Add(hist1,1);
        hist1 = (TH1I*)histrootfile->Get("h_NPoints");
        merge_NPoints->Add(hist1,1);
        hist2 = (TH1F*)histrootfile->Get("h_PointX");
        merge_PointX->Add(hist2,1);
        hist2 = (TH1F*)histrootfile->Get("h_PointY");
        merge_PointY->Add(hist2,1);
        hist2 = (TH1F*)histrootfile->Get("h_PointZ");
        merge_PointZ->Add(hist2,1);
        histrootfile->Close();
    }
    logfile.close();
    TFile* ofile = new TFile(mergefile.c_str(),"recreate");
    TCanvas* c1 = new TCanvas("c1","c1",1000,800);
    c1->Divide(2,3);

    c1->cd(1);
    gStyle->SetNdivisions(510);
    merge_NTotPoints->GetXaxis()->SetTitle("NTotPoints");
    merge_NTotPoints->GetXaxis()->SetTitleSize(0.042);
    merge_NTotPoints->Draw();

    c1->cd(2);
    gStyle->SetNdivisions(510);
    merge_NPoints->GetXaxis()->SetTitle("NPoints");
    merge_NPoints->GetXaxis()->SetTitleSize(0.042);
    merge_NPoints->Draw();

    c1->cd(3);
    gStyle->SetNdivisions(510);
    merge_NTracks->GetXaxis()->SetTitle("NTracks");
    merge_NTracks->GetXaxis()->SetTitleSize(0.042);
    merge_NTracks->Draw();

    c1->cd(4);
    gStyle->SetNdivisions(510);
    merge_PointX->GetXaxis()->SetTitle("PointX [mm]");
    merge_PointX->GetXaxis()->SetTitleSize(0.042);
    merge_PointX->Draw();

    c1->cd(5);
    gStyle->SetNdivisions(510);
    merge_PointY->GetXaxis()->SetTitle("PointY [mm]");
    merge_PointY->GetXaxis()->SetTitleSize(0.042);
    merge_PointY->Draw();

    c1->cd(6);
    gStyle->SetNdivisions(510);
    merge_PointZ->GetXaxis()->SetTitle("PointZ [mm]");
    merge_PointZ->GetXaxis()->SetTitleSize(0.042);
    merge_PointZ->Draw();

    c1->SaveAs(Form("%s.pdf",mergefile.c_str()));
    merge_PointZ->Write();
    merge_PointY->Write();
    merge_PointX->Write();
    merge_NTracks->Write();
    merge_NPoints->Write();
    merge_NTotPoints->Write();
    ofile->Close();
    cout<<"Merge histo successful"<<endl;
    return 0;
}


int main(int argc, char *argv[]){

    if (atoi(argv[1]) == 0)
    { 
        if (strcmp(argv[2],"cd") == 0) DrawHisto_cd(argv[3],argv[4],argv[5]);
        else if (strcmp(argv[2],"tt") == 0) DrawHisto_tt(argv[3],argv[5]);
    }
    else if (atoi(argv[1]) == 1)
    {
        if (strcmp(argv[2],"cd") == 0) MergeHisto_cd(argv[3],argv[4],argv[5]);
        else if (strcmp(argv[2],"tt") == 0) MergeHisto_tt(argv[3],argv[4],argv[5]);
    }
    else cout<<"Unknowed model number, draw or merge histo failed !!!!!!"<<endl;

    return 0;

}


