#include <boost/python.hpp>
#include "GtOpScintTool.h"

#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

// GEANT4 Related
#include <globals.hh>
#include "G4Material.hh" 
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "Randomize.hh"                                                         
#include "G4ThreeVector.hh"

using namespace CLHEP;

DECLARE_TOOL(GtOpScintTool);

GtOpScintTool::GtOpScintTool(const std::string& name)
    : ToolBase(name)
{
    aPhysicsOrderedFreeVector = 0;

    // default is from DetSimOptions/src/OpticalProperty.icc
    declProp("PhotonsPerEvent", m_total_numbers=11522); 
    // EnergyMode and TimeMode:
    // * LS: LS emission, including the time
    // * Fixed: fixed energy, fixed time
    declProp("EnergyMode", m_energy_mode="LS");
    declProp("TimeMode", m_time_mode="LS");

    // Diffuser ball
    declProp("EnableDfball", m_dfball=false);
    declProp("OffsetInX", m_dfball_x=0.);
    declProp("OffsetInY", m_dfball_y=0.);
    declProp("OffsetInZ", m_dfball_z=0.);

    // in geant4, default unit is MeV.
    // so in python, we need to write 3.e-6
    declProp("FixedEnergy", fixed_energy=3.*eV);

    // FIXME
    // please note, these values can be got from other objects.
    // to keep the consistency, some values can got from geant4 directly.
    declProp("FastTimeConst", fastTimeConstant=4.93*ns);
    declProp("SlowTimeConst", slowTimeConstant=20.6*ns);
    declProp("SlowerTimeConst", slowerTimeConstant=190*ns);
    declProp("yieldRatio", yieldRatio=0.799);
    declProp("slowerRatio", slowerRatio=0.15);

    // direction, using theta (from z axis)
    // cos(theta) [-1, 1]
    declProp("cosThetaLower", ct1=-1.);
    declProp("cosThetaUpper", ct2=+1.);

}

GtOpScintTool::~GtOpScintTool() {

}

bool
GtOpScintTool::configure() {
    return true;
}

bool
GtOpScintTool::init_spec() {

    if (aPhysicsOrderedFreeVector) {
        return true;
    }
    // init
    //
    G4Material* aMaterial = 0;
    G4String materialname = "LS";

    aMaterial = G4Material::GetMaterial(materialname);
    if (!aMaterial) {
        return false;
    }

    G4MaterialPropertiesTable* aMaterialPropertiesTable =
      aMaterial->GetMaterialPropertiesTable();
    if (!aMaterialPropertiesTable) {
        return false;
    }
    G4MaterialPropertyVector* theFastLightVector = 
      aMaterialPropertiesTable->GetProperty("FASTCOMPONENT");
    if (!theFastLightVector) {
        return false;
    }

    aPhysicsOrderedFreeVector =
      new G4PhysicsOrderedFreeVector();

    G4double currentIN = (*theFastLightVector)[0];

    if (currentIN >= 0.0) 
    {
      G4double currentPM = theFastLightVector->Energy(0);

      G4double currentCII = 0.0;

      aPhysicsOrderedFreeVector->
        InsertValues(currentPM , currentCII);

      G4double prevPM  = currentPM;
      G4double prevCII = currentCII;
      G4double prevIN  = currentIN;

      for (size_t ii = 1;
           ii < theFastLightVector->GetVectorLength();
           ++ii)
      {
        currentPM = theFastLightVector->Energy(ii);

        currentIN = (*theFastLightVector)[ii];

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

    return true;
}

bool 
GtOpScintTool::add_optical_photon(HepMC::GenEvent& event)
{

    G4PhysicsOrderedFreeVector* ScintillationIntegral = aPhysicsOrderedFreeVector;
    // energy
    G4double sampledEnergy = 0.0;
    if (m_energy_mode == "LS") {
        G4double CIIvalue = G4UniformRand()*
            ScintillationIntegral->GetMaxValue();
        sampledEnergy=
            ScintillationIntegral->GetEnergy(CIIvalue);
    } else if (m_energy_mode == "Fixed") {
        sampledEnergy = fixed_energy;
    } else {
        LogError << "Unknown mode " << m_energy_mode << std::endl;
        return false;
    }
    
    // momentum
    G4double cost = ct1 + (ct2-ct1)*G4UniformRand();
    G4double sint = sqrt((1.-cost)*(1.+cost));

    G4double phi = twopi*G4UniformRand();
    G4double sinp = sin(phi);
    G4double cosp = cos(phi);

    G4double px = sint*cosp;
    G4double py = sint*sinp;
    G4double pz = cost;

    G4ThreeVector photonMomentum(px, py, pz);

    // polarization
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
    
    // time
    double init_time = 0;
    const double dfsigma = 0.23;
    if (m_time_mode == "LS") {
        double ScintillationTime = 0.*ns;
        if (G4UniformRand() < yieldRatio) {
            ScintillationTime = fastTimeConstant;
        } else {
            if (G4UniformRand() < slowerRatio) {
                ScintillationTime = slowerTimeConstant;
            } else {
                ScintillationTime = slowTimeConstant;
            }
        }
        init_time = -ScintillationTime*log(G4UniformRand());
    } else if (m_dfball) {
        init_time = G4RandGauss::shoot(0., dfsigma)*ns;
    } else if (m_time_mode == "Fixed") {
        init_time = 0;
    } else {
        LogError << "Unknown time mode " << m_time_mode << std::endl;
        return false;
    }
    // add particle
    // = create OP =
    px *= sampledEnergy;
    py *= sampledEnergy;
    pz *= sampledEnergy;
    double polx = photonPolarization.x();
    double poly = photonPolarization.y();
    double polz = photonPolarization.z();
    double p_mom = std::sqrt(px*px+py*py+pz*pz);

    double x = 0; 
    double y = 0; 
    double z = 0;
    double t = init_time;
    const double dfradius = 15.*mm;
    if (m_dfball) {
        x = m_dfball_x + dfradius*px/p_mom;
        y = m_dfball_y + dfradius*py/p_mom;
        z = m_dfball_z + dfradius*pz/p_mom;
    }

    HepMC::GenParticle* particle = new HepMC::GenParticle(
                                        HepMC::FourVector(px,py,pz, p_mom),
                                        20022, // optical photon
                                        1);
    particle->set_polarization(HepMC::Polarization(
                                HepMC::ThreeVector(polx,poly,polz)));
    // = create Vertex =
    HepMC::GenVertex* vertex = new HepMC::GenVertex(HepMC::FourVector(x,y,z,t));
    vertex->add_particle_out(particle);
    // = =
    event.set_signal_process_vertex(vertex);

    return true;
}

bool
GtOpScintTool::mutate(HepMC::GenEvent& event)
{
    // initialize spec
    if (m_energy_mode=="LS" && !aPhysicsOrderedFreeVector) {
        bool st = init_spec();
        if (!st) { 
            LogError << "Faild to initialize the emission spectrum." << std::endl;
            return false; 
        }
    }
    // total generated photons
    // TODO Gaus(mean, sigma)
    int total = m_total_numbers;
    for (int i = 0; i < total; ++i) {
        bool st = add_optical_photon(event);
        if (not st) { return false; }
    }
    return true;
}
