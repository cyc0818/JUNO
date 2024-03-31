// -------------------------------------------------------------------
//
// GEANT4 Class header file
//
//
// File name:     G4PhotonConversion
//
// Paolo Crivelli (ETHZ)
//
// Implemented in SNiPER by Akira Takenaka (SJTU)
// -------------------------------------------------------------------
//

#ifndef G4PhotonConversion_h
#define G4PhotonConversion_h 1

#include "G4PhotonSource.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"

class G4PhotonConversion : public G4VProcess

{

public:

  G4PhotonConversion(const G4String& name = "PhotonConversion");

  virtual ~G4PhotonConversion();

  virtual G4bool IsApplicable(const G4ParticleDefinition& p);

  virtual G4VParticleChange* AtRestDoIt(
                             const G4Track& track,
                             const G4Step& stepData);

  // Print out of the class parameters
  virtual void PrintInfo();
  
  virtual G4double PostStepGetPhysicalInteractionLength(const G4Track& aTrack,
          G4double ,
          G4ForceCondition* ) {return 0;};
  virtual G4VParticleChange* PostStepDoIt(const G4Track& aTrack,
                      const G4Step&  aStep) {return 0;};
  virtual G4double AlongStepGetPhysicalInteractionLength(
          const G4Track&,
          G4double  ,
          G4double  ,
          G4double& ,
          G4GPILSelection*
          ) { return -1.0; };

  virtual G4double AtRestGetPhysicalInteractionLength(
          const G4Track& ,
          G4ForceCondition*
          ) { return -1.0; };
  virtual G4VParticleChange* AlongStepDoIt(
          const G4Track& ,
          const G4Step&
          ) {return 0;};

  void SetnProducedPhotons(const int nphotons);
  G4int GetnProducedPhotons() const;
  void SetDfBall(const bool flag);
  bool GetDfBall() const;
  void SetOpEnergy(const double energy);
  double GetOpEnergy() const;
  void SetDfBallPosX(const double posx);
  double GetDfBallPosX() const;
  void SetDfBallPosY(const double posy);
  double GetDfBallPosY() const;
  void SetDfBallPosZ(const double posz);
  double GetDfBallPosZ() const;


protected:

  virtual void InitialiseProcess();

private:

 // hide assignment operator
  G4PhotonConversion & operator=(const G4PhotonConversion &right);
  G4PhotonConversion(const G4PhotonConversion&);
  
  G4bool isInitialised;
  bool m_photonconversion_flag;
  int m_nProducedPhotons;
  bool m_dfball_flag;
  double m_fixed_energy;
  double m_dfball_x;
  double m_dfball_y;
  double m_dfball_z;
  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline G4bool G4PhotonConversion::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4PhotonSource::PhotonSource());
}

inline
void G4PhotonConversion::SetnProducedPhotons(const G4int nphotons)
{
        m_nProducedPhotons = nphotons;
}

inline
void G4PhotonConversion::SetDfBall(const bool flag)
{
        m_dfball_flag = flag;
}

inline
void G4PhotonConversion::SetOpEnergy(const double energy)
{
        m_fixed_energy = energy;
}

inline
void G4PhotonConversion::SetDfBallPosX(const double posx)
{
        m_dfball_x = posx;
}

inline
void G4PhotonConversion::SetDfBallPosY(const double posy)
{
        m_dfball_y = posy;
}

inline
void G4PhotonConversion::SetDfBallPosZ(const double posz)
{
        m_dfball_z = posz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

