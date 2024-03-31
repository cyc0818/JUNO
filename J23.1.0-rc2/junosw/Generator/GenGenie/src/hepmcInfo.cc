#include "hepmcInfo.h"
#include "CLHEP/Units/SystemOfUnits.h"

// BELOW is from GENIE gNtpConv.cxx
#include <cassert>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TFolder.h>
#include <TBits.h>
#include <TObjString.h>
#include <TMath.h>
#include "TVector3.h"
#include <TRotation.h>
#include <TRandom.h>
#include <CLHEP/Vector/ThreeVector.h>
#include <CLHEP/Units/PhysicalConstants.h>
#include "HepMC/GenEvent.h"

bool isotropic_direction(HepMC::GenEvent& event)
{

  TRotation rota;
  double Pi = 3.1415926;
  double deg2rad = Pi / 180.;
  double xcostheta = gRandom->Uniform(-1, 1);
  double xphi = gRandom->Uniform(0, 360) * deg2rad;
  double xtheta = acos(xcostheta);
  rota.RotateX(xtheta);
  rota.RotateZ(xphi);

  // iterating over all particles in the event
  for (HepMC::GenEvent::particle_const_iterator p = event.particles_begin(); p != event.particles_end(); ++p) {
    //(*p)->print();
    HepMC::FourVector mom = (*p)->momentum();
    double ppx = mom.px();
    double ppy = mom.py();
    double ppz = mom.pz();
    TVector3 pPar(ppx, ppy, ppz);
    pPar = rota * pPar;
    mom.setPx(pPar(0));
    mom.setPy(pPar(1));
    mom.setPz(pPar(2));
    (*p)->set_momentum(mom);
    //(*p)->print();
    // std::cout << "Jie test  px py pz " << ppx << "\t" << ppy << "\t" << ppz << std::endl;
  }

  return true;
}
