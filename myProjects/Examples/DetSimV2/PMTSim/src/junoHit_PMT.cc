//------------------------------------------------------------------
//                       junoHit_PMT
//                       
// The data members of a hit on sensitive detector is defined.
// Their values are obtained in junoSD_PMT using the information of
// steps which hit the sensitive detector.
// -----------------------------------------------------------------
//  Author: Liang Zhan, 2006/01/27
//  Modified by: Weili Zhong, 2006/03/01
// -----------------------------------------------------------------

#include "junoHit_PMT.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4UnitsTable.hh"
using namespace CLHEP;

////////////////////////////////////////////////////////////////////

G4ThreadLocal G4Allocator<junoHit_PMT>* junoHit_PMT_Allocator = 0;

junoHit_PMT::junoHit_PMT()
{ this->Init();}

junoHit_PMT::~junoHit_PMT()
{;}

junoHit_PMT::junoHit_PMT(const junoHit_PMT &right):G4VHit()
{
  this->Init();
  pmtID = right.pmtID;
  edep = right.edep;
  pos = right.pos;
}

junoHit_PMT::junoHit_PMT(G4int i,G4double t)
{
  this->Init();
  pmtID = i;
  time = t;
}

junoHit_PMT::junoHit_PMT(G4int z)
{
  this->Init();
  pmtID = z;
}

const junoHit_PMT& junoHit_PMT::operator=(const junoHit_PMT &right)
{
  this->Init();
  pmtID = right.pmtID;
  edep = right.edep;
  pos = right.pos;
  return *this;
}

void junoHit_PMT::Init()
{
    edep = 0;
    energy = 0;
    pmtID = 0;
    time = 0;
    iHitCount = 0;
    weight = 0;
    wavelength = 0;
    producerID = -1;
    isFromCerenkov = false;
    isReemission = false;

    isOriginalOP = false;
    OriginalOPStartT = 0;
}

void junoHit_PMT::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  
  //Dump("junoHit_PMT::Draw");  
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void junoHit_PMT::Print(){}

void junoHit_PMT::Dump(const char* prefix)
{
    G4cout << prefix << " energy:" << G4BestUnit( energy , "Energy" ) << " wavelength:" << G4BestUnit( wavelength , "Length")  << G4endl ; 
    
}

#ifdef GNU_GCC
  template class G4Allocator<junoHit_PMT>;
#endif
