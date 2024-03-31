#include "DepEnAnaMgr.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "RootWriter/RootWriter.h"

#include "G4SDManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include "NormalTrackInfo.hh"
#include "BirksLawCalculator.hh"

#include "G4OpticalPhoton.hh"
#include "G4Positronium.hh"
#include "G4Positron.hh"
#include "G4Electron.hh"
#include "G4AntiNeutrinoE.hh"
#include "TMath.h"
#include <vector>
#include "TROOT.h"
#include "G4Gamma.hh"
#include "G4LossTableManager.hh"

#include <fstream>
#include <iostream>
using namespace std;

DECLARE_TOOL(DepEnAnaMgr);
DepEnAnaMgr::DepEnAnaMgr(const std::string& name) : ToolBase(name)
{
    m_AN_tree = 0;
    m_AN_Isotope_tree = 0; 
}

DepEnAnaMgr::~DepEnAnaMgr()
{

}

// == Run Action
void
DepEnAnaMgr::BeginOfRunAction(const G4Run* /*aRun*/) {
    // Birks' law calculator
    m_birksLawCalculator = BirksLawCalculator::Instance();

    if(not m_birksLawCalculator->getMCParamsSvc()){
        SniperPtr<IMCParamsSvc> mcgt(*getParent(), "MCParamsSvc");
        if(mcgt.invalid()){
            G4cout<<"Can't find MCParamsSvc."<<G4endl;
            assert(0);
        }
        m_birksLawCalculator->setMCParamsSvc(mcgt.data());
    }

    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
    if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
             << "enalbe it in your job option file." << std::endl;
    return;
    }

    gROOT->ProcessLine("#include <vector>");
    //m_SecPDGID.resize(maxParticleN, std::vector<int>(maxSecondaryMult)); 

    m_AN_tree = svc->bookTree(*m_par, "SIMEVT/AtmoNeu","Interesting in Atmospheric neutrino");
    m_AN_tree->Branch("evtID", &m_eventID, "evtID/I");
    m_AN_tree->Branch("Edep", &m_Edep, "Edep/D");
    m_AN_tree->Branch("Edep_x", &m_Edep_x, "Edep_x/D");
    m_AN_tree->Branch("Edep_y", &m_Edep_y, "Edep_y/D");
    m_AN_tree->Branch("Edep_z", &m_Edep_z, "Edep_z/D");
    m_AN_tree->Branch("QEdep", &m_QEdep, "QEdep/D");
    m_AN_tree->Branch("QEdep_x", &m_QEdep_x, "QEdep_x/D");
    m_AN_tree->Branch("QEdep_y", &m_QEdep_y, "QEdep_y/D");
    m_AN_tree->Branch("QEdep_z", &m_QEdep_z, "QEdep_z/D");
    m_AN_tree->Branch("Water_Edep", &m_Water_Edep, "Water_Edep/D");
    m_AN_tree->Branch("vetoWater_Edep", &m_vetoWater_Edep, "vetoWater_Edep/D");
    m_AN_tree->Branch("ParticleMult", &m_maxParticleMult,"ParticleMult/I");
    m_AN_tree->Branch("InitPDGID",m_InitPDGID, "InitPDGID[ParticleMult]/I");
    m_AN_tree->Branch("InitPosx", m_InitPosx,"InitPosx[ParticleMult]/D");
    m_AN_tree->Branch("InitPosy", m_InitPosy,"InitPosy[ParticleMult]/D");
    m_AN_tree->Branch("InitPosz", m_InitPosz,"InitPosz[ParticleMult]/D");
    m_AN_tree->Branch("ExitPosx", m_ExitPosx,"ExitPosx[ParticleMult]/D");
    m_AN_tree->Branch("ExitPosy", m_ExitPosy,"ExitPosy[ParticleMult]/D");
    m_AN_tree->Branch("ExitPosz", m_ExitPosz,"ExitPosz[ParticleMult]/D");
    m_AN_tree->Branch("InitPx",m_InitPx,"InitPx[ParticleMult]/D");
    m_AN_tree->Branch("InitPy",m_InitPy,"InitPy[ParticleMult]/D");
    m_AN_tree->Branch("InitPz",m_InitPz,"InitPz[ParticleMult]/D");
    m_AN_tree->Branch("TrackLength", m_TrackLength,"TrackLength[ParticleMult]/D");
    m_AN_tree->Branch("InitKineticE", m_InitKineticE,"InitKineticE[ParticleMult]/D");
    m_AN_tree->Branch("InitEdepInLS", m_InitEdepInLS, "InitEdepInLS[ParticleMult]/D");
    m_AN_tree->Branch("InitQEdepInLS", m_InitQEdepInLS, "InitQEdepInLS[ParticleMult]/D");
    m_AN_tree->Branch("InitLossEInLS", m_InitLossEInLS, "InitLossEInLS[ParticleMult]/D");
    m_AN_tree->Branch("InitTrackLengthInLS", m_InitTrackLengthInLS, "InitTrackLengthInLS[ParticleMult]/D");
    m_AN_tree->Branch("InitEdepOutLS", m_InitEdepOutLS, "InitEdepOutLS[ParticleMult]/D");
    m_AN_tree->Branch("InitTrackLengthOutLS", m_InitTrackLengthOutLS, "InitTrackLengthOutLS[ParticleMult]/D");
    m_AN_tree->Branch("InitEdepInVetoWater", m_InitEdepInVetoWater, "InitEdepInVetoWater[ParticleMult]/D");
    m_AN_tree->Branch("InitTrackLengthInVetoWater", m_InitTrackLengthInVetoWater, "InitTrackLengthInVetoWater[ParticleMult]/D");
    m_AN_tree->Branch("InitEdepInCDWater", m_InitEdepInCDWater, "InitEdepInCDWater[ParticleMult]/D");
    m_AN_tree->Branch("InitTrackLengthInCDWater", m_InitTrackLengthInCDWater, "InitTrackLengthInCDWater[ParticleMult]/D");
    m_AN_tree->Branch("SecondaryMult", m_SecondaryMult,"SecondaryMult[ParticleMult]/I");
    m_AN_tree->Branch("SecPDGID", &m_SecPDGID);
    m_AN_tree->Branch("SecKineticE", &m_SecKineticE);
    m_AN_tree->Branch("SecGlobalT", &m_SecGlobalT);
    m_AN_tree->Branch("SecLocalT", &m_SecLocalT);
    m_AN_tree->Branch("SecStartPosx", &m_SecStartPosx);
    m_AN_tree->Branch("SecStartPosy", &m_SecStartPosy);
    m_AN_tree->Branch("SecStartPosz", &m_SecStartPosz);
    m_AN_tree->Branch("SecProcessName", &m_SecProcessName);
    m_AN_tree->Branch("SecParentName", &m_SecParentName);
    m_AN_tree->Branch("SecParentMaterial", &m_SecParentMaterial);

    m_AN_Isotope_tree = svc->bookTree(*m_par, "SIMEVT/AtmoNeuIso","Isotopes in Atmospheric neutrino");
    m_AN_Isotope_tree->Branch("evtID", &m_eventID, "evtID/I");
    m_AN_Isotope_tree->Branch("IsoX", &m_IsoX, "IsoX/D");
    m_AN_Isotope_tree->Branch("IsoY", &m_IsoY, "IsoY/D");
    m_AN_Isotope_tree->Branch("IsoZ", &m_IsoZ, "IsoZ/D");
    m_AN_Isotope_tree->Branch("IsoMomX", &m_IsoMomX, "IsoMomX/D");
    m_AN_Isotope_tree->Branch("IsoMomY", &m_IsoMomY, "IsoMomY/D");
    m_AN_Isotope_tree->Branch("IsoMomZ", &m_IsoMomZ, "IsoMomZ/D");
    m_AN_Isotope_tree->Branch("IsoKineE", &m_IsoKineE, "IsoKineE/D");
    m_AN_Isotope_tree->Branch("IsoLocalTime", &m_IsoLocalTime, "IsoLocalTime/D");
    m_AN_Isotope_tree->Branch("IsoGlobalTime", &m_IsoGlobalTime, "IsoGlobalTime/D");
    m_IsoName = new std::vector<TString>();
    m_IsoGenProcName = new std::vector<TString>();
    m_IsoGenMaterialName = new std::vector<TString>();
    m_IsoPostProcName = new std::vector<TString>();
    m_IsoMotherName = new std::vector<TString>();
    m_AN_Isotope_tree->Branch("IsoName",  m_IsoName);
    m_AN_Isotope_tree->Branch("IsoGenProcName",  m_IsoGenProcName);
    m_AN_Isotope_tree->Branch("IsoGenMaterialName",  m_IsoGenMaterialName);
    m_AN_Isotope_tree->Branch("IsoPostProcName",  m_IsoPostProcName);
    m_AN_Isotope_tree->Branch("IsoMotherName",  m_IsoMotherName);
    m_AN_Isotope_tree->Branch("IsoSecndaryNum", &m_IsoSecndaryNum, "IsoSecndaryNum/I");
    m_IsoSecName = new std::vector<TString>(); 
    m_IsoSecKineE = new std::vector<double>();
    m_IsoSecLocalTime = new std::vector<double>();
    m_IsoSecGlobalTime = new std::vector<double>();
    m_AN_Isotope_tree->Branch("IsoSecName",  m_IsoSecName);
    m_AN_Isotope_tree->Branch("IsoSecKineE",  m_IsoSecKineE);    
    m_AN_Isotope_tree->Branch("IsoSecLocalTime",  m_IsoSecLocalTime);
    m_AN_Isotope_tree->Branch("IsoSecGlobalTime",  m_IsoSecGlobalTime);
    
    // Read the isotopes to be recorded in Historian
  TString isotopes[] = {"B12", "N12", "Li8", "B8", "C9", "Li9", "He8", "C10", "C11", "Be11", "He6", "Be7", "B13", "O15", "N13", "Li11"};
  const int size = sizeof(isotopes)/sizeof(TString);
  std::cout << "--------------------------------------------------------" << std::endl ;
  std::cout << "In this simulation, following isotopes will be recorded:" << std::endl ;
  for(int i = 0; i < size; i++){
    m_isotopes.insert(isotopes[i]);
    std::cout << isotopes[i] << "  |  ";
  }
  std::cout << std::endl ;
  std::cout << "--------------------------------------------------------" << std::endl ;

       
}

void
DepEnAnaMgr::EndOfRunAction(const G4Run* /*aRun*/) {

}

// == Event Action
void
DepEnAnaMgr::BeginOfEventAction(const G4Event* evt) {

  std::cout << "event begin, eventID = "<<evt->GetEventID()<<std::endl;
    m_eventID = evt->GetEventID();
    m_Edep = 0;
    m_QEdep = 0;
    m_Edep_x = 0;
    m_Edep_y = 0;
    m_Edep_z = 0;
    m_QEdep_x = 0;
    m_QEdep_y = 0;
    m_QEdep_z = 0;
    m_Water_Edep = 0;
    m_vetoWater_Edep = 0;
    m_ParticleMult = 0;
    m_maxParticleMult = 0;
    for(int j=0;j<maxParticleN;j++) {
    m_InitPDGID[j] = 0;
    m_InitPosx[j] = 0;
    m_InitPosy[j] = 0;
    m_InitPosz[j] = 0;
    m_ExitPosx[j]  =0;
    m_ExitPosy[j]  =0;
    m_ExitPosz[j]  =0;
    m_InitPx[j] = 0;
    m_InitPy[j] = 0;
    m_InitPz[j] = 0;
    m_TrackLength[j]  =0;
    m_InitKineticE[j] = 0;
    m_InitEdepInLS[j] = 0;
    m_InitLossEInLS[j] = 0;
    m_InitEdepOutLS[j] = 0;
    m_InitEdepInVetoWater[j] = 0;
    m_InitEdepInCDWater[j] = 0;
    m_InitQEdepInLS[j] = 0;
    m_InitTrackLengthInLS[j] = 0;
    m_InitTrackLengthOutLS[j] = 0;
    m_InitTrackLengthInVetoWater[j] = 0;
    m_InitTrackLengthInCDWater[j] = 0;
    m_SecondaryMult[j] = 0;
    }
   
    InitMult.clear();
    InitMult_temp.clear();
    m_SecPDGID.clear();
    SecPDG.clear();
    m_SecGlobalT.clear();
    SecGT.clear();
    m_SecLocalT.clear();
    SecLT.clear();
    m_SecStartPosx.clear();
    Secx.clear();
    m_SecStartPosy.clear();
    Secy.clear();
    m_SecStartPosz.clear();
    Secz.clear();
    m_SecKineticE.clear();
    SecE.clear();
    m_SecParentMaterial.clear();
    SecMaterial.clear();
    m_SecParentName.clear();
    SecParName.clear();
    m_SecProcessName.clear();
    SecProName.clear();

    m_IsotopeMotherName.clear();

}

void
DepEnAnaMgr::EndOfEventAction(const G4Event* /*evt*/) {
   m_SecPDGID.push_back(InitMult);
   m_SecPDGID.push_back(SecPDG);
   m_SecKineticE.push_back(InitMult_temp);
   m_SecKineticE.push_back(SecE);
   m_SecGlobalT.push_back(InitMult_temp);
   m_SecGlobalT.push_back(SecGT);
   m_SecLocalT.push_back(InitMult_temp);
   m_SecLocalT.push_back(SecLT);
   m_SecStartPosx.push_back(InitMult_temp);
   m_SecStartPosx.push_back(Secx);
   m_SecStartPosy.push_back(InitMult_temp);
   m_SecStartPosy.push_back(Secy);
   m_SecStartPosz.push_back(InitMult_temp);
   m_SecStartPosz.push_back(Secz);
   m_SecParentMaterial.push_back(InitMult);
   m_SecParentMaterial.push_back(SecMaterial);
   m_SecParentName.push_back(InitMult);
   m_SecParentName.push_back(SecParName);
   m_SecProcessName.push_back(InitMult);
   m_SecProcessName.push_back(SecProName);

   if (m_Edep>0){
       m_Edep_x /= m_Edep;
       m_Edep_y /= m_Edep;
       m_Edep_z /= m_Edep;
   }
   if (m_QEdep>0){
       m_QEdep_x /= m_QEdep;
       m_QEdep_y /= m_QEdep;
       m_QEdep_z /= m_QEdep;
   }

   m_AN_tree->Fill();
}

// == Tracking Action
void
DepEnAnaMgr::PreUserTrackingAction(const G4Track* /*aTrack*/) {
}

void
DepEnAnaMgr::PostUserTrackingAction(const G4Track* aTrack) {
    //NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());

    if (aTrack->GetParentID() != 0) {
        return;
    }
    G4String pname = aTrack->GetDefinition()->GetParticleName();
    G4int current_trkid = aTrack->GetTrackID();
    //std::cout<<" ParentID = "<<aTrack->GetParentID()<<"; trackId ="<<current_trkid<<std::endl;
    const G4ThreeVector& start_pos = aTrack->GetVertexPosition();
    const G4ThreeVector& stop_pos  = aTrack->GetPosition();

    m_ExitPosx[current_trkid-1] = stop_pos.x();
    m_ExitPosy[current_trkid-1] = stop_pos.y();
    m_ExitPosz[current_trkid-1] = stop_pos.z();

    m_TrackLength[current_trkid-1] = aTrack->GetTrackLength();

}

// == Stepping Action
void
DepEnAnaMgr::UserSteppingAction(const G4Step* step) {

    static G4Material* s_mat_LS = nullptr;
    static G4Material* s_mat_Rock = nullptr;
    static G4Material* s_mat_Scintillator = nullptr;
    static G4Material* s_mat_vetoWater = nullptr;
    static G4Material* s_mat_Water = nullptr;
    static G4Material* s_mat_Acrylic = nullptr;
    static G4Material* s_mat_Steel = nullptr;
    if (!s_mat_LS) {
        s_mat_LS = G4Material::GetMaterial("LS");
        s_mat_Rock = G4Material::GetMaterial("Rock");
        s_mat_Scintillator = G4Material::GetMaterial("Scintillator");
        s_mat_vetoWater = G4Material::GetMaterial("vetoWater");
        s_mat_Water = G4Material::GetMaterial("Water");
        s_mat_Acrylic = G4Material::GetMaterial("Acrylic");
        s_mat_Steel = G4Material::GetMaterial("Steel");
    }
   G4Track* fTrack = step->GetTrack();
   G4int StepNo = fTrack->GetCurrentStepNumber();
   G4String process;
   if(step->GetTrack()->GetCreatorProcess()==NULL)  process = "First";
   else process = step->GetTrack()->GetCreatorProcess()->GetProcessName();
   G4String particle;
   particle = step->GetTrack()->GetDefinition()->GetParticleName();
   G4double kineticE = step->GetTrack()->GetKineticEnergy();
   G4double PDG = step->GetTrack()->GetDefinition()->GetPDGEncoding();
   G4ThreeVector Deppos = step->GetPreStepPoint()->GetPosition();
   G4ThreeVector Vertex = step->GetTrack()->GetVertexPosition();
   //G4double totalE = step->GetTrack()->GetTotalEnergy();
   G4double time = step->GetTrack()->GetGlobalTime();
   G4double TrackID = step->GetTrack()->GetTrackID();
   G4double ParentID = step->GetTrack()->GetParentID();
   G4StepPoint* thePrePoint;
   thePrePoint = step->GetPreStepPoint();
   G4StepPoint* thePostPoint;
   thePostPoint = step->GetPostStepPoint();
   G4Material* material =  fTrack->GetMaterial();
   material = thePrePoint->GetMaterial();
   G4String materialname = material->GetName();
   G4String volumename = step->GetTrack()->GetVolume()->GetName();

   m_edep_step = step->GetTotalEnergyDeposit();
   if (step->GetTotalEnergyDeposit() > 0 and fTrack->GetDefinition()!= G4OpticalPhoton::Definition() and material == s_mat_LS) {
   m_Edep += m_edep_step;
   G4ThreeVector pos = step -> GetPreStepPoint() -> GetPosition();
   m_Edep_x += m_edep_step * pos.x();
   m_Edep_y += m_edep_step * pos.y();
   m_Edep_z += m_edep_step * pos.z();
   m_Qedep_step = m_birksLawCalculator->calQuenchedEnergy(step);
   m_QEdep += m_Qedep_step;
   m_QEdep_x += m_Qedep_step * pos.x();
   m_QEdep_y += m_Qedep_step * pos.y();
   m_QEdep_z += m_Qedep_step * pos.z();
   }
   if (step->GetTotalEnergyDeposit() > 0 and fTrack->GetDefinition()!= G4OpticalPhoton::Definition() and material == s_mat_Water) {
   m_Water_Edep += m_edep_step; 
   }
   if (step->GetTotalEnergyDeposit() > 0 and fTrack->GetDefinition()!= G4OpticalPhoton::Definition() and material == s_mat_vetoWater) {
   m_vetoWater_Edep += m_edep_step;
   }
   //std::cout<<" Step num = "<<StepNo<<"parentId ="<<ParentID<<" ; particle name ="<<particle<<std::endl;
   if(ParentID==0){
   //std::cout<<"1---parentId ="<<ParentID<<"; Step num = "<<StepNo<<" ; particle name ="<<particle<<" ; trackId ="<<TrackID<<"; KinE = "<<kineticE<<std::endl;
   m_ParticleMult = TrackID - 1;
    if(m_ParticleMult>=maxParticleN){
      m_ParticleMult = maxParticleN - 1;
      std::cout<<" WARNING: maxMuN = "<<maxParticleN<<" is not enough!! number = "<<m_ParticleMult<<std::endl;
    }
    m_InitPDGID[m_ParticleMult] = PDG;
    if(StepNo==1){
         m_InitPosx[m_ParticleMult] =thePrePoint->GetPosition().x();
         m_InitPosy[m_ParticleMult] =thePrePoint->GetPosition().y();
         m_InitPosz[m_ParticleMult] =thePrePoint->GetPosition().z();
         m_InitPx[m_ParticleMult] =thePrePoint->GetMomentum().x();
         m_InitPy[m_ParticleMult] =thePrePoint->GetMomentum().y();
         m_InitPz[m_ParticleMult] =thePrePoint->GetMomentum().z();
         m_InitKineticE[m_ParticleMult] =thePrePoint->GetKineticEnergy();
    }
    if(material == s_mat_LS){
    m_InitTrackLengthInLS[m_ParticleMult] += step->GetStepLength();
    m_InitEdepInLS[m_ParticleMult] += step->GetTotalEnergyDeposit();
    m_InitQEdepInLS[m_ParticleMult] += m_birksLawCalculator->calQuenchedEnergy(step);
    //m_InitLossEInLS[m_ParticleMult] += (-1)*step->GetDeltaEnergy();
    m_InitLossEInLS[m_ParticleMult] += thePrePoint->GetKineticEnergy() - thePostPoint->GetKineticEnergy();
    }else if(material == s_mat_vetoWater){
    m_InitTrackLengthInVetoWater[m_ParticleMult] += step->GetStepLength();
    m_InitEdepInVetoWater[m_ParticleMult] += step->GetTotalEnergyDeposit();
    }else if(material == s_mat_Water){
    m_InitTrackLengthInCDWater[m_ParticleMult] += step->GetStepLength();
    m_InitEdepInCDWater[m_ParticleMult] += step->GetTotalEnergyDeposit();
    }
   if(material != s_mat_LS){
   m_InitTrackLengthOutLS[m_ParticleMult] += step->GetStepLength();
   m_InitEdepOutLS[m_ParticleMult] += step->GetTotalEnergyDeposit();
   }
   const std::vector<const G4Track*>* Sec = step->GetSecondaryInCurrentStep();
   //G4cout<<"2---Sec num in current step = "<<(*Sec).size()<<G4endl;
   //G4TrackingManager* tm = G4EventManager::GetEventManager()->GetTrackingManager();
   //G4SteppingManager* sm = tm->GetSteppingManager();
   //const G4TrackVector* secs = sm->GetSecondary();
   //G4TrackVector* fSecondary = tm->GimmeSecondaries();
   //std::cout<<"3---Sec size = "<<secs->size()<<"; track Sec size = "<<fSecondary->size()<<std::endl;
   for(size_t i = 0; i< (*Sec).size(); i++){
      //G4Track* sectrk = (*secs)[i];
      const G4VProcess* creatorProcess = (*Sec)[i]->GetCreatorProcess();
      if((*Sec)[i]->GetDefinition()->GetParticleName() != "opticalphoton" && 
          creatorProcess->GetProcessName()!= "Scintillation" && 
          creatorProcess->GetProcessName()!= "muIoni" &&
          creatorProcess->GetProcessName()!= "muBrems" &&   // mu+/mu- -> gamma
          creatorProcess->GetProcessName()!= "hIoni" && 
          creatorProcess->GetProcessName()!= "eIoni" && 
          creatorProcess->GetProcessName()!= "eBrem" &&
          creatorProcess->GetProcessName()!= "compt" &&
          creatorProcess->GetProcessName()!= "phot" ) {
      if(m_SecondaryMult[m_ParticleMult] > maxSecondaryMult){
      m_SecondaryMult[m_ParticleMult] = maxSecondaryMult-1;
      std::cout<<"!!!!!!Sec num exceed setvalue of max sec. num"<<"; Sec name = "<<(*Sec)[i]->GetDefinition()->GetParticleName()<<"; Sec creator process name ="<<creatorProcess->GetProcessName()<<std::endl;
      }
      InitMult.push_back(m_ParticleMult+1);
      InitMult_temp.push_back(m_ParticleMult*1.0+1.0);
      if(material==s_mat_LS)            SecMaterial.push_back(1);
      else if(material==s_mat_Acrylic)  SecMaterial.push_back(2);
      else if(material==s_mat_Water)    SecMaterial.push_back(3); // inner water
      else if(material==s_mat_Steel)    SecMaterial.push_back(4);//support structure
      else if(material==s_mat_vetoWater)SecMaterial.push_back(5); // outer veto water
      else if(material==s_mat_Rock)     SecMaterial.push_back(6);
      else { SecMaterial.push_back(-1); G4cout<<" !!!!unkonwn material = "<<materialname<<G4endl;}

      if(creatorProcess->GetProcessName() == "neutronInelastic") SecProName.push_back(1); //n -> n gamma C11 B7 Be8 p pi0 pi- deuteron triton 
      else if(creatorProcess->GetProcessName() == "lambdaInelastic") SecProName.push_back(2); //lambda -> lambda n gamma C11
      else if(creatorProcess->GetProcessName() == "hadElastic") SecProName.push_back(3);    // n -> p; n -> C12; n->alpha ; n->deuteron; p -> p; pi+ -> C12; pi- -> O16;
      else if(creatorProcess->GetProcessName() == "protonInelastic") SecProName.push_back(4); //p -> p gamma Be11
      else if(creatorProcess->GetProcessName() == "pi+Inelastic") SecProName.push_back(5);  //pi+ -> pi+ alpha gamma He3
      else if(creatorProcess->GetProcessName() == "pi-Inelastic") SecProName.push_back(6);  //pi- -> pi- pi0 alpha gamma n p deuteron B11
      else if(creatorProcess->GetProcessName() == "kaon-Inelastic") SecProName.push_back(7);
      else if(creatorProcess->GetProcessName() == "kaon+Inelastic") SecProName.push_back(8); //kaon+ -> kaon+ gamma n p deuteron Be8 C11 kaon0L pi0 pi+ pi- B9 Be11
      else if(creatorProcess->GetProcessName() == "muMinusCaptureAtRest") SecProName.push_back(9);
      else if(creatorProcess->GetProcessName() == "hBertiniCaptureAtRest") SecProName.push_back(10); //pi- -> e- + gamma + n + p +alpha
      else if(creatorProcess->GetProcessName() == "Decay") SecProName.push_back(11);          //kaon+ -> nu_mu + mu+; kaon0 -> kaon0S; sigma- -> pi- n; sigma+ -> pi0 + p; sigma_c++ ->pi+ + lamda_c+; mu+ -> e+ + nu_e + anti_nu_mu; pi+ -> nu_mu + mu+;
      else if(creatorProcess->GetProcessName() == "nCapture") SecProName.push_back(12);
      else if(creatorProcess->GetProcessName() == "muPairProd") SecProName.push_back(13);    //mu+ -> e+ + e-
      else if(creatorProcess->GetProcessName() == "PsFormation") SecProName.push_back(14);   //e+ -> gamma; e+ -> positronium;
      else if(creatorProcess->GetProcessName() == "conv") SecProName.push_back(15);          //gamma -> e+ + e-
      else if(creatorProcess->GetProcessName() == "CoulombScat") SecProName.push_back(16);   //mu- ->Al27; mu+ -> C12; mu- -> O16; mu+ -> Si28; mu+ -> p;
      else if(creatorProcess->GetProcessName() == "photonNuclear") SecProName.push_back(17); //gamma -> pi+ p eta_prime alpha Be7
      else if(creatorProcess->GetProcessName() == "electronNuclear") SecProName.push_back(18);//e- -> gamma
      else if(creatorProcess->GetProcessName() == "muonNuclear") SecProName.push_back(19);    //mu- -> 3n + 2p + gamma + C11
      else if(creatorProcess->GetProcessName() == "sigma-Inelastic") SecProName.push_back(20);
      else if(creatorProcess->GetProcessName() == "sigma+Inelastic") SecProName.push_back(21);
      else if(creatorProcess->GetProcessName() == "hPairProd") SecProName.push_back(22);  //pi+ -> e+ + e-
      else {SecProName.push_back(-1); G4cout<<" !!!!unkonwn process = "<<creatorProcess->GetProcessName()<<G4endl;}

      if(particle == "proton") SecParName.push_back(1);
      else if(particle == "neutron") SecParName.push_back(2);
      else if(particle == "nu_e") SecParName.push_back(3);
      else if(particle == "anti_nu_e") SecParName.push_back(4);
      else if(particle == "e-") SecParName.push_back(5);
      else if(particle == "e+") SecParName.push_back(6);
      else if(particle == "nu_mu") SecParName.push_back(7);
      else if(particle == "anti_nu_mu") SecParName.push_back(8);
      else if(particle == "mu-") SecParName.push_back(9);
      else if(particle == "mu+") SecParName.push_back(10);
      else if(particle == "pi+") SecParName.push_back(11);
      else if(particle == "pi-") SecParName.push_back(12);
      else if(particle == "pi0") SecParName.push_back(13);
      else if(particle == "kaon+") SecParName.push_back(14);
      else if(particle == "kaon-") SecParName.push_back(15);
      else if(particle == "kaon0") SecParName.push_back(16);
      else if(particle == "anti_kaon0") SecParName.push_back(17);
      else if(particle == "gamma") SecParName.push_back(18);
      else if(particle == "lambda") SecParName.push_back(19);
      else if(particle == "sigma+") SecParName.push_back(20);
      else if(particle == "sigma-") SecParName.push_back(21);
      else if(particle == "sigma_c++") SecParName.push_back(22);
      else {SecParName.push_back(-1); G4cout<<" !!!!unkonwn particle = "<<particle<<G4endl;}
      SecPDG.push_back((*Sec)[i]->GetDefinition()->GetPDGEncoding());
      SecE.push_back((*Sec)[i]->GetKineticEnergy());
      SecGT.push_back(thePrePoint->GetGlobalTime());
      SecLT.push_back(thePrePoint->GetLocalTime());
      Secx.push_back(Deppos.x());
      Secy.push_back(Deppos.y());
      Secz.push_back(Deppos.z());
      m_SecondaryMult[m_ParticleMult] += 1;
     }
   }
   m_ParticleMult += 1;
   if(m_maxParticleMult < m_ParticleMult){
      m_maxParticleMult = m_ParticleMult;
   }
 }

 /*if(process == "muMinusCaptureAtRest") {
    ofstream out("/dybfs/users/zhangyp/juno_version/juno-J20v2r0/works/AN/root/20210817/particleinfo",ios::app);
    out<<PDG<<"  "<<"  "<<particle<<"  "<<process<<"  "<<StepNo<<"  "<<TrackID<<"  "<<ParentID<<"  "<<time/ns<<"  "<<(step->GetTrack()->GetVertexKineticEnergy())/MeV<<endl;
    out.close();
  }*/
  if(StepNo == 1) m_nSecondaries = 0; // set m_nSecondaries as 0 at the beginning of each track
  const G4TrackVector* fSecondary = step->GetSecondary();

  bool newIsotope = false;
  int neutronNum = 0;
  for(unsigned int iSec = m_nSecondaries; iSec < (*fSecondary).size(); iSec++){
    TString secPartName = (*fSecondary)[iSec]->GetDefinition()->GetParticleName();
    // check if the 2ndary particle is an isotope
    for(std::set<TString>::iterator isoIter = m_isotopes.begin();isoIter != m_isotopes.end(); isoIter++){
      if(secPartName.Contains(*isoIter)){
        //std::cout<<"find an isotope, current particle name="<<particle<<"; m_nSecondaries="<<m_nSecondaries<<"; Sec num="<<(*fSecondary).size()<<"; stepNum="<<StepNo<<"; trackID="<<TrackID<<"; parentID="<<ParentID<<"; sec name = "<<secPartName<<std::endl; 
        newIsotope = true;
        break; // it could be only one of the interesting isotope, so break the loop after mapping it.
      }
    }
    // accumulate the number of neutron produced in current step
    if((*fSecondary)[iSec]->GetDefinition()->GetParticleName() == "neutron"){
      neutronNum++;
    }
 }
 if(newIsotope == true){
    m_IsotopeMotherName[TrackID] = particle;
  }
  if(m_nSecondaries < (*fSecondary).size()){ // only interested in isotope decay step  

  bool isIsotopeOrNot = false;
  TString particlename = step->GetTrack()->GetDefinition()->GetParticleName();
  for(std::set<TString>::iterator isoIter = m_isotopes.begin();isoIter != m_isotopes.end(); isoIter++){
    if(particlename.Contains(*isoIter)){
       particlename = *isoIter;
       isIsotopeOrNot = true;
       break;    //current is an isotope, next need to record its information 
    }
  }
  if(isIsotopeOrNot == true){
    if(material == s_mat_LS){
    isotope_initialize();
    m_IsoX = Vertex.x();
    m_IsoY = Vertex.y();
    m_IsoZ = Vertex.z(); 
    m_IsoMomX = thePrePoint->GetMomentum().x();
    m_IsoMomY = thePrePoint->GetMomentum().y(); 
    m_IsoMomZ = thePrePoint->GetMomentum().z();
    m_IsoKineE = fTrack->GetVertexKineticEnergy();
    m_IsoName->push_back(particlename);
    if(fTrack->GetCreatorProcess()) m_IsoGenProcName->push_back(fTrack->GetCreatorProcess()->GetProcessName());
    m_IsoGenMaterialName->push_back(material->GetName());
    m_IsoLocalTime = fTrack->GetLocalTime()/(1000*ns);
    m_IsoGlobalTime = fTrack->GetGlobalTime()/(1000*ns);
    if(step->GetPostStepPoint()->GetProcessDefinedStep()){
       m_IsoPostProcName->push_back(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
    }
    TString m_isotopeMother = "";
    std::map<int, TString>::iterator iterIso;
    iterIso = m_IsotopeMotherName.find(ParentID);
    if(iterIso != m_IsotopeMotherName.end()){
       m_isotopeMother = iterIso->second;
    } else {
      std::cout << "##### Warning, Isotope mother not found!" << std::endl;
    }
    m_IsoMotherName->push_back(m_isotopeMother); 
    
    m_IsoSecndaryNum = (*fSecondary).size() - m_nSecondaries;
    for(unsigned int iSec = m_nSecondaries; iSec < (*fSecondary).size(); iSec++){
       m_IsoSecKineE->push_back((*fSecondary)[iSec]->GetKineticEnergy());
       m_IsoSecName->push_back((*fSecondary)[iSec]->GetDefinition()->GetParticleName());
      G4Track* sectrk = (*fSecondary)[iSec];
      const G4VProcess* creatorProcess = sectrk->GetCreatorProcess();
      if(creatorProcess->GetProcessName() == "Decay" || creatorProcess->GetProcessName() == "RadioactiveDecay"){
         m_IsoSecLocalTime->push_back(sectrk->GetLocalTime()/(1000*ns));
         m_IsoSecGlobalTime->push_back(sectrk->GetGlobalTime()/(1000*ns)); 
        }
    }
    m_AN_Isotope_tree->Fill();
    }
  }
  }
  m_nSecondaries = (*fSecondary).size();
}

void
DepEnAnaMgr::isotope_initialize(){

  m_IsoX = 0.;
  m_IsoY = 0.;
  m_IsoZ = 0.;
  m_IsoMomX = 0.;
  m_IsoMomY = 0.;
  m_IsoMomZ = 0.;
  m_IsoKineE = 0.;
  m_IsoLocalTime = 0.;
  m_IsoGlobalTime = 0.;
  m_IsoName->clear();
  m_IsoGenProcName->clear();
  m_IsoPostProcName->clear();
  m_IsoGenMaterialName->clear();
  m_IsoMotherName->clear();
  m_IsoSecndaryNum = 0;
  m_IsoSecName->clear();
  m_IsoSecKineE->clear();
  m_IsoSecLocalTime->clear();
  m_IsoSecGlobalTime->clear();

}
