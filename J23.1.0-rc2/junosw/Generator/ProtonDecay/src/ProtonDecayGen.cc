/** Author
********************************************************************************************************

   proton decay generator include deexcitation process, author : Jie Cheng ,email : chengjie@ihep.ac.cn
   this proton decay generator was updated by Hu Yuxiang 2021/4/16, email :huyuxiang@ihep.ac.cn

********************************************************************************************************
**/



/** The work flow:
********************************************************************************************************
    *  Genie -->  pd1.1.ghep.histo.root  -->  ProtonDecayGen.exe  -->  HepMCEvent format --> Geant4
********************************************************************************************************
**/


/** The details information of pd1.1.ghep.histo.root 
********************************************************************************************************
    pd1.1.ghep.histo.root has a "events" tree which was used by this exe file. The following is the 
explanation of the variable of the tree.
    * tPdg              :  The pdg of initial nucleus that produces proton decay event
    * tPx, tPy, tPz     :  The momentum of proton in the initial nucleus(H, C12 ...)
    * tE                :  The energy of proton in the initial nucleus(H, C12 ...)
    * tMass             :  The mass of initial nucleus that produces proton decay event
    * tRemovalE         :  Removal Energy , tE = m_p - tRemovalE ( m_p is mass of free proton ) 
    * m_isoPdg          :  The pdg of residual nuclear 
    * m_isoPx, m_isoPy, m_isoPz   : The momentum of residual nuclear
    * m_isoE            :  The energy of residual nuclear
    * m_isoMass         :  The mass of residual nuclear which is in ground state
    * m_isoEffMass      :  The effective mass of residual nuclear
    			   m_isoEffMass := sqrt( m_isoE^2 - m_isoPx^2 - m_isoPy^2 - m_isoPz^2 )
    * m_isoEx           :  Excited energy of residual nuclear
                           m_isoEx = m_isoEffMass - m_isoMass

    ***********************************************************
    The following information will be used by Geant4	
    ***********************************************************

    * Npars             :  The number of final particles
    * pdg               :  The pdg of final particles
    * px, py, pz        :  The momentum of final particles 
    * energy            :  The energy of final particles
                           satisfy E^2 - p^2 = m^2
    * mass              :  The mass of final particles
    * energyT           :  The total energy of final particles 
                            1) w/   deexcitation : energyT = tMass
		            2) w/o  deexcitation : energyT + m_isoE = tMass		
    * Kpx, Kpy, Kpz     :  The momentum when K+ generated (the momentum before FSI) 
    * Kenergy           :  The kinetic energy when K+ generated ( before FSI)
                           Kenergy^2 + 2*m_k * K_energy = Kpx^2 + Kpy^2 + Kpz^2


**/
/**Usage
******************************************************************************************************************
  1. just run the command below:
      ProtonDecay.exe   
  
      *  You may get some error output like that:
      
          Error in <TFile::TFile>: file /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Generator/ProtonDecay/data//pd1.1.ghep.histo.root does not exist
          Can not find file: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/data/Generator/ProtonDecay/data//pd1.1.ghep.histo.root                                                                                    

          Once you meet the problem as showing above you should set the path to the GENIE file and TALYS file. You can run the command below:

          ProtonDecay.exe -data_dir ~/junofs/juno_centos7/data/Generator/ProtonDecay/data/ -talys_dir ~/junofs/juno_centos7/data/Generator/AtmNC/data/talys_sample/


  2.  Argument interpretation
      [-h ]  :list the all arguments you can choose.
      [-seed SEED]: Specifies the random number seed.
      [-n  nEvents]: Specifies the number of event.
      [-o outputfilename] : Specifies the output file.
      [-user_output]: this file has picked event information 
      [-noDeex ] : if you don't want use Residual nuclear,you can use this.
      [-data_dir /path/to/GenieFile/]:Specifies the path to GENIE file.
      [-f GENIE-file-name] : Specifies the GENIE file name. Default is pd1.1.ghep.histo.root.
      [-talys_dir /path/to/talysdata/] : Specifies the TALYS data. it is associated with deexcitation.

********************************************************************************************************************
**/

 
#include "deex.h"
#include "ProtonDecayGen.h"

using namespace std;
using namespace CLHEP;


int main(int argc, char** argv)
{
  
  Params params;
  ProcessArgs(argc, argv, params);
  gRandom->SetSeed(params.rseed);
  TString path_name = params.data_dir+params.GenieFilename;
//  cout<<"## Genie data is in "<<path_name<<endl;
//  cout<<"## talys data is used in"<<params.talys_dir<<endl;   
  deex* DEProcess = new deex(params.talys_dir);
  ProtonDecayEvent* aEvent = new ProtonDecayEvent(path_name);
  if( params.outFilename != NULL ){
       FILE* stream = fopen(params.outFilename, "w");
       if (stream == NULL) {
           printf("Please enetr a valid filename.\n");
           Usage();
           exit(0);
        }
        aEvent->SetStream(stream);
     }
  //print input parameters from user.
  PrintParams(params,aEvent->GetStream());

 
  int nentries = aEvent->GetEntries();
  for (int i = 0; i < params.nEvents; i++) {
    int ievent = (int)(gRandom->Uniform() * (nentries - 1));
    aEvent->LoadEvent(ievent);
    aEvent->AddPickEvtId(ievent);

    int PARS = aEvent->GetNpars();
    //if doDeex == false , then we just print normal particles and we don't consider residual nuclear.
    if(!params.doDeex){
        aEvent->PrintTotalPars(PARS);
        aEvent->PrintNormPars();
        continue;      
      }

    if (aEvent->GetIsNoIsoFlag()) {
      aEvent-> PrintTotalPars(PARS);
      aEvent->PrintNormPars();
    } else if (aEvent -> GetIsStableFlag() ) {
      aEvent->PrintTotalPars(PARS+1);
      aEvent->PrintNormPars();
      aEvent->PrintIos();
    } else {
      
      int rp = aEvent->GetIsoP();
      int rn = aEvent->GetIsoN();
      double exe = -1 ;
    
      
      DEProcess->SetParams(rp,rn);
      //exe = DEProcess-> GetExE();
       
	  DEProcess->SetParams(rp,rn);
 	  DEProcess->execute();
	  aEvent->Info = DEProcess->GetAfterDeexInfo();


      if (aEvent->Info.NPars > 0) {
        //aEvent -> Info = DEProcess -> GetAfterDeexInfo();
        aEvent->PrintDeexInfo();
        aEvent->PrintNormPars();
      } else {
        aEvent->PrintTotalPars(PARS+1);
        aEvent->PrintNormPars();
        aEvent->PrintIos();   
      }
    }
  }
	
//if you want to know the infomation of event you picked, you shoud 
//specify the following output file	
   if(params.user_output){
      aEvent->SetUserOutFileName(params.user_output);
      aEvent->OutputPickEvent();
   }


  delete DEProcess;  
  return 0;
}

void ProcessArgs(int argc, char** argv, Params& params)
{
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-seed") == 0) {
      i++;
      sscanf(argv[i], "%ld", &params.rseed);
    } else if (strcmp(argv[i], "-n") == 0) {
      i++;
      sscanf(argv[i], "%d", &params.nEvents);
    } else if (strcmp(argv[i], "-data_dir") == 0) {
      i++;
      char * data_dir = new char[strlen(argv[i]) + 1];
      strcpy(data_dir, argv[i]);
      params.data_dir = data_dir;
    } else if (strcmp(argv[i], "-talys_dir") == 0) {
      i++;
      char* talys_dir = new char[strlen(argv[i]) + 1];
      strcpy(talys_dir, argv[i]);
      params.talys_dir = talys_dir;
    } 

      else if (strcmp(argv[i], "-f") == 0) {
      i++;
      char * genie = new char[strlen(argv[i]) + 1];
      strcpy(genie, argv[i]);
      params.GenieFilename = genie;
    }  

      else if(strcmp(argv[i], "-o") == 0) {
      i++;
      params.outFilename = new char[strlen(argv[i]) + 1];
      strcpy(params.outFilename, argv[i]);
    } 
	
      else if(strcmp(argv[i], "-user_output") == 0) {
      i++;
      params.user_output = new char[strlen(argv[i]) + 1];
      strcpy(params.user_output, argv[i]);
    }

      else if(strcmp(argv[i], "-noDeex") == 0) {
      params.doDeex = false ; 
    }
     else if (strcmp(argv[i], "-h") == 0) {
      Usage();
      exit(0);
    }
  }
 
}
void PrintParams(Params a , FILE * stream){
   
  fprintf(stream, "# GENIE data is used from  %s.\n", (a.data_dir+a.GenieFilename).Data());
  fprintf(stream, "# TALYS data is used from  %s.\n", (a.talys_dir).Data());
  fprintf(stream, "# Random seed is %d.\n", a.rseed);
  fprintf(stream, "# Event number == %d.\n", a.nEvents);
  if(a.doDeex){
     fprintf(stream, "# invoke Deexcitation.\n");
  }
  else{
     fprintf(stream, "# not invoke Deexcitation.\n");
  }
  
}

void Usage()
{
  cout << "eg. \"ProtonDecay.exe [-h help] [-seed seed] [-o outputfilename] [-user_output user.root][-n nevents] [-noDeex ][-data_dir /path/to/GenieFile/] [-f GENIE-file-name] [-talys_dir /path/to/talysdata/]\" " << endl;
}

