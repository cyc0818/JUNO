// ------------------------------------------------------------
//      GEANT 4 class header file
//
// Paolo Crivelli (ETHZ)
// Implemented in SNiPER by C.Jollet and A.Meregaglia (CENBG)
// ----------------------------------------------------------------

#ifndef G4PhotonSource_h
#define G4PhotonSource_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"


// ######################################################################
// ###                         POSITRONIUM                            ###
// ######################################################################

class G4PhotonSource : public G4ParticleDefinition
{
 private:
   static G4PhotonSource* theInstance;
   G4PhotonSource(){}
   ~G4PhotonSource(){}

 public:
   static G4PhotonSource* Definition(G4double lifetime);
   static G4PhotonSource* PhotonSourceDefinition(G4double lifetime);
   static G4PhotonSource* PhotonSource(G4double lifetime);
  static G4PhotonSource* Definition();
  static G4PhotonSource* PhotonSourceDefinition();
  static G4PhotonSource* PhotonSource();
};


#endif




