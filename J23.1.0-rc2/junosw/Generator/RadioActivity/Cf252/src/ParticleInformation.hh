#ifndef PARTICLEINFORMATION_H
#define PARTICLEINFORMATION_H 1

#include <iostream>
#include <fstream>
#include <cassert>

#include <vector>

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "TROOT.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TFile.h"
#include "TH1.h"

class ParticleInformation
{ 
public:
  ParticleInformation(){number=0;}
  ~ParticleInformation()
  {
    Energy.clear();
    momentumx.clear();
    momentumy.clear();
    momentumz.clear();
    PDG.clear();
    x.clear();
    y.clear();
    z.clear();
  }

  double GetEnergy(int i)
  { return Energy[i]; }

  int GetEventType()
  { return eventType; }

  int GetPDG(int i)
  { return PDG[i]; }

  int GetNumber()
  { return number; }

  int GetSize()
  { return x.size(); }

  double GetMomentum_x(int i)
  { return momentumx[i]; }

  double GetMomentum_y(int i)
  { return momentumy[i]; }

  double GetMomentum_z(int i)
  { return momentumz[i]; }

  double GetX(int i)
  { return x[i]; }

  double GetY(int i)
  { return y[i]; }

  double GetZ(int i)
  { return z[i]; }

  void SetNumber(int num)
  { number=num; }

  void SetEnergy(double energy)
  { Energy.push_back(energy); }
  
  void SetEventType(int evtType)
  { eventType=evtType; }
 
  void SetPDG(int pdg)
  { PDG.push_back(pdg); }

  void SetMomentum(double xx,double yy,double zz)
  { 
    momentumx.push_back(xx);
    momentumy.push_back(yy);
    momentumz.push_back(zz);
  }
  
  void SetPosition(double xx,double yy,double zz)
  { 
    x.push_back(xx);
    y.push_back(yy);
    z.push_back(zz);
  }
  
  void Resize()
  { 
    Energy.resize(0);
    PDG.resize(0);
    momentumx.resize(0);
    momentumy.resize(0);
    momentumz.resize(0);
    x.resize(0);
    y.resize(0);
    z.resize(0);
  }
  
private:
  std::vector<double> Energy;
  std::vector<double> momentumx;
  std::vector<double> momentumy;
  std::vector<double> momentumz; 
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  std::vector<int> PDG; 
  int eventType;
  int number;
};
#endif // PARTICLEINFORMATION_H
