#ifndef GtPelletronBeamerTool_h
#define GtPelletronBeamerTool_h

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

#include <vector>

class GtPelletronBeamerTool: public ToolBase,
                             public IGenTool
{
    public:
        GtPelletronBeamerTool(const std::string& name);
        ~GtPelletronBeamerTool();
        // The current toolbase don't have configure.
        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        HepMC::GenVertex* createNewVertex();
        HepMC::GenParticle* createNewParticle();

        int getPdgid(const std::string& particle_name);
        double getMass(const std::string& particle_name);

    private:

        // = particle related =
        std::string m_particle_name;
        // = position related =
        // three vector for pos and direction
        // the plane is defined by this pos and the dir.
        std::vector<double> m_plane_pos;
        std::vector<double> m_plane_dir;
        double m_plane_radius; // if the plane is a disk???
        // = momentum = 
        // == beam divergence ==
        double m_beam_theta_max;
        // == beam momentum ==
        double m_beam_momentum;
        double m_beam_momentum_spread;

        int m_nparticles;
};

#endif
