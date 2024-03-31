#include <iostream>
#include <fstream>
#include <TTree.h>
#include <TChain.h>
#include <math.h>
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

void SimAnalysisMu(TString inputsimpath="lists_detsim_mu.txt", TString inputsimuserpath="lists_detsim_user_mu.txt", TString outputpath="detsim_ana/detsim_ana_mu.root"){
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
    TH1D *ha_ELossScint[nSteps];
    TH1D *ha_TrackLengthScint[nSteps];
    int entry_cnt[nSteps];
    int totalPE_cnt[nSteps];
    std::string ids[nSteps];
    char number[2];
    for(Int_t i=0; i<nSteps; i++) {
        sprintf(number,"%02i",i*stepWidth);
        ids[i] = std::string(number);
    }
    for(Int_t n=0; n<nSteps; n++){
        ha_fht[n] = new TH1D( ("fht_"+ids[n]).c_str(), (ids[n]).c_str(), 750, 0, 750);
        ha_nPE[n] = new TH1D( ("nPE_"+ids[n]).c_str(), (ids[n]).c_str(), 1000, 0, 10000);
        ha_PMT[n] = new TH1D( ("PMT_"+ids[n]).c_str(), (ids[n]).c_str(), 17746, 0, 17746);
        ha_ELossScint[n] = new TH1D( ("ELossScint_"+ids[n]).c_str(), (ids[n]).c_str(), 500, -50000, 0);
        ha_TrackLengthScint[n] = new TH1D( ("TrackLengthScint_"+ids[n]).c_str(), (ids[n]).c_str(), 400, 0, 40000);
        ha_fht[n]->GetXaxis()->SetTitle("fht [ns]");
        ha_fht[n]->GetYaxis()->SetTitle("entries");
        ha_nPE[n]->GetXaxis()->SetTitle("nPE");
        ha_nPE[n]->GetYaxis()->SetTitle("entries");
        ha_PMT[n]->GetXaxis()->SetTitle("PMT ID");
        ha_PMT[n]->GetYaxis()->SetTitle("avg. fire rate");
        entry_cnt[n] = 0;
        totalPE_cnt[n] = 0;
    }
    TH1D *h_dEdx = new TH1D( "dEdx", "Energy loss dE/dx", 40, 0, 20);
    TGraph *g_totPE_D = new TGraph();
    g_totPE_D->SetName("g_totPE_D");
    g_totPE_D->GetXaxis()->SetTitle("D [m]");
    g_totPE_D->GetYaxis()->SetTitle("avg. total PE per event");
    //==========================================
    // Init of variables to hold tree content
    int m_MuMult;
    double m_ELossInScint[2];
    double m_TrackLengthInScint[2];

    std::vector<TString> sim_files_list;
    std::vector<TString> sim_user_files_list;
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
    // Get all Sim user files into chain
    std::ifstream sufp(inputsimuserpath);
    if (!sufp.is_open()) {
        std::cerr << "ERROR: input lists does not exists." << std::endl;
        return;
    }
    std::string sim_user_tmp_line;
    while(sufp.good()) {
        std::getline(sufp, sim_user_tmp_line);
        if ( sim_user_tmp_line.size() ==0 ) {
            continue;
        }
        sim_user_files_list.push_back(sim_user_tmp_line);
    }
    TChain* sim_user_ch = new TChain("mu");
    for (std::vector<TString>::iterator it = sim_user_files_list.begin();
            it != sim_user_files_list.end(); ++it) {
        std::cout << "add sim user file: " << *it << std::endl;
        sim_user_ch->Add(*it);
    }
    //==========================================
    // Set branch addresses
    TTree* sim = (TTree*)sim_ch;
    JM::SimEvent* se = new JM::SimEvent();
    sim->SetBranchAddress("SimEvent",&se);
    sim->GetBranch("SimEvent")->SetAutoDelete(true);

    TTree* sim_user = (TTree*)sim_user_ch;
    sim_user->SetBranchAddress("MuMult",&m_MuMult);
    sim_user->SetBranchAddress("ELossInScint", m_ELossInScint);
    sim_user->SetBranchAddress("TrackLengthInScint", m_TrackLengthInScint);
    //==========================================
    // Loop & fill
    int nentries = sim->GetEntries();
    if(nentries != sim_user->GetEntries()) std::cout << "Entry missmatch!\n";
    std::cout << "There are " << nentries << " entries in the chain" << std::endl;
    for(int i=0;i<nentries;i++){
        //======================================
        // Get entries and prepare arrays
        sim->GetEntry(i);
        sim_user->GetEntry(i);
        double a_nPE[PMTN]={0};
        double a_fht[PMTN]={0};
        for(int j=0;j<PMTN;j++){
            a_nPE[j]=0;
            a_fht[j]=99999;
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
        //std::cout << "Event #" << i << " is a track with D="<< d_sim/1000. << "m and theta=" << acos(sim_dir.Z())*180./TMath::Pi() << "degree" << std::endl;
        //======================================
        // Get the sim info and fill arrays
        int totalPE=0;
        const std::vector<JM::SimPMTHit*> hits = se->getCDHitsVec();
        for (std::vector<JM::SimPMTHit*>::const_iterator it = hits.begin(); it != hits.end(); ++it) {
            JM::SimPMTHit* hit = *it;
            int pmtid = hit->getPMTID();
            if(pmtid > PMTN) continue;
            totalPE += hit->getNPE();
            a_nPE[pmtid] += hit->getNPE();
            double ht = hit->getHitTime();
            if(ht<a_fht[pmtid]) a_fht[pmtid]=ht; 
        }
        totalPE_cnt[idx]+=totalPE;
        //======================================
        // Fill diff between calib and sim into histo
        for(int j=0;j<PMTN;j++){
            if(a_nPE[j]>0){
                ha_nPE[idx]->Fill(a_nPE[j]);
                ha_fht[idx]->Fill(a_fht[j]);
                ha_PMT[idx]->Fill(j);
            }
        }
        //=====================================
        // Fill energyloss and  tracklength histos
        if(m_MuMult == 1){
            ha_ELossScint[idx]->Fill(m_ELossInScint[0]);
            ha_TrackLengthScint[idx]->Fill(m_TrackLengthInScint[0]);
            h_dEdx->Fill(-1.*m_ELossInScint[0]/(0.863*m_TrackLengthInScint[0]/10.));
        }
        else std::cout <<"There are more than 1 muons! SKIPPING!"<< std::endl;
    }
    // Make some averages
    for(Int_t n=0; n<nSteps; n++){
        totalPE_cnt[n] = totalPE_cnt[n]/(1.*entry_cnt[n]);
        g_totPE_D->SetPoint(n, stepWidth*n, totalPE_cnt[n]);
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
    h_dEdx->Write();
    for(Int_t n=0; n<nSteps; n++){
        ha_fht[n]->Write();
        ha_nPE[n]->Write();
        ha_PMT[n]->Write();
        ha_ELossScint[n]->Write();
        ha_TrackLengthScint[n]->Write();
    }
 }
