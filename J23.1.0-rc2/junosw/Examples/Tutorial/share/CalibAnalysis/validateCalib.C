
#include<iostream>
#include<TFile.h>
#include<TTree.h>
#include<TH1.h>
#include<TH2.h>
#include<TString.h>

const int PMTN = 17746;
const int MaxHit = 15000;

void validateCalib(TString elecpath="sample_elecsim_user.root", TString calibpath="sample_calib_user.root", TString outputpath="calibval.root"){
	
	TFile n1(calibpath);
	TFile n2(elecpath);

	TTree* calib = (TTree*) n1.Get("calibevt");
        TTree* elec = (TTree*) n2.Get("SIMEVT");

	vector<float>* charge;
	vector<int>* calib_PMTID;
	float totalcharge;

	charge = NULL;
	calib_PMTID = NULL;
	
	calib->SetBranchAddress("Charge",&charge);
	calib->SetBranchAddress("PMTID",&calib_PMTID);
        calib->SetBranchAddress("TotalPE",&totalcharge);

	int elec_NPMT(0);
	int elec_NPulse(0);
        std::vector<int>* ptr_elec_pmtID=0;
        std::vector<int>* ptr_elec_NPE_perPMT=0;
        std::vector<double>* ptr_elec_Amplitude=0;
        std::vector<int>* ptr_elec_PMTID_perPulse=0;

	//PMT informations
	elec->SetBranchAddress("nPMT",&elec_NPMT);
	elec->SetBranchAddress("PMTID",&ptr_elec_pmtID);
	elec->SetBranchAddress("nPE_perPMT",&ptr_elec_NPE_perPMT);
	//Pulse informations
	elec->SetBranchAddress("nPulse",&elec_NPulse);
	elec->SetBranchAddress("Amplitude", &ptr_elec_Amplitude);
	elec->SetBranchAddress("PMTID_perPulse",&ptr_elec_PMTID_perPulse);

	
	if(calib->GetEntries()!=elec->GetEntries()){
		std::cerr<<"CalibVal: Number of events in elec_user file and calib_user file is not equal! CalibEvents: "<<calib->GetEntries()<<"\t ElecEvents: "<<elec->GetEntries()<<std::endl;
		return; 
		}

	// Output histograms:	
	TH2F* QrecQtruthvsNPE 		= new TH2F("QrecQtruthvsNPE","Qrec/Qtruth vs NPE",10,0,10,100,0,4);
	TH2F* QrecQtruthvsQtruth 	= new TH2F("QrecQtruthvsQtruth","Qrec/Qtruth vs Qtruth",40,0,10,100,0,4);
	TH2F* QrecvsQtruth 		= new TH2F("QrecvsQtruth","Qrec vs Qtruth",40,0,10,40,0,10);
	TH1F* QrecQtruth 		= new TH1F("QrecQtruth","Qrec/Qtruth",100,0.,5.);
	TH1F* Qtruth 			= new TH1F("Qtruth","Qtruth",100,0.,10.);
	TH1F* Qrec 			= new TH1F("Qrec","Qrec",100,0.,10.);
	
	
	QrecQtruthvsNPE 	->GetXaxis()->SetTitle("NPE");	
	QrecQtruthvsQtruth 	->GetXaxis()->SetTitle("Qtruth");		
	QrecvsQtruth		->GetXaxis()->SetTitle("Qtruth");
        QrecQtruth 		->GetXaxis()->SetTitle("Qrec/Qtruth");
	QrecQtruthvsNPE 	->GetYaxis()->SetTitle("Qrec/Qtruth");	
	QrecQtruthvsQtruth 	->GetYaxis()->SetTitle("Qrec/Qtruth");		
	QrecvsQtruth 		->GetYaxis()->SetTitle("Qrec");
        QrecQtruth 		->GetYaxis()->SetTitle("Entries");
	QrecQtruthvsNPE 	->GetZaxis()->SetTitle("Entries");	
	QrecQtruthvsQtruth 	->GetZaxis()->SetTitle("Entries");
	QrecvsQtruth 		->GetZaxis()->SetTitle("Entries");
        Qrec 			->GetXaxis()->SetTitle("Qrec(pe)");
        Qrec 			->GetYaxis()->SetTitle("Entries");
        Qtruth 			->GetXaxis()->SetTitle("Qtruth(pe)");
        Qtruth 			->GetYaxis()->SetTitle("Entries");

		

	for(int i=0; i<calib->GetEntries();i++){
		double calibQ[PMTN]={0};
		double elecPE[PMTN]={0};
		double elecQ[PMTN]={0};
		calib->GetEntry(i);
		elec->GetEntry(i);


		std::vector<int>&    elec_pmtID = *ptr_elec_pmtID;
		std::vector<int>&    elec_NPE_perPMT = *ptr_elec_NPE_perPMT;
		std::vector<double>& elec_Amplitude = *ptr_elec_Amplitude;
		std::vector<int>&    elec_PMTID_perPulse = *ptr_elec_PMTID_perPulse;


		//Obtain Qrec for each PMT
		for(int j=0;j<calib_PMTID->size();j++){
			int pmtid = (((*calib_PMTID)[j]) & 0x00FFFF00)>>8; //Conversion from rec pmt nr to sim pmt nr
			if(pmtid>PMTN) continue; // wp
			calibQ[pmtid] 			+= (*charge)[j];
		}
		//Obtain Qtruth for each Pulse and sum up for each PMT
		for(int j=0;j<elec_NPulse;j++){
			if(elec_PMTID_perPulse[j]>PMTN) continue;
			elecQ[elec_PMTID_perPulse[j]] 	+= elec_Amplitude[j];
		}
		//Obtain NPE for each PMT
		for(int j=0;j<elec_NPMT;j++){
			if(elec_pmtID[j]>PMTN) continue;
			elecPE[elec_pmtID[j]]		= elec_NPE_perPMT[j];
		}
		//Fill histograms
		for(int j=0;j<PMTN;j++){
			//if(calibQ[j]>0) cout <<"PMTID: "<<j <<"\tCharge(elec) = " << elecPE[j] <<"\t Charge(calib) = " <<calibPE[j] <<'\n';
			if(elecQ[j]>0) {
				QrecQtruth		->Fill(calibQ[j]/elecQ[j]);
				QrecQtruthvsNPE		->Fill(elecPE[j]	,calibQ[j]/elecQ[j]);
				QrecQtruthvsQtruth	->Fill(elecQ[j]		,calibQ[j]/elecQ[j]);
				QrecvsQtruth		->Fill(elecQ[j]		,calibQ[j]);
				Qrec	 		->Fill(calibQ[j]);
				Qtruth			->Fill(elecQ[j]);
				}
			else if(calibQ[j]>0) cout<<"Warning: NO elec hit but with reconstructed hit! Event:"<<i<<"\t PMT: "<<j<<"\t Qrec: "<<calibQ[j]<<endl;
		}
	}
	TFile n3(outputpath,"recreate");
	QrecQtruth->Write();
	QrecQtruthvsNPE->Write();
	QrecQtruthvsQtruth->Write();
	QrecvsQtruth->Write();
	Qrec->Write();
	Qtruth->Write();
}
