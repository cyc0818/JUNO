//-----------------------------------------------------------------------------
//                              junoSD_TT
//
// Top Tracker is defined as sensitive detector. They collect hits on them.
// The OPERA parametrizaion is applied to the hits from the scintillator.
// Two hit collections used because of different size in information stored
// "hitCollectionTT" stores the DetSim level inforamtion, and
// "userhitCollectionTT" stores the user level information
//
//
//-----------------------------------------------------------------------------
// Author: Karaparambil Rajan Rebin, J. P. A. M. de Andre (IPHC)
// OPERA parametriation based on the implementation by A.Meregaglia, C.Jollet.
//-----------------------------------------------------------------------------

#include "junoSD_TT.hh"
#include "junoHit_TT.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4SDManager.hh"
#include <cassert>

#include "G4Box.hh"
#include "Randomize.hh"
#include "G4OpticalPhoton.hh"
#include "Identifier/TtID.h"

using namespace CLHEP;

//-----------------------------------------------------------------------------

junoSD_TT::junoSD_TT(const std::string& name):G4VSensitiveDetector(name)
                                             ,hitCollectionTT(0), userhitCollectionTT(0)
{
  G4String HCname, HCnameUser;
  collectionName.insert(HCname="hitCollectionTT");
  collectionName.insert(HCnameUser="userhitCollectionTT");
  InitParams();
}

junoSD_TT::~junoSD_TT()
{;}

void junoSD_TT::Initialize(G4HCofThisEvent *HCE)
{
  //TT DetSim hit collection "hitCollectionTT" 
  hitCollectionTT = new junoHit_TT_Collection(SensitiveDetectorName, collectionName[0]);

  int HCID = -1;
  if(HCID<0) HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollectionTT);
  HCE->AddHitsCollection(HCID, hitCollectionTT);

  //TT user level hit collection "userhitCollectionTT"
  userhitCollectionTT = new junoHit_TT_Collection(SensitiveDetectorName, collectionName[1]);

  int HCIDuser = -1;
  if(HCIDuser<0) HCIDuser = G4SDManager::GetSDMpointer()->GetCollectionID(userhitCollectionTT);
  HCE->AddHitsCollection(HCIDuser, userhitCollectionTT);
  

}

G4bool junoSD_TT::ProcessHits(G4Step * step, G4TouchableHistory*)
{

  G4Track* track = step->GetTrack();
  G4double edep = step->GetTotalEnergyDeposit();
  G4ThreeVector pos = (step->GetPreStepPoint()->GetPosition() + step->GetPostStepPoint()->GetPosition())/2;

  if(edep > 0.001 and track->GetDefinition() != G4OpticalPhoton::Definition() and track->GetMaterial()->GetName() == "Scintillator")
  {

    G4VPhysicalVolume* mother1 = track->GetTouchable()->GetVolume(1);
    G4String mother1name="";
    if(mother1)
      mother1name = mother1->GetName();

    G4VPhysicalVolume* mother3 = track->GetTouchable()->GetVolume(3);
    G4String mother3name="";
    if(mother3)
      mother3name = mother3->GetName();

    G4VPhysicalVolume* mother4 = track->GetTouchable()->GetVolume(4);
    G4String mother4name="";
    if(mother4)
      mother4name = mother4->GetName();

    G4VPhysicalVolume* mother5 = track->GetTouchable()->GetVolume(5);
    G4String mother5name="";
    if(mother5)
      mother5name = mother5->GetName();

    int barnum;
    sscanf(mother1name.c_str(),"pCoating_%d",&barnum);

    int panelnum;
    sscanf(mother3name.c_str(),"pPanel_%d",&panelnum);

    int planenum;
    sscanf(mother4name.c_str(),"pPlane_%d",&planenum);

    int wallnum;
    sscanf(mother5name.c_str(),"pWall_%d",&wallnum);


    m_dep_trackID.push_back(track->GetTrackID());
    m_dep_pdg.push_back(track->GetDefinition()->GetPDGEncoding());
    m_dep_E.push_back(edep);
    m_dep_E0.push_back(track->GetVertexKineticEnergy ());
    m_dep_nbar.push_back(barnum);
    m_dep_npanel.push_back(panelnum);
    m_dep_nplane.push_back(planenum);
    m_dep_nwall.push_back(wallnum);
    m_dep_x.push_back(pos.x());
    m_dep_y.push_back(pos.y());
    m_dep_z.push_back(pos.z());
    m_dep_t.push_back((step->GetPreStepPoint()->GetGlobalTime()+step->GetPostStepPoint()->GetGlobalTime())/2);
    int isMuon(0);
    if(m_dep_pdg[m_NDeposits]==13 || m_dep_pdg[m_NDeposits]==-13)
      isMuon = 1;
    m_isMuonDeposits.push_back(isMuon);



    // add the extra length out from the bar to the PMT, they are different depending on the bar num
    // the fitting result is len=-2.45263806631*barnum+ 91.6040322685 (cm) only one side
    int barnum32 = barnum < 32 ? barnum:63-barnum;
    double a_fiber_len = -2.45263806631;
    double b_fiber_len = 91.6040322685;
    double fiber_len =  a_fiber_len*barnum32+ b_fiber_len; // cm

    // Transform coordinate system from JUNO global, to the one of the scintillator strip.
    // With this conversion, any rotation they might have suffered is taken into account
    // and we don't need to treat separately the x-aligned and y-aligned strips.
    G4ThreeVector local_pos = step->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().TransformPoint(pos);
    double length_scintillator = ((G4Box*) track->GetTouchable()->GetVolume(0)->GetLogicalVolume()->GetSolid())->GetXHalfLength();

    double distanceEL = ( length_scintillator + local_pos.x() )/cm + fiber_len;
    double distanceER = ( length_scintillator - local_pos.x() )/cm + fiber_len;

    double a0(0);
    double a1(0);
    double b0(0);
    double b1(0);

    while(a0<=0 || a1<=0 || b0<=0 || b1<=0)
      {
        if(barnum<32)
          {
            a0=G4RandGauss::shoot(coeff[0][int(barnum/2)],0.2*coeff[0][int(barnum/2)]);
            b0=G4RandGauss::shoot(coeff[1][int(barnum/2)],0.1*coeff[1][int(barnum/2)]);
            a1=G4RandGauss::shoot(coeff[2][int(barnum/2)],0.2*coeff[2][int(barnum/2)]);
            b1=G4RandGauss::shoot(coeff[3][int(barnum/2)],0.1*coeff[3][int(barnum/2)]);
          }
        else
          {
            a0=G4RandGauss::shoot(coeff[0][int((63 - barnum)/2)],0.2*coeff[0][int((63 - barnum)/2)]);
            b0=G4RandGauss::shoot(coeff[1][int((63 - barnum)/2)],0.1*coeff[1][int((63 - barnum)/2)]);
            a1=G4RandGauss::shoot(coeff[2][int((63 - barnum)/2)],0.2*coeff[2][int((63 - barnum)/2)]);
            b1=G4RandGauss::shoot(coeff[3][int((63 - barnum)/2)],0.1*coeff[3][int((63 - barnum)/2)]);
          }
      }

    m_dep_dL.push_back(distanceEL);
    m_dep_dR.push_back(distanceER);
    m_dep_peL.push_back((edep/2.15) * (a0 * exp (-b0*distanceEL) + a1 * exp (-b1*distanceEL)));
    m_dep_peR.push_back((edep/2.15) * (a0 * exp (-b0*distanceER) + a1 * exp (-b1*distanceER)));

    //      std::cout<<"Nb p.e Left: "<<(edep/2.15) * (a0 * exp (-b0*distanceEL) + a1 * exp (-b1*distanceEL))<<" pe right: "<<(edep/2.15) * (a0 * exp (-b0*distanceER) + a1 * exp (-b1*distanceER))<<std::endl;

    // n of WLS fiber: 1.4 - 1.6 [http://www.phenix.bnl.gov/WWW/publish/donlynch/RXNP/Safety%20Review%206_22_06/Kuraray-PSF-Y11.pdf]
    // n of polystyrene: 1.55-1.59 [google]
    // => use n=1.6 to be conservative
    const double c_scintilator=c_light/1.6;
    const double time_spread_slope = -0.2265;
    // The random component below comes from measurements done where muons were traversing a
    // horizontal scintillator strip vertically, at the center of the strip. The measured
    // time difference between the time on each end followed an exponential with slope -0.2265
    // (time_spread_slope). Given that no other measures were made, the positional dependence
    // is assumed to be linear for now.
    m_dep_tL.push_back((distanceEL*cm/c_scintilator)/ns + G4RandExponential::shoot(-1./time_spread_slope) * distanceEL/((distanceEL+distanceER)/2));
    m_dep_tR.push_back((distanceER*cm/c_scintilator)/ns + G4RandExponential::shoot(-1./time_spread_slope) * distanceER/((distanceEL+distanceER)/2));

    m_NDeposits++;

  }

  return true;
}

void junoSD_TT::EndOfEvent(G4HCofThisEvent*)
{

  
  bool TTHits = SaveHits();

  G4cout<<"hitCollectionTT.size: "<< hitCollectionTT->entries() <<"\t"<<"userhitCollectionTT.size: "<< userhitCollectionTT->entries() <<G4endl;
  clear();
}

bool junoSD_TT::SaveHits()
{

  unsigned int DMChannel;
  int Nnwall(0);
  int Nnplane(0);
  int Nnpanel(0);
  int Nnbar(0);

  G4int idx =0;
  G4int NDeposits = m_NDeposits;
  int count =0;
  for(int i=0; i<m_NDeposits; i++)
    {
      Nnwall = m_dep_nwall[i];
      Nnplane = m_dep_nplane[i];
      Nnpanel = m_dep_npanel[i];
      Nnbar = m_dep_nbar[i];
      
      //For userfile "collection: userhitCollectionTT"
      junoHit_TT* userhit = new junoHit_TT();
      userhit->SetTrackID(m_dep_trackID[i]);
      userhit->SetPdgID(m_dep_pdg[i]);
      userhit->SetDepE(m_dep_E[i]);
      userhit->SetVertexKE(m_dep_E0[i]);
      userhit->SetNbar(Nnbar);
      userhit->SetNpanel(Nnpanel);
      userhit->SetNplane(Nnplane);
      userhit->SetNwall(Nnwall);
      userhit->SetDepX(m_dep_x[i]);
      userhit->SetDepY(m_dep_y[i]);
      userhit->SetDepZ(m_dep_z[i]);
      userhit->SetDepT(m_dep_t[i]);
      userhit->SetDepDL(m_dep_dL[i]);
      userhit->SetDepDR(m_dep_dR[i]);
      userhit->SetDepPEL(m_dep_peL[i]);
      userhit->SetDepPER(m_dep_peR[i]);
      userhit->SetDepTL(m_dep_tL[i]);
      userhit->SetDepTR(m_dep_tR[i]);
      userhit->SetIsMuonDeposit(m_isMuonDeposits[i]);

      G4int idy = userhitCollectionTT->insert(userhit);

      for(int hh=0; hh<2; hh++)
        {
          count++;
          //DetSim hits "collection: hitCollectionTT"
          junoHit_TT* hit = new junoHit_TT();
      
          int pmt = TtID::pmt_from_geant(hh, Nnplane, Nnpanel);
          DMChannel = TtID::getIntID(Nnwall, pmt, Nnbar);

          hit->SetChannelID(DMChannel);
          
          if(hh==0)
            {
              hit->SetEdep(m_dep_peL[i]);
              hit->SetTime(m_dep_t[i] + m_dep_tL[i]);
              idx = hitCollectionTT->insert(hit);
            }
          else
            {
              hit->SetEdep(m_dep_peR[i]);
              hit->SetTime(m_dep_t[i] + m_dep_tR[i]);
              idx = hitCollectionTT->insert(hit);
            }

        }
    }

  return true;
}

void junoSD_TT::InitParams()
{
  m_NDeposits=0;

  coeff[0][0]=0.80;
  coeff[1][0]=0.013;
  coeff[2][0]=8.9;
  coeff[3][0]=1.92E-3;

  coeff[0][1]=2.24;
  coeff[1][1]=9.52E-3;
  coeff[2][1]=8.38;
  coeff[3][1]=1.73E-3;

  coeff[0][2]=3.76;
  coeff[1][2]=9.515E-3;
  coeff[2][2]=7.61;
  coeff[3][2]=1.57E-3;

  coeff[0][3]=5.65;
  coeff[1][3]=5.12E-3;
  coeff[2][3]=5.08;
  coeff[3][3]=1.015E-3;

  coeff[0][4]=4.08;
  coeff[1][4]=8.48E-3;
  coeff[2][4]=7.39;
  coeff[3][4]=1.49E-3;

  coeff[0][5]=3.73;
  coeff[1][5]=7.04E-3;
  coeff[2][5]=7.56;
  coeff[3][5]=1.54E-3;

  coeff[0][6]=3.2;
  coeff[1][6]=7.37E-3;
  coeff[2][6]=8.09;
  coeff[3][6]=1.63E-3;

  coeff[0][7]=4.73;
  coeff[1][7]=6.30E-3;
  coeff[2][7]=6.63;
  coeff[3][7]=1.31E-3;

  coeff[0][8]=4.7;
  coeff[1][8]=6.23E-3;
  coeff[2][8]=6.76;
  coeff[3][8]=1.34E-3;

  coeff[0][9]=4.19;
  coeff[1][9]=7.27E-3;
  coeff[2][9]=7.67;
  coeff[3][9]=1.53E-3;

  coeff[0][10]=4.48;
  coeff[1][10]=8.34E-3;
  coeff[2][10]=7.8;
  coeff[3][10]=1.52E-3;

  coeff[0][11]=5.6;
  coeff[1][11]=7.39E-3;
  coeff[2][11]=6.85;
  coeff[3][11]=1.317E-3;

  coeff[0][12]=4.54;
  coeff[1][12]=7.4E-3;
  coeff[2][12]=7.7;
  coeff[3][12]=1.52E-3;

  coeff[0][13]=4.83;
  coeff[1][13]=0.012;
  coeff[2][13]=8.31;
  coeff[3][13]=1.64E-3;

  coeff[0][14]=4.44;
  coeff[1][14]=9.8E-3;
  coeff[2][14]=8.57;
  coeff[3][14]=1.67E-3;

  coeff[0][15]=4.9;
  coeff[1][15]=0.0107;
  coeff[2][15]=8.3;
  coeff[3][15]=1.63E-3;
}

void junoSD_TT::clear()
{
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

void junoSD_TT::DrawAll(){}

void junoSD_TT::PrintAll(){}
