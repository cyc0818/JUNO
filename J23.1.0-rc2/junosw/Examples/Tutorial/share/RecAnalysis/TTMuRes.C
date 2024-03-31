#include <iostream>
#include <fstream>

#include <TString.h>
#include <TH1F.h>
#include <TChain.h>
#include <TFile.h>
#include <TMath.h>

void TTMuRes(TString inputsimuserpath="lists_detsim_user_TT.txt", TString inputrecuserpath="lists_rec_user_TT.txt", TString outputpath="rec_woelec_ana/TTMuRes.root"){
    std::cout << "Starting TTplots!" << std::endl;

    TChain * geninfo = new TChain("geninfo");
    TChain * TTDigit = new TChain("TTDigit");
    TChain * TT_rec  = new TChain("TT");

    // Read root file names from list
    std::ifstream sim_user_files(inputsimuserpath);
    if (!sim_user_files.is_open()) {
        std::cerr << "ERROR: sim input lists does not exists." << std::endl;
        return;
    }
    std::ifstream rec_user_files(inputrecuserpath);
    if (!rec_user_files.is_open()) {
        std::cerr << "ERROR: rec input lists does not exists." << std::endl;
        return;
    }
    bool has_rate = true;
    int n_files = 0;
    float avg_rate = 0;
    float weight = 1;
    while(true) {
        std::string sim_filename, rec_filename;
        std::getline(sim_user_files, sim_filename);
        std::getline(rec_user_files, rec_filename);
        if(sim_user_files.eof()) break;
        if(rec_user_files.eof()) break;
        geninfo->Add(sim_filename.c_str());
        TTDigit->Add(sim_filename.c_str());
        TT_rec->Add(rec_filename.c_str());

        if(has_rate){
            std::string simtxt_filename = sim_filename + ".txt";
            int found = simtxt_filename.rfind("user-");
            simtxt_filename.erase(found,5);
            // std::cout << simtxt_filename << std::endl;
            std::ifstream simtxt_file(simtxt_filename.c_str());
            if(!simtxt_file.is_open()){
                has_rate = false;
                continue;
            }
            std::string last_line = "";
            while(true){
                std::string this_line;
                std::getline(simtxt_file, this_line);
                if(simtxt_file.eof()) break;
                last_line = this_line;
            }
            simtxt_file.close();
            // std::cout << last_line << std::endl;
            // std::cout << last_line.substr(12,8) << std::endl;
            avg_rate += std::atof(last_line.substr(12,8).c_str());
            n_files++;
        }
    }
    sim_user_files.close();
    rec_user_files.close();

    if(has_rate){
        avg_rate = avg_rate/n_files;
        weight = avg_rate/geninfo->GetEntries() ;
    }

    // Open output file
    TFile * output_file = new TFile(outputpath,"recreate");
    if(!output_file){
        std::cerr << "Can't open file " << output_file << std::endl;
        return;
    }

    // Define resolution histogram
    TH1F * TTMuRes_h = new TH1F("TTMuRes","",100,0,2);
    TTMuRes_h->GetXaxis()->SetTitle("Distance between True and Reco at bottom of WP (m)");
    if(has_rate){
        TTMuRes_h->GetYaxis()->SetTitle("Rate (Hz)");
        TTMuRes_h->Sumw2();
    }
    else{
        TTMuRes_h->GetYaxis()->SetTitle("Number of events");
    }

    // Setting for TTree readout
    int NTouchedChannel;
    TTDigit->SetBranchAddress("NTouchedChannel", &NTouchedChannel);
    // TTDigit->SetBranchStatus("*",0);
    // TTDigit->SetBranchStatus("NTouchedChannel",1);

    float InitX[6],InitY[6],InitZ[6],InitPX[6],InitPY[6],InitPZ[6];
    geninfo->SetBranchAddress("InitX" , InitX);
    geninfo->SetBranchAddress("InitY" , InitY);
    geninfo->SetBranchAddress("InitZ" , InitZ);
    geninfo->SetBranchAddress("InitPX", InitPX);
    geninfo->SetBranchAddress("InitPY", InitPY);
    geninfo->SetBranchAddress("InitPZ", InitPZ);

#define NTRACK_MAX 100
    int NTracks;
    int NPoints[NTRACK_MAX];
    double Coeff0[NTRACK_MAX], Coeff1[NTRACK_MAX], Coeff2[NTRACK_MAX], Coeff3[NTRACK_MAX], Coeff4[NTRACK_MAX], Coeff5[NTRACK_MAX], Chi2[NTRACK_MAX];
    TT_rec->SetBranchAddress("NTracks",&NTracks);
    TT_rec->SetBranchAddress("NPoints", NPoints);
    TT_rec->SetBranchAddress("Coeff0" , Coeff0);
    TT_rec->SetBranchAddress("Coeff1" , Coeff1);
    TT_rec->SetBranchAddress("Coeff2" , Coeff2);
    TT_rec->SetBranchAddress("Coeff3" , Coeff3);
    TT_rec->SetBranchAddress("Coeff4" , Coeff4);
    TT_rec->SetBranchAddress("Coeff5" , Coeff5);
    TT_rec->SetBranchAddress("Chi2"   , Chi2);

    // Calculate resolution
    long int i_sim = -1, i_rec = -1;

    while(true){
        i_sim++;
        if(i_sim > geninfo->GetEntries()) break;
        TTDigit->GetEntry(i_sim);
        if(NTouchedChannel == 0) continue;


        i_rec++;
        if(i_rec > TT_rec->GetEntries()) break;
        TT_rec->GetEntry(i_rec);
        if(NTracks==0) continue;
        if(NPoints[0] <= 2) continue;

        geninfo->GetEntry(i_sim);

        int i_best_chi2 = 0;
        for(int ic=1; ic<NTracks; ic++){
            if(Chi2[ic] < Chi2[i_best_chi2]){
                i_best_chi2 = ic;
            }
        }

        // zref : set at bottom of water pool [=-WP_HEIGHT/2]
        static double zref = -43.5e3/2;
        // zoffset : offset between global (simulation) and TT (reconstruction) coordinate systems
        //           [=(WP_HEIGHT+TT_HEIGHT)/2]
        static double zshift = (43.5e3+8.4e3)/2;

        double rec_lambda = (zref - (Coeff2[i_best_chi2] + zshift))/Coeff5[i_best_chi2];
        double rec_x = Coeff0[i_best_chi2] + rec_lambda * Coeff3[i_best_chi2];
        double rec_y = Coeff1[i_best_chi2] + rec_lambda * Coeff4[i_best_chi2];

        double sim_lambda = (zref - InitZ[0])/InitPZ[0];
        double sim_x = InitX[0] + sim_lambda * InitPX[0];
        double sim_y = InitY[0] + sim_lambda * InitPY[0];

        double distance_WP = TMath::Sqrt(TMath::Power(sim_x-rec_x,2)+TMath::Power(sim_y-rec_y,2));

        TTMuRes_h->Fill(distance_WP/1e3,weight); // convert from mm to m

    }

    TTMuRes_h->Write();
    output_file->Close();
}

// vim: sw=4:sts=4:ts=4:et
