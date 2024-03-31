#include <boost/python.hpp>
#include <map>
#include <iostream>

#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "TDatabasePDG.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "GtPelletronBeamerTool.h"

DECLARE_TOOL(GtPelletronBeamerTool);

GtPelletronBeamerTool::GtPelletronBeamerTool(const std::string& name)
    : ToolBase(name)
{
    declProp("particleName", m_particle_name);
    declProp("planeCentrePos", m_plane_pos);
    declProp("planeDirection", m_plane_dir);
    declProp("planeRadius", m_plane_radius);

    declProp("beamThetaMax", m_beam_theta_max);
    declProp("beamMomentum", m_beam_momentum);
    declProp("beamMomentumSpread", m_beam_momentum_spread);

    declProp("nparticles", m_nparticles);

    // default value
    m_particle_name = "e+";
    m_nparticles = 1;
    //
    m_plane_pos.push_back(0);
    m_plane_pos.push_back(0);
    m_plane_pos.push_back(0);
    //
    m_plane_dir.push_back(0);
    m_plane_dir.push_back(0);
    m_plane_dir.push_back(1);
    // 
    m_plane_radius = 10*CLHEP::mm;
    //
    m_beam_theta_max = 1.1e-6*CLHEP::rad;
    m_beam_momentum = 1.0*CLHEP::MeV;
    m_beam_momentum_spread = 0.1*CLHEP::MeV;
}

GtPelletronBeamerTool::~GtPelletronBeamerTool()
{

}

bool
GtPelletronBeamerTool::configure()
{
    // check the user input
    // == particle name ==
    TDatabasePDG* db_pdg = TDatabasePDG::Instance();
    if (not db_pdg->GetParticle(m_particle_name.c_str())) {
        LogError << "Can't find particle " << m_particle_name << std::endl;
        return false;
    }
    // == plane ==
    if (m_plane_radius <= 0.) {
        LogError << "The radius of plane should be greater than zero." << std::endl;
        return false;
    }
    // === direction ===
    double tot = std::sqrt(m_plane_dir[0]*m_plane_dir[0]
                         + m_plane_dir[1]*m_plane_dir[1]
                         + m_plane_dir[2]*m_plane_dir[2]);
    if (tot <= 0) {
        LogError << "There are some problems in  plane direction ("
                 << m_plane_dir[0] << ", "
                 << m_plane_dir[1] << ", "
                 << m_plane_dir[2] << ") "
                 << std::endl;
        return false;
    }
    m_plane_dir[0] /= tot; 
    m_plane_dir[1] /= tot;
    m_plane_dir[2] /= tot;
    // == beam ==
    if (m_beam_theta_max < 0 or m_beam_theta_max > CLHEP::pi) {
        LogError << "Beam Theta should be in [0, pi]" << std::endl;
        return false;
    }
    if (m_beam_momentum < 0) {
        LogError << "Beam momentum should be >= zero." << std::endl;
        return false;
    }
    if (m_beam_momentum_spread < 0) {
        LogError << "Beam momentum spread should be >= zero." << std::endl;
        return false;
    }
    return true;
}

bool
GtPelletronBeamerTool::mutate(HepMC::GenEvent& event)
{
    for (int i = 0; i < m_nparticles; ++i) {
        // always create a new vertex.
        HepMC::GenVertex* vertex = createNewVertex();
        event.set_signal_process_vertex(vertex);
        // in the vertex, only one particle
        HepMC::GenParticle* particle = createNewParticle();
        vertex->add_particle_out(particle);
    }
    return true;
}

HepMC::GenVertex*
GtPelletronBeamerTool::createNewVertex()
{
    // = generate a 2-D position in the plane =
    double localx; double localy;
    // == method 1. generate point in the disk ==
    while (true) {
        localx = CLHEP::RandFlat::shoot(-m_plane_radius, m_plane_radius);
        localy = CLHEP::RandFlat::shoot(-m_plane_radius, m_plane_radius);
        if (std::sqrt(localx*localx+localy*localy) < m_plane_radius) {
            break;
        }
    }
    // = tranform the point into the global =
    // == rotate the point first ==
    // original is assume it is in the local coordinate (dir is (0,0,1))
    CLHEP::Hep3Vector localpoint(localx, localy, 0);
    CLHEP::Hep3Vector newdir(m_plane_dir[0], m_plane_dir[1], m_plane_dir[2]);

    localpoint.rotateUz(newdir);
    // == translate the point ==
    // from (0,0,0) to newpos
    CLHEP::Hep3Vector newpos(m_plane_pos[0], m_plane_pos[1], m_plane_pos[2]);
    localpoint += newpos;
    // now, the localpoint is the new point.
    double t = 0;
    HepMC::FourVector postime(localpoint.x(),localpoint.y(),localpoint.z(),t);
    HepMC::GenVertex* vertex = new HepMC::GenVertex(postime);
    return vertex;
}

HepMC::GenParticle*
GtPelletronBeamerTool::createNewParticle()
{
    HepMC::ThreeVector mom;
    double p_mom = CLHEP::RandGauss::shoot(m_beam_momentum, m_beam_momentum_spread);
    double mass = getMass(m_particle_name); // return unit is GeV
    double energy = sqrt(p_mom*p_mom+mass*mass);
    // = direction =
    // == beam divergence ==
    double theta; double phi;
    while (true) {
        theta=std::acos(1-CLHEP::RandFlat::shoot()*(1.0-std::cos(m_beam_theta_max)))*CLHEP::rad;
        if (theta<=m_beam_theta_max) {
            break;
        }
    }
    phi=CLHEP::twopi*CLHEP::RandFlat::shoot()*CLHEP::rad;
    

    // == rotate the direction ==
    CLHEP::Hep3Vector localpoint(std::sin(theta)*std::cos(phi), 
                                 std::sin(theta)*std::sin(phi),
                                 std::cos(theta));
    CLHEP::Hep3Vector newdir(m_plane_dir[0], m_plane_dir[1], m_plane_dir[2]);

    localpoint.rotateUz(newdir);

    mom.setX(p_mom*localpoint.x());
    mom.setY(p_mom*localpoint.y());
    mom.setZ(p_mom*localpoint.z());
    /// = =
    HepMC::FourVector fourmom(mom.x(),mom.y(),mom.z(),energy);
    int pdg_id = getPdgid(m_particle_name);
    HepMC::GenParticle* particle = new HepMC::GenParticle(fourmom, pdg_id, 1);

    return particle;
}

// = helper =
// == particle related ==
int 
GtPelletronBeamerTool::getPdgid(const std::string& particle_name)
{
    static std::map<std::string, int> ls_cache;

    if (not ls_cache.count(particle_name)) {

        TDatabasePDG* db_pdg = TDatabasePDG::Instance();
        TParticlePDG* particle = db_pdg->GetParticle(particle_name.c_str());

        ls_cache[particle_name] = particle->PdgCode();

    }

    return ls_cache[particle_name];
}

double
GtPelletronBeamerTool::getMass(const std::string& particle_name)
{
    static std::map<std::string, double> ls_cache;

    if (not ls_cache.count(particle_name)) {
        TDatabasePDG* db_pdg = TDatabasePDG::Instance();
        TParticlePDG* particle = db_pdg->GetParticle(particle_name.c_str());

        ls_cache[particle_name] = particle->Mass()*CLHEP::GeV;
    }

    return ls_cache[particle_name];
}
