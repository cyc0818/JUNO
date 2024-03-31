// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     G4PhotonConversion
//
// Implemented in SNiPER by Akira Takenaka (SJTU)
//
//

//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4PhotonConversion.hh"
#include "G4PhotonSource.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4EventManager.hh"
#include "G4DecayTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4PhotonConversion::G4PhotonConversion(const G4String& name)
  : G4VProcess(name), isInitialised(false)
{
 enableAtRestDoIt=true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4PhotonConversion::~G4PhotonConversion()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4PhotonConversion::InitialiseProcess()
{
    G4cout << "Here is Photon Conversion! Initialized." << G4endl;
  if(!isInitialised) {
    isInitialised = true;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4PhotonConversion::PrintInfo()
{
  G4cout << "      Formation of positronium with annihilation probability included"
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4PhotonConversion::AtRestDoIt(const G4Track& aTrack,
                                                     const G4Step& aStep)
//
{
  aParticleChange.SetNumberOfSecondaries(m_nProducedPhotons);
  G4String volname = aTrack.GetStep()->GetPreStepPoint()->GetPhysicalVolume()->GetName();

  const double dfsigma = 0.23;
  double sampledEnergy = m_fixed_energy;
  const double timeoffset = 100.;
  int nphoton = m_nProducedPhotons;
  const double dfradius = 15.*mm;

  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();

  G4double      t0 = pPreStepPoint->GetGlobalTime();
  double init_time = t0;

  G4String matname = aTrack.GetStep()->GetPreStepPoint()->GetMaterial()->GetName();

  // momentum
  G4double cost, sint;
  G4double phi, sinp, cosp;
  G4double px, py, pz;

  G4ThreeVector photonMomentum;
  G4double sx = cost*cosp;
  G4double sy = cost*sinp; 
  G4double sz = -sint;
  
  G4ThreeVector perp;

  const double ct1 = -1.;
  const double ct2 = +1.;

  double polx, poly, polz, p_mom;

  G4ThreeVector photonPolarization;
  double radius = dfradius;
  int lessradius = 0;
  int ntotal = 0;

  for (int i = 0; i < nphoton; i++) {

      ntotal++;
      cost = ct1 + (ct2-ct1)*G4UniformRand();
      sint = sqrt((1.-cost)*(1.+cost));

      phi = twopi*G4UniformRand();
      sinp = sin(phi);
      cosp = cos(phi);

      px = sint*cosp;
      py = sint*sinp;
      pz = cost;

      photonMomentum.setX(px);
      photonMomentum.setY(py);
      photonMomentum.setZ(pz);

      sx = cost*cosp;
      sy = cost*sinp;
      sz = -sint;

      photonPolarization.setX(sx);
      photonPolarization.setY(sy);
      photonPolarization.setZ(sz);

      perp = photonMomentum.cross(photonPolarization);

      phi = twopi*G4UniformRand();
      sinp = sin(phi);
      cosp = cos(phi);

      photonPolarization = cosp * photonPolarization + sinp * perp;

      photonPolarization = photonPolarization.unit();

      init_time = t0 + timeoffset + G4RandGauss::shoot(0., dfsigma)*ns;
      if (init_time<0.) init_time = 0.;

      px *= sampledEnergy;
      py *= sampledEnergy;
      pz *= sampledEnergy;
      polx = photonPolarization.x();
      poly = photonPolarization.y();
      polz = photonPolarization.z();
      p_mom = std::sqrt(px*px+py*py+pz*pz);

      G4DynamicParticle* aScintillationPhoton =
          new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(),
                  photonMomentum);
      aScintillationPhoton->SetPolarization
          (photonPolarization.x(),
           photonPolarization.y(),
           photonPolarization.z());

      aScintillationPhoton->SetKineticEnergy(sampledEnergy);

      G4ThreeVector aSecondaryPosition;
      if (!m_dfball_flag) {
        aSecondaryPosition= pPostStepPoint->GetPosition();
      } else {
        aSecondaryPosition.setX(m_dfball_x + dfradius*px/p_mom);
        aSecondaryPosition.setY(m_dfball_y + dfradius*py/p_mom);
        aSecondaryPosition.setZ(m_dfball_z + dfradius*pz/p_mom);
        radius = std::sqrt((m_dfball_x + dfradius*px/p_mom)*(m_dfball_x + dfradius*px/p_mom)+(m_dfball_y + dfradius*py/p_mom)*(m_dfball_y + dfradius*py/p_mom)+(m_dfball_z + dfradius*pz/p_mom)*(m_dfball_z + dfradius*pz/p_mom));
        if (radius<dfradius) {
            lessradius++;
        }
      }
      G4double aSecondaryTime = init_time;

      G4Track* aSecondaryTrack =
          new G4Track(aScintillationPhoton,aSecondaryTime,aSecondaryPosition);


      aSecondaryTrack->SetTouchableHandle(aStep.GetPreStepPoint()->GetTouchableHandle());
      aSecondaryTrack->SetParentID(aTrack.GetTrackID());
      aParticleChange.AddSecondary(aSecondaryTrack);
  }


  // Kill the incident positron

  aParticleChange.ProposeTrackStatus(fStopAndKill);
  return &aParticleChange;
    
}


