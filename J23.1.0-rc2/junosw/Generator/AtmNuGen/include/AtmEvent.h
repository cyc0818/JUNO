#ifndef ATMEVENT
#define ATMEVENT


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
//#include "PreSelect.h"


#include "Data.h"
#include "AfterDeexInfo.h"
#include "hepevt_data.h"
//struct Data;

using namespace CLHEP;
using namespace std;
class PreSelect;

class AtmEvent{
   public: 
   AtmEvent(TString path, TString rtreename);
   ~AtmEvent();
   
   private:
     Data data;
	 hepevt_data hep_data; // record the particle which will be print.
   private:
     TFile* genieFile;
     TTree* geTree;

     TFile* useroutFile;
     TTree* useroutTree;
     TString useroutFilename;
   public:
     void SetUserOutFileName(TString s){ useroutFilename = s;} 
  public:
     void LoadReadFile(TString infile, TString rtreename);
     void LoadEvent(int evtid);
     int GetNpars();
     int GetIsoP();
     int GetIsoN();
     int GetIsoPdg() const;
     bool GetIsStableFlag();
     bool GetIsNoIsoFlag();
  private:
     int entries;
     int iso_rp;
     int iso_rn;
     bool isstable;
     bool isnoiso;
  public:
     int GetEntries() { return entries; }
  public:
     AfterDeexInfo  Info;

  public:
     void PrintTotalPars( int num );
     void PrintNormPars();
     void PrintIos();
     void PrintDeexInfo();
  public:
     void CollectNormPars();
     void CollectIos();
     void CollectDeexInfo();
     void PrintHepevtData();
     bool IsContainParticles(std::vector<int>& pdg_Codes_select);
     void ClearHepevtData();
     void ResetHepevtDataIsoStatus(int status);

  private:
     FILE* stream = stdout;
  public:
     void SetStream(FILE* a){ stream = a;}
     FILE * GetStream(){return stream;}
  private:
    std::map<int, double> pdg2mass;     

  public:
    Hep3Vector PointPv(double Energy, double Mass);


  public:
    void OutputPickEvent();
    void AddPickEvtId(int id){ PickEvtIdList.push_back(id); }
  private:
    std::vector<int> PickEvtIdList;

  friend PreSelect;
   
};
#endif
