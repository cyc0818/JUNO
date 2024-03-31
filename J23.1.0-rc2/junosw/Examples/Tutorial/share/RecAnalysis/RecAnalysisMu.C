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
#include <Event/CDRecEvent.h>
#include <Event/CDTrackRecEvent.h>
#include <Event/RecTrack.h>

using std::cout;
using std::endl;

void RecAnalysisMu(TString inputrecpath="lists_rec_mu.txt", TString inputsimpath="lists_detsim_user_mu.txt",TString outputpath="rec_ana/rec_ana_mu.root"){
    gSystem->Load("libRecEvent.so");
    const int stepWidth = 4;
    const int nSteps = 17/stepWidth +1;
    // Temp histo arrays
    TH1D *ha_dD[nSteps];
    TH1D *ha_al[nSteps];
    std::string ids[nSteps];
    char number[2];
    for(Int_t i=0; i<nSteps; i++) {
        sprintf(number,"%02i",i*stepWidth);
        ids[i] = std::string(number);
    }
    for(Int_t n=0; n<nSteps; n++){
        ha_dD[n] = new TH1D( ("dD_"+ids[n]).c_str(), (ids[n]).c_str(), 4000, -20000, 20000);
        ha_al[n] = new TH1D( ("al_"+ids[n]).c_str(), (ids[n]).c_str(), 5400, 0, 180);
    }
    // Final Graphs
    TGraph *g_dD_m = new TGraph(nSteps);
    g_dD_m->SetName("g_dD_m");
    TGraph *g_dD_r = new TGraph(nSteps);
    g_dD_r->SetName("g_dD_r");
    TGraph *g_al_m = new TGraph(nSteps);
    g_al_m->SetName("g_al_m");
    TGraph *g_al_r = new TGraph(nSteps);
    g_al_r->SetName("g_al_r");

    int sim_nInitParticles;
    float sim_initX[2];
    float sim_initY[2];
    float sim_initZ[2];
    float sim_initPX[2];
    float sim_initPY[2];
    float sim_initPZ[2];
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
    TChain* rec_ch = new TChain("Event/Rec/CDTrackRecEvent");
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
    TChain* sim_ch = new TChain("geninfo");
    for (std::vector<TString>::iterator it = sim_files_list.begin();
            it != sim_files_list.end(); ++it) {
        std::cout << "add sim file: " << *it << std::endl;
        sim_ch->Add(*it);
    }

    TTree* rec = (TTree*)rec_ch;
    JM::CDTrackRecEvent* rh = new JM::CDTrackRecEvent();
    rec->SetBranchAddress("CDTrackRecEvent",&rh);

    TTree* sim = (TTree*)sim_ch;
    sim -> SetBranchAddress("nInitParticles", &sim_nInitParticles);
    sim -> SetBranchAddress("InitX",sim_initX);
    sim -> SetBranchAddress("InitY",sim_initY);
    sim -> SetBranchAddress("InitZ",sim_initZ);
    sim -> SetBranchAddress("InitPX",sim_initPX);
    sim -> SetBranchAddress("InitPY",sim_initPY);
    sim -> SetBranchAddress("InitPZ",sim_initPZ);

    int nentries = rec->GetEntries();
    for(int i=0;i<nentries;i++){
        sim->GetEntry(i);
        rec->GetEntry(i);
        // First get rec track and vectors
        JM::RecTrack* rec_trk = (JM::RecTrack*)rh->getTrack(0);
        CLHEP::HepLorentzVector start = rec_trk->start();
        CLHEP::HepLorentzVector end = rec_trk->end();
        // Transform to TVector3, more convenient
        TVector3 rec_start(start.x(), start.y(), start.z());
        TVector3 rec_end(end.x(), end.y(), end.z());
        TVector3 rec_dir = (rec_end-rec_start).Unit();
        rec_start.Print();
        rec_dir.Print();

        // Get sim info
        if(sim_nInitParticles != 1){
            std::cout << "There are more than one initial particles! ABORT!" << std::endl;
            break;
        }
        TVector3 sim_start(sim_initX[0], sim_initY[0], sim_initZ[0]);
        TVector3 sim_dir(sim_initPX[0], sim_initPY[0], sim_initPZ[0]);
        double sim_P = sim_dir.Mag();
        sim_dir = sim_dir.Unit();
        sim_start.Print();
        sim_dir.Print();

        // get D and alpha
        double d_rec = (-rec_start.Cross(rec_dir)).Mag();
        double d_sim = (-sim_start.Cross(sim_dir)).Mag();
        double deltaD = d_sim - d_rec;
        double alpha = sim_dir.Angle(rec_dir)*180./TMath::Pi();

        std::cout << "deltaD= " << deltaD << ", alpha=" << alpha << std::endl;

        int idx = int(d_sim/1000 + 0.5)/stepWidth;
        ha_dD[idx]->Fill(deltaD);
        ha_al[idx]->Fill(alpha);
    }
    // Set graphs from histo array
    for(Int_t n=0; n<nSteps; n++){
        g_dD_m->SetPoint(n, stepWidth*n, ha_dD[n]->GetMean()/10.);
        g_dD_r->SetPoint(n, stepWidth*n, ha_dD[n]->GetRMS()/10.);
        g_al_m->SetPoint(n, stepWidth*n, ha_al[n]->GetMean());
        g_al_r->SetPoint(n, stepWidth*n, ha_al[n]->GetRMS());
    }

            
    TCanvas *cg = new TCanvas("cg","Reco eval",0,0,1600,800);
    cg->Divide(2,2);
    cg->cd(1);
    gPad->SetGrid();
    g_dD_m->Draw("A*");
    g_dD_m->SetTitle("Bias of #DeltaD");
    g_dD_m->GetXaxis()->SetTitle("D [m]");
    g_dD_m->GetYaxis()->SetTitle("mean #DeltaD [cm]");
    g_dD_m->GetYaxis()->CenterTitle();
    cg->cd(2);
    gPad->SetGrid();
    g_dD_r->Draw("A*");
    g_dD_r->SetTitle("Resolution of #DeltaD");
    g_dD_r->GetXaxis()->SetTitle("D [m]");
    g_dD_r->GetYaxis()->SetTitle("RMS #DeltaD [cm]");
    g_dD_r->GetYaxis()->CenterTitle();
    cg->cd(3);
    gPad->SetGrid();
    g_al_m->Draw("A*");
    g_al_m->SetTitle("Bias of #alpha");
    g_al_m->GetXaxis()->SetTitle("D [m]");
    g_al_m->GetYaxis()->SetTitle("mean #alpha [#circ]");
    g_al_m->GetYaxis()->CenterTitle();
    cg->cd(4);
    gPad->SetGrid();
    g_al_r->Draw("A*");
    g_al_r->SetTitle("Resolution of #alpha");
    g_al_r->GetXaxis()->SetTitle("D [m]");
    g_al_r->GetYaxis()->SetTitle("RMS #alpha [#circ]");
    g_al_r->GetYaxis()->CenterTitle();

    //output file
    TFile *f2= TFile::Open(outputpath,"recreate");
    if (!f2) {
        std::cerr << "Can't open file " << outputpath << std::endl;
        return;
    }
    g_dD_m->Write();
    g_dD_r->Write();
    g_al_m->Write();
    g_al_r->Write();
    cg->Write();

    delete cg;
    delete g_dD_m;
    delete g_dD_r;
    delete g_al_m;
    delete g_al_r;
 }