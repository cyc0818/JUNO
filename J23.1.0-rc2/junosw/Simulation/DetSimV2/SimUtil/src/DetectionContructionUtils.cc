#include "DetectionContructionUtils.hh"
#include <cmath>
#include "G4PhysicalConstants.hh"
using namespace CLHEP;
namespace DYB2 {
namespace Ball {

G4int GetMaxiumNinCircle(G4double r_tube,
        G4double r_pmt,
        G4double gap)
{
    G4int N=0;
    G4double theta = 2 * atan(r_pmt / r_tube);
    G4double phi = 2* asin(gap / (2*sqrt(r_pmt*r_pmt + r_tube*r_tube)));
    N = int(2*pi / (theta + phi)); 
    return N;
}

G4int GetMaxiumNinHalfHeight(G4double h_tube,
        G4double r_pmt,
        G4double gap)
{
    return int(0.5 * (h_tube - 2*r_pmt -2*gap)/(2*r_pmt+gap));
}

}
}
