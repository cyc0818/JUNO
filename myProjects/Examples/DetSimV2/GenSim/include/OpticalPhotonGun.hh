
#ifndef OpticalPhotonGun_hh
#define OpticalPhotonGun_hh

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
class OpticalPhotonGun{
  public:

    OpticalPhotonGun(std::string name,G4int number,G4double wave);
    OpticalPhotonContainer next();
  private:
    OpticalPhotonInfo GetOpticalInfo();
    G4String particleName; 
    std::string  m_name;
    G4int m_number;
    G4double m_wave;
};
}
}
#endif 
