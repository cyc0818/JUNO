#ifndef OpticalPhotonGunRan_hh
#define OpticalPhotonGunRan_hh

#include "G4Material.hh" 
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "OpticalDecription.hh"
#include <vector>
#include <string>
#include <globals.hh>
#include <cassert>
namespace Generator {                                         
     namespace Utils {
class OpticalPhotonGunRan{
  public:

    OpticalPhotonGunRan(std::string name,G4int number);
    void BuildEngTable();
    OpticalPhotonContainer next();
  private:
    OpticalPhotonInfo GetOpticalInfoRan();
    G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector;
    G4String particleName; 
    std::string  m_name;
    G4int m_number;
    G4double fastTimeConstant;
    G4double slowTimeConstant;
    G4double YieldRatio;
};
}
}
#endif 
