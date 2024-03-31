#include "GtGenDeexTool.h"

#include <iostream>
#include <map>
#include <stdio.h>

#include "TROOT.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TDatabasePDG.h"
#include "TString.h"

#include "CLHEP/Units/SystemOfUnits.h"

using namespace std;

DECLARE_TOOL(GtGenDeexTool);

GtGenDeexTool::GtGenDeexTool(const std::string& name) : ToolBase(name)
{
  declProp("TalysDir", Deedata);
}

GtGenDeexTool::~GtGenDeexTool()
{
   std::map<TString,TFile*>::iterator it;
   for (it=TString2File.begin(); it!=TString2File.end(); ++it){
          (it->second)->Close();
         //std::cout<<it->first<<std::endl;
      }
}

bool GtGenDeexTool::configure()
{
  LogInfo << "Configure GtGenDeexTool" << std::endl;
  initial();
  return true;
}

bool GtGenDeexTool::mutate(HepMC::GenEvent &hepmc_event)
{
  // calculate the residual nuclear of hepmc event
  auto hepmc_vertex = hepmc_event.signal_process_vertex();
  auto pTarget = hepmc_vertex->particles_in_const_begin()+1;
  const int TargetPDG = (*pTarget)->pdg_id();
  int RemPDG = TargetPDG;
  if(RemPDG==2212) RemPDG = 1000010010;
  int nN = 0, chargeN = 0;
  int arrayBar[] = {2212,2112,3122,3222,3212,3112,3322,3312,3332,4122,4112,4212,4222,4132,4232,4332};
  int arrayQP[] = {2212,3222,4122,4212,4222,4222,4232,211,213,321,-11,-13,-15};
  int arrayQM[] = {3112,3312,3332,-211,-213,-321,11,13,15};
  for(auto itp = hepmc_event.particles_begin(); itp != hepmc_event.particles_end(); ++itp)
  {
    int pdgid = (*itp)->pdg_id();
    nN += std::count(std::begin(arrayBar),std::end(arrayBar),pdgid);
    chargeN += std::count(std::begin(arrayQP),std::end(arrayQP),pdgid);
    chargeN -= std::count(std::begin(arrayQM),std::end(arrayQM),pdgid);
  }
  RemPDG -= nN*10 + chargeN*1e4;
  int AllowedPDG[] = {1000060110,1000060100,1000060090,1000050110,1000050100,1000050090,1000050080,1000040100,1000040090,1000040080,1000040070,1000030090,1000030080,1000030070};
  if(std::count(std::begin(AllowedPDG),std::end(AllowedPDG),RemPDG)>0)
  {
    auto hepmc_iso = new HepMC::GenParticle(HepMC::FourVector(0.*CLHEP::GeV, 
                                                              0.*CLHEP::GeV, 
                                                              0.*CLHEP::GeV, 
                                                              MM.pdg2mass[RemPDG]*CLHEP::GeV),
                                            RemPDG,
                                            0x6000 /* status */);
    hepmc_vertex->add_particle_out(hepmc_iso);
    // Do deexcitation
    auto hepmc_devtx = new HepMC::GenVertex(HepMC::FourVector(0*CLHEP::meter,
                                                              0*CLHEP::meter,
                                                              0*CLHEP::meter,
                                                              0*CLHEP::second));
    hepmc_event.add_vertex(hepmc_devtx);
    hepmc_devtx->add_particle_in(hepmc_iso);
    SetDeexParPdg(RemPDG);
    execute();
    LogInfo << "AfterDeexInfo = " << aAfterDeexInfo << std::endl;
    
    for(int iPar = 0; iPar < aAfterDeexInfo.NPars; ++iPar)
    {
      auto db_pdg = TDatabasePDG::Instance();
      double mass_deex = db_pdg->GetParticle(aAfterDeexInfo.Pdg[iPar])->Mass();
      double E_deex = aAfterDeexInfo.Energy[iPar]*1e-3+mass_deex;
      double p_deex = sqrt(E_deex*E_deex-mass_deex*mass_deex);
      double tempct = gRandom->Uniform(-1,1);
      double tempst = sqrt(1-tempct*tempct);
      double tempphi = gRandom->Uniform(0,2*TMath::Pi());
      auto hepmc_deex = new HepMC::GenParticle(HepMC::FourVector(p_deex*TMath::Cos(tempphi)*tempst*CLHEP::GeV, 
                                                                 p_deex*TMath::Sin(tempphi)*tempst*CLHEP::GeV, 
                                                                 p_deex*tempct*CLHEP::GeV, 
                                                                 E_deex*CLHEP::GeV),
                                              aAfterDeexInfo.Pdg[iPar],
                                              1 /* status */);
      hepmc_devtx->add_particle_out(hepmc_deex);
    }
    if(aAfterDeexInfo.NPars != 0)
    {
      auto hepmc_deexN = new HepMC::GenParticle(HepMC::FourVector(0.*CLHEP::GeV, 
                                                                0.*CLHEP::GeV, 
                                                                0.*CLHEP::GeV, 
                                                                MM.pdg2mass[aAfterDeexInfo.ResidualPdg]*CLHEP::GeV),
                                              aAfterDeexInfo.ResidualPdg,
                                              1 /* status */);
      hepmc_devtx->add_particle_out(hepmc_deexN);
    }
  }
  
  return true;
}

void GtGenDeexTool::SetDeexParPdg(int pdg_code){

	int iso_rp = int((pdg_code - 1000000000) / 10000);
	int iso_rn = int((pdg_code - 1000000000 - iso_rp * 10000) / 10) - iso_rp;
	SetParams(iso_rp, iso_rn);	

}

void GtGenDeexTool::SetParams(int Z, int N){
 
  deexZ = Z;
  deexN = N;
  //caculateExE();
  
 }

AfterDeexInfo GtGenDeexTool::GetAfterDeexInfo() const{
	
	//GetDeeProcess();
	return aAfterDeexInfo;
}

// Crucial API to calculate deexcitation
void GtGenDeexTool::execute(){
	
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

void GtGenDeexTool::calculateExE()
{
  int temA = deexZ + deexN;
  IsotopeIdx isoidx = kOther;
  if(temA == 11) isoidx = kC11B11;
  else if(temA == 10)
  {
    if(deexZ> 5 || deexN > 5) isoidx = kC10Be10;
    else isoidx = kB10;
  }
  else if(temA == 9)
  {
    if(deexZ == 3 || deexN == 3) isoidx = kLi9C9;
    else isoidx = kBe9B9;
  }
  else if(temA == 8)
  {
    if(deexZ == 3 || deexN == 3) isoidx = kB8Li8;
    else isoidx = kBe8;
  }
  else isoidx = kOther;
  double exeval = getExEval(fracIsos[isoidx]);
  exe = exeval;
  Extag = (int)exeval; // Extag will be used in "LoadDeeData()"
}


double GtGenDeexTool::get_exe() const{
	return exe;
}

double GtGenDeexTool::getExEval(ExProb frac){

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

void GtGenDeexTool::initial()
{
  isoname[6] = "C";
  isoname[5] = "B";
  isoname[4] = "Be";
  isoname[3] = "Li";
}


void GtGenDeexTool::LoadDeeData()
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
       TFile* Deefile = TFile::Open(Deefilename, "read");
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

void GtGenDeexTool::fill_after_deex_info()
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

std::vector<double> GtGenDeexTool::GetDeeParE()
{
  return aAfterDeexInfo.Energy;
}

std::vector<int> GtGenDeexTool::GetDeePDG()
{
  return aAfterDeexInfo.Pdg;
}

int GtGenDeexTool::GetResNuelZ()
{  
  return aAfterDeexInfo.ResidualZ;
}

int GtGenDeexTool::GetResNuelN()
{
  return aAfterDeexInfo.ResidualN;
}

int GtGenDeexTool::GetDeexChannelID()
{
  return aAfterDeexInfo.DeexChannelID;
}
