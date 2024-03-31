#include <boost/python.hpp>
#include <iostream>

#include "DepositEnergyTTAnaMgr.hh"
#include "NormalTrackInfo.hh"

#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/ToolFactory.h"
#include "RootWriter/RootWriter.h"
#include "G4Event.hh"
#include "G4Box.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4LossTableManager.hh"
// using CLHEP's random engine
#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4OpticalPhoton.hh"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/SimHeader.h"
#include "Event/SimTTHit.h"

#include "DetSimAlg/DetSimAlg.h"
#include "DetSimAlg/IDetElement.h"
#include "Geometry/TTGeomSvc.h"
#include "Identifier/TtID.h"

#include "TROOT.h"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "junoHit_TT.hh"

using namespace CLHEP;
DECLARE_TOOL(DepositEnergyTTAnaMgr);

DepositEnergyTTAnaMgr::DepositEnergyTTAnaMgr(const std::string& name)
  : ToolBase(name)
{
  declProp("EnableNtuple", m_flag_ntuple=false);
  m_evt_treeTT = 0;
  m_evt_treeTTDigit = 0;
  
  m_rnd = 0;
    
}

DepositEnergyTTAnaMgr::~DepositEnergyTTAnaMgr()
{
  
}

void
DepositEnergyTTAnaMgr::BeginOfRunAction(const G4Run* /*aRun*/) {
  m_rnd=new TRandom3();
   
  SniperPtr<DetSimAlg> detsimalg(getParent(), "DetSimAlg");
  if (detsimalg.invalid()) {
    // std::cout << "Can't Load DetSimAlg" << std::endl;
    assert(0);
  }
  std::string name = "TopTrackerConstruction";
  ToolBase* t = detsimalg->findTool(name);
  assert(t);
  
  de=dynamic_cast<IDetElement*>(t);
  
#if 0
  std::ofstream WallCorrespondenceTXT("wall_correspondence.txt");
  for(int iwall=0;iwall<3*7*3; iwall++){
    int wall   = de->geom_info("WallNum", iwall);
    int layer  = de->geom_info("Wall.Layer" ,wall);
    int column = de->geom_info("Wall.Column",wall);
    int row    = de->geom_info("Wall.Row"   ,wall);
    WallCorrespondenceTXT << iwall << "\t" << layer << "\t" << column << "\t" << row << std::endl;
  }
  WallCorrespondenceTXT.close();

  // Positioning following code in ComputeDigitTT...
  std::ofstream ChannelPositionTXT("ChannelPosition.txt");
  G4LogicalVolume * TT_vol = de->getLV();
  for(int iwall=0; iwall < TT_vol->GetNoDaughters(); ++iwall){
    G4VPhysicalVolume * Wall_vol = TT_vol->GetDaughter(iwall);
    G4AffineTransform Wall_transf(Wall_vol->GetRotation(), Wall_vol->GetTranslation());
    int n_wall;
    sscanf(Wall_vol->GetName().c_str(),"pWall_%d",&n_wall);
    for(int iplane=0; iplane < Wall_vol->GetLogicalVolume()->GetNoDaughters(); ++iplane){
      G4VPhysicalVolume * Plane_vol = Wall_vol->GetLogicalVolume()->GetDaughter(iplane);
      G4AffineTransform Plane_transf(Plane_vol->GetRotation(), Plane_vol->GetTranslation());
      int n_plane;
      sscanf(Plane_vol->GetName().c_str(),"pPlane_%d",&n_plane);
      for(int ipannel=0; ipannel < Plane_vol->GetLogicalVolume()->GetNoDaughters(); ++ipannel){
        G4VPhysicalVolume * Pannel_vol = Plane_vol->GetLogicalVolume()->GetDaughter(ipannel);
        G4AffineTransform Pannel_transf(Pannel_vol->GetRotation(), Pannel_vol->GetTranslation());
        int n_pannel;
        sscanf(Pannel_vol->GetName().c_str(),"pPanel_%d",&n_pannel);
        G4VPhysicalVolume * Adhesive_vol = Pannel_vol->GetLogicalVolume()->GetDaughter(0);
        for(int istrip=0; istrip < Adhesive_vol->GetLogicalVolume()->GetNoDaughters(); ++istrip){
          G4VPhysicalVolume * Strip_vol = Adhesive_vol->GetLogicalVolume()->GetDaughter(istrip);
          G4AffineTransform Strip_transf(Strip_vol->GetRotation(), Strip_vol->GetTranslation());
          int n_strip;
          sscanf(Strip_vol->GetName().c_str(),"pCoating_%d",&n_strip);

          // G4AffineTransform TT_transf = Wall_transf * Plane_transf * Pannel_transf * Strip_transf;
          G4AffineTransform TT_transf = Strip_transf * Pannel_transf * Plane_transf * Wall_transf;
          G4ThreeVector TT_global_pos = TT_transf.TransformPoint(G4ThreeVector(0,0,0));

          G4ThreeVector x_disp = TT_transf.TransformAxis(G4ThreeVector(1,0,0));

          if(std::abs(x_disp.y()) < 0.5){ // Should be 0 or +-1 if everything aligned
            TT_global_pos.setX(1e9);
          }
          else{
            TT_global_pos.setY(1e9);
          }
          for(int side=0; side < 2; side++){
            int n_pmt  = TtID::pmt_from_geant(side, n_plane, n_pannel);
            unsigned int chid = TtID::getIntID(n_wall, n_pmt, n_strip);
            ChannelPositionTXT << chid << " " << TT_global_pos.x() << " " << TT_global_pos.y() << " " << TT_global_pos.z() << std::endl;
          }
        }
      }
    }
  }
  ChannelPositionTXT.close();
#endif

  if (not m_flag_ntuple) {
    return;
  }
  // check the RootWriter is Valid.
  SniperPtr<RootWriter> svc(getParent(), "RootWriter");
  if (svc.invalid()) {
    LogError << "Can't Locate RootWriter. If you want to use it, please "
	     << "enable it in your job option file."
	     << std::endl;
    return;
  }
  
  gROOT->ProcessLine("#include <vector>");
  
  m_evt_treeTT = svc->bookTree(*m_par, "SIMEVT/TT", "Deposit Energy TT");
  m_evt_treeTT->Branch("evtID", &m_eventID, "evtID/I");
  m_evt_treeTT->Branch("NDeposits",&m_NDeposits,"NDeposits/I");
  m_evt_treeTT->Branch("dep_trackID",&m_dep_trackID);
  m_evt_treeTT->Branch("dep_pdg",&m_dep_pdg);
  m_evt_treeTT->Branch("dep_E",&m_dep_E);
  m_evt_treeTT->Branch("dep_E0",&m_dep_E0);
  m_evt_treeTT->Branch("dep_nbar",&m_dep_nbar);
  m_evt_treeTT->Branch("dep_npanel",&m_dep_npanel);
  m_evt_treeTT->Branch("dep_nplane",&m_dep_nplane);
  m_evt_treeTT->Branch("dep_nwall",&m_dep_nwall);
  m_evt_treeTT->Branch("dep_x",&m_dep_x);
  m_evt_treeTT->Branch("dep_y",&m_dep_y);
  m_evt_treeTT->Branch("dep_z",&m_dep_z);
  m_evt_treeTT->Branch("dep_t",&m_dep_t);
  m_evt_treeTT->Branch("dep_dL",&m_dep_dL);
  m_evt_treeTT->Branch("dep_dR",&m_dep_dR);
  m_evt_treeTT->Branch("dep_peL",&m_dep_peL);
  m_evt_treeTT->Branch("dep_peR",&m_dep_peR);
  m_evt_treeTT->Branch("dep_tL",&m_dep_tL);
  m_evt_treeTT->Branch("dep_tR",&m_dep_tR);
  m_evt_treeTT->Branch("isMuonDeposits",&m_isMuonDeposits);
    
}

void
DepositEnergyTTAnaMgr::EndOfRunAction(const G4Run* /*aRun*/) {
  
}

void
DepositEnergyTTAnaMgr::BeginOfEventAction(const G4Event* evt) {
  m_eventID = evt->GetEventID();
  
  m_NDeposits=0;
  m_dep_trackID.clear();
  m_dep_pdg.clear();
  m_dep_E.clear();
  m_dep_E0.clear();
  m_dep_nbar.clear();
  m_dep_npanel.clear();
  m_dep_nplane.clear();
  m_dep_nwall.clear();
  m_dep_x.clear();
  m_dep_y.clear();
  m_dep_z.clear();
  m_dep_t.clear();
  m_dep_dL.clear();
  m_dep_dR.clear();
  m_dep_peL.clear();
  m_dep_peR.clear();
  m_dep_tL.clear();
  m_dep_tR.clear();
  m_isMuonDeposits.clear();

}

void
DepositEnergyTTAnaMgr::EndOfEventAction(const G4Event* evt) {
  
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4int CollID = SDman->GetCollectionID("userhitCollectionTT");

  junoHit_TT_Collection * col_tt =0;
  G4HCofThisEvent * HCE = evt->GetHCofThisEvent();

  if (col_tt) {
    LogError << "No TT hits collection found." << std::endl;
  } else {
    col_tt = (junoHit_TT_Collection*)(HCE->GetHC(CollID));
  }

  if (col_tt) {

    int n_hit = col_tt->entries();
    m_NDeposits = n_hit;

    for(int i=0; i<n_hit; i++) {

      m_dep_trackID.push_back((*col_tt)[i]->GetTrackID());
      m_dep_pdg.push_back((*col_tt)[i]->GetPdgID());
      m_dep_E.push_back((*col_tt)[i]->GetDepE());
      m_dep_E0.push_back((*col_tt)[i]->GetVertexKE());
      m_dep_nbar.push_back((*col_tt)[i]->GetNbar());
      m_dep_npanel.push_back((*col_tt)[i]->GetNpanel());
      m_dep_nplane.push_back((*col_tt)[i]->GetNplane());
      m_dep_nwall.push_back((*col_tt)[i]->GetNwall());
      m_dep_x.push_back((*col_tt)[i]->GetDepX());
      m_dep_y.push_back((*col_tt)[i]->GetDepY());
      m_dep_z.push_back((*col_tt)[i]->GetDepZ());
      m_dep_t.push_back((*col_tt)[i]->GetDepT());
      m_dep_dL.push_back((*col_tt)[i]->GetDepDL());
      m_dep_dR.push_back((*col_tt)[i]->GetDepDR());
      m_dep_peL.push_back((*col_tt)[i]->GetDepPEL());
      m_dep_peR.push_back((*col_tt)[i]->GetDepPER());
      m_dep_tL.push_back((*col_tt)[i]->GetDepTL());
      m_dep_tR.push_back((*col_tt)[i]->GetDepTR());
      m_isMuonDeposits.push_back((*col_tt)[i]->GetIsMuonDeposit());

    }
  }


  if (m_flag_ntuple and m_evt_treeTT) {
    m_evt_treeTT -> Fill();
  }

std::cout<<"end of event action "<<std::endl;

}

void
DepositEnergyTTAnaMgr::PreUserTrackingAction(const G4Track* /*aTrack*/) {
  
}

void
DepositEnergyTTAnaMgr::PostUserTrackingAction(const G4Track* /*aTrack*/) {
  
}

void
DepositEnergyTTAnaMgr::UserSteppingAction(const G4Step* /*step*/) {

}

/* vim: set noexpandtab shiftwidth=2 tabstop=8 softtabstop=2: */
