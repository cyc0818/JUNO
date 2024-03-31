

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4StateManager.hh"

#include "OpticalPhotonGunRan.hh"
#include "globals.hh"

#include <string>
#include <sstream>
#include <cassert>

namespace Generator {
  namespace Utils {
    OpticalPhotonGunRan::OpticalPhotonGunRan(std::string name,G4int number)
      :m_name(name),m_number(number)
    {
      aPhysicsOrderedFreeVector = NULL;
    }


    void 
      OpticalPhotonGunRan::BuildEngTable()
      {

        const G4MaterialTable* theMaterialTable = 
          G4Material::GetMaterialTable();
        G4int numOfMaterials = G4Material::GetNumberOfMaterials();

        G4Material* aMaterial = NULL;
        // create new physics table
        G4String materialname = "LS";
        G4String FastTimeConstant = "GammaFASTTIMECONSTANT";
        G4String SlowTimeConstant = "GammaSLOWTIMECONSTANT";
        G4String strYieldRatio = "GammaYIELDRATIO";
        fastTimeConstant = 0.0;
        slowTimeConstant = 0.0;
        YieldRatio = 0.0;
        for (G4int i=0 ; i < numOfMaterials; i++) 
        {
          if ( (*theMaterialTable)[i]->GetName() == materialname) 
          {
            aMaterial = (*theMaterialTable)[i];
          }
        }

        G4MaterialPropertiesTable* aMaterialPropertiesTable =
          aMaterial->GetMaterialPropertiesTable();
        G4MaterialPropertyVector* theFastLightVector = 
          aMaterialPropertiesTable->GetProperty("FASTCOMPONENT");

        const G4MaterialPropertyVector* ptable_fast =
          aMaterialPropertiesTable->GetProperty(FastTimeConstant.c_str());
        if (ptable_fast) 
        {
          fastTimeConstant = ptable_fast->GetProperty(0);
        }

        const G4MaterialPropertyVector* ptable_slow =
          aMaterialPropertiesTable->GetProperty(SlowTimeConstant.c_str());
        if (ptable_slow)
        {
          slowTimeConstant = ptable_slow->GetProperty(0);
        }

        const G4MaterialPropertyVector* ptable_yield =
          aMaterialPropertiesTable->GetProperty(strYieldRatio.c_str());
        if (ptable_yield) 
        {
          YieldRatio = ptable_yield->GetProperty(0);
        }

        aPhysicsOrderedFreeVector =
          new G4PhysicsOrderedFreeVector();
        theFastLightVector->ResetIterator();
        ++(*theFastLightVector);  

        G4double currentIN = theFastLightVector->
          GetProperty();
        G4cout <<"currentIN : "<<currentIN<<G4endl;
        if (currentIN >= 0.0) 
        {
          G4double currentPM = theFastLightVector->
            GetPhotonEnergy();

          G4double currentCII = 0.0;

          aPhysicsOrderedFreeVector->
            InsertValues(currentPM , currentCII);

          G4double prevPM  = currentPM;
          G4double prevCII = currentCII;
          G4double prevIN  = currentIN;

          while(++(*theFastLightVector)) 
          {
            currentPM = theFastLightVector->
              GetPhotonEnergy();

            currentIN=theFastLightVector->  
              GetProperty();

            currentCII = 0.5 * (prevIN + currentIN);

            currentCII = prevCII +
              (currentPM - prevPM) * currentCII;
            aPhysicsOrderedFreeVector->
              InsertValues(currentPM, currentCII);

            prevPM  = currentPM;
            prevCII = currentCII;
            prevIN  = currentIN;
          }

        }
      }

    OpticalPhotonInfo 
      OpticalPhotonGunRan::GetOpticalInfoRan() 
      {
        OpticalPhotonInfo opt_info_ran;

        if ( aPhysicsOrderedFreeVector == NULL) {
          BuildEngTable();
        }

        G4double sampleEnergy = 0.0;
          G4double CIIvalue = G4UniformRand()*aPhysicsOrderedFreeVector->GetMaxValue();
          sampleEnergy = aPhysicsOrderedFreeVector->GetEnergy(CIIvalue);
          G4double cost = 1. - 2.*G4UniformRand();
          G4double sint = sqrt((1.-cost)*(1.+cost));

          G4double phi = twopi*G4UniformRand();
          G4double sinp = sin(phi);
          G4double cosp = cos(phi);

          G4double px = sint*cosp;
          G4double py = sint*sinp;
          G4double pz = cost;


          G4ThreeVector photonMomentum(px,py, pz);

          G4double m_px = sampleEnergy*px;
          G4double m_py = sampleEnergy*py;
          G4double m_pz = sampleEnergy*pz; 

          G4double sx = cost*cosp;
          G4double sy = cost*sinp; 
          G4double sz = -sint;

          G4ThreeVector photonPolarization(sx, sy, sz);

          G4ThreeVector perp = photonMomentum.cross(photonPolarization);

          phi = twopi*G4UniformRand();
          sinp = sin(phi);
          cosp = cos(phi);

          photonPolarization = cosp * photonPolarization + sinp * perp;

          photonPolarization = photonPolarization.unit();

          G4double InitTime = 0.0;
          G4double ratio = G4UniformRand();

          if (ratio<=YieldRatio)
          {
            InitTime = (0-fastTimeConstant* log( G4UniformRand() ));
          }
          else
          {
            G4double ratioslow = G4UniformRand();
            if (ratioslow < 0.15)
            {
              InitTime = (0 - 190 * log( G4UniformRand() )); 
            }
            else
            {
              InitTime = (0-slowTimeConstant* log( G4UniformRand() ));
            }

          }

        opt_info_ran.name = m_name;
        opt_info_ran.px = m_px;
        opt_info_ran.py = m_py;
        opt_info_ran.pz = m_pz;
        opt_info_ran.eng = sampleEnergy;
        opt_info_ran.dt = InitTime;
        opt_info_ran.photonPol = photonPolarization;

        return opt_info_ran;
      }

    OpticalPhotonContainer
      OpticalPhotonGunRan::next() 
      {
        OpticalPhotonContainer new_opt_ran;
        for (G4int i = 0; i <m_number; ++i ) {
          // Parse Per line.
          new_opt_ran . push_back(GetOpticalInfoRan());

        }
        return new_opt_ran;

      }

  }
}
