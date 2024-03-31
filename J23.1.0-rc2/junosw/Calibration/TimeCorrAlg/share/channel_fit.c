/*****************************************/
// Read the deltaT hist for all PMTs and fit partly
//to get the peak positions as time offsets.
//Authot: miaoyu@ihep.ac.cn
//Sep, 2019
/****************************************/

#include <iostream>
#include "TH1D.h"
#include "TF1.h"
#include "TString.h"
#include "TMath.h"
#include "TCanvas.h"
//#include <fstream.h>

using namespace std;

void channel_fit(){
     gStyle->SetOptFit(1101);

    const int m_totalPMT = 17613;
    double MIN_BIN = -200;
    double MAX_BIN = 800;
    int BIN_NUM = 1000;
    
    double sigma1 = 5;
    double sigma2 = 8;

    //std::string dir = "/junofs/users/yumiao/reconstruction/waveform_reconstruction/pmt_calib/time_corr/led_results/";
    //std::string dir = "/junofs/users/yumiao/reconstruction/waveform_reconstruction/time_rec/timeOffset/";
    //std::string dir = "/junofs/users/yumiao/reconstruction/waveform_reconstruction/time_rec/led4/";
    //std::string dir = "/junofs/users/yumiao/reconstruction/waveform_reconstruction/pmt_calib/time_corr/ledym_z15m/";
    //std::string dir = "/afs/ihep.ac.cn/users/y/yumiao/junofs/reconstruction/waveform_reconstruction/tOffset_release/";
    //std::string file = "ledhx_all.root";
    //std::string file = "ledhx_first1000test.root";
    //std::string file = "ledhx_allpmt.root";
    //std::string file = "ledhx_global_linear.root";
    //std::string file = "ledhx_force0_fixed.root";
    //std::string file = "ledhx_force_lienar_timeOffset.root";
    //std::string file = "ledhx_force4small_lienar_timeOffset.root";
    //std::string file = "ledhx4_lienar_C14.root";
    //std::string file = "ledhx_force00_lienar_timeOffset.root";
    //std::string file = "led0all_force_linear_allcharge.root";
    //std::string file = "led0all_force_linear_only1charge.root";
    //std::string file = "ledhx_force0all_lienar_timeOffset.root";
    //std::string file = "led430_force_linear_5pe.root";
    //std::string file = "led012_force_linear_only1charge.root";
    //std::string file = "led430_force_linear_2pe.root";
    //std::string file = "ledymz15m_force_linear.root";
    std::string dir = "../430nmLED/mu05/";
    std::string file = "led430_force_linear_1pe.root";
    TH1D* onechannel;
    TF1 *fit_func, *add_func;
    TFile *pmt_file =TFile::Open((dir+file).c_str());

    // print abnormal channel hit time
    TCanvas* cc = new TCanvas();
    //cc->Print("abnormalHama.pdf(");

    //user's definition
    double deltaT[m_totalPMT];
    double deltaT_sigma[m_totalPMT];
    double deltaT_error[m_totalPMT];
    double sigma_error[m_totalPMT];
    double chi2NDF[m_totalPMT];
    double pmtType[m_totalPMT];
    TH1D* timeOffset = new TH1D("timeOffset","timeOffset",100,-50,50);
    
    // pmt type
    for(int i=0; i<17613; i++){ pmtType[i] = 0;}
    ifstream pmtdata;
    pmtdata.open("./pmtdata.txt");
    int tmp;
    string line;
    while(getline(pmtdata,line)){
        istringstream ss(line);
        ss >> tmp;
        pmtType[tmp] = 1;
    }



    //TCanvas* cc = new TCanvas();
    //cc->Print("refit.pdf(");
    
//    int abnormalID[30] = {8260 ,  9387 ,  10549,  10571,  11769,  11986,  12799,  13417,  13662,  13737,  14057,  14763,  14874,  14896,  14957,  15260,  15401,  15523,  15991,  16217,  16263,  16382,  16549,  16618,  16820,  16894,  16951,  17435,  17468,  17488};
//    for(int ii =0; ii<30; ii++){
//    for(int i=0; i<m_totalPMT; i++){
    for(int i=14085 ;i<14086 ;i++){
//    for(int i=abnormalID[ii]; i<abnormalID[ii]+1; i++){
        //if(i>=0 && i<21) continue;
        //if(i>=17592 && i<17613) continue;
        cout << "Processing channel " << i << endl;
        TString chName = Form("ch%d_deltaT",i);
        onechannel = (TH1D*)pmt_file->Get(chName);
        if(!onechannel) cout << "Getting hist failed " << endl;
        
        Int_t peak_pos = onechannel->GetMaximumBin();   
        double mean_init = peak_pos *(MAX_BIN-MIN_BIN)*BIN_NUM+MIN_BIN;
        // if it is a hama_pmt
        // set integral range for different type pmts
        


        double sigma;
        if(pmtType[i]){
       /* 
            //   average finding peak
            double sum_bin = 0;
            double sum = 0;
            for(int kk =0; kk<5; kk++){
                sum_bin += onechannel->GetBinContent(peak_pos -2 +kk);
                sum += onechannel->GetBinContent(peak_pos-2+kk) * (peak_pos-2+kk);
            }
            
            deltaT[i] = (sum / sum_bin) *(MAX_BIN-MIN_BIN)/BIN_NUM+MIN_BIN; 
            deltaT_error[i] = 0.01;
            deltaT_sigma[i] = 0.1;
            sigma_error[i] = 0.001;
            chi2NDF[i] = 1.0;
   */         
            double min_fit = (peak_pos - 8) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
            double max_fit = (peak_pos + 3) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
            sigma = sigma1; sigma = 2;
            fit_func = new TF1("gauss_fit", "[0]*TMath::Exp( -(x-[1])*(x-[1])/2/[2]/[2] )", min_fit, max_fit);    // new ref time(evt start time)
            //add_func = new TF1("expo_fit", "[0]*TMath::Exp(-(x-[1])/[2])", min_fit, max_fit);
            fit_func -> SetParameter(0,onechannel->GetMaximum());
            fit_func -> SetParameter(1,peak_pos*(MAX_BIN-MIN_BIN)/BIN_NUM+MIN_BIN);
            fit_func -> SetParameter(2,sigma);
            //add_func -> SetParameter(0,10);
            //add_func -> SetParameter(1,140);
            //add_func -> SetParameter(2,10);

            //Double_t fitFunction(Double_t *x, Double_t *par1){
            //    return fit_func(x,par1)+add_func(x &par[3]);
            //}
            //TF1 *fitFcn = new TF1("fitFcn",fitFunction,min_fit,max_fit,6);
            onechannel->Fit(fit_func,"RQ");
            
            //onechannel->Fit(fitFunction,"RQ");

            //cout << "offset for hama: " << fit_func->GetParameter(1) - deltaT[i] << endl;
            deltaT[i] = fit_func->GetParameter(1);
            deltaT_sigma[i] = fit_func->GetParameter(2);
            deltaT_error[i] = fit_func->GetParError(1);
            sigma_error[i] = fit_func->GetParError(2);
            chi2NDF[i] = fit_func->GetChisquare()/fit_func->GetNDF();  //cout << "chi2: " << chi2NDF[i] << endl;
              
        }

        if(!pmtType[i]){
            double min_fit = (peak_pos - 15) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
            double max_fit = (peak_pos + 8 ) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
            //sigma = sigma2;
            sigma = 10;
            fit_func = new TF1("gauss_fit", "[0]*TMath::Exp( -(x-[1])*(x-[1])/2/[2]/[2] )", min_fit, max_fit);    // new ref time(evt start time)
            fit_func -> SetParameter(0,onechannel->GetMaximum());
            fit_func -> SetParameter(1,peak_pos*(MAX_BIN-MIN_BIN)/BIN_NUM+MIN_BIN);
            fit_func -> SetParameter(2,sigma);

            onechannel->Fit(fit_func,"RQ");
            deltaT[i] = fit_func->GetParameter(1);
            deltaT_sigma[i] = fit_func->GetParameter(2);
            deltaT_error[i] = fit_func->GetParError(1);
            sigma_error[i] = fit_func->GetParError(2);
            chi2NDF[i] = fit_func->GetChisquare()/fit_func->GetNDF();  //cout << "chi2: " << chi2NDF[i] << endl;
     cout << i << " "<< deltaT[i] << " " <<deltaT_error[i] << " " 
        << deltaT_sigma[i] <<" " << sigma_error[i] << " " << chi2NDF[i]<< "\n";   
        }      

        // re-fit for abnormal channel
        //    double min_fit = (peak_pos - 5) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
        //    double max_fit = (peak_pos + 5) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
        //    sigma = 4;

        //cout << peak_pos *(MAX_BIN-MIN_BIN)/BIN_NUM+MIN_BIN<< min_fit << " " << max_fit << endl;
        //fit_func = new TF1("gauss_fit", "[0]*TMath::Exp( -(x-[1])*(x-[1])/2/[2]/[2] )", min_fit, max_fit);
       
        //double sigma = 5;
        //double min_fit = (peak_pos - 12) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
        //double max_fit = (peak_pos + 12) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
        //if(i == 8687){
        //double sigma = 3;
        //double min_fit = (peak_pos - 10) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
        //double max_fit = (peak_pos + 10) *(MAX_BIN - MIN_BIN)/BIN_NUM + MIN_BIN;
            
//        }
        

        
       
        //record bad channels
//        if(chi2NDF[i] > 25 || deltaT[i] > 10000 || deltaT_sigma[i]>10000) {
//        if(i == 8687) {
//            cc->cd(); onechannel->DrawClone(); cc->Print("abnormalHama.pdf");
//        } 
        //cc->cd();       
        onechannel->SetXTitle("time/ns");
        onechannel->DrawClone("");
        //cc->Print("refit.pdf(");
        onechannel->Delete();
        //cout << fit_func->GetParameter(1) << endl;
//     cout << i << " "<< deltaT[i] << " " <<deltaT_error[i] << " " 
//        << deltaT_sigma[i] <<" " << sigma_error[i] << " " << chi2NDF[i]<< "\n";   
    }
  //  }

    //pmt_file->Close();
    
//    ofstream out("./pmt_param_force266nmZ15_linear_allpe_RootFit.txt");
//    ofstream out("./pmt_param_force266mZ0_linear_allpe_averagePeak.txt");
//    for(int i=0; i<m_totalPMT;i++){ out << i << " "<< deltaT[i] << " " <<deltaT_error[i] << " " 
//        << deltaT_sigma[i] <<" " << sigma_error[i] << " " << chi2NDF[i]<< "\n";   }
    

//    cc->Print("refit.pdf)");

//    TCanvas* c2 = new TCanvas();
//    onechannel->Draw();
    //    timeOffset->Draw();

//    cc->Print("refit.pdf)");
    return;
}
