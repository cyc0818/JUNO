
#ifndef __dyb2NylonFilmOpticalModel_hh__
#define __dyb2NylonFilmOpticalModel_hh__

#include "G4VFastSimulationModel.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4VPhysicalVolume.hh"
#include "G4UImessenger.hh"


class dyb2NylonFilmOpticalModel : public G4VFastSimulationModel
{
  public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  //  dyb2NylonFilmOpticalModel (G4String, G4VPhysicalVolume*, G4Envelope*);
  dyb2NylonFilmOpticalModel (G4String,  G4Envelope*);
  // Note: There is no dyb2NylonFilmOpticalModel(G4String) constructor.
  ~dyb2NylonFilmOpticalModel ();

  //------------------------------
  // Virtual methods of the base
  // class to be coded by the user
  //------------------------------

  G4bool IsApplicable(const G4ParticleDefinition&);
  G4bool ModelTrigger(const G4FastTrack &);
  void DoIt(const G4FastTrack&, G4FastStep&);

  /*
  // following two methods are for G4UImessenger
  void SetNewValue(G4UIcommand * command,G4String newValues);
  G4String GetCurrentValue(G4UIcommand * command);
  */

  inline void SetTransparency(G4double trans) {m_transparency = trans;}
  
private:

  // verbose level -- how verbose to be (diagnostics and such)
  G4int _verbosity;

  G4double m_transparency;

  G4ThreeVector CalculateNewDirection( G4ThreeVector );
  G4double CalculateGaussianVariance( G4double, G4double, G4double, G4double);

};

#endif
