//#include "GenDecay/Radiation.h"
#include "Radiation.h"
//#include "GaudiKernel/ServiceHandle.h"
//#include "GaudiKernel/IRndmGenSvc.h"
//#include "GaudiKernel/IMessageSvc.h"
//#include "GaudiKernel/MsgStream.h"
//#include "GaudiKernel/GaudiException.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperLog.h"
#include "CLHEP/Random/RandFlat.h"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <cmath>
#include <sstream>
#include <exception>

using namespace std;
using namespace GenDecay;

std::ostream& operator<<(std::ostream& o, const Radiation& r) 
{
    return o << r.asString(); 
}

// base

Radiation::Radiation(double energy) : m_energy(energy) 
{
}

Radiation::~Radiation() 
{
}

double Radiation::kineticEnergy() const
{
    return m_energy; 
}

// alpha

AlphaRadiation::AlphaRadiation(double energy, int parentA)
    : Radiation(energy), m_parentA(parentA)
{
}
AlphaRadiation::~AlphaRadiation()
{
}

std::string AlphaRadiation::asString() const
{
    stringstream ss;
    ss << "alpha: A=" << m_parentA << ", KE=" << m_energy << ends;
    return ss.str().c_str();
}

double AlphaRadiation::kineticEnergy() const
{
    // NNDC tables give alpha kinetic energy, not Qvalue
    //return (m_parentA-4.0)/m_parentA * m_energy;
    return m_energy;
}

int AlphaRadiation::pid() const
{
    return 1000020040;
}
double AlphaRadiation::mass() const
{
    return 3.727000*CLHEP::GeV;
}

// beta

BetaRadiation::BetaRadiation(double energy, int parentZ)
    : Radiation(energy)
    , m_parentZ(parentZ)
    , m_daughterZ(0)
    , m_betaSign(0)
    , m_endpoint(0.0)
{
    // SniperPtr<IRandomSvc> svc("RandomSvc");
    // if (svc.valid()) {
    //     m_rs = svc.data();
    // }

    // if ( 0 == m_rs )
    // {
    //     LogError << "Can not load RandomSvc." << std::endl;
    //     throw std::exception();
    // }    

    if (parentZ < 0) {          // beta+ decay 
        m_parentZ = -parentZ;
        m_daughterZ = m_parentZ - 1;
        m_betaSign = +1;
        m_endpoint = m_energy - 2.0*CLHEP::electron_mass_c2;
    }
    else {                      // # beta- decay
        m_parentZ = parentZ;
        m_daughterZ = m_parentZ + 1;
        m_betaSign = -1;
        m_endpoint = m_energy;
    }

//    ServiceHandle<IRndmGenSvc> rgsh("RndmGenSvc","BetaRadiation");
//    IRndmGenSvc *rgs = rgsh.operator->();
//    if (m_rand.initialize(rgs, Rndm::Flat(0,1)).isFailure()) {
//        throw GaudiException("Failed to initialize uniform random numbers",
//                             "GenDecay::Radiation",StatusCode::FAILURE);
//    }
//    if (!m_rand) {
//        throw GaudiException("Got null Rndm::Numbers object",
//                             "GenDecay::Radiation",StatusCode::FAILURE);
//    }

    this->normalize();
}

void BetaRadiation::normalize()
{
    const int steps = 1000;
    double dx = m_endpoint/steps;
    double lo = dx/2.0;

    m_norm = 1.0;
    double sum = 0.0;
    for (int ind=0; ind<steps; ++ind) {
        sum += dx * this->dnde(ind*dx+lo);
    }
    m_norm = sum;

    double max = 0;
    for (int ind=0; ind<steps; ++ind) {
        double tmp = this->dnde(ind*dx+lo);
        if (tmp > max) max = tmp;
    }
    m_maximum = max;
}

BetaRadiation::~BetaRadiation()
{
}

std::string BetaRadiation::asString() const
{
    stringstream ss;
    ss << "beta"<< (m_betaSign < 0 ? '-' : '+') <<": Z=" 
       << m_parentZ << ", E_endpoint=" << m_energy << ends;
    return ss.str().c_str();
}

double BetaRadiation::kineticEnergy() const
{
    // MC/rejection method to sample dN/dE spectrum
    while (true) {
//        double T = m_rand() * m_endpoint;
//        double P = m_rand() * m_maximum;
	double T = CLHEP::RandFlat::shoot() * m_endpoint;
	double P = CLHEP::RandFlat::shoot() * m_maximum;

	// double T = m_rs->random() * m_endpoint;
	// double P = m_rs->random() * m_maximum;


        if (P <= this->dnde(T)) return T;
    }
    return 0.0;
}

double BetaRadiation::dnde(double kineticEnergy) const
{
    if (kineticEnergy > m_endpoint) return 0.0;
    if (kineticEnergy < 0.0) return 0.0;
    return this->fermi_function(kineticEnergy) * this->dnde_noff(kineticEnergy) / m_norm;
}

double BetaRadiation::dnde_noff(double kineticEnergy) const
{
    double W = m_endpoint / CLHEP::electron_mass_c2 + 1.0;
    double E = kineticEnergy / CLHEP::electron_mass_c2 + 1.0;
    return sqrt(E*E-1.0) * (W-E)*(W-E) * E;
}

double BetaRadiation::fermi_function(double kineticEnergy) const
{
    double E = kineticEnergy / CLHEP::electron_mass_c2 + 1.0;
    double P = sqrt(E*E-1.0);
    double U = -1*m_betaSign*m_daughterZ/137.0;
    double S = sqrt(1.0 - U*U) - 1.0;
    double Y = 2.0*M_PI*U*E/P;
    double A1 = U*U * E*E + P*P/4.0;
    double A2 = fabs(Y/(1.0-exp(-Y)));
    return pow(A1,S)*A2;
}

int BetaRadiation::pid() const
{
    return -1*m_betaSign*11;
}
double BetaRadiation::mass() const
{
    return CLHEP::electron_mass_c2;
}

GammaRadiation::GammaRadiation(double energy)
    : Radiation(energy)
{
}
GammaRadiation::~GammaRadiation()
{
}

std::string GammaRadiation::asString() const
{
    stringstream ss;
    ss << "gamma: Energy=" << m_energy << ends;
    return ss.str().c_str();
}

int GammaRadiation::pid() const
{
    return 22;
}

double GammaRadiation::mass() const
{
    return 0;
}

ElectronCapture::ElectronCapture(double characteristic_energy)
    : Radiation(characteristic_energy)
{
}
ElectronCapture::~ElectronCapture()
{
}

std::string ElectronCapture::asString() const
{
    stringstream ss;
    ss << "electroncapture: Energy=" << m_energy << ends;
    return ss.str().c_str();
}

int ElectronCapture::pid() const
{
    return 0;                   // What to return?
}

double ElectronCapture::mass() const
{
    return 0.0;
}

