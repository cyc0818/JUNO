#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "TDatabasePDG.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"

#include "TTree.h"
#include "TFile.h"
#include "TDirectory.h"

#include "GtNeutronTool.h"

DECLARE_TOOL(GtNeutronTool);

GtNeutronTool::GtNeutronTool(const std::string& name)
    : ToolBase(name)
{
    declProp("inputFile", m_neutron_filename);
    declProp("startIndex", m_start_idx = 0);
    declProp("neutronEnergy", m_neutron_energy=1.e-10*CLHEP::MeV);
    file = 0;
    evt = 0;
}

GtNeutronTool::~GtNeutronTool()
{

}

bool
GtNeutronTool::configure() {
    if (m_neutron_filename.size() == 0) {
        LogError << "Missing input data file for GtNeutronTool." << std::endl;
        return false;
    }
    const char* preDir = gDirectory->GetPath();
    file = TFile::Open(m_neutron_filename.c_str());
    gDirectory->cd(preDir);
    if (file == 0) {
        LogError << "Failed to open file " << m_neutron_filename << std::endl;
        return false;
    }

    evt = dynamic_cast<TTree*>(file->Get("spall_neutron_gen"));
    if (evt == 0) {
        LogError << "Failed to load tree spall_neutron_gen in file "
                 << m_neutron_filename << std::endl;
        return false;
    }

    return initTree();
}

bool
GtNeutronTool::mutate(HepMC::GenEvent& event) 
{
    if (m_start_idx>=evt->GetEntries()) {
        LogWarn << "!!! Can't Load more data from the neutrino file. "
                << "The index reset to 0."
                << std::endl;
        m_start_idx = 0;
    }

    evt->GetEntry(m_start_idx++);

    // each neutron has its own vertex.
    for (int i = 0; i < multi; ++i) {
        double costheta = CLHEP::RandFlat::shoot(-1, 1);
        double phi = 360*CLHEP::RandFlat::shoot()*CLHEP::degree;
        double sintheta = sqrt(1.-costheta*costheta);

        double mass = CLHEP::neutron_mass_c2;
        // p is momentum = E*E-m*m = (Ek+m)*(Ek+m)-m*m
        double p = sqrt(m_neutron_energy*m_neutron_energy+2*m_neutron_energy*mass);
        double px = p*sintheta*cos(phi);
        double py = p*sintheta*sin(phi);
        double pz = p*costheta;
        int pid = 2112;
        int status = 1;
        HepMC::GenParticle* particle = new HepMC::GenParticle
            (HepMC::FourVector(px,
                               py,
                               pz,
                               m_neutron_energy+mass),
             pid, status);
        LogInfo << "-> " << posx[i] << ", " << posy[i] << ", " << posz[i] << std::endl;
        HepMC::GenVertex* vertex = new HepMC::GenVertex
            (HepMC::FourVector(posx[i],
                               posy[i],
                               posz[i],
                               captureT[i]));
        event.add_vertex(vertex);
        vertex->add_particle_out(particle);

    }

    return true;
}

bool
GtNeutronTool::initTree() {
    evt->SetBranchAddress("multi", &multi);
    evt->SetBranchAddress("captureT", &captureT);
    evt->SetBranchAddress("posx", &posx);
    evt->SetBranchAddress("posy", &posy);
    evt->SetBranchAddress("posz", &posz);
    return true;
}
