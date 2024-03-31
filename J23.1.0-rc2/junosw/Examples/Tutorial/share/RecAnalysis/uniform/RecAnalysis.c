#define TH_MIN -2000
#define TH_MAX 2000
#define TH_NUM 300
#define EN_MIN 0.0
#define EN_MAX 10.0
#define EN_NUM 600
#include <iostream>
#include <sstream>
#include <stdlib.h>  
#include "stdio.h"  
#include <unistd.h>
using std::cout;
using std::endl;
float vsigma_wo[5];
float emean_wo[5];
float esigma_wo[5];
float vsigma[5];
float emean[5];
float esigma[5];
TH1F* h_X[5];
TH1F* h_Y[5];
TH1F* h_Z[5];
TH1F* h_Z_wo[5];
TH1F* h_E[5];
TH1F* h_E_wo[5];
TH1F* h_PE[5];
TH1F* h_PE_wo[5];
TH1F* h1_PE[5];
TH1F* h1_PE_wo[5];
TH1F* h1_E[5];
TH1F* h1_E_wo[5];
TH2D* th2_E[5];
TH2D* th2_E_wo[5];
TH2D* th3_E[5];
TH2D* th3_E_wo[5];
TH1F* h_V[5];
TH1F* h_V_wo[5];
TH1F* h1_V[5];
TH1F* h1_V_wo[5];
TH2D* th2_V[5];
TH2D* th2_V_wo[5];
TH2D* th3_V[5];
TH2D* th3_V_wo[5];
void RecAnalysis()
{
  for(int i=0;i<5;i++)
  {
    vsigma_wo[i]=0;
    emean_wo[i]=0;
    esigma_wo[i]=0;
    vsigma[i]=0;
    emean[i]=0;
    esigma[i]=0;
  }  
  std::string topdir = "/junofs/production/validation/J16v2r1-Pre4/zhaobq/Positron/uniform";
  vector<std::string> subdirs;
  subdirs.push_back("e+_0.0MeV/");
  subdirs.push_back("e+_1.0MeV/");
  subdirs.push_back("e+_3.0MeV/");
  subdirs.push_back("e+_5.0MeV/");
  subdirs.push_back("e+_7.0MeV/");
  int evtnum=20;
  int jobnum=100;
  int startseed=210;
  ofstream txtfile("result.txt");
  std::string dir="";
  std::string simdir="";
  std::string recdir="";
  std::string recwodir="";
  for(int i=0;i<5;i++)
  {
    dir=topdir+"/"+subdirs[i];
    simdir=dir+"detsim";
    recdir=dir+"rec";
    recwodir=dir+"rec-woelec";
    cout<<"energy:"<<subdirs[i]<<endl;
    RecAnalysisSingleEnergy(simdir.c_str(),recdir.c_str(),recwodir.c_str(), evtnum, jobnum, startseed, i);
    txtfile<<vsigma_wo[i]<<" "<<emean_wo[i]<<" "<<esigma_wo[i]<<" "<<vsigma[i]<<" "<<emean[i]<<" "<<esigma[i]<<std::endl;
  }
  txtfile.close();
}
void RecAnalysisSingleEnergy(const char* simFilePath,const char* recFilePath,const char* recwoFilePath,int evtNum,int jobNum,int& baseline, int energyIndex){
    gStyle->SetOptFit(1111);
    gSystem -> Load("$RECEVENTROOT/$CMTCONFIG/libRecEvent.so");
   
    h_E[energyIndex]= new TH1F(Form("Erec%d",energyIndex+1),Form("Erec_%d",energyIndex+1),600,energyIndex,energyIndex*2+3);
    h_PE[energyIndex]= new TH1F(Form("h_PE_%d",energyIndex+1),Form("total_PE",energyIndex+1),600,(energyIndex)*1300., (energyIndex*2+3)*1300.);
    h1_E[energyIndex]= new TH1F(Form("(Erec-Edep)/Edep_%d",energyIndex+1),Form("(Erec-Edep)/Edep_%d",energyIndex+1),100,-1,1);
    h1_PE[energyIndex]= new TH1F(Form("(nPE_rec-nPE_dep)/nPE_dep_%d",energyIndex+1),Form("(nPE_rec-nPE_dep)/nPE_dep_%d",energyIndex+1),300,-0.3,0.3);
    th2_E[energyIndex]= new TH2D(Form("(Erec-Edep)/Edep vs Rdep^{3}",energyIndex+1),Form("(Erec-Edep)/Edep vs Rdep^{3}_%d",energyIndex+1),EN_NUM,-0,6000,EN_NUM,-1,1);
    th3_E[energyIndex]= new TH2D(Form("(Erec-Edep)/Edep vs Edep",energyIndex+1),Form("(Erec-Edep)/Edep vs Edep_%d",energyIndex+1),EN_NUM,0,10,EN_NUM,-1,1);
  
    h_Z[energyIndex]= new TH1F(Form("Z_{rec}-Z_{dep}",energyIndex+1),Form("Z_{rec}-Z_{dep}_%d",energyIndex+1),1000,-2000,2000);
    h_V[energyIndex]= new TH1F(Form("Rrec-Rdep",energyIndex+1),Form("Rrec-Rdep_%d:",energyIndex+1),1000,-2000,2000); 
    h1_V[energyIndex]= new TH1F(Form("(Rrec-Rdep)/Rdep_%d",energyIndex+1),Form("(Rrec-Rdep)/Rdep_%d",energyIndex+1),TH_NUM,-0.2,0.2);
    th2_V[energyIndex]= new TH2D(Form("(Rrec-Rdep)/Rdep vs Rdep^{3}",energyIndex+1),Form("(Rrec-Rdep)/Rdep vs Rdep^{3}_%d",energyIndex+1),TH_NUM,-0,6000,TH_NUM,-1,1);
    th3_V[energyIndex]= new TH2D(Form("(Rrec-Rdep)/Rdep vs Edep",energyIndex+1),Form("(Rrec-Rdep)/Rdep vs Edep_%d",energyIndex+1),TH_NUM,0,10,TH_NUM,-1,1);
   
    h_E_wo[energyIndex]= new TH1F(Form("Erec_wo_%d",energyIndex+1),Form("Erec_%d",energyIndex+1),600,energyIndex,energyIndex*2+3);  
    h_PE_wo[energyIndex]= new TH1F(Form("h_PE_wo_%d",energyIndex+1),Form("total_PE",energyIndex+1),600,(energyIndex)*1300., (energyIndex*2+3)*1300.);
    h1_E_wo[energyIndex]= new TH1F(Form("(Erec-Edep)/Edep_wo_%d",energyIndex+1),Form("(Erec-Edep)/Edep_%d",energyIndex+1),100,-1,1);
    h1_PE_wo[energyIndex]= new TH1F(Form("(nPE_rec-nPE_dep)/nPE_dep_wo%d",energyIndex+1),Form("(nPE_rec-nPE_dep)/nPE_dep_%d",energyIndex+1),300,-0.3,0.3);
    th2_E_wo[energyIndex]= new TH2D(Form("(Erec-Edep)/Edep vs Rdep^{3}_wo",energyIndex+1),Form("(Erec-Edep)/Edep vs Rdep^{3}_%d",energyIndex+1),EN_NUM,-0,6000,EN_NUM,-1,1);
    th3_E_wo[energyIndex]= new TH2D(Form("(Erec-Edep)/Edep vs Edep_wo",energyIndex+1),Form("(Erec-Edep)/Edep vs Edep_%d",energyIndex+1),EN_NUM,0,10,EN_NUM,-1,1);

    h_Z_wo[energyIndex]= new TH1F(Form("Z_{rec}-Z_{dep}_wo",energyIndex+1),Form("Z_{rec}-Z_{dep}_%d",energyIndex+1),1000,-2000,2000);
    h_V_wo[energyIndex]= new TH1F(Form("Rrec-Rdep_wo",energyIndex+1),Form("Rrec-Rdep_%d:",energyIndex+1),1000,-2000,2000);
    h1_V_wo[energyIndex]= new TH1F(Form("(Rrec-Rdep)/Rdep_wo_%d",energyIndex+1),Form("(Rrec-Rdep)/Rdep_%d",energyIndex+1),TH_NUM,-0.2,0.2);
    th2_V_wo[energyIndex]= new TH2D(Form("(Rrec-Rdep)/Rdep vs Rdep^{3}_wo",energyIndex+1),Form("(Rrec-Rdep)/Rdep vs Rdep^{3}_%d",energyIndex+1),TH_NUM,-0,6000,TH_NUM,-1,1);
    th3_V_wo[energyIndex]= new TH2D(Form("(Rrec-Rdep)/Rdep vs Edep_wo",energyIndex+1),Form("(Rrec-Rdep)/Rdep vs Edep_%d",energyIndex+1),TH_NUM,0,10,TH_NUM,-1,1); 
  
    TChain ch_sim("evt");
    TChain ch_rec("Event/RecEvent/RecHeader");
    TChain ch_rec_wo("Event/RecEvent/RecHeader");
       for(int j=0;j<jobNum;j++){
         string n_flag;
         stringstream ss;
         int k = baseline + j + energyIndex*jobNum;
         ss << k;
         ss >>n_flag;
         TString simFileAdd = Form("%s/user-detsim-%s.root",simFilePath,n_flag.c_str());
         TString recFileAdd = Form("%s/rec-%s.root",recFilePath,n_flag.c_str());
         TString recwoFileAdd = Form("%s/rec-woelec-%s.root",recwoFilePath,n_flag.c_str());
         ch_sim.Add(simFileAdd);
         ch_rec.Add(recFileAdd);
         ch_rec_wo.Add(recwoFileAdd);
         }
            TObjArray* simFileElements=ch_sim.GetListOfFiles();
            TObjArray* recFileElements=ch_rec.GetListOfFiles();
            TObjArray* recwoFileElements=ch_rec_wo.GetListOfFiles();
            TIter simnext(simFileElements);
            TIter recnext(recFileElements);
            TIter recwonext(recwoFileElements);
            TChainElement* simChEl=0;
            TChainElement* recChEl=0;
            TChainElement* recwoChEl=0;            
            while (( recChEl=(TChainElement*)recnext() )&&( recwoChEl=(TChainElement*)recwonext())){
                simChEl=(TChainElement*)simnext();
                TFile* simf =new TFile(simChEl->GetTitle());
                TFile* recf =new TFile(recChEl->GetTitle());
                TFile* rec_wof =new TFile(recwoChEl->GetTitle());
                cout<<simChEl->GetTitle()<<","<<recChEl->GetTitle()<<endl;
               
                TTree* rec_ch = (TTree*)recf -> Get("Event/RecEvent/RecHeader");
                rh = new JM::RecHeader();
                rec_ch->SetBranchAddress("RecHeader",&rh);
               
                TTree* rec_wo_ch = (TTree*)rec_wof -> Get("Event/RecEvent/RecHeader");
                rh_wo = new JM::RecHeader();
                rec_wo_ch->SetBranchAddress("RecHeader",&rh_wo);
               
                TTree* sim_ch = (TTree*)simf -> Get("evt");
                Float_t sim_x =0;
                Float_t sim_y =0;
                Float_t sim_z =0;
                Float_t sim_E =0;
                Int_t sim_nPE =0;
                sim_ch -> SetBranchAddress("edepX",&sim_x);
                sim_ch -> SetBranchAddress("edepY",&sim_y);
                sim_ch -> SetBranchAddress("edepZ",&sim_z);
                sim_ch -> SetBranchAddress("edep",&sim_E);
                sim_ch -> SetBranchAddress("totalPE",&sim_nPE);
                for(int i=0;i<evtNum;i++)
               {
                sim_ch ->GetEntry(i);
                rec_ch ->GetEntry(i);
                rec_wo_ch ->GetEntry(i);
                Float_t total_PE = rh->peSum();
                Float_t total_PE_wo = rh_wo->peSum();                
                Float_t rec_x = rh->x();
                Float_t rec_wo_x = rh_wo->x();
                Float_t rec_y = rh->y();
                Float_t rec_wo_y = rh_wo->y();
                Float_t rec_z = rh->z();
                Float_t rec_wo_z = rh_wo->z();
                Float_t rec_E = rh->eprec();
                Float_t rec_wo_E = rh_wo->eprec(); 
                Float_t diff_x = rec_x - sim_x;
                Float_t diff_wo_x = rec_wo_x - sim_x;
                Float_t diff_y = rec_y - sim_y;
                Float_t diff_wo_y = rec_wo_y - sim_y;
                Float_t diff_z = rec_z - sim_z;
                Float_t diff_wo_z = rec_wo_z - sim_z;
                Float_t diff_E = rec_E / sim_E;
                Float_t diff_wo_E = rec_wo_E / sim_E;
                Float_t rec_r = sqrt(rec_x*rec_x + rec_y*rec_y + rec_z*rec_z);
                Float_t rec_wo_r = sqrt(rec_wo_x*rec_wo_x + rec_wo_y*rec_wo_y + rec_wo_z*rec_wo_z);
                Float_t edep_r = sqrt(sim_x*sim_x + sim_y*sim_y + sim_z*sim_z);
                Float_t delta_E = rec_E - sim_E;
                Float_t delta_wo_E = rec_wo_E - sim_E;
                Float_t e1 = delta_E / sim_E;
                Float_t e1_wo = delta_wo_E / sim_E;
                Float_t delta_r = rec_r - edep_r;
                Float_t delta_wo_r = rec_wo_r - edep_r;
                Float_t r1 = delta_r / edep_r;
                Float_t r1_wo = delta_wo_r / edep_r;
                Float_t dPE = total_PE - sim_nPE;
                Float_t dPE_wo = total_PE_wo - sim_nPE;
                Float_t PE = dPE / sim_nPE;
                Float_t PE_wo = dPE_wo / sim_nPE;
                h_E[energyIndex]->Fill(rec_E);
                h_E_wo[energyIndex]->Fill(rec_wo_E);
                h1_E[energyIndex]->Fill(e1); 
                h1_E_wo[energyIndex]->Fill(e1_wo);
                h_PE[energyIndex] ->Fill(total_PE);
                h_PE_wo[energyIndex] ->Fill(total_PE_wo);
                h1_PE[energyIndex] ->Fill(PE);
                h1_PE_wo[energyIndex] ->Fill(PE_wo);
                h_Z[energyIndex] -> Fill(diff_z,1);
                h_Z_wo[energyIndex] -> Fill(diff_wo_z,1);
                h_V[energyIndex] -> Fill(delta_r,1);
                h_V_wo[energyIndex] -> Fill(delta_wo_r,1);
                h1_V[energyIndex]->Fill(r1); 
                h1_V_wo[energyIndex]->Fill(r1_wo);
                th2_E[energyIndex]->Fill(pow(edep_r/1000,3),e1,1);
                th2_E_wo[energyIndex]->Fill(pow(edep_r/1000,3),e1_wo,1);
                th3_E[energyIndex]->Fill(sim_E,e1,1);
                th3_E_wo[energyIndex]->Fill(sim_E,e1_wo,1);
                th2_V[energyIndex]->Fill(pow(edep_r/1000,3),r1,1);
                th2_V_wo[energyIndex]->Fill(pow(edep_r/1000,3),r1_wo,1);
                th3_V[energyIndex]->Fill(sim_E,r1,1);
                th3_V_wo[energyIndex]->Fill(sim_E,r1_wo,1);
               }     
                simf->Close();
                recf->Close();
                rec_wof->Close();
     }

        h_Z_wo[energyIndex]->SetLineColor(1);
        h_Z_wo[energyIndex]->Fit("gaus","W","C",-2000,2000);
        TF1  *fit_Z_wo =  (TF1*)h_Z_wo[energyIndex]->GetFunction("gaus");
             fit_Z_wo->SetLineColor(3);
        h_Z[energyIndex]->Fit("gaus","W","C",-2000,2000);

        h_V_wo[energyIndex]->SetLineColor(1);
        h_V_wo[energyIndex]->Fit("gaus","W","C",-2000,2000);
        TF1  *fit_V_wo =  (TF1*)h_V_wo[energyIndex]->GetFunction("gaus");
             fit_V_wo->SetLineColor(3);
        h_V[energyIndex]->Fit("gaus","W","C",-2000,2000);


        h_PE_wo[energyIndex]-> Fit("gaus","W","C",(energyIndex)*1300., (energyIndex*2+3)*1300.);
          TF1  *fit_PE_wo =  (TF1*)h_PE_wo[energyIndex]->GetFunction("gaus");
             fit_PE_wo->SetLineColor(3);
        h1_PE_wo[energyIndex]-> Fit("gaus","W","C",-0.3,0.3);
            TF1  *fit1_PE_wo =  (TF1*)h1_PE_wo[energyIndex]->GetFunction("gaus");
             fit1_PE_wo->SetLineColor(3);
        h_PE[energyIndex]-> Fit("gaus","W","C",(energyIndex)*1300., (energyIndex*2+3)*1300.);
        h1_PE[energyIndex]-> Fit("gaus","W","C",-0.3,0.3);

        h_Z[energyIndex]->GetXaxis()->SetTitle("Zrec-Zdep");
        h_Z[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        h_Z[energyIndex]->GetYaxis()->CenterTitle();
        h_V[energyIndex]->GetXaxis()->SetTitle("Rrec-Rdep");
        h_V[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        h_V[energyIndex]->GetYaxis()->CenterTitle();
        h_Z_wo[energyIndex]->GetXaxis()->SetTitle("Zrec-Zdep");
        h_Z_wo[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        h_Z_wo[energyIndex]->GetYaxis()->CenterTitle();
        h_V_wo[energyIndex]->GetXaxis()->SetTitle("Rrec-Rdep");
        h_V_wo[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
        h_V_wo[energyIndex]->GetYaxis()->CenterTitle();

        h_E_wo[energyIndex]->Fit("gaus","W","C",energyIndex,energyIndex*2+3);
        TF1  *fit_E_wo =  (TF1*)h_E_wo[energyIndex]->GetFunction("gaus");
             fit_E_wo->SetLineColor(3);
        h_E[energyIndex]->GetXaxis()->SetTitle("Erec");
        h_E[energyIndex]->Fit("gaus","W","C",energyIndex,energyIndex*2+3);

        h1_V_wo[energyIndex]->Fit("gaus","W","C",-0.2,0.2);
        h1_V_wo[energyIndex]->GetXaxis()->SetTitle("(Rrec-Rdep)/Rdep");
       TF1  *fit1_V_wo =  (TF1*)h1_V_wo[energyIndex]->GetFunction("gaus");
             fit1_V_wo->SetLineColor(3);

        h1_V[energyIndex]->Fit("gaus","W","C",-0.2,0.2);
        h1_V[energyIndex]->GetXaxis()->SetTitle("(Rrec-Rdep)/Rdep");

        h1_E_wo[energyIndex]->Fit("gaus","W","C",-1,1);
        TF1  *fit1_E_wo =  (TF1*)h1_E_wo[energyIndex]->GetFunction("gaus");
             fit1_E_wo->SetLineColor(3);
        h1_E_wo[energyIndex]->GetXaxis()->SetTitle("(Erec-Edep)/Edep");
        h1_E[energyIndex]->Fit("gaus","W","C",-1,1);
        h1_E[energyIndex]->GetXaxis()->SetTitle("(Erec-Edep)/Edep");

        TF1  *fitPE = (TF1*)h_PE_wo[energyIndex]->GetFunction("gaus");
        Float_t total_PE_mean = fitPE ->GetParameter(1);
        Float_t total_PE_sigma = fitPE ->GetParameter(2);
        h_PE_wo[energyIndex]->GetXaxis()->SetTitle("total_PE");
        h1_PE_wo[energyIndex]->GetXaxis()->SetTitle("(nPE_rec-nPE_dep)/nPE_dep");

  TCanvas* vertex_z=new TCanvas("vertex_z","a canvas",10,10,900,500);
        TPad *pad1 = new TPad("pad1","",0,0,1,1);
        pad1->Draw();
        pad1->cd();
     h_Z_wo[energyIndex]->Draw();
        pad1->Update();
        TPaveStats *ps1 = (TPaveStats*)h_Z_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
           ps1->SetX1NDC(0.1);
           ps1->SetX2NDC(0.3);
           ps1->SetTextColor(3);
           pad1->Modified();

      vertex_z->cd();
        pad1->Draw();
        pad1->cd();
        h_Z[energyIndex]->Draw("sames");
        pad1->Update();
        TPaveStats *ps2 = (TPaveStats*)h_Z[energyIndex]->GetListOfFunctions()->FindObject("stats");
           ps2->SetX1NDC(0.7);
           ps2->SetX2NDC(0.9);
           ps2->SetTextColor(2);

  TCanvas* vertex_r=new TCanvas("vertex_r","a canvas",10,10,900,500);
        TPad *pad1_r = new TPad("pad1_r","",0,0,1,1);
        pad1_r->Draw();
        pad1_r->cd();
     h_V_wo[energyIndex]->Draw();
        pad1_r->Update();
        TPaveStats *ps1_r = (TPaveStats*)h_V_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
           ps1_r->SetX1NDC(0.1);
           ps1_r->SetX2NDC(0.3);
           ps1_r->SetTextColor(3);
           pad1_r->Modified();
           vertex_r->cd();
        pad1_r->Draw();
        pad1_r->cd();
        h_V[energyIndex]->Draw("sames");
        pad1_r->Update();
        TPaveStats *ps2_r = (TPaveStats*)h_V[energyIndex]->GetListOfFunctions()->FindObject("stats");
           ps2_r->SetX1NDC(0.7);
           ps2_r->SetX2NDC(0.9);
         ps2_r->SetTextColor(2);

   TCanvas* erec=new TCanvas("erec","a canvas",10,10,900,500);
     TPad *pad1_E = new TPad("pad1_E","",0,0,1,1);
          pad1_E->Draw();
          pad1_E->cd();
      h_E_wo[energyIndex]->Draw();
          pad1_E->Update();
         TPaveStats *ps1_E = (TPaveStats*)h_E_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps1_E->SetX1NDC(0.1);
            ps1_E->SetX2NDC(0.3);
            ps1_E->SetTextColor(3);
            pad1_E->Modified();
            erec->cd();
            pad1_E->Draw();
            pad1_E->cd();
            h_E[energyIndex]->SetLineColor(1);
            h_E[energyIndex]->Draw("sames");
            pad1_E->Update();
          TPaveStats *ps2_E = (TPaveStats*)h_E[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps2_E->SetX1NDC(0.7);
            ps2_E->SetX2NDC(0.9);
            ps2_E->SetTextColor(2);

    TCanvas* vertex1=new TCanvas("vertex1","a canvas",10,10,900,500);

     TPad *pad1_V = new TPad("pad1_V","",0,0,1,1);
          pad1_V->Draw();
          pad1_V->cd();
      h1_V_wo[energyIndex]->Draw();
          pad1_V->Update();
         TPaveStats *ps1_V = (TPaveStats*)h1_V_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps1_V->SetX1NDC(0.1);
            ps1_V->SetX2NDC(0.3);
            ps1_V->SetTextColor(3);
            pad1_V->Modified();
            vertex1->cd();
            pad1_V->Draw();
            pad1_V->cd();
            h1_V[energyIndex]->SetLineColor(1);
            h1_V[energyIndex]->Draw("sames");
            pad1_V->Update();
          TPaveStats *ps2_V = (TPaveStats*)h1_V[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps2_V->SetX1NDC(0.7);
            ps2_V->SetX2NDC(0.9);
            ps2_V->SetTextColor(2);

    TCanvas* erec1=new TCanvas("erec1","a canvas",10,10,900,500);

      TPad *pad1_e = new TPad("pad1_e","",0,0,1,1);
          pad1_e->Draw();
          pad1_e->cd();
         h1_E_wo[energyIndex]->Draw();
           pad1_e->Update();
         TPaveStats *ps1_e = (TPaveStats*)h1_E_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps1_e->SetX1NDC(0.1);
            ps1_e->SetX2NDC(0.3);
            ps1_e->SetTextColor(3);
            pad1_e->Modified();
            erec1->cd();
            pad1_e->Draw();
            pad1_e->cd();
            h1_E[energyIndex]->SetLineColor(1);
            h1_E[energyIndex]->Draw("sames");
            pad1_e->Update();
          TPaveStats *ps2_e = (TPaveStats*)h1_E[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps2_e->SetX1NDC(0.7);
            ps2_e->SetX2NDC(0.9);
            ps2_e->SetTextColor(2);

    TCanvas* npe=new TCanvas("npe","a canvas",10,10,900,500);

     TPad *pad1_npe = new TPad("pad1_npe","",0,0,1,1);
          pad1_npe->Draw();
          pad1_npe->cd();
      h_PE_wo[energyIndex]->Draw();
          pad1_npe->Update();
         TPaveStats *ps1_npe = (TPaveStats*)h_PE_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps1_npe->SetX1NDC(0.1);
            ps1_npe->SetX2NDC(0.3);
            ps1_npe->SetTextColor(3);
            pad1_npe->Modified();
            npe->cd();
            pad1_npe->Draw();
            pad1_npe->cd();
            h_PE[energyIndex]->SetLineColor(1);
            h_PE[energyIndex]->Draw("sames");
            pad1_npe->Update();
          TPaveStats *ps2_npe = (TPaveStats*)h_PE[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps2_npe->SetX1NDC(0.7);
            ps2_npe->SetX2NDC(0.9);
            ps2_npe->SetTextColor(2);

  TCanvas* pe=new TCanvas("pe","a canvas",10,10,900,500);

     TPad *pad1_pe = new TPad("pad1_pe","",0,0,1,1);
          pad1_pe->Draw();
          pad1_pe->cd();
      h1_PE_wo[energyIndex]->Draw();
          pad1_pe->Update();
         TPaveStats *ps1_pe = (TPaveStats*)h1_PE_wo[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps1_pe->SetX1NDC(0.1);
            ps1_pe->SetX2NDC(0.3);
            ps1_pe->SetTextColor(3);
            pad1_pe->Modified();
            pe->cd();
            pad1_pe->Draw();
            pad1_pe->cd();
            h1_PE[energyIndex]->SetLineColor(1);
            h1_PE[energyIndex]->Draw("sames");
            pad1_pe->Update();
          TPaveStats *ps2_pe = (TPaveStats*)h1_PE[energyIndex]->GetListOfFunctions()->FindObject("stats");
            ps2_pe->SetX1NDC(0.7);
            ps2_pe->SetX2NDC(0.9);
            ps2_pe->SetTextColor(2);


    TCanvas new TCanvas("myc_vertex","vertex distribution",10,10,900,500);
        myc_vertex->Divide(2,1);
         th2_V[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
         th2_V[energyIndex]->GetYaxis()->SetTitle("(Rrec-Rdep)/Rdep");
         th2_V[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th2_V[energyIndex]->GetYaxis()->CenterTitle();
         th2_V[energyIndex]->SetLineColor(2);
         th3_V[energyIndex]->GetXaxis()->SetTitle("Edep[MeV]");
         th3_V[energyIndex]->GetYaxis()->SetTitle("(Rrec-Rdep)/Rdep");
         th3_V[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th3_V[energyIndex]->GetYaxis()->CenterTitle();
        myc_vertex->cd(1);
         th3_V[energyIndex]->Draw("colz");
        myc_vertex->cd(2);
         th2_V[energyIndex]->Draw("colz");
  TCanvas* myc_vertex_wo=new TCanvas("myc_vertex_wo","vertex distribution",10,10,900,500);
        myc_vertex_wo->Divide(2,1);
         th2_V_wo[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
         th2_V_wo[energyIndex]->GetYaxis()->SetTitle("(Rrec-Rdep)/Rdep");
         th2_V_wo[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th2_V_wo[energyIndex]->GetYaxis()->CenterTitle();
         th2_V_wo[energyIndex]->SetLineColor(2);
         th3_V_wo[energyIndex]->GetXaxis()->SetTitle("Edep[MeV]");
         th3_V_wo[energyIndex]->GetYaxis()->SetTitle("(Rrec-Rdep)/Rdep");
         th3_V_wo[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th3_V_wo[energyIndex]->GetYaxis()->CenterTitle();
        myc_vertex_wo->cd(1);
         th3_V_wo[energyIndex]->Draw("colz");
        myc_vertex_wo->cd(2);
         th2_V_wo[energyIndex]->Draw("colz");


     TCanvas* myc_energy=new TCanvas("myc_energy","energy distribution",10,10,900,500);
         myc_energy->Divide(2,1);
         th2_E[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
         th2_E[energyIndex]->GetYaxis()->SetTitle("(Erec-Edep)/Edep");
         th2_E[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th2_E[energyIndex]->GetYaxis()->CenterTitle();
         th2_E[energyIndex]->SetLineColor(2);
         th3_E[energyIndex]->GetXaxis()->SetTitle("Edep[MeV]");
         th3_E[energyIndex]->GetYaxis()->SetTitle("(Erec-Edep)/Edep");
         th3_E[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th3_E[energyIndex]->GetYaxis()->CenterTitle();
        myc_energy->cd(1);
         th2_E[energyIndex]->Draw("colz");
        myc_energy->cd(2);
         th3_E[energyIndex]->Draw("colz");

  TCanvas* myc_energy_wo=new TCanvas("myc_energy_wo","energy distribution",10,10,900,500);
         myc_energy_wo->Divide(2,1);
         th2_E_wo[energyIndex]->GetXaxis()->SetTitle("R_{edep}^{3}[m^{3}]");
         th2_E_wo[energyIndex]->GetYaxis()->SetTitle("(Erec-Edep)/Edep");
         th2_E_wo[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th2_E_wo[energyIndex]->GetYaxis()->CenterTitle();
         th2_E_wo[energyIndex]->SetLineColor(2);
         th3_E_wo[energyIndex]->GetXaxis()->SetTitle("Edep[MeV]");
         th3_E_wo[energyIndex]->GetYaxis()->SetTitle("(Erec-Edep)/Edep");
         th3_E_wo[energyIndex]->GetYaxis()->SetTitleOffset(1.2);
         th3_E_wo[energyIndex]->GetYaxis()->CenterTitle();
        myc_energy_wo->cd(1);
         th2_E_wo[energyIndex]->Draw("colz");
        myc_energy_wo->cd(2);
         th3_E_wo[energyIndex]->Draw("colz");    

  TF1  *fit1_V_wo =  (TF1*)h1_V_wo[energyIndex]->GetFunction("gaus");
             fit1_V_wo->SetLineColor(3);
        TF1  *fitV_wo =  (TF1*)h_V_wo[energyIndex]->GetFunction("gaus");
        Float_t vertex_sigma_wo = fitV_wo->GetParameter(2);
  TF1  *fit_E_wo =  (TF1*)h_E_wo[energyIndex]->GetFunction("gaus");
             fit_E_wo->SetLineColor(3);
        TF1  *fitE_wo = (TF1*)h_E_wo[energyIndex]->GetFunction("gaus");
        Float_t energy_mean_wo = fitE_wo->GetParameter(1);
        Float_t energy_sigma_wo = fitE_wo->GetParameter(2)/fitE_wo->GetParameter(1);

  TF1  *fit1_V =  (TF1*)h1_V[energyIndex]->GetFunction("gaus");
             fit1_V->SetLineColor(2);
        TF1  *fitV =  (TF1*)h_V[energyIndex]->GetFunction("gaus");
        Float_t vertex_sigma = fitV->GetParameter(2);
  TF1  *fit_E =  (TF1*)h_E[energyIndex]->GetFunction("gaus");
             fit_E->SetLineColor(2);
        TF1  *fitE = (TF1*)h_E[energyIndex]->GetFunction("gaus");
        Float_t energy_mean = fitE->GetParameter(1);
        Float_t energy_sigma = fitE->GetParameter(2)/fitE->GetParameter(1);
   

  TFile *f1 = new TFile(Form("Plot_%d.root",energyIndex+1),"RECREATE");
           vertex_z->Write();
        vertex_r->Write();
        erec->Write();
        vertex1->Write();
        erec1->Write();
        npe->Write();
        pe->Write();
        myc_vertex->Write();
        myc_energy->Write();
        myc_vertex_wo->Write();
        myc_energy_wo->Write();
        f1->Close();  
 
        vsigma_wo[energyIndex]=vertex_sigma_wo;
        emean_wo[energyIndex]=energy_mean_wo;
        esigma_wo[energyIndex]=100*energy_sigma_wo;
        vsigma[energyIndex]=vertex_sigma;
        emean[energyIndex]=energy_mean;
        esigma[energyIndex]=100*energy_sigma;
       cout<<"*******************************************"<<endl;
       cout<<"    vertex(R)_wo resolution: "<<vertex_sigma_wo<<" mm"<<endl;
       cout<<"    energy_wo mean:"<< energy_mean_wo<<" MeV" <<endl;
       cout<<"    energy_wo resolution: "<<100*energy_sigma_wo<<" % "<<endl;
       cout<<"    vertex(R) resolution: "<<vertex_sigma<<" mm"<<endl;
       cout<<"    energy mean:"<< energy_mean<<" MeV" <<endl;
       cout<<"    energy resolution: "<<100*energy_sigma<<" % "<<endl;
       cout<<"*******************************************"<<endl;
    
       vertex_z->SaveAs(Form("vertex_z_bias%d.png",energyIndex+1));
       vertex_r->SaveAs(Form("vertex_r_bias%d.png",energyIndex+1));
       erec->SaveAs(Form("Erec_%d.png",energyIndex+1));
       vertex1->SaveAs(Form("Vertex_%d.png",energyIndex+1));
       erec1->SaveAs(Form("Energy_%d.png",energyIndex+1));
       myc_vertex->SaveAs(Form("vertex_distribution_%d.png",energyIndex+1));
       myc_energy->SaveAs(Form("energy_distribution_%d.png",energyIndex+1));
       myc_vertex_wo->SaveAs(Form("vertex_wo_distribution_%d.png",energyIndex+1));
       myc_energy->SaveAs(Form("energy_wo_distribution_%d.png",energyIndex+1));
       npe->SaveAs(Form("totalPE_%d.png",energyIndex+1));
       pe->SaveAs(Form("nPE_%d.png",energyIndex+1));

   }
