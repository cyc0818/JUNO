#include <iostream>
#include <fstream>
#include <TTree.h>
#include <TChain.h>
#include <math.h>
#include <vector>
#include <TFile.h>
#include <TH2.h>
#include <TH1.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMath.h>
#include <TSystem.h>
#include <TROOT.h>
#include <Event/SimEvent.h>
#include <Event/SimPMTHit.h>
#include <Event/SimTrack.h>

using std::cout;
using std::endl;

void CalibAnalysisMu(TString inputcalibpath="lists_calib_mu.txt", TString inputsimpath="lists_detsim_mu.txt",  TString outputpath="calib_ana/calib_ana_mu.root"){
    gSystem->Load("libSimEventV2.so");
    //==========================================
    // Step width in D of samplesize, at the moment 4m steps from 0 to 16 
    const int stepWidth = 4;
    const int nSteps = 17/stepWidth +1;
    const int PMTN = 17739;
    //==========================================
    // Init of histos and graphs
    TH1D *ha_fht[nSteps];
    TH1D *ha_nPE[nSteps];
    TH1D *ha_PMT[nSteps];
    int entry_cnt[nSteps];
    int totalPE_calib_cnt[nSteps];
    int totalPE_sim_cnt[nSteps];
    std::string ids[nSteps];
    char number[2];
    for(Int_t i=0; i<nSteps; i++) {
        sprintf(number,"%02i",i*stepWidth);
        ids[i] = std::string(number);
    }
    for(Int_t n=0; n<nSteps; n++){
        ha_fht[n] = new TH1D( ("fht_"+ids[n]+"_calib").c_str(), (ids[n]).c_str(), 750, 0, 750);
        ha_nPE[n] = new TH1D( ("nPE_"+ids[n]+"_calib").c_str(), (ids[n]).c_str(), 1000, 0, 10000);
        ha_PMT[n] = new TH1D( ("PMT_"+ids[n]+"_calib").c_str(), (ids[n]).c_str(), 17746, 0, 17746);
        ha_fht[n]->GetXaxis()->SetTitle("fht [ns]");
        ha_fht[n]->GetYaxis()->SetTitle("entries");
        ha_nPE[n]->GetXaxis()->SetTitle("nPE");
        ha_nPE[n]->GetYaxis()->SetTitle("entries");
        ha_PMT[n]->GetXaxis()->SetTitle("PMT ID");
        ha_PMT[n]->GetYaxis()->SetTitle("avg. fire rate");
        entry_cnt[n] = 0;
        totalPE_calib_cnt[n] = 0;
        totalPE_sim_cnt[n] = 0;
    }
    TGraph *g_totPE_D = new TGraph();
    g_totPE_D->SetName("g_totPE_D_calib");
    g_totPE_D->GetXaxis()->SetTitle("D [m]");
    g_totPE_D->GetYaxis()->SetTitle("avg. total PE per event");
    TH1D *h_diff_fht = new TH1D("h_diff_fht", "Deviation in fht", 1000, -500, 500);
    TH1D *h_diff_nPE = new TH1D("h_diff_nPE", "Deviation in nPE", 1000, -5000, 5000);
    TH1D *h_diff_totnPE = new TH1D("h_diff_totnPE", "Deviation in total nPE", 10000, -50000, 50000);
    h_diff_fht->GetXaxis()->SetTitle("fht_{sim} - fht_{rec} [ns]");
    h_diff_fht->GetYaxis()->SetTitle("entries");
    h_diff_nPE->GetXaxis()->SetTitle("nPE_{sim} - nPE_{rec}");
    h_diff_nPE->GetYaxis()->SetTitle("entries");
    h_diff_totnPE->GetXaxis()->SetTitle("nPE_{tot,sim} - nPE_{tot,rec}");
    h_diff_totnPE->GetYaxis()->SetTitle("entries");
    TH2D *h2_diff_fht = new TH2D("h2_diff_fht", "Diff of fht vs true nPE", 2000, 0, 20000, 1000, -500, 500);
    TH2D *h2_diff_nPE = new TH2D("h2_diff_nPE", "Diff of nPE vs true nPE", 2000, 0, 20000, 10000, -50000, 50000);
    h2_diff_fht->GetXaxis()->SetTitle("nPE_{sim}");
    h2_diff_fht->GetYaxis()->SetTitle("fht_{sim} - fht_{rec} [ns]");
    h2_diff_nPE->GetXaxis()->SetTitle("nPE_{sim}");
    h2_diff_nPE->GetYaxis()->SetTitle("nPE_{sim} - nPE_{rec}");
    //==========================================
    // Init of variables to hold tree content
    vector<float>* charge;
    vector<float>* time;
    vector<int>* calib_PMTID;
    float totalcharge;
    charge = NULL;
    time = NULL;
    calib_PMTID = NULL;
    std::vector<TString> sim_files_list;
    std::vector<TString> calib_files_list;
    //==========================================
    // Get all Calib files into chain
    std::ifstream cfp(inputcalibpath);
    if (!cfp.is_open()) {
        std::cerr << "ERROR: input lists does not exists." << std::endl;
        return;
    }
    std::string calib_tmp_line;
    while(cfp.good()) {
        std::getline(cfp, calib_tmp_line);
        if ( calib_tmp_line.size() ==0 ) {
            continue;
        }
        calib_files_list.push_back(calib_tmp_line);
    }
    TChain* calib_ch = new TChain("CALIBEVT");
    for (std::vector<TString>::iterator it = calib_files_list.begin();
            it != calib_files_list.end(); ++it) {
        std::cout << "add calib file: " << *it << std::endl;
        calib_ch->Add(*it);
    }
    //==========================================
    // Get all Sim files into chain
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
    TChain* sim_ch = new TChain("Event/Sim/SimEvent");
    for (std::vector<TString>::iterator it = sim_files_list.begin();
            it != sim_files_list.end(); ++it) {
        std::cout << "add sim file: " << *it << std::endl;
        sim_ch->Add(*it);
    }
    //==========================================
    // Set branch addresses
    TTree* calib = (TTree*)calib_ch;
    calib->SetBranchAddress("Charge",&charge);
    calib->SetBranchAddress("Time",&time);
    calib->SetBranchAddress("PMTID",&calib_PMTID);
    calib->SetBranchAddress("TotalPE",&totalcharge);

    TTree* sim = (TTree*)sim_ch;
    JM::SimEvent* se = new JM::SimEvent();
    sim->SetBranchAddress("SimEvent",&se);
    sim->GetBranch("SimEvent")->SetAutoDelete(true);

    if(calib->GetEntries()!=sim->GetEntries()){
        std::cerr<<"CalibVal: Number of events in calib_user file and detsim_user file is not equal! CalibEvents: "<<calib->GetEntries()<<"\t DetsimEvents: "<<sim->GetEntries()<<std::endl;
        return; 
    }
    //==========================================
    // Loop & fill
    int nentries = calib->GetEntries();
    std::cout << "There are " << nentries << " entries in the chain" << std::endl;
    for(int i=0;i<nentries;i++){
        //======================================
        // Get entries and prepare arrays
        calib->GetEntry(i);
        sim->GetEntry(i);
        double calib_nPE[PMTN]={0};
        double calib_fht[PMTN]={0};
        double sim_nPE[PMTN]={0};
        double sim_fht[PMTN]={0};
        for(int j=0;j<PMTN;j++){
            calib_fht[j]=999999;
            sim_fht[j]=999999;
        }
        //======================================
        // Get track orientation and histoIdentifier idx
        JM::SimTrack *sim_trk = se->findTrackByTrkID(0);   // TODO: Change for multi-track events!
        TVector3 sim_start(sim_trk->getInitX(), sim_trk->getInitY(),sim_trk->getInitZ());
        TVector3 sim_dir(sim_trk->getInitPx(), sim_trk->getInitPy(),sim_trk->getInitPz());
        double sim_P = sim_dir.Mag();
        sim_dir = sim_dir.Unit();
        double d_sim = (-sim_start.Cross(sim_dir)).Mag();
        int idx = int(d_sim/1000 + 0.5)/stepWidth;
        entry_cnt[idx]++;
        std::cout << "Event #" << i << " is a track with D="<< d_sim/1000. << "m and theta=" << acos(sim_dir.Z())*180./TMath::Pi() << "degree" << std::endl;
        //======================================
        // Get the calib info and fill into histogram arrays
        for(int j=0;j<calib_PMTID->size();j++){
            int pmtid = (((*calib_PMTID)[j]) & 0x00FFFF00)>>8; //Conversion from rec pmt nr to sim pmt nr
            if(pmtid>PMTN) continue; // wp
            calib_nPE[pmtid] += (*charge)[j];
            float ht = (*time)[j];
            if(ht<calib_fht[pmtid]) calib_fht[pmtid]=ht;
        }
        totalPE_calib_cnt[idx]+=totalcharge;
        //======================================
        // Get the sim info and fill arrays
        int totalPE_sim=0;
        const std::vector<JM::SimPMTHit*> hits = se->getCDHitsVec();
        for (std::vector<JM::SimPMTHit*>::const_iterator it = hits.begin(); it != hits.end(); ++it) {
            JM::SimPMTHit* hit = *it;
            int pmtid = hit->getPMTID();
            if(pmtid > PMTN) continue;
            totalPE_sim += hit->getNPE();
            sim_nPE[pmtid] += hit->getNPE();
            double ht = hit->getHitTime();
            if(ht<sim_fht[pmtid]) sim_fht[pmtid]=ht; 
        }
        totalPE_sim_cnt[idx]+=totalPE_sim;

        //======================================
        // Fill diff between calib and sim into histo
        for(int j=0;j<PMTN;j++){
            if(calib_nPE[j]>0) ha_PMT[idx]->Fill(j);
            if(sim_nPE[j]>0){
                ha_fht[idx]->Fill(calib_fht[j]);
                ha_nPE[idx]->Fill(calib_nPE[j]);
                h_diff_fht->Fill(sim_fht[j]-calib_fht[j]);
                h_diff_nPE->Fill(sim_nPE[j]-calib_nPE[j]);
                h2_diff_fht->Fill(sim_nPE[j],sim_fht[j]-calib_fht[j]);
                h2_diff_nPE->Fill(sim_nPE[j],sim_nPE[j]-calib_nPE[j]);
            }
            else if(calib_nPE[j]>0) cout<<"Warning: NO sim hit but with reconstructed hit! Event:"<<i<<"\t PMT: "<<j<<"\t Qrec: "<<calib_nPE[j]<<endl;
        }
        h_diff_totnPE->Fill(totalPE_sim - totalcharge);
    }
    //==========================================
    // Make some averages
    for(Int_t n=0; n<nSteps; n++){
        totalPE_sim_cnt[n] = totalPE_sim_cnt[n]/(1.*entry_cnt[n]);
        totalPE_calib_cnt[n] = totalPE_calib_cnt[n]/(1.*entry_cnt[n]);
        g_totPE_D->SetPoint(n, stepWidth*n, totalPE_calib_cnt[n]);
        if(entry_cnt[n]>0){
            ha_fht[n]->Scale(1./entry_cnt[n]);
            ha_nPE[n]->Scale(1./entry_cnt[n]);
            ha_PMT[n]->Scale(1./entry_cnt[n]);
        }
    }
    
    //output file
    TFile *f2= TFile::Open(outputpath,"recreate");
    if (!f2) {
        std::cerr << "Can't open file " << outputpath << std::endl;
        return;
    }

    g_totPE_D->Write();
    h_diff_fht->Write();
    h_diff_nPE->Write();
    h_diff_totnPE->Write();
    h2_diff_fht->Write();
    h2_diff_nPE->Write();
    for(Int_t n=0; n<nSteps; n++){
        ha_fht[n]->Write();
        ha_nPE[n]->Write();
        ha_PMT[n]->Write();
    }
 }
