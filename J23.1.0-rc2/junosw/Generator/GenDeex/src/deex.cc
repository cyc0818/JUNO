#include "deex.h"
#include <iostream>
#include <TROOT.h>
#include <TRandom.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <map>
#include <stdio.h>

using namespace std;

deex::deex(TString datadir)
{
  Deedata = datadir;  
  initial();
}

deex::~deex(){

   std::map<TString,TFile*>::iterator it;
   for (it=TString2File.begin(); it!=TString2File.end(); ++it){
          (it->second)->Close();
         //std::cout<<it->first<<std::endl;
      }
}
void deex::SetDeexParPdg( int pdg_code){

	int iso_rp = int((pdg_code - 1000000000) / 10000);
	int iso_rn = int((pdg_code - 1000000000 - iso_rp * 10000) / 10) - iso_rp;
	SetParams(iso_rp, iso_rn);	

}

void deex::SetParams(int Z, int N){
 
  deexZ = Z;
  deexN = N;
  //caculateExE();
  
 }

AfterDeexInfo deex::GetAfterDeexInfo() const{
	
	//GetDeeProcess();
	return aAfterDeexInfo;
}

// Crucial API to calculate deexcitation
void deex::execute(){
	
	//initial after deex info
	AfterDeexInfo a;
	aAfterDeexInfo = a;

	calculateExE(); // calculate excitation energy	
	
	//std::cout<<" exe = " << exe << std::endl;
	if( exe > 0) {
		LoadDeeData();
		fill_after_deex_info();
	}
	
}

void deex::calculateExE(){
  int temA = deexZ + deexN;
  double exeval = -1;
  if (temA == 11) {
    exeval = getExEval(fracIso1);
  } else if (temA == 10) {
    if (deexZ> 5 || deexN > 5) {
      exeval = getExEval(fracIso2);
    } else {
      exeval = getExEval(fracIso3);
    }
  } else if (temA == 9) {
    if (deexZ == 3 || deexN == 3) {
      exeval = getExEval(fracIso4);
    } else {
      exeval = getExEval(fracIso5);
    }
  } else if (temA == 8) {
    if (deexZ == 3 || deexN == 3) {
      exeval = getExEval(fracIso6);
    } else {
      exeval = getExEval(fracIso7);
    }
  } else {

    exeval = getExEval(fracIso8);
  }
  
   exe = exeval;
   Extag = (int)exeval; // Extag will be used in "LoadDeeData()"

}


double deex::get_exe() const{
	return exe;
}

double deex::getExEval(double*frac){

  double exeval0 = -1;
  exe_pro.clear();
  for (int i = 0; i < 5; i++) {
    exe_pro.insert(std::map<double, double>::value_type(ExcitedE[i], frac[i]));
  }
  std::map<double, double>::iterator it;

  double last_pro = 0.;
  double curr_pro = 0.;
  double seed1 = gRandom->Uniform(0, 1);
  for (it = exe_pro.begin(); it != exe_pro.end(); ++it) {
    curr_pro = (it->second) + last_pro;
    if (seed1 < curr_pro && seed1 >= last_pro) {
      exeval0 = it->first;
      break;
    }
    last_pro = (it->second) + last_pro;
  }
  return exeval0;
}

void deex::initial()
{
 isoname[6] = "C";
 isoname[5] = "B";
 isoname[4] = "Be";
 isoname[3] = "Li";
}


void deex::LoadDeeData()
{
  int A = deexZ + deexN;
  TString Deefilename = Deedata+"/"+isoname[deexZ]+A+"_"+Extag+"_deexcitation.root";
 // std::cout<<Deefilename<<std::endl;
  std::map<TString,TFile*>::iterator it;
  it = TString2File.find(Deefilename);
 
  if(it != TString2File.end()) {
       // std::cout<<"no new file??"<<std::endl;
        Nucldee = (TTree*)(it->second)->Get("TreeNucldee");
     }
  else{
     //  std::cout<<"add new file??"<<std::endl;
       TFile* Deefile = new TFile(Deefilename, "read");
       if (!Deefile) {
           cout << "not find right file!" << endl;
           exit(0);
           }
       Nucldee = (TTree*)(Deefile)->Get("TreeNucldee");       
       TString2File.insert(std::pair<TString,TFile*>(Deefilename,Deefile));  

    }

  	Nucldee->SetBranchAddress("deexchiID", &aAfterDeexInfo.DeexChannelID);
  	Nucldee->SetBranchAddress("isoRZ", &aAfterDeexInfo.ResidualZ);
  	Nucldee->SetBranchAddress("isoRN", &aAfterDeexInfo.ResidualN);
  	Nucldee->SetBranchAddress("ndeexPars", &aAfterDeexInfo.NPars);
  	Nucldee->SetBranchAddress("deexPartag", deexPartag);
  	Nucldee->SetBranchAddress("deexParEn", deexParEn);

}

void deex::fill_after_deex_info()
{
  aAfterDeexInfo.Energy.clear();
  aAfterDeexInfo.Pdg.clear();
  int entries = (int)Nucldee->GetEntries();
  int entry = int(gRandom->Uniform(0, entries));
  Nucldee->GetEntry(entry);
 // DeexID = deexchiID;
  for (int jj = 0; jj < aAfterDeexInfo.NPars; jj++) {
     aAfterDeexInfo.Energy.push_back(deexParEn[jj]);
     aAfterDeexInfo.Pdg.push_back(deexPartag[jj]);
  }
 // ResidualZ = isoRZ;
 // ResidualN = isoRN;
  aAfterDeexInfo.ResidualPdg = 1000000000 + aAfterDeexInfo.ResidualZ * 10000 + (aAfterDeexInfo.ResidualZ + aAfterDeexInfo.ResidualN) * 10; 
  
  
  //CloseDeeFile();
}

std::vector<double> deex::GetDeeParE()
{
  return aAfterDeexInfo.Energy;
}

std::vector<int> deex::GetDeePDG()
{
  return aAfterDeexInfo.Pdg;
}

int deex::GetResNuelZ()
{  
  return aAfterDeexInfo.ResidualZ;
}

int deex::GetResNuelN()
{
  return aAfterDeexInfo.ResidualN;
}

int deex::GetDeexChannelID()
{
  return aAfterDeexInfo.DeexChannelID;
}
