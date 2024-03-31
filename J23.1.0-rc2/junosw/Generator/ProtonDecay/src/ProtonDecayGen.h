#ifndef PRODECAYGEN_hh
#define PRODECAYGEN_hh


#include <iostream>
#include <fstream>
#include <cassert>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include <TROOT.h>
#include <TRandom.h>
#include <vector>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <cstdlib>
#include <TChain.h>
#include <TString.h>
#include "deex.h"
#include <map>

//we use two structs. One is Params which is used to manage parameters from user, the other is Data which include event information of GENIE file. 

using namespace std;
using namespace CLHEP;
void Usage();

struct Params{

   Params(){
     rseed = 0;
     nEvents = 1000000;
     outFilename=NULL;
     user_output = NULL;
     const char* path = getenv("JUNOTOP");
     data_dir = path;
     data_dir += "/data/Generator/ProtonDecay/data/";
     GenieFilename = "/pd1.1.ghep.histo.root";
      
     talys_dir = path; 
     talys_dir += "/data/Generator/AtmNC/data/talys_sample/";
     doDeex = true ;
    // stream = stdout ;   
   }

   long rseed;
   unsigned long int nEvents;
   TString data_dir;
   TString GenieFilename;
   TString talys_dir;
   char* outFilename;
   char* user_output;
   bool  doDeex;
  // FILE* stream ;
};

const int MAX_PAR = 50;

struct Data{

  Data(){
  
     evtID = 0 ;
     tPdg  = 0 ;
     tPx = 0. ;
     tPy  = 0.;
     tPz  = 0.;
     tRemovalE =0.;
     tE = 0.;
     tMass = 0.;

     m_isoPDG = 0 ;
     m_isoPdg = 0 ;
     m_isoTag = 0 ;
     m_isoPx  = 0.;
     m_isoPy  = 0.;
     m_isoPz  = 0.;
     m_isoE   = 0.;
     m_isoMass = 0.;
     m_isoEffMass = 0.;

     Npars = 0.;
     energyT = 0;

     Kpx = 0 ;
     Kpy = 0  ;
     Kpz = 0  ;
     Kenergy = 0;
}
  
  int evtID;
  int tPdg;
  double tPx;
  double tPy;
  double tPz;
  double tRemovalE;
  double tE;
  double tMass;
  
  int m_isoPDG;
  int m_isoPdg;
  int m_isoTag;
  double m_isoPx;
  double m_isoPy;
  double m_isoPz;
  double m_isoE;
  double m_isoMass;
  double m_isoEffMass;


  int Npars;
  int pdg[MAX_PAR];
  double px[MAX_PAR];      // GeV
  double py[MAX_PAR];      // GeV
  double pz[MAX_PAR];      // GeV
  double energy[MAX_PAR];  // GeV
  double mass[MAX_PAR];    // GeV
  double energyT;

 
  double Kpx ;
  double Kpy  ;           
  double Kpz   ;          
  double Kenergy;         
};

// ProtonDecayEvent is a class. We can use this class to download event information from GENIE file, do deexcitationand output particle information in hepevt format which will be used as the input of Geant4
//
class ProtonDecayEvent{
   public: 
   ProtonDecayEvent(TString path);
   ~ProtonDecayEvent();
   
   private:
     Data data; //save data from GENIE file
   private:
     TFile* genieFile;
     TTree* geTree;
 //    TFile* outFile;
 //    TTree* outTree; 
  public:
     void LoadReadFile(TString infile);// download GENIE file. the Argument of this function is the GENIE file name  
     void LoadEvent(int evtid);// load an event from GENIE file. 
     int GetNpars();  // get number of particles of an event. 
     int GetIsoP();
     int GetIsoN();
     bool GetIsStableFlag();
     bool GetIsNoIsoFlag();
  private:
     int entries; //entries of GENIE file.
     int iso_rp; // number of proton of Residual nuclear.
     int iso_rn; // number of neutron of Residual nuclear.
     bool isstable; // Determine whether the residual nucleus is stable. 

     bool isnoiso;//Determine whether there is a residual nucleus.
  public:
     int GetEntries() {return entries; }
  public:
     AfterDeexInfo  Info;// Residual nuclear information after deexcitation. 

  public:
     void PrintTotalPars( int num ); //print number of total particles. 
     void PrintNormPars() ;//print particles without Residual nuclear.
     void PrintIos(); //print Residual nuclear.
     void PrintDeexInfo();//print Residual nuclear after deexcitation.
  private:
     FILE* stream = stdout;
  public:
     void SetStream(FILE* a){ stream = a;}
     FILE* GetStream() {return stream ; }

  private:
    std::map<int, double> pdg2mass; // the map of PDG ID to particle mass.    

  public:
    Hep3Vector PointPv(double Energy, double Mass);


//out put generator information which we picked
  private:
     TFile* useroutFile;
     TTree* useroutTree;
     TString useroutFilename;
     std::vector<int> PickEvtIdList;
   public:
     void SetUserOutFileName(TString s){ useroutFilename = s;} 

     void OutputPickEvent();
     void AddPickEvtId(int id) { PickEvtIdList.push_back(id); }
    
};

ProtonDecayEvent::ProtonDecayEvent(TString path){
    iso_rp = 0 ;
    iso_rn = 0 ;
    isstable = false ;
    isnoiso = false ;
    genieFile = NULL;
    geTree  = NULL;
    //outFile = NULL;
   // outTree = NULL;
    entries = 0;
  
    pdg2mass[22] = 0;
    pdg2mass[2112] = 0.93957;
    pdg2mass[2212] = 0.93827;
    pdg2mass[1000010020] = 1.8756;
    pdg2mass[1000010030] = 2.8089;
    pdg2mass[1000020030] = 2.8084;
    pdg2mass[1000020040] = 3.7274;
    pdg2mass[1000030060] = 5.6015;
    pdg2mass[1000030070] = 6.5335;
    pdg2mass[1000030080] = 7.4708;
    pdg2mass[1000030090] = 8.4061;
    pdg2mass[1000040070] = 6.5344;
    pdg2mass[1000040080] = 7.4548;
    pdg2mass[1000040090] = 8.3925;
    pdg2mass[1000040100] = 9.3249;
    pdg2mass[1000050080] = 7.4728;
    pdg2mass[1000050090] = 8.3935;
    pdg2mass[1000050100] = 9.3244;
    pdg2mass[1000050110] = 10.2522;
    pdg2mass[1000060090] = 8.4100;
    pdg2mass[1000060100] = 9.3280;
    pdg2mass[1000060110] = 10.2542;
    pdg2mass[1000060120] = 11.1748;    

    LoadReadFile(path);  
 }

ProtonDecayEvent::~ProtonDecayEvent(){
  
   delete geTree;
   delete genieFile;

}


void ProtonDecayEvent::OutputPickEvent(){
     
     if(!useroutFilename){
       fprintf(stream, "plese input valid outFilename\n");
       exit(0);
     }
     if(!PickEvtIdList.size()){
       fprintf(stream, "no picked event!");
       return;
     }
     useroutFile = new TFile(useroutFilename, "RECREATE");
     useroutTree = new TTree("PickEvt", "PickEvt");
     useroutTree->Branch("evtID",&data.evtID,"evtID/I");
     useroutTree->Branch("tPdg", &data.tPdg,"tPdg/I");
     useroutTree->Branch("tPx", &data.tPx,"tPx/D");
     useroutTree->Branch("tPy", &data.tPy,"tPy/D");
     useroutTree->Branch("tPz", &data.tPz,"tPz/D");
	

     useroutTree->Branch("tRemovalE", &data.tRemovalE,"tRemovalE/D");

     useroutTree->Branch("tE", &data.tE,"tE/D");
     useroutTree->Branch("tMass", &data.tMass,"tMass/D");

     
     useroutTree->Branch("m_isoPDG", &data.m_isoPDG,"m_isoPDG/I");
     
     useroutTree->Branch("m_isoPdg", &data.m_isoPdg,"m_isoPdg/I");
     useroutTree->Branch("m_isoTag", &data.m_isoTag,"m_isoTag/I");
     useroutTree->Branch("m_isoPx", &data.m_isoPx,"m_isoPx/D");
     useroutTree->Branch("m_isoPy", &data.m_isoPy,"m_isoPy/D");
     useroutTree->Branch("m_isoPz", &data.m_isoPz,"m_isoPz/D");

     useroutTree->Branch("m_isoE", &data.m_isoE,"m_isoE/D"); 
     useroutTree->Branch("m_isoMass", &data.m_isoMass,"m_isoMass/D"); 
     useroutTree->Branch("m_isoEffMass", &data.m_isoEffMass,"m_isoEffMass/D"); 

     useroutTree->Branch("Npars", &data.Npars,"Npars/I");
     useroutTree->Branch("pdg", data.pdg,"pdg[Npars]/I");
     useroutTree->Branch("px", data.px,"px[Npars]/D");
     useroutTree->Branch("py", data.py,"py[Npars]/D");
     useroutTree->Branch("pz", data.pz,"pz[Npars]/D");
     useroutTree->Branch("energy", data.energy,"energy[Npars]/D");
     useroutTree->Branch("mass", data.mass,"mass[Npars]/D");   
     
     useroutTree->Branch("energyT", &data.energyT,"energyT/D");   

     useroutTree->Branch("Kpx", &data.Kpx,"Kpx/D");
     useroutTree->Branch("Kpy", &data.Kpy,"Kpy/D");
     useroutTree->Branch("Kpz", &data.Kpz,"Kpz/D");
     useroutTree->Branch("Kenergy", &data.Kenergy,"Kenergy/D");
	
     for(int idx = 0 ; idx < PickEvtIdList.size() ; idx ++ ){

         geTree->GetEntry(PickEvtIdList[idx]);
         data.evtID = PickEvtIdList[idx] ; 
        // std::cout<<"evtID="<<data.evtID<<std::endl;
         useroutTree->Fill();

   }
     useroutFile->cd();
     useroutTree->Write();
     useroutFile->Close();        
}



void ProtonDecayEvent::PrintTotalPars( int num ){
    fprintf(stream, "%d\n", num);    
}
void ProtonDecayEvent::PrintNormPars(){
    
    double epsilon = 1e-8;
    double ek = 0 ;
   //GENIE may has some bugs as it produces some neutrons with no kinetic energy. The neutron with no kinetic energy will decay in Geant4, thus we could give this neutron some kinetic energy.  
    for (int i = 0; i < data.Npars; i++) {
       ek = sqrt(data.px[i]*data.px[i]+data.py[i]*data.py[i]+data.pz[i]*data.pz[i]+data.mass[i]*data.mass[i])-data.mass[i];       
       if( std::fabs(ek) < epsilon ){          
           ek = 1e-6;           
           Hep3Vector newPv = PointPv(ek, data.mass[i]);           
           data.px[i] = newPv.x();
           data.py[i] = newPv.y();
           data.pz[i] = newPv.z();
        }  
       fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", data.pdg[i], data.px[i], data.py[i], data.pz[i], data.mass[i]);
     }
}
 
void ProtonDecayEvent::PrintIos(){
     fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", data.m_isoPDG,data.m_isoPx,data.m_isoPy, data.m_isoPz, pdg2mass[data.m_isoPDG]);
}

void ProtonDecayEvent::PrintDeexInfo(){
   
    int count = 0;
    count = data.Npars + Info.NPars;
    if ( Info.ResidualZ >= 3 && Info.ResidualN >= 3)  {
          count = count + 1;
          fprintf(stream, "%d\n", count);
          fprintf(stream, "1\t%d 0 0 %f %f %f %f\n",Info.ResidualPdg , data.m_isoPx, data.m_isoPy,data.m_isoPz,pdg2mass[Info.ResidualPdg]);
       }
    else{
          fprintf(stream, "%d\n", count);
     }

   
    for (int k = 0; k < Info.NPars; k++) {
 
           double MASS = pdg2mass[Info.Pdg[k]];
           Hep3Vector pv = PointPv(Info.Energy[k], MASS);
           fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", Info.Pdg[k], pv.x(), pv.y(), pv.z(), MASS);     
     }

 }

Hep3Vector ProtonDecayEvent::PointPv(double Energy, double Mass ){

  double xdir = 0;
  double ydir = 0;
  double zdir = 0;
  double cost = 0.;
  double sint =0.;
  double phi = 0.;
  double Ekval = Energy*1e-3;
  double Pval = sqrt(pow(Ekval,2)+2*Mass*Ekval);

  cost = 1-2*gRandom->Uniform(0., 1.);
  sint = sqrt(1-cost*cost);
  phi = gRandom->Uniform(0, twopi);
  xdir = sint * cos(phi);
  ydir = sint * sin(phi);
  zdir = cost;

  double px = xdir * Pval;  // GeV
  double py = ydir * Pval;  // GeV
  double pz = zdir * Pval;  // GeV
  Hep3Vector Pv;
  Pv = Hep3Vector(px, py, pz);
  return Pv;

}



void ProtonDecayEvent::LoadReadFile(TString infile){

     genieFile = TFile::Open(infile, "read");
     if (!genieFile) {
        cout << "Can not find file: " << infile << endl;
        exit(-1);
     }
     geTree = (TTree*)genieFile->Get("events"); 
     entries = (int)geTree->GetEntries();
    
  geTree->SetBranchAddress("evtID", &data.evtID);
  geTree->SetBranchAddress("tPdg", &data.tPdg);
  geTree->SetBranchAddress("tPx", &data.tPx);
  geTree->SetBranchAddress("tPy", &data.tPy);
  geTree->SetBranchAddress("tPz", &data.tPz);
  geTree->SetBranchAddress("tE", &data.tE);
  geTree->SetBranchAddress("tMass", &data.tMass);
  geTree->SetBranchAddress("tRemovalE", &data.tRemovalE);
  geTree->SetBranchAddress("m_isoPDG", &data.m_isoPDG);
  geTree->SetBranchAddress("m_isoPdg", &data.m_isoPdg);
  geTree->SetBranchAddress("m_isoPx", &data.m_isoPx);
  geTree->SetBranchAddress("m_isoPy", &data.m_isoPy);
  geTree->SetBranchAddress("m_isoPz", &data.m_isoPz);
  geTree->SetBranchAddress("m_isoE", &data.m_isoE);
  geTree->SetBranchAddress("m_isoMass", &data.m_isoMass);
  geTree->SetBranchAddress("m_isoEffMass", &data.m_isoEffMass);  

  geTree->SetBranchAddress("Npars", &data.Npars);
  geTree->SetBranchAddress("pdg", data.pdg);
  geTree->SetBranchAddress("px", data.px);
  geTree->SetBranchAddress("py", data.py);
  geTree->SetBranchAddress("pz", data.pz);
  geTree->SetBranchAddress("energy", data.energy);
  geTree->SetBranchAddress("mass", data.mass);
  geTree->SetBranchAddress("energyT", &data.energyT);
  
  geTree->SetBranchAddress("Kpx", &data.Kpx);
  geTree->SetBranchAddress("Kpy", &data.Kpy);
  geTree->SetBranchAddress("Kpz", &data.Kpz);
  geTree->SetBranchAddress("Kenergy", &data.Kenergy);
}

void ProtonDecayEvent::LoadEvent(int evtid){
     geTree->GetEntry(evtid);
     if (data.Npars > MAX_PAR){
        std::cout<<" Error : data.Npars need less than MAX_PAR ";
        exit(0);
     };

     iso_rp = int((data.m_isoPDG - 1000000000) / 10000);
     iso_rn = int((data.m_isoPDG - 1000000000 - iso_rp * 10000) / 10) - iso_rp;

     if (data.m_isoPDG == 1000060120 || data.m_isoPDG == 1000030060 || iso_rp < 3 || iso_rn < 3 || iso_rp>6 || iso_rn>6) {
         isstable = true;
     }
     else{
         isstable = false;
     }
     if (data.m_isoPDG == 0 || iso_rp + iso_rn > 11) {
         isnoiso = true;
     }
     else{
         isnoiso = false;
     }                    
}

int ProtonDecayEvent::GetNpars(){
    return data.Npars;
}

int ProtonDecayEvent::GetIsoP()
{
     return iso_rp;
}

int ProtonDecayEvent::GetIsoN()
{
     return iso_rn ;
}

bool ProtonDecayEvent::GetIsStableFlag()
{
  return isstable ;
}

bool ProtonDecayEvent::GetIsNoIsoFlag(){
  return isnoiso ;
}


void ProcessArgs(int argc, char** argv, Params& params);
void PrintParams(Params a ,FILE * stream);
#endif
