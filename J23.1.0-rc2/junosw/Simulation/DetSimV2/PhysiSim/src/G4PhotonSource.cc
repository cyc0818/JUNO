// ----------------------------------------------------------------------
//      GEANT 4 class implementation file
//
// Implemented in SNiPER by Akira Takenaka (SJTU)
// ----------------------------------------------------------------------

#include "G4PhotonSource.hh"
#include "G4ParticleTable.hh"

#include "G4DecayTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhaseSpaceDecayChannel.hh"

// ######################################################################
// ###                         PhotonSource                           ###
// ######################################################################
G4PhotonSource* G4PhotonSource::theInstance = 0;

G4PhotonSource* G4PhotonSource::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "PhotonSource";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
  {
  // create particle
  //
  //    Arguments for constructor are as follows
  //               name             mass          width         charge
  //             2*spin           parity  C-conjugation
  //          2*Isospin       2*Isospin3       G-parity
  //               type    lepton number  baryon number   PDG encoding
  //             stable         lifetime    decay table
  //             shortlived      subType    anti_encoding

  anInstance = new G4ParticleDefinition(
                 name,  0.0*MeV,       0.0*MeV,    0, 
		    0,               0,             0,          
		    0,               0,             0,             
	     "boson",              0,             0,          100,
		 false,            0*ns,          NULL,
		 false,             "PhotonSource"
              );
  }
  theInstance = reinterpret_cast<G4PhotonSource*>(anInstance);
  return theInstance;
}

G4PhotonSource*  G4PhotonSource::PhotonSourceDefinition()
{
  return Definition();
}

G4PhotonSource*  G4PhotonSource::PhotonSource()
{
  return Definition();
}


