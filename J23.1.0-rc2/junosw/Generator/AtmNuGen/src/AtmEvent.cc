#include "AtmEvent.h"

AtmEvent::AtmEvent(TString path, TString rtreename){
    iso_rp = 0 ;
    iso_rn = 0 ;
    isstable = false ;
    isnoiso = false ;
    genieFile = NULL;
    geTree  = NULL;
    useroutFile = NULL;
    useroutFilename = "";
    useroutTree = NULL;
    entries = 0;
    PickEvtIdList.clear();
  
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

    LoadReadFile(path, rtreename);  
 }

AtmEvent::~AtmEvent(){
  
   delete geTree;
   delete genieFile;

}

void AtmEvent::OutputPickEvent(){
     
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
     useroutTree->Branch("pPdg", &data.pPdg,"pPdg/I");
     useroutTree->Branch("pPx", &data.pPx,"pPx/D");
     useroutTree->Branch("pPy", &data.pPy,"pPy/D");
     useroutTree->Branch("pPz", &data.pPz,"pPz/D");
     useroutTree->Branch("pEn", &data.pEn,"pEn/D");
     useroutTree->Branch("interType", &data.interType,"interType/I");
     useroutTree->Branch("ctag", &data.ctag,"ctag/I");
     
     useroutTree->Branch("m_isoPdg", &data.m_isoPdg,"m_isoPdg/I");
     useroutTree->Branch("m_isoPx", &data.m_isoPx,"m_isoPx/D");
     useroutTree->Branch("m_isoPy", &data.m_isoPy,"m_isoPy/D");
     useroutTree->Branch("m_isoPz", &data.m_isoPz,"m_isoPz/D");
     useroutTree->Branch("m_isoMass", &data.m_isoMass,"m_isoMass/D"); 

     useroutTree->Branch("Npars", &data.Npars,"Npars/I");
     useroutTree->Branch("pdg", data.pdg,"pdg[Npars]/I");
     useroutTree->Branch("px", data.px,"px[Npars]/D");
     useroutTree->Branch("py", data.py,"py[Npars]/D");
     useroutTree->Branch("pz", data.pz,"pz[Npars]/D");
     useroutTree->Branch("energy", data.energy,"energy[Npars]/D");
     useroutTree->Branch("mass", data.mass,"mass[Npars]/D");   
     useroutTree->Branch("weight",&data.weight,"weight/D");
     
     std::vector<int>::iterator it;
     for(it = PickEvtIdList.begin() ; it != PickEvtIdList.end() ; it++){
         geTree->GetEntry(*it);
         data.evtID = *it ; 
         useroutTree->Fill();
       } 
     useroutFile->cd();
     useroutTree->Write();
     useroutFile->Close();        
}


void AtmEvent::PrintTotalPars( int num ){
    fprintf(stream, "%d\n", num);    
}

/*
 * The below is to check whether the pdgcode is in hep_datapdgs. therefore we can select the specify particle.
 *
 * */

bool AtmEvent::IsContainParticles(std::vector<int> & pdg_Codes_select ){
	if (pdg_Codes_select.size() == 0){
		return true;
	}

	int hep_data_count = hep_data.get_par_num();
	for(int idx = 0 ; idx < pdg_Codes_select.size();idx++){
		int sel_pdg = pdg_Codes_select[idx];
		bool flag = false;

		for(int jdx = 0 ; jdx < hep_data_count; jdx++ ){
			int final_pdg = hep_data.m_pdgs[jdx];
			if(final_pdg == sel_pdg){
				flag = true;
				break;
			}
		}

		if(!flag){
			return false;
		}
	}

	return true;


}

void AtmEvent::CollectNormPars(){
		
     double epsilon = 1e-8;
     double ek = 0 ;
     for (int i = 0; i < data.Npars; i++) {
       ek = sqrt(data.px[i]*data.px[i]+data.py[i]*data.py[i]+data.pz[i]*data.pz[i]+data.mass[i]*data.mass[i])-data.mass[i];
       if( std::fabs(ek) < epsilon ){
           ek = 1e-6;
           Hep3Vector newPv = PointPv(ek, data.mass[i]);
           data.px[i] = newPv.x();
           data.py[i] = newPv.y();
           data.pz[i] = newPv.z(); 
        }
		hep_data.add_one_par(data.pdg[i], data.px[i], data.py[i], data.pz[i], data.mass[i]);
     }

}

void AtmEvent::PrintNormPars(){
    
     double epsilon = 1e-8;
     double ek = 0 ;
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

void AtmEvent::CollectIos(){

     hep_data.add_one_par( data.m_isoPdg, data.m_isoPx, data.m_isoPy, data.m_isoPz, pdg2mass[data.m_isoPdg] );

}
 
void AtmEvent::PrintIos(){
     fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", data.m_isoPdg,data.m_isoPx,data.m_isoPy, data.m_isoPz, pdg2mass[data.m_isoPdg]);
}

void AtmEvent::CollectDeexInfo(){
	
	// This function will collect deex information.
	// if deex info is none, then just collectIos.
	if(!Info.NPars){
		CollectIos();
		return;
	}
	
	assert( Info.NPars > 0 );
  	for (int k = 0; k < Info.NPars; k++) {
        double MASS = pdg2mass[Info.Pdg[k]];
        Hep3Vector pv = PointPv(Info.Energy[k], MASS);
	    hep_data.add_one_par(Info.Pdg[k], pv.x(), pv.y(), pv.z(), MASS);
	}
    if ( Info.ResidualZ >= 3 && Info.ResidualN >= 3){  
        hep_data.add_one_par(Info.ResidualPdg , data.m_isoPx, data.m_isoPy,data.m_isoPz,pdg2mass[Info.ResidualPdg]);
	}

}

void AtmEvent::ResetHepevtDataIsoStatus(int status){
	hep_data.reset_iso_status(status);
}

void AtmEvent::PrintHepevtData(){
	
	int count = hep_data.get_par_num();
	fprintf(stream, "%d\n", count);
	for(int i = 0 ;  i < count ; i++){
		int pdg = (hep_data.m_pdgs)[i];
		int status = (hep_data.m_statuss)[i];
		double mass = (hep_data.m_masss)[i];
		double px = (hep_data.m_moms)[i][0];
		double py = (hep_data.m_moms)[i][1];
		double pz = (hep_data.m_moms)[i][2];
		fprintf(stream, "%d\t%d 0 0 %f %f %f %f\n", status, pdg  , px, py, pz, mass);
	}

}

void AtmEvent::ClearHepevtData(){
		hep_data.clear();// clear the hepevt data.
}

void AtmEvent::PrintDeexInfo(){
   
    int count = 0;
    count = data.Npars + Info.NPars;
    if ( Info.ResidualZ >= 3 && Info.ResidualN >= 3)  {
          count = count + 1;
       }
     fprintf(stream, "%d\n", count);
  
    for (int k = 0; k < Info.NPars; k++) {
 
           double MASS = pdg2mass[Info.Pdg[k]];
           Hep3Vector pv = PointPv(Info.Energy[k], MASS);
           fprintf(stream, "1\t%d 0 0 %f %f %f %f\n", Info.Pdg[k], pv.x(), pv.y(), pv.z(), MASS);     
     }
    
    if ( Info.ResidualZ >= 3 && Info.ResidualN >= 3)  {
          fprintf(stream, "1\t%d 0 0 %f %f %f %f\n",Info.ResidualPdg , data.m_isoPx, data.m_isoPy,data.m_isoPz,pdg2mass[Info.ResidualPdg]);
       }



 }

Hep3Vector AtmEvent::PointPv(double Energy, double Mass ){

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



void AtmEvent::LoadReadFile(TString infile, TString rtreename = "particleT"){

     genieFile = TFile::Open(infile, "read");
     if (!genieFile) {
        cout << "Can not find file: " << infile << endl;
        exit(-1);
     }
     geTree = (TTree*)genieFile->Get(rtreename); 
     entries = (int)geTree->GetEntries();
    
  geTree->SetBranchAddress("tPdg", &data.tPdg);
  geTree->SetBranchAddress("pPdg", &data.pPdg);
  geTree->SetBranchAddress("pPx", &data.pPx);
  geTree->SetBranchAddress("pPy", &data.pPy);
  geTree->SetBranchAddress("pPz", &data.pPz);
  geTree->SetBranchAddress("pEn", &data.pEn);
  geTree->SetBranchAddress("interType", &data.interType);
  geTree->SetBranchAddress("ctag", &data.ctag);
  geTree->SetBranchAddress("m_isoPdg", &data.m_isoPdg);
  geTree->SetBranchAddress("m_isoPx", &data.m_isoPx);
  geTree->SetBranchAddress("m_isoPy", &data.m_isoPy);
  geTree->SetBranchAddress("m_isoPz", &data.m_isoPz);
  geTree->SetBranchAddress("m_isoMass", &data.m_isoMass);

  geTree->SetBranchAddress("Npars", &data.Npars);
  geTree->SetBranchAddress("pdg", data.pdg);
  geTree->SetBranchAddress("px", data.px);
  geTree->SetBranchAddress("py", data.py);
  geTree->SetBranchAddress("pz", data.pz);
  geTree->SetBranchAddress("energy", data.energy);
  geTree->SetBranchAddress("mass", data.mass);
  geTree->SetBranchAddress("weight",&data.weight);
  
}

void AtmEvent::LoadEvent(int evtid){
     geTree->GetEntry(evtid);
     if (data.Npars > MAX_PAR ){
        std::cout<<"error: MAXPAR < Npars !!";
        exit(0); 
     }; // Npars must less than MAX_PAR ; 
     iso_rp = int((data.m_isoPdg - 1000000000) / 10000);
     iso_rn = int((data.m_isoPdg - 1000000000 - iso_rp * 10000) / 10) - iso_rp;

     if (data.m_isoPdg == 1000060120 || data.m_isoPdg == 1000030060 || iso_rp < 3 || iso_rn < 3 || iso_rp>6 || iso_rn>6) {
         isstable = true;
     }
     else{
         isstable = false;
     }
     if (data.m_isoPdg == 0 || iso_rp + iso_rn > 11) {
         isnoiso = true;
     }
     else{
         isnoiso = false;
     }                    
}

int AtmEvent::GetNpars(){
    return data.Npars;
}

int AtmEvent::GetIsoP()
{
     return iso_rp;
}

int AtmEvent::GetIsoN()
{
     return iso_rn ;
}

int AtmEvent::GetIsoPdg() const{
	return data.m_isoPdg;
}

bool AtmEvent::GetIsStableFlag()
{
  return isstable ;
}

bool AtmEvent::GetIsNoIsoFlag(){
  return isnoiso ;
}
