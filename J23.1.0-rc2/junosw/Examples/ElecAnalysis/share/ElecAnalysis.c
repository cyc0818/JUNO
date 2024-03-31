#include <TH2F.h>
#include <TTree.h>
#include <math.h>
#include "TFile.h"
#include "TH2.h"
#include "TH1.h"
#include "TGraph.h"

void ElecAnalysis(){

   int m_nPMT;
   int m_nPulse;
   int m_nPE_perPMT[20000];
   int m_PMTID[20000];
   int m_adc_firstPE[1250];
   int m_tdc[1250];
   int m_adc_average[1250];

   TFile f("elec_user_20.root");//the input file

   TTree* sim = (TTree*)f.Get("SIMEVT");

   sim->SetBranchAddress("nPMT",&m_nPMT);
   sim->SetBranchAddress("nPulse",&m_nPulse);
   sim->SetBranchAddress("nPE_perPMT", m_nPE_perPMT);
   sim->SetBranchAddress("PMTID", m_PMTID);
   sim->SetBranchAddress("adc_firstPE", m_adc_firstPE);

   int nentries = sim->GetEntries();
   TH1F* h_nPMT = new TH1F("nPMT","nPMT",18000,0,18000);
   TH1F* h_nPulse = new TH1F("nPulse","nPulse",20000,0,20000);
   TH1F* h_nPE_perPMT = new TH1F("nPE_perPMT","nPE_perPMT",10,0,10);
   TH1F* h_PMTID = new TH1F("PMTID","PMTID",18000,0,18000);
   TH2D* adc_time = new TH2D("FADC_vs_time_allevents","FADC_vs_time_allevents",1250,0,1250,70,0,70);
   TH2D* adc_time_average = new TH2D("FADC_vs_time_average","FADC_vs_time_average",1250,0,1250,70,-10,60);



   for (int i=0;i<nentries;i++){
           sim->GetEntry(i);
           h_nPMT->Fill(m_nPMT);
           h_nPulse->Fill(m_nPulse);

       for(int j=0; j<m_nPMT;j++){
         h_nPE_perPMT->Fill(m_nPE_perPMT[j]);
         h_PMTID->Fill(m_PMTID[j]);
           }
             
   }
   for(int j=0; j<1250; j++){
             m_tdc[j] = j;
     for (int i=0;i<nentries;i++){
           sim->GetEntry(i);
           m_adc_average[j] += m_adc_firstPE[j]-16;
           adc_time->Fill(m_tdc[j],m_adc_firstPE[j]);
       }
           m_adc_average[j] =(m_adc_average[j]/nentries);          
           adc_time_average->Fill(m_tdc[j],m_adc_average[j]);

   }
     TFile f2("ElecAnalysis.root","recreate");//output file
    
         adc_time->GetXaxis()->SetTitle("time{ns}");
         adc_time->GetYaxis()->SetTitle("adc");
         adc_time_average->GetXaxis()->SetTitle("time{ns}");
         adc_time_average->GetYaxis()->SetTitle("adc");
    
         adc_time->Draw();
         adc_time_average->Draw();
         h_nPMT->Write();
         h_nPulse->Write();
         h_nPE_perPMT->Write();
         h_PMTID->Write();
      	 adc_time->Write();
         adc_time_average->Write();
         delete h_nPMT;
         delete h_nPulse;
         delete h_nPE_perPMT;
         delete h_PMTID;
         delete adc_time;
         delete adc_time_average;
   }
