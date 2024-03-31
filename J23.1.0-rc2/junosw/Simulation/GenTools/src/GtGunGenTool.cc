#include <boost/python.hpp>
#include <map>
#include <iostream>
#include <fstream>

#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "TDatabasePDG.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"

// G4 Ion Table
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4NucleiProperties.hh"
#include "G4UImanager.hh"

#include "GtGunGenTool.h"

std::ostream& operator<<(std::ostream& os, const std::vector<double>& v) {
    os << "[";
    for (std::vector<double>::const_iterator it = v.begin();
            it != v.end(); ++it) {
        os << *it << ",";
    }
    os << "]";
    return os;
}

DECLARE_TOOL(GtGunGenTool);

GtGunGenTool::GtGunGenTool(const std::string& name)
    : ToolBase(name)
{

    declProp("particleNames", m_particleNames);
    // mean value of momenta
    declProp("particleMomentums", m_particleMomentums);
    // momentum mode: Fix, Uniform, Range, Gaus
    // params: parameters used by different modes
    // * uniform -> [mean-param, mean+param] flat distribution
    //     mean + (uniform()*2-1)*param
    // * range -> [mean, param] 
    //             (min) (max)
    // * gaus -> sigma
    //     mean + gaus(0, param)
    declProp("particleMomentumMode", m_particleMomentumMode="Fix"); 
    declProp("particleMomentumParams", m_particleMomentumParams);
    declProp("particleMomentumInterp", m_momOrKEOrTE="Momentum");
    declProp("DirectionMode", m_direction_mode="Random");
    declProp("Directions", m_particleDirections);

    declProp("PositionMode", m_position_mode="Omit");
    declProp("Positions", m_particlePositions);

    declProp("TimeMode", m_time_mode="Omit");
    declProp("Times", m_particleTimes);
}

GtGunGenTool::~GtGunGenTool()
{

}

bool
GtGunGenTool::configure()
{
    // check the size of particle names and momentums.
    if ( m_particleNames.size() != m_particleMomentums.size()
            or (m_particleMomentumMode != "Fix"
                and m_particleMomentumParams.size() != m_particleMomentums.size())
            or (m_direction_mode == "Fix"
                and m_particleNames.size() != m_particleDirections.size())
            or (m_position_mode == "FixMany" 
                and m_particleNames.size() != m_particlePositions.size())
            or (m_time_mode == "FixMany"
                and m_particleNames.size() != m_particleTimes.size())
         ) {
        LogError << "the size of particle names and momentum (direction)s are mismatch"
            << std::endl;
        return false;
    }

    if (m_position_mode != "Omit" and m_time_mode != "Omit"
        and m_position_mode != m_time_mode) {
        LogError << "The position mode and time mode are mismatch. " << std::endl;
        return false;
    }

    if (m_position_mode == "FixOne") {
        LogInfo << "Positions Mode: FixOne"
                << std::endl;
        std::vector<double>& per_pos = m_particlePositions[0];
        LogInfo << " + Position : ("
                << per_pos[0] << ", "
                << per_pos[1] << ", "
                << per_pos[2] << ") "
                << std::endl;
    }
    if (m_time_mode == "FixOne") {
        LogInfo << "Time Mode: FixOne" << std::endl;
        LogInfo << " + Time : " << m_particleTimes[0] << std::endl;
    }
        
    TDatabasePDG* db_pdg = TDatabasePDG::Instance();
    for (size_t i=0; i < m_particleNames.size(); ++i) {
        // check the particle exists or not
        // * from ROOT TParticlePDG
        int pdgcode = 0;
        double mass = 0;
        int istatus = 1;
        TParticlePDG* particle = db_pdg->GetParticle(m_particleNames[i].c_str());
        // if the name is pdg code, then try to convert from string to number and 
        // get the particle again.
        long pdgcode_cnv_from_name = atol(m_particleNames[i].c_str());
        if ((not particle) and pdgcode_cnv_from_name) {
            LogInfo << "Convert particleName to pdgcode " << pdgcode_cnv_from_name << std::endl;
            particle = db_pdg->GetParticle(pdgcode_cnv_from_name);
        }
        if (particle) {
            pdgcode = particle->PdgCode();
            mass = particle->Mass()*CLHEP::GeV;
        }
        // * from G4 Ion Table
        if (not particle) {
            // assuming the particle name is PDG ID
            if (pdgcode_cnv_from_name) {
                pdgcode = pdgcode_cnv_from_name;
                // During initialization, G4 Ion Table is not ready yet.
                // G4ParticleTable* particletbl = G4ParticleTable::GetParticleTable();
                // G4IonTable *theIonTable = particletbl->GetIonTable();
                // G4ParticleDefinition* particle_def = theIonTable->GetIon(pdgcode);
                LogInfo << "Find pdgcode " << pdgcode << std::endl;
                if (pdgcode >= 1000000000) {
                    int encoding = pdgcode;
                    encoding -= 1000000000;
                    int Z = encoding/10000;
                    encoding -= 10000*Z;
                    int A = encoding/10;
                    mass = G4NucleiProperties::GetNuclearMass(A, Z);
                    istatus = 0x1000; // need G4 radioactivity decay
                }
            }

            // assuming the particle name is a file contains following:
            //   pdgcode ratio Amin Amax Zmin Zmax
            std::ifstream file(m_particleNames[i].c_str());
            if (file) {
                LogInfo << "Find GRDM Selector file: " << m_particleNames[i] << std::endl;
                std::shared_ptr<GRDMSelector> grdmselector(new GRDMSelector(file));
                grdm_selectors.push_back(grdmselector);

                if (grdmselector->is_available()) {
                    // set the dummy pdgcode
                    pdgcode = grdmselector->records()[0].pdgcode;
                    mass = grdmselector->records()[0].mass;
                    istatus = 0x1000; // need G4 radioactivity decay
                }
            }
        }

        if ((not pdgcode) and (not particle) and (m_particleNames[i]=="PhotonSource")) {
            pdgcode = 100;
            mass = 0.;
            istatus = 1;
        } else if (not pdgcode and not particle) {
            LogError << "Can't find particle [" << m_particleNames[i] << "]" << std::endl;
            return false;
        }

        // save pdg
        m_particlePDG.push_back(pdgcode);
        m_particleMass.push_back(mass);
        m_particleStatus.push_back(istatus);

        LogInfo << "Particle/Mass/Momentum: "
                << m_particleNames[i] << "(" << pdgcode << ")"
                << "/"
                << m_particleMass[i]/CLHEP::MeV << "MeV"
                << "/"
                << m_particleMomentums[i]/CLHEP::MeV << "MeV"
                << std::endl;
        if (m_direction_mode == "Fix") {
            std::vector<double>& per_dir = m_particleDirections[i];
            LogInfo << " + Directions: "
                    << per_dir[0] << ", "
                    << per_dir[1] << ", "
                    << per_dir[2] << std::endl;
        }

    }
    return true;
}

bool
GtGunGenTool::mutate(HepMC::GenEvent& event)
{

    // get the vertex 
    HepMC::GenVertex* vertex = event.signal_process_vertex();
    if (!vertex and (m_position_mode != "FixMany") and (m_time_mode != "FixMany")) {
        double per_vertex[4] = {0,0,0,0};
        // create vertex
        if (m_position_mode == "FixOne") {
            std::vector<double>& per_pos = m_particlePositions[0];
            per_vertex[0] = per_pos[0];
            per_vertex[1] = per_pos[1];
            per_vertex[2] = per_pos[2];
        }

        // time
        if (m_time_mode == "FixOne") {
            per_vertex[3] = m_particleTimes[0];
        }
        if (!(std::find(m_particleNames.begin(), m_particleNames.end(), "PhotonSource") == m_particleNames.end())) per_vertex[3] = per_vertex[3] -100.;

        vertex = new HepMC::GenVertex(HepMC::FourVector(per_vertex[0],
                                                        per_vertex[1],
                                                        per_vertex[2],
                                                        per_vertex[3]));
        // set in event
        event.set_signal_process_vertex(vertex);
    }
    for (size_t i=0; i < m_particleNames.size(); ++i) {
        HepMC::GenParticle* particle = appendParticle(i);
        if ( !particle ) {
            LogError << "Append Particle "
                     << m_particleNames[i]
                     << " Failed"
                     << std::endl;
            return false;
        }
        if (m_position_mode == "FixMany" or m_time_mode == "FixMany") {
            // Create Vertex for every particle
            double per_vertex[4] = {0,0,0,0};
            
            if (m_position_mode == "FixMany") {
                std::vector<double>& per_pos = m_particlePositions[i];
                per_vertex[0] = per_pos[0];
                per_vertex[1] = per_pos[1];
                per_vertex[2] = per_pos[2];
            }
            if (m_time_mode == "FixMany") {
                per_vertex[3] = m_particleTimes[i];
            }
            if (m_particleNames[i]=="PhotonSource") per_vertex[3] = per_vertex[3] -100.;

            vertex = new HepMC::GenVertex(HepMC::FourVector(per_vertex[0],
                                                            per_vertex[1],
                                                            per_vertex[2],
                                                            per_vertex[3]));
            event.set_signal_process_vertex(vertex);
        }
        vertex->add_particle_out(particle);
    }

    return true;
}

HepMC::GenParticle*
GtGunGenTool::appendParticle(int index_)
{
    HepMC::GenParticle* particle = NULL;

    if (index_<0) {
        LogWarn << " Index can't be smaller than zero." << std::endl;
        return particle;
    }
    size_t index = (size_t) index_;

    // check index first
    if (index < m_particleNames.size()) {
        // const std::string& p_name = m_particleNames[index];

        // check if GRDM selector exists
        const GRDMRecord* grdm_record = nullptr;
        if (index < grdm_selectors.size()) {
            auto grdm = grdm_selectors[index];
            if (grdm->is_available()) {
                grdm_record = grdm->get_randomly();
            }
        }


        double p_mom = m_particleMomentums[index]*CLHEP::MeV;
        // if momentum mode is not Fix, try to sampling
        if (m_particleMomentumMode=="Fix") {

        } else if (m_particleMomentumMode=="Uniform") {
            double mean = p_mom;
            double param = m_particleMomentumParams[index]*CLHEP::MeV;
            while ((p_mom=(mean+param*CLHEP::RandFlat::shoot(-1, 1))) <0.0) {}
        } else if (m_particleMomentumMode=="Range") {
            double p_min = p_mom;
            double p_max = m_particleMomentumParams[index]*CLHEP::MeV;
            while ((p_mom=(CLHEP::RandFlat::shoot(p_min, p_max))) <0.0) {}
        } else if (m_particleMomentumMode=="Gaus") {
            double mean = p_mom;
            double sigma = m_particleMomentumParams[index]*CLHEP::MeV;
            while ((p_mom=(CLHEP::RandGauss::shoot(mean, sigma))) <0.0) {}
        } else {
            LogWarn << "Unknown momentum mode: " << m_particleMomentumMode
                    << std::endl;
        }
        // end momentum mode
        double mass = m_particleMass[index];
        if (grdm_record) {
            mass = grdm_record->mass;
        }


        double energy = sqrt(p_mom*p_mom+mass*mass);
        // interprete momentum
        if (m_momOrKEOrTE == "Momentum") {

        } else if (m_momOrKEOrTE == "KineticEnergy") {
            // Ek = P4-m
            // -> P4 = Ek+m
            double ke = p_mom;
            energy = ke + mass;
            // P4^2 = P3^2 + m^2
            // -> P3 = sqrt(P4^2 - m^2)
            p_mom = sqrt(energy*energy-mass*mass);
        } else if (m_momOrKEOrTE == "TotalEnergy") {
            energy = p_mom;
            if (energy<mass) {
                LogError << "Total Energy (" << energy 
                         << ") is less than Mass (" << mass << ")"
                         << std::endl;
                return 0;
            }
            p_mom = sqrt(energy*energy-mass*mass);
        }
        // end interprete

        HepMC::ThreeVector mom;
        if (m_direction_mode == "Random") {
            mom = getMomentum(p_mom);
        } else if (m_direction_mode == "Fix") {
            std::vector<double>& per_dir = m_particleDirections[index];
            double tot = std::sqrt( per_dir[0]*per_dir[0] 
                                  + per_dir[1]*per_dir[1] 
                                  + per_dir[2]*per_dir[2]);

            mom .setX( p_mom* per_dir[0]/tot );
            mom .setY( p_mom* per_dir[1]/tot );
            mom .setZ( p_mom* per_dir[2]/tot );

        }
        HepMC::FourVector fourmom(mom.x(),mom.y(),mom.z(),energy);

        int pdg_id = m_particlePDG[index];
        if (grdm_record) {
            pdg_id = grdm_record->pdgcode;
        }
        int istatus = m_particleStatus[index];

        particle = new HepMC::GenParticle(fourmom, pdg_id, istatus /* status */);

        // IMPORTANT NOTE:
        //   the magic for the GRDM is that the SetNucleusLimits is invoked 
        //   to limit the decay chain
        // 
        //   So the limitation is that only one limit is used for each event.
        //   That means we can't simulate multiple particles with decay together.
        //
        // Tao Lin <lintao AT ihep.ac.cn>, 30 Nov 2021
        if (grdm_record) {
            grdm_record->Dump();

            G4UImanager* UI = G4UImanager::GetUIpointer(); 
            TString ui_cmd = TString::Format("/grdm/nucleusLimits %d %d %d %d",
                                             grdm_record->Amin,
                                             grdm_record->Amax,
                                             grdm_record->Zmin,
                                             grdm_record->Zmax);
            LogInfo << " grdm cmd: " << ui_cmd << std::endl;
            UI->ApplyCommand(ui_cmd);
        }
    }

    return particle;
}

HepMC::ThreeVector
GtGunGenTool::getMomentum(double p)
{
    double costheta = CLHEP::RandFlat::shoot(-1, 1);
    double phi = 360*CLHEP::RandFlat::shoot()*CLHEP::degree;
    double sintheta = sqrt(1.-costheta*costheta);
    HepMC::ThreeVector tv;
    tv.setX(p*sintheta*cos(phi));
    tv.setY(p*sintheta*sin(phi));
    tv.setZ(p*costheta);

    return tv;
}

int 
GtGunGenTool::getPdgid(const std::string& particle_name)
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
GtGunGenTool::getMass(const std::string& particle_name)
{
    static std::map<std::string, double> ls_cache;

    if (not ls_cache.count(particle_name)) {
        TDatabasePDG* db_pdg = TDatabasePDG::Instance();
        TParticlePDG* particle = db_pdg->GetParticle(particle_name.c_str());

        ls_cache[particle_name] = particle->Mass()*CLHEP::GeV;
    }

    return ls_cache[particle_name];
}
