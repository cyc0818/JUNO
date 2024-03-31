//------------------------------------------------------------------
//                       junoHit_PMT_muon
//                       
// The data members of a hit on sensitive detector is defined.
// Their values are obtained in junoSD_PMT using the information of
// steps which hit the sensitive detector.
// -----------------------------------------------------------------
//  Author: Liang Zhan, 2006/01/27
//  Modified by: Weili Zhong, 2006/03/01
// -----------------------------------------------------------------

#include "junoHit_PMT_muon.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "G4UnitsTable.hh"
using namespace CLHEP;

////////////////////////////////////////////////////////////////////

G4ThreadLocal G4Allocator<junoHit_PMT_muon>* junoHit_PMT_muon_Allocator = 0;

junoHit_PMT_muon::junoHit_PMT_muon()
{ this->Init();}

junoHit_PMT_muon::~junoHit_PMT_muon()
{;}

junoHit_PMT_muon::junoHit_PMT_muon(const junoHit_PMT_muon &/*right*/):G4VHit()
{
  this->Init();
}

junoHit_PMT_muon::junoHit_PMT_muon(G4int i,G4double t)
{
  this->Init();
  pmtID = i;
  time = t;
}

junoHit_PMT_muon::junoHit_PMT_muon(G4int z)
{
  this->Init();
  pmtID = z;
}

const junoHit_PMT_muon& junoHit_PMT_muon::operator=(const junoHit_PMT_muon &/*right*/)
{
  this->Init();
  return *this;
}

void junoHit_PMT_muon::Init()
{
    pmtID = 0;
    time = 0;
    iHitCount = 0;
}

void junoHit_PMT_muon::Draw()
{
  //G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  
  //Dump("junoHit_PMT_muon::Draw");  
}

void junoHit_PMT_muon::Print(){}

void junoHit_PMT_muon::Dump(const char* /*prefix*/)
{
    
}

#ifdef GNU_GCC
  template class G4Allocator<junoHit_PMT_muon>;
#endif
