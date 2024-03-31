#define TH_MIN -2000
#define TH_MAX 2000
#define TH_NUM 1000
#define EN_MIN 0.0
#define EN_MAX 10.0
#define EN_NUM 3000

#include <iostream>
#include <sstream>
#include <stdlib.h>  
#include "stdio.h"  
#include <unistd.h>

using std::cout;
using std::endl;

float vsigma[7];
float emean[7];
float esigma[7];
TH1F* h_X[7];
TH1F* h_Y[7];
TH1F* h_Z[7];
TH1F* h_E[7];
TH2D* th2_vertex[7];
TH2D* th2_energy[7];
TProfile* pro_vertex[7];
TProfile* pro_energy[7];

void RecAnalysis_uniform()
{

  for(int i=0;i<7;i++)
  {
    vsigma[i]=0;
    emean[i]=0;
    esigma[i]=0;
  }  

  vector<std::string> subdirs;
  subdirs.push_back("e+_1.022MeV");
  subdirs.push_back("e+_2.0MeV");
  subdirs.push_back("e+_3.0MeV");
  subdirs.push_back("e+_4.0MeV");
  subdirs.push_back("e+_5.0MeV");
  subdirs.push_back("e+_6.0MeV");
  subdirs.push_back("e+_7.0MeV");

  int evtnum=1000;
  int jobnum=100;
  int startseed=1000;

  ofstream txtfile("result_uniform.txt");
  std::string dir="";
  for(int i=2;i<3;i++)
  {
    dir=subdirs[i];
    cout<<"energy:"<<subdirs[i]<<endl;
    RecAnalysisSingleEnergy(dir.c_str(),dir.c_str(), evtnum, jobnum, startseed, i);
    txtfile<<vsigma[i]<<" "<<emean[i]<<" "<<esigma[i]<<std::endl;
  }
  txtfile.close();
}

void RecAnalysisSingleEnergy(const char* simFilePath,const char* recFilePath,int evtNum,int jobNum,int baseline, int energyIndex){
    gStyle->SetOptFit(1111);
    gSystem -> Load("$RECEVENTROOT/$CMTCONFIG/libRecEvent.so");
    h_X[energyIndex]= new TH1F(Form("h_X_%dMeV",energyIndex+1),Form("X_{rec}-X_{edep}_%dMeV:",energyIndex+1),TH_NUM,TH_MIN,TH_MAX);
    h_Y[energyIndex]= new TH1F(Form("h_Y_%dMeV",energyIndex+1),Form("Y_{rec}-Y_{edep}_%dMeV:",energyIndex+1),TH_NUM,TH_MIN,TH_MAX);
    h_Z[energyIndex]= new TH1F(Form("h_Z_%dMeV",energyIndex+1),Form("Z_{rec}-Z_{edep}_%dMeV:",energyIndex+1),TH_NUM,TH_MIN,TH_MAX);
    h_E[energyIndex]= new TH1F(Form("h_E_%dMeV",energyIndex+1),Form("E_{rec}_%dMeV:",energyIndex+1),EN_NUM,1.05*energyIndex,1.05*energyIndex+2);
    th2_vertex[energyIndex]= new TH2D(Form("th2_%dMeV_vertex",energyIndex+1),Form("th2_%dMeV_vertex",energyIndex+1),100,0,6000,600,-3000,3000);
    th2_energy[energyIndex]= new TH2D(Form("th2_%dMeV_energy",energyIndex+1),Form("th2_%dMeV_energy",energyIndex+1),100,0,6000,600,1.05*energyIndex-1,1.05*energyIndex+3);
    pro_vertex[energyIndex]=new TProfile(Form("profile_%dMeV_vertex",energyIndex+1),Form("profile_%dMeV_vertex",energyIndex+1),25,0,6000,-3000,3000);
    pro_energy[energyIndex]=new TProfile(Form("profile_%dMeV_energy",energyIndex+1),Form("profile_%dMeV_energy",energyIndex+1),25,0,6000,1.05*energyIndex-1,1.05*energyIndex+3);
    TChain ch_sim("evt");
    TChain ch_rec("Event/RecEvent/RecHeader");
       for(int j=0;j<jobNum;j++){
         string n_flag;
         stringstream ss;
         int k = baseline + j + energyIndex*jobNum;
         ss << k;
         ss >>n_flag;
         TString simFileAdd = Form("%s/user-sim-%s.root",simFilePath,n_flag.c_str());
         TString recFileAdd = Form("%s/rec-%s.root",recFilePath,n_flag.c_str());
         ch_sim.Add(simFileAdd);
         ch_rec.Add(recFileAdd);
         }
            TObjArray* simFileElements=ch_sim.GetListOfFiles();
            TObjArray* recFileElements=ch_rec.GetListOfFiles();
            TIter simnext(simFileElements);
            TIter recnext(recFileElements);
            TChainElement* simChEl=0;
            TChainElement* recChEl=0;
            while (( recChEl=(TChainElement*)recnext() )){
                simChEl=(TChainElement*)simnext();
                TFile* simf =new TFile(simChEl->GetTitle());
                TFile* recf =new TFile(recChEl->GetTitle());
                cout<<simChEl->GetTitle()<<","<<recChEl->GetTitle()<<endl;
                TTree* rec_ch = (TTree*)recf -> Get("Event/RecEvent/RecHeader");
                rh = new JM::RecHeader();
                rec_ch->SetBranchAddress("RecHeader",&rh);
                TTree* sim_ch = (TTree*)simf -> Get("evt");
                Float_t sim_x =0;
                Float_t sim_y =0;
                Float_t sim_z =0;
                Float_t sim_E =0;
                sim_ch -> SetBranchAddress("edepX",&sim_x);
                sim_ch -> SetBranchAddress("edepY",&sim_y);
                sim_ch -> SetBranchAddress("edepZ",&sim_z);
                sim_ch -> SetBranchAddress("edep",&sim_E);
                for(int i=0;i<evtNum;i++){
                sim_ch ->GetEntry(i);
                rec_ch ->GetEntry(i);
                Float_t rec_x = rh->x();
                Float_t rec_y = rh->y();
                Float_t rec_z = rh->z();
                Float_t rec_E = rh->eprec(); 
                Float_t rec_nfit = rh->energy();
                Float_t diff_x = rec_x - sim_x;
                Float_t diff_y = rec_y - sim_y;
                Float_t diff_z = rec_z - sim_z;
                Float_t diff_E = rec_E / sim_E;
                Float_t rec_r = sqrt(rec_x*rec_x + rec_y*rec_y + rec_z*rec_z);
                Float_t edep_r = sqrt(sim_x*sim_x + sim_y*sim_y + sim_z*sim_z);
                Float_t delta_r = sqrt(diff_x*diff_x + diff_y*diff_y + diff_z*diff_z);
                h_Z[energyIndex] -> Fill(diff_z,1);
                h_E[energyIndex] ->Fill(rec_E);
                th2_vertex[energyIndex]->Fill(pow(edep_r/1000.,3),rec_r-edep_r,1);
                th2_energy[energyIndex]->Fill(pow(edep_r/1000.,3),rec_E,1);
                pro_vertex[energyIndex]->Fill(pow(edep_r/1000.,3),rec_r-edep_r,1);
                pro_energy[energyIndex]->Fill(pow(edep_r/1000.,3),rec_E,1);
                }
                simf->Close();
                recf->Close();
            }
            
   TCanvas* myc=new TCanvas("myc","a canvas",10,10,700,500);
        h_Z[energyIndex] -> Fit("gaus","W","C",TH_MIN,TH_MAX);
        TF1  *fitV = (TF1*)h_Z[energyIndex]->GetFunction("gaus");
        Float_t vertex_sigma = fitV->GetParameter(2);
        h_Z[energyIndex]->GetXaxis()->SetTitle("Z_{rec}-Z_{edep}[mm]");
        h_Z[energyIndex]->Draw();
   TCanvas* myc1=new TCanvas("myc1","a canvas",10,10,700,500);
        h_E[energyIndex]-> Fit("gaus","W","C",EN_MIN,EN_MAX);
        TF1  *fitE = (TF1*)h_E[energyIndex]->GetFunction("gaus");
        Float_t energy_mean = fitE ->GetParameter(1);
        Float_t energy_sigma = fitE->GetParameter(2)/fitE->GetParameter(1);
        h_E[energyIndex]->GetXaxis()->SetTitle("energy[MeV]");
        h_E[energyIndex]->Draw();
   TCanvas* myc_vertex=new TCanvas("myc_vertex","vertex distribution",10,10,900,500);
        myc_vertex->Divide(2,1);
        pro_vertex[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        pro_vertex[energyIndex]->GetYaxis()->SetTitle("R_{rec}-R_{edep}[mm]");
        pro_vertex[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        pro_vertex[energyIndex]->GetYaxis()->CenterTitle();
        pro_vertex[energyIndex]->SetLineColor(2);
        th2_vertex[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        th2_vertex[energyIndex]->GetYaxis()->SetTitle("R_{rec}-R_{edep}[mm]");
        th2_vertex[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        th2_vertex[energyIndex]->GetYaxis()->CenterTitle();
        myc_vertex->cd(1);
        th2_vertex[energyIndex]->Draw("colz");
        myc_vertex->cd(2);
        pro_vertex[energyIndex]->GetYaxis()->SetRangeUser(-3000,3000);
        pro_vertex[energyIndex]->Draw();
   TCanvas* myc_energy=new TCanvas("myc_energy","energy distribution",10,10,900,500);
        myc_energy->Divide(2,1);
        pro_energy[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        pro_energy[energyIndex]->GetYaxis()->SetTitle("energy[MeV]");
        pro_energy[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        pro_energy[energyIndex]->GetYaxis()->CenterTitle();
        pro_energy[energyIndex]->SetLineColor(2);
        th2_energy[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
        th2_energy[energyIndex]->GetYaxis()->SetTitle("energy[MeV]");
        th2_energy[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        th2_energy[energyIndex]->GetYaxis()->CenterTitle();
        myc_energy->cd(1);
        th2_energy[energyIndex]->Draw("colz");
        myc_energy->cd(2);
        pro_energy[energyIndex]->GetYaxis()->SetRangeUser(1.05*energyIndex-1,1.05*energyIndex+3);
        pro_energy[energyIndex]->Draw();

        vsigma[energyIndex]=vertex_sigma;
        emean[energyIndex]=energy_mean;
        esigma[energyIndex]=100*energy_sigma;
       cout<<"*******************************************"<<endl;
       cout<<"    vertex(Z-axis) resolution: "<<vertex_sigma<<" mm"<<endl;
       cout<<"    energy mean:"<< energy_mean<<" MeV" <<endl;
       cout<<"    energy resolution: "<<100*energy_sigma<<" % "<<endl;
       cout<<"*******************************************"<<endl;
       myc->SaveAs(Form("vertex_resloution_%dMeV.png",energyIndex+1));
       myc->SaveAs(Form("vertex_resloution_%dMeV.eps",energyIndex+1));
       myc1->SaveAs(Form("energy_resolution_%dMeV.png",energyIndex+1));
       myc1->SaveAs(Form("energy_resolution_%dMeV.eps",energyIndex+1));
       myc_vertex->SaveAs(Form("vertex_distribution_%dMeV.png",energyIndex+1));
       myc_vertex->SaveAs(Form("vertex_distribution_%dMeV.eps",energyIndex+1));
       myc_energy->SaveAs(Form("energy_distribution_%dMeV.png",energyIndex+1));
       myc_energy->SaveAs(Form("energy_distribution_%dMeV.eps",energyIndex+1));
 }
