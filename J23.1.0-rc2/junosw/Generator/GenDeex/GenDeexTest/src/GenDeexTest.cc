#include "TString.h"
#include "deex.h"
#include "AfterDeexInfo.h"

#include "G4ParticleTable.hh"
#include "G4String.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
#include "G4GenericIon.hh"
#include "G4LeptonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4NuclideTable.hh"
#include "G4ProcessManager.hh"

struct TestGenDeex{
	TestGenDeex();
	G4IonTable * m_ion_table;
	deex * m_deex;	

	void init_ion_table();
	void init_deex();
	void test_C11();
	bool is_this_channel( AfterDeexInfo &adi , std::vector<int> channel_pdgs );
	
};

TestGenDeex::TestGenDeex(){
	init_ion_table();
	init_deex();		
}

void TestGenDeex::init_deex(){
	
	const char* path = getenv("JUNOTOP");
	TString talys_dir = path; 
    talys_dir += "/data/Generator/AtmNC/data/talys_sample/";
	m_deex = new deex(talys_dir);

}

void TestGenDeex::init_ion_table(){

	G4IonConstructor icon;
	icon.ConstructParticle();
	G4NuclideTable* nuclide_table = G4NuclideTable::GetInstance();
	G4ParticleTable* particletbl = G4ParticleTable::GetParticleTable();

	G4IonTable *theIonTable = particletbl->GetIonTable();
	assert(theIonTable);

	G4ParticleDefinition* genericIon = G4ParticleTable::GetParticleTable()->GetGenericIon();
	assert(genericIon);
	G4ProcessManager* pman= new G4ProcessManager(genericIon);
	genericIon->SetProcessManager(pman);
	assert(pman);

	m_ion_table = theIonTable;

}

void TestGenDeex::test_C11(){
		
	G4ParticleDefinition* par = m_ion_table->GetIon(6,11); // C11
	int iso_pdgcode = par->GetPDGEncoding() ;

	int p_d_Be8_num = 0;
	std::vector<int> p_d_Be8_pdgs = { 2212 , 1000010020, 1000040080};

	int p_alpha_Li6_num = 0;
	std::vector<int> p_alpha_Li6_pdgs = { 2212 , 1000020040, 1000030060};

	int p_B10_num = 0;
	std::vector<int> p_B10_pdgs = { 2212, 1000050100 };

	int p2_Be9_num = 0;
	std::vector<int> p2_Be9_pdgs = { 2212,2212, 1000040090 };

	int d_B9_num = 0;
	std::vector<int> d_B9_pdgs = { 1000010020 , 1000050090 };
		
	int n_C10_num = 0;
	std::vector<int> n_C10_pdgs = { 2112 , 1000060100 };

	int n_p_B9_num = 0;
	std::vector<int> n_p_B9_pdgs = { 2112 , 2212, 1000050090 };

	int alpha_Be7_num = 0;
	std::vector<int> alpha_Be7_pdgs = { 1000020040 , 1000040070 };
	
	int He3_Be8_num = 0;
	std::vector<int> He3_Be8_pdgs = { 1000020030 , 1000040080 };
	int test_num = 100000;

	for(int i = 0 ; i < test_num ; i++){
		
		assert(iso_pdgcode == 1000060110);
		m_deex->SetDeexParPdg(iso_pdgcode);
		m_deex->execute();

		double exe = m_deex->get_exe();
		if ((exe > 24) or (exe < 22)){
			i--;
			continue;
		}

		AfterDeexInfo adi = m_deex->GetAfterDeexInfo();
		if(is_this_channel( adi , p_d_Be8_pdgs)){
			p_d_Be8_num++;
			continue;
		}
	
		if(is_this_channel( adi , p_alpha_Li6_pdgs)){
			p_alpha_Li6_num++;
			continue;
		}	
	
		if(is_this_channel( adi , p_B10_pdgs)){
			p_B10_num++;
			continue;
		}	
		if(is_this_channel( adi , p2_Be9_pdgs)){
			p2_Be9_num++;
			continue;
		}	
		
		if(is_this_channel( adi , d_B9_pdgs)){
			d_B9_num++;
			continue;
		}	

		if(is_this_channel( adi , n_C10_pdgs)){
			n_C10_num++;
			continue;
		}
	
		if(is_this_channel( adi , n_p_B9_pdgs)){
			n_p_B9_num++;
			continue;
		}
	
		if(is_this_channel( adi , alpha_Be7_pdgs)){
			alpha_Be7_num++;
			continue;
		}
			
		if(is_this_channel( adi , He3_Be8_pdgs)){
			He3_Be8_num++;
			continue;
		}
			
		std::cout<<" test_id = "<< i
				 <<" DeexInfo: " << adi
				 << std::endl;
	}
	
	std::cout<<" channel			ratio" <<std::endl;
	std::cout<<" p + d + Be8 	: 	" << p_d_Be8_num/(test_num*1.0) << std::endl;
	std::cout<<" p + alpha + Li6: 	" << p_alpha_Li6_num/(test_num*1.0) << std::endl;
	std::cout<<" p + B10 	 	: 	" << p_B10_num/(test_num*1.0) << std::endl;
	std::cout<<" 2p+ Be9	 	: 	" << p2_Be9_num/(test_num*1.0) << std::endl;
	std::cout<<" d + B9 		: 	" << d_B9_num/(test_num*1.0) << std::endl;
	std::cout<<" n + C10 		: 	" << n_C10_num/(test_num*1.0) << std::endl;
	std::cout<<" n + p + B9 	: 	" << n_p_B9_num/(test_num*1.0) << std::endl;
	std::cout<<" alpha + Be7 	: 	" << alpha_Be7_num/(test_num*1.0) << std::endl;
	std::cout<<" He3   + Be8 	: 	" << He3_Be8_num/(test_num*1.0) << std::endl;

}

bool TestGenDeex::is_this_channel( AfterDeexInfo &adi , std::vector<int> channel_pdgs ){
	
	if(std::find(channel_pdgs.begin(), channel_pdgs.end(), adi.ResidualPdg) == channel_pdgs.end()){
		return false;
	}

	for(int i = 0 ; i < adi.NPars ; i++ ){

		if(adi.Pdg[i] != 22){
			//if it is gamma, we skip it.
			if(std::find(channel_pdgs.begin(), channel_pdgs.end(), adi.Pdg[i]) == channel_pdgs.end()){
        		return false;
    		}
		}
	}
	return true;
}


void TestC11(){

	TestGenDeex tgd = TestGenDeex();
	tgd.test_C11();	
}


void TestG4ParticleTable(){

	G4IonConstructor icon;
	icon.ConstructParticle();
	G4NuclideTable* nuclide_table = G4NuclideTable::GetInstance();
	G4ParticleTable* particletbl = G4ParticleTable::GetParticleTable();

	G4IonTable *theIonTable = particletbl->GetIonTable();
	assert(theIonTable);
	int pdgcode = 1000030080;
	
	G4ParticleDefinition* genericIon = G4ParticleTable::GetParticleTable()->GetGenericIon();
	assert(genericIon);
	G4ProcessManager* pman= new G4ProcessManager(genericIon);
	genericIon->SetProcessManager(pman);
	if (genericIon!=0) pman = genericIon->GetProcessManager();
	assert(pman);

	G4ParticleDefinition* par = theIonTable->GetIon(pdgcode);
	par = theIonTable->GetIon(4,9);

	std::cout<<" pdg_code = "<< par->GetPDGEncoding()
			 <<" name = "<< par-> GetParticleName()
			 <<" AtomicNumber = "<< par->GetAtomicNumber()
			 <<" GetAtomicMass = "<< par->GetAtomicMass()
			 <<" PDGMass = "<< par->GetPDGMass()
			 <<std::endl;

}

void ExampleGenDeex(){


	const char* path = getenv("JUNOTOP");
	TString talys_dir = path; 
    talys_dir += "/data/Generator/AtmNC/data/talys_sample/";

	deex* DEProcess = new deex(talys_dir);
	int iso_pdgcode = 1000030080 ;
	DEProcess->SetDeexParPdg(iso_pdgcode);
	DEProcess->execute();
	AfterDeexInfo adi = DEProcess->GetAfterDeexInfo();
	std::cout <<" AfterDeexInfo = "<< adi << std::endl;

}

int main(int argc, char ** argv){

	TestG4ParticleTable();
	ExampleGenDeex();
	TestC11();	

}
