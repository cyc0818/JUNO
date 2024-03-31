//----------------------------------------------------------------------------
//                                 junoHit_TT
//
// The data members of a hit on TT sensitive detector is defined.
//----------------------------------------------------------------------------
// Author: Karaparambil Rajan Rebin, J. P. A. M. de Andre (IPHC)
//----------------------------------------------------------------------------

#include "junoHit_TT.hh"

#include "G4UnitsTable.hh"
using namespace CLHEP;

//----------------------------------------------------------------------------

G4ThreadLocal G4Allocator<junoHit_TT>* junoHit_TT_Allocator = 0;

junoHit_TT::junoHit_TT()
{ this->Init(); }

junoHit_TT::~junoHit_TT()
{;}

junoHit_TT::junoHit_TT(const junoHit_TT &right):G4VHit()
{
  this->Init();
  channelID = right.channelID;
  edep = right.edep;
  time = right.time;
}

junoHit_TT::junoHit_TT(G4int i, G4double t)
{
  this->Init();
  channelID = i;
  time = t;
}

junoHit_TT::junoHit_TT(G4int z)
{
  this->Init();
  channelID = z;
}

const junoHit_TT& junoHit_TT::operator=(const junoHit_TT &right)
{
  this->Init();
  channelID = right.channelID;
  edep = right.edep;
  time = right.time;
  return *this;
}

void junoHit_TT::Init()
{
  channelID = 0;
  edep = 0;
  time = 0;
  nDeposits =0;
}

void junoHit_TT::Draw(){}

void junoHit_TT::Print(){}

void junoHit_TT::Dump(const char* prefix){}

#ifdef GNU_GCC
template class G4Allocator<junoHit_TT>;
#endif
