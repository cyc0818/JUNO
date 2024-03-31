#ifndef GTGENDEEXTOOL_H
#define GTGENDEEXTOOL_H

#include <iostream>
#include <TROOT.h>
#include <TRandom.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>
#include <map>
#include <vector>
#include <string>

#include <boost/python.hpp>

#include "SniperKernel/ToolBase.h"
#include "SniperKernel/SniperLog.h"
#include "GenTools/IGenTool.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/ToolFactory.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"


struct MassMap {
  MassMap()
  {
    pdg2mass.clear();
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
    pdg2mass[1000060130] = 12.1123;
    pdg2mass[1000080160] = 14.8984;
    pdg2mass[1000070140] = 13.0438;
    pdg2mass[1000160320] = 29.7819;
    pdg2mass[1000140280] = 26.0596;
    pdg2mass[1000130270] = 25.1328;
    pdg2mass[1000260560] = 52.1025;
    pdg2mass[1000120240] = 22.342;
    pdg2mass[1000200400] = 37.2246;
    pdg2mass[1000110230] = 21.4143;
    pdg2mass[1000190390] = 36.294;
  }

  std::map<int, double> pdg2mass;
};


struct  AfterDeexInfo{
  AfterDeexInfo();
  std::vector<double> Energy;
  std::vector<int>    Pdg;
  int  ResidualZ;
  int  ResidualN;
  int  DeexChannelID;
  int  NPars;
  int  ResidualPdg;
 };
 AfterDeexInfo::AfterDeexInfo(){
    Energy.clear();
    Pdg.clear();
    ResidualZ = 0;
    ResidualN = 0;
    DeexChannelID = 0;
    NPars = 0 ;   
    ResidualPdg = 0;
}


std::ostream &operator<<(std::ostream & os , const AfterDeexInfo & adi)
{
	os << " ResidualZ = "<< adi.ResidualZ
	   << " ResidualN = "<< adi.ResidualN
	   << " DeexChannelID = " << adi.DeexChannelID
	   << " ResidualPdg = " << adi.ResidualPdg
	   << " NPars = " << adi.NPars
	   <<std::endl;
	
	
	for( int i = 0 ; i < adi.Energy.size(); i++){
		os << " idx = " << i
		   << " energy = "<< adi.Energy[i]
		   << " pdg_code = "<< adi.Pdg[i]
		   << std::endl;
	}

	return os;		
};

enum IsotopeIdx
{
  kC11B11 = 0,
  kC10Be10 = 1,
  kB10 = 2,
  kLi9C9 = 3,
  kBe9B9 = 4,
  kB8Li8 = 5,
  kBe8 = 6,
  kOther = 7,
  kIsotopeSize
};
typedef std::array<double, 5> ExProb;
typedef std::array<ExProb, kIsotopeSize> FracIso;

class GtGenDeexTool : public ToolBase, 
                      public IGenTool {
public:
  GtGenDeexTool(const std::string& name);
  ~GtGenDeexTool();
  
  bool configure();
  bool mutate(HepMC::GenEvent& event);
private:
  void initial(); 
  // void CloseDeeFile();
  void LoadDeeData();
  //void GetDeeProcess();
  void fill_after_deex_info();
  void SetParams(int Z,int N);
  void SetDeexParPdg(int pdg_code);
  void execute();
  double get_exe() const;

  AfterDeexInfo  GetAfterDeexInfo() const;
    //AfterDeexInfo &  GetAfterDeexInfo(){ GetDeeProcess(); return aAfterDeexInfo;}
    //-------------------------------------//
private:
  int Extag;
  double exe;
public:
  void calculateExE();
  //double GetExE(){return exe;}
  
  double getExEval(ExProb frac);

  
//-----------------------------------//
  std::vector<double> GetDeeParE();
  std::vector<int> GetDeePDG();
  int GetResNuelZ();
  int GetResNuelN();
  int GetDeexChannelID();

private:
  AfterDeexInfo  aAfterDeexInfo;            

private:
  TTree *Nucldee;
  std::string Deedata;
  std::map<TString,TFile*> TString2File;
private:
  int deexZ;
  int deexN;
  std::map<int, TString> isoname;

  int deexPartag[20];
  double deexParEn[20];
   
    //--------------------------//
   
private:
  std::map<double, double> exe_pro;
  double ExcitedE[5] = {0, 23, 46, 69, 92};
  // double fracIso1[5] = {2. / 3., 1. / 3., 0., 0., 0.};                                 //--------------> for C-11 and B-11
  // double fracIso2[5] = {6. / 15., 8. / 15., 1. / 15., 0., 0.};                         //--------------> for C-10 and Be-10
  //double fracIso3[5] = {4. / 7., 2. / 7., 1. / 7., 0., 0.};                            
  // double fracIso3[5] = {4./9., 4./9., 1./9., 0., 0.};                                  //--------------> for B-10
  // double fracIso4[5] = {1. / 5., 3. / 5., 1. / 5., 0., 0.};                            //--------------> for Li-9/C-9
  // double fracIso5[5] = {12. / 45., 22. / 45., 10. / 45., 1. / 45., 0.};                //--------------> for Be-9/B-9
  // double fracIso6[5] = {2. / 15., 7. / 15., 5. / 15., 1. / 15., 0.};                   //--------------> for B-8/Li-8
  // double fracIso7[5] = {36. / 225., 96. / 225., 76. / 225., 16. / 225., 1. / 225.};    //--------------> for Be-8
  // double fracIso8[5] = {24. / 300., 104. / 300., 124. / 300., 44. / 300., 4. / 300.};

  FracIso fracIsos {
    {
    {2. / 3., 1. / 3., 0., 0., 0.},
    {6. / 15., 8. / 15., 1. / 15., 0., 0.},
    {4./9., 4./9., 1./9., 0., 0.},
    {1. / 5., 3. / 5., 1. / 5., 0., 0.},
    {12. / 45., 22. / 45., 10. / 45., 1. / 45., 0.},
    {2. / 15., 7. / 15., 5. / 15., 1. / 15., 0.},
    {36. / 225., 96. / 225., 76. / 225., 16. / 225., 1. / 225.},
    {24. / 300., 104. / 300., 124. / 300., 44. / 300., 4. / 300.}
    }
  };

  MassMap MM;
};

#endif

