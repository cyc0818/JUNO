#include <iostream>
#include <math.h>
#include "beta.h"
using namespace std;
void histogram(){
    TFile* betaSpec = new TFile("CesiumBetaSpec.root","recreate");
    TCanvas *C = new TCanvas("c1","",600,400);
    TH1F* hB1 = new TH1F("Cs137Branch1","Cs137 beta Decay, E(ave)=174 keV, E(max)=514 keV", 514, 0, 514);
    for (int i=1; i<=514;i++)
	hB1->SetBinContent(i, beta(55,514,1.0*i)/2.17899e+20*0.947);   //normalization const  & *branch ratio
	hB1->Draw();
        hB1->GetXaxis()->SetTitle("Energy(keV)");
        hB1->GetYaxis()->SetTitle("Intensity (Betas/10**6 decay/keV)");
        cout<<"Branch 1   "<<hB1->Integral(0,514)<<endl;

    TH1F* hB2 = new TH1F("Cs137Branch2","Cs137 beta Decay, E(ave)=416 keV, E(max)=1176 keV", 1176, 0, 1176);
    for (int i=1; i<=1176;i++)
        hB2->SetBinContent(i, beta(55,1176,1.0*i)/8.9938e+21*0.053);   //normalization const  & *branch ratio
        hB2->Draw("same");
        hB2->GetXaxis()->SetTitle("Energy(keV)");
        hB2->GetYaxis()->SetTitle("Intensity (Betas/10**6 decay/keV)");
        cout<<"Branch 2   "<<hB2->Integral(0,1176)<<endl;

    betaSpec->Write(); 

}

