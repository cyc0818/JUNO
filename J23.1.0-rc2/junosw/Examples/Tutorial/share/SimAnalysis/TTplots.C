#include <iostream>
#include <fstream>

#include <TString.h>
#include <TH1F.h>
#include <TChain.h>
#include <TFile.h>

void TTplots(TString inputsimuserpath="lists_detsim_user_TT.txt", TString outputpath="detsim_ana/detsim_ana_TT.root"){
    std::cout << "Starting TTplots!" << std::endl;
    TChain* TTDigit = new TChain("TTDigit");
    TChain* TT      = new TChain("TT");

    // Read root file names from list
    std::ifstream user_files(inputsimuserpath);
    if (!user_files.is_open()) {
        std::cerr << "ERROR: input lists does not exists." << std::endl;
        return;
    }
    while(true) {
        std::string filename;
        std::getline(user_files, filename);
        if(user_files.eof()) break;
        if ( filename.size() ==0 ) continue;
        TTDigit->Add(filename.c_str());
        TT     ->Add(filename.c_str());
    }
    user_files.close();

    // Open output file
    TFile * output_file = new TFile(outputpath,"recreate");
    if(!output_file){
        std::cerr << "Can't open file " << output_file << std::endl;
        return;
    }

    // Make plots
    TH1F * TTDigit_NTouchedChannel = new TH1F("TTDigit_NTouchedChannel","",50,0,50);
    TTDigit_NTouchedChannel->GetXaxis()->SetTitle("Number of hits in TT");
    TTDigit->Draw("NTouchedChannel>>TTDigit_NTouchedChannel","","GOFF");
    TTDigit_NTouchedChannel->Write();

    TH1F * TTDigit_TB_ADC = new TH1F("TTDigit_TB_ADC","",100,0,800);
    TTDigit_TB_ADC->GetXaxis()->SetTitle("Hit Charge (ADC)");
    TTDigit->Draw("TB_ADC>>TTDigit_TB_ADC","","GOFF");
    TTDigit_TB_ADC->Write();

    TH1F * TTDigit_TB_time = new TH1F("TTDigit_TB_time","",150,0,150);
    TTDigit_TB_time->GetXaxis()->SetTitle("Hit time (ns)");
    TTDigit->Draw("TB_time>>TTDigit_TB_time","","GOFF");
    TTDigit_TB_time->Write();

#if 0
    TH1F * TTDigit_TB_DMchannel = new TH1F("TTDigit_TB_DMchannel","",153*16,805305344,805462016);
    TTDigit_TB_DMchannel->GetXaxis()->SetTitle("Channel ID number [one PMT per bin]");
#else
    TH1F * TTDigit_TB_DMchannel = new TH1F("TTDigit_TB_DMchannel","",153,805305344,805462016);
    TTDigit_TB_DMchannel->GetXaxis()->SetTitle("Channel ID number [one wall per bin]");
#endif
    //for(int i=0;i<TTDigit_TB_DMchannel->GetNbinsX();i++)
    //    std::cout << i << " : " << (long int) TTDigit_TB_DMchannel->GetXaxis()->GetBinLowEdge(i+1) << " -> " << (long int) TTDigit_TB_DMchannel->GetXaxis()->GetBinUpEdge(i+1) << std::endl;
    TTDigit->Draw("TB_DMchannel>>TTDigit_TB_DMchannel","","GOFF");
    TTDigit_TB_DMchannel->Write();

    TH1F * TTDigit_TB_is_ctC = new TH1F("TTDigit_TB_is_ctC","",2,0,2);
    TTDigit_TB_is_ctC->GetXaxis()->SetTitle("Is hit a XTalk hit?");
    TTDigit->Draw("TB_is_ctC>>TTDigit_TB_is_ctC","","GOFF");
    TTDigit_TB_is_ctC->Write();

    TH1F * TT_dep_tL = new TH1F("TT_dep_tL","",150,0,150);
    TT_dep_tL->GetXaxis()->SetTitle("Transit time of deposit to left end of fiber (ns)");
    TT->Draw("dep_tL>>TT_dep_tL","","GOFF");
    TT_dep_tL->Write();

    output_file->Close();
}

// vim: sw=4:sts=4:ts=4:et
