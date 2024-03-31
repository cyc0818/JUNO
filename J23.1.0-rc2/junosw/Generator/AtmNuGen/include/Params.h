#ifndef PARAMS_hh
#define PARAMS_hh


#include <TString.h>
#include <map>
#include <vector>
struct Params{

   Params(){
     rseed = 0;
     nEvents = 1000000;
     isoStatus = 1;
     outFilename=NULL;
     user_output=NULL;
     const char* path = getenv("JUNOTOP");
     data_dir = path;
     data_dir += "/data/Generator/AtmNC/data/";
     GenieFilename = "/genie3.0.2_atm.root";

     talys_dir = path; 
     talys_dir += "/data/Generator/AtmNC/data/talys_sample/";  
   
     interType.clear();
     currentTag = 0 ;
     flavorTag.clear();
     pdgCodes.clear();
     TransEMax  = 0 ;
     TransEMin  = 0 ;
     GenieFileMap[0] = "/genie3.0.2_atm.root";
     GenieFileMap[1] = "/genie_ccdata.root";
     GenieFileMap[2] = "/genie_ncdata_02.root";
     GenieFileMap[3] = "/nuwro_nc_rfg990_02.root";
     GenieFileMap[4] = "/nuwro_nc_rfg1030_02.root";
     GenieFileMap[5] = "/nuwro_nc_rfg1350_02.root";
     GenieFileMap[6] = "/nuwro_nc_rfg990withTEM_02.root";
     GenieFileMap[7] = "/nuwro_nc_sf990_02.root";
     GenieFileMap[8] = "/genie3.0.6_atm.root";
     doDeex = true ;
     dosampling = true;
     rndstart = false;
     treename = "particleT";
  }

   long rseed;
   unsigned long int nEvents;
   int isoStatus;
   TString data_dir;
   TString GenieFilename;
   TString talys_dir;
   char* outFilename;
   char* user_output;

   std::vector<int> interType;
   int currentTag;
   std::vector<int> flavorTag;
   double TransEMax;
   double TransEMin;
   std::map<int,TString> GenieFileMap;
   bool doDeex;
   bool dosampling;
   bool rndstart;
   TString treename;

   std::vector<int> pdgCodes;
};
#endif
