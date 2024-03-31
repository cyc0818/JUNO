#define TH_MIN -2000
#define TH_MAX 2000
#define TH_NUM 1000
#define EN_MIN 0.0
#define EN_MAX 10.0
#define EN_NUM 3000

#include <iostream>
#include <fstream>
#include <TH2F.h>
#include <TTree.h>
#include <TChain.h>
#include <math.h>
#include "TFile.h"
#include "TH2.h"
#include "TH1.h"
#include "TF1.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <TSystem.h>
#include <TROOT.h>
#include "Event/RecHeader.h"

using std::cout;
using std::endl;

void RecAnalysis(TString inputrecpath="lists_rec.txt", TString inputsimpath="lists_detsim.txt",TString outputpath="rec_ana/rec_ana.root"){
    gSystem->Load("libRecEvent.so");
    TH1F* h_X= new TH1F("h_X","X_{rec}-X_{edep}",TH_NUM,TH_MIN,TH_MAX);
    TH1F* h_Y= new TH1F("h_Y","Y_{rec}-Y_{edep}",TH_NUM,TH_MIN,TH_MAX);
    TH1F* h_Z= new TH1F("h_Z","Z_{rec}-Z_{edep}",TH_NUM,TH_MIN,TH_MAX);
    TH2F* h_R2vsZ= new TH2F("h_R2vsZ","X_{rec}^2+Y_{rec}^2 : Z_{rec}",TH_NUM,0,3.3e8,TH_NUM,-20000,20000);
    //TH1F* h_R2vsZ= new TH1F("h_R2vsZ","X_{rec}^2+Y_{rec}^2 : Z_{rec}",TH_NUM,TH_MIN,TH_MAX);
    TH1F* h_E= new TH1F("h_E","E_{rec}",EN_NUM,0,10);
    TH2D* th2_vertex= new TH2D("th2_vertex","th2_vertex",100,0,6000,600,-3000,3000);
    TH2D* th2_energy= new TH2D("th2_energy","th2_energy",100,0,6000,6000,0,10);
    TProfile* pro_vertex=new TProfile("profile_vertex","profile_vertex",25,0,6000,-3000,3000);
    TProfile* pro_energy=new TProfile("profile_energy","profile_energy",25,0,6000,0,10);
    float sim_x;
    float sim_y;
    float sim_z;
    float sim_E;
    float vsigma;
    float emean;
    float esigma;

    std::vector<TString> rec_files_list;
    std::vector<TString> sim_files_list;

    std::ifstream fp(inputrecpath);
    if (!fp.is_open()) {
        std::cerr << "ERROR: input lists does not exists." << std::endl;
        return;
    }
    std::string rec_tmp_line;
    while(fp.good()) {
        std::getline(fp, rec_tmp_line);
        if ( rec_tmp_line.size() ==0 ) {
            continue;
        }
        rec_files_list.push_back(rec_tmp_line);
    }
    TChain* rec_ch = new TChain("Event/Rec/CDRecEvent");
    for (std::vector<TString>::iterator it = rec_files_list.begin();
            it != rec_files_list.end(); ++it) {
        std::cout << "add rec file: " << *it << std::endl;
        rec_ch->Add(*it);
    }

    std::ifstream sfp(inputsimpath);
    if (!sfp.is_open()) {
        std::cerr << "ERROR: input lists does not exists." << std::endl;
        return;
    }
    std::string sim_tmp_line;
    while(sfp.good()) {
        std::getline(sfp, sim_tmp_line);
        if ( sim_tmp_line.size() ==0 ) {
            continue;
        }
        sim_files_list.push_back(sim_tmp_line);
    }
    TChain* sim_ch = new TChain("evt");
    for (std::vector<TString>::iterator it = sim_files_list.begin();
            it != sim_files_list.end(); ++it) {
        std::cout << "add sim file: " << *it << std::endl;
        sim_ch->Add(*it);
    }

    TTree* rec = (TTree*)rec_ch;
    JM::CDRecEvent* rh = new JM::CDRecEvent();
    rec->SetBranchAddress("CDRecEvent",&rh);

    TTree* sim = (TTree*)sim_ch;
    sim -> SetBranchAddress("edepX",&sim_x);
    sim -> SetBranchAddress("edepY",&sim_y);
    sim -> SetBranchAddress("edepZ",&sim_z);
    sim -> SetBranchAddress("edep",&sim_E);

    int nentries = rec->GetEntries();
       for(int i=0;i<nentries;i++){
                sim->GetEntry(i);
                rec->GetEntry(i);
                Float_t rec_x = rh->x();
                Float_t rec_y = rh->y();
                Float_t rec_z = rh->z();
                Float_t rec_E = rh->eprec(); 
                Float_t rec_nfit = rh->energy();
                Float_t rec_r = sqrt(rec_x*rec_x + rec_y*rec_y + rec_z*rec_z);
                Float_t diff_x = rec_x - sim_x;
                Float_t diff_y = rec_y - sim_y;
                Float_t diff_z = rec_z - sim_z;
                Float_t diff_E = rec_E / sim_E;
                Float_t edep_r = sqrt(sim_x*sim_x + sim_y*sim_y + sim_z*sim_z);
                Float_t delta_r = sqrt(diff_x*diff_x + diff_y*diff_y + diff_z*diff_z);
                h_E ->Fill(rec_E);
                h_X -> Fill(diff_x,1);
                h_Y -> Fill(diff_y,1);
                h_Z -> Fill(diff_z,1);
		h_R2vsZ -> Fill(rec_x*rec_x+rec_y*rec_y,rec_z);
                th2_vertex->Fill(pow(edep_r/1000.,3),rec_r-edep_r,1);
                th2_energy->Fill(pow(edep_r/1000.,3),rec_E,1);
                pro_vertex->Fill(pow(edep_r/1000.,3),rec_r-edep_r,1);
                pro_energy->Fill(pow(edep_r/1000.,3),rec_E,1);
            }
            
   TCanvas* myc=new TCanvas("myc","a canvas",10,10,700,500);
        h_Z -> Fit("gaus","W","C",TH_MIN,TH_MAX);
   	h_Z -> GetFunction("gaus") -> SetLineColor(kGreen);
       // TF1  *fitV = (TF1*)h_Z->GetFunction("gaus");
       // Float_t vertex_sigma = fitV->GetParameter(2);
        h_Z->GetXaxis()->SetTitle("Z_{rec}-Z_{edep}[mm]");
        h_Z->Draw();
   TCanvas* myc1=new TCanvas("myc1","a canvas",10,10,700,500);
        h_E-> Fit("gaus","W","C",EN_MIN,EN_MAX);
   	h_E-> GetFunction("gaus") -> SetLineColor(kGreen);
       // TF1  *fitE = (TF1*)h_E->GetFunction("gaus");
       // Float_t energy_mean = fitE ->GetParameter(1);
       // Float_t energy_sigma = fitE->GetParameter(2)/fitE->GetParameter(1);
        h_E->GetXaxis()->SetTitle("energy[MeV]");
        h_E->Draw();
   TCanvas* myc_vertex=new TCanvas("myc_vertex","vertex distribution",10,10,900,500);
        myc_vertex->Divide(2,1);
        pro_vertex->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        pro_vertex->GetYaxis()->SetTitle("R_{rec}-R_{edep}[mm]");
        pro_vertex->GetYaxis()->SetTitleOffset(1.2);
        pro_vertex->GetYaxis()->CenterTitle();
        pro_vertex->SetLineColor(2);
        th2_vertex->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        th2_vertex->GetYaxis()->SetTitle("R_{rec}-R_{edep}[mm]");
        th2_vertex->GetYaxis()->SetTitleOffset(1.2);
        th2_vertex->GetYaxis()->CenterTitle();
        myc_vertex->cd(1);
        th2_vertex->Draw("colz");
        myc_vertex->cd(2);
        pro_vertex->GetYaxis()->SetRangeUser(-3000,3000);
        pro_vertex->Draw();
   TCanvas* myc_energy=new TCanvas("myc_energy","energy distribution",10,10,900,500);
        myc_energy->Divide(2,1);
        pro_energy->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        pro_energy->GetYaxis()->SetTitle("energy[MeV]");
        pro_energy->GetYaxis()->SetTitleOffset(1.2);
        pro_energy->GetYaxis()->CenterTitle();
        pro_energy->SetLineColor(2);
        th2_energy->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        th2_energy->GetYaxis()->SetTitle("energy[MeV]");
        th2_energy->GetYaxis()->SetTitleOffset(1.2);
        th2_energy->GetYaxis()->CenterTitle();
        myc_energy->cd(1);
        th2_energy->Draw("colz");
        myc_energy->cd(2);
        pro_energy->GetYaxis()->SetRangeUser(0,10);
        pro_energy->Draw();

//output file
       TFile *f2= TFile::Open(outputpath,"recreate");
    if (!f2) {
        std::cerr << "Can't open file " << outputpath << std::endl;
        return;
    }
    h_X->Write();
    h_Y->Write();
    h_Z->Write();
    h_R2vsZ->Write();
    h_E->Write();

    th2_vertex->Write();
    th2_energy->Write();
    pro_vertex->Write();
    pro_energy->Write();

    delete h_X;
    delete h_Y;
    delete h_Z;
    delete h_R2vsZ;
    delete h_E;
    delete th2_vertex; 
    delete th2_energy;
    delete pro_vertex;         
    delete pro_energy;
 }
