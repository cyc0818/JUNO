#ifndef GtGunGenTool_h
#define GtGunGenTool_h

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

#include "GRDMSelector.hh"

#include <vector>
#include <memory>

/*
 *
 * # Extension (1) GRDM (Geant4 Radioactivity Decay Module)
 *   There are several ways to invoke the GRDM:
 *   - If the particle name is a PDGID and the number is greater than 1000000000, 
 *     then the G4 radioactivity decay will be applied. 
 *   - If the particle name is a file containing following:
 *       pdgcode ratio Amin Amax Zmin Zmax
 *       pdgcode ratio Amin Amax Zmin Zmax
 *       ...
 *     then one of the particle will be selected randomly at each event.
 *
 */


class GtGunGenTool: public ToolBase,
                    public IGenTool
{
    public:
        GtGunGenTool(const std::string& name);
        ~GtGunGenTool();
        // The current toolbase don't have configure.
        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        HepMC::GenParticle* appendParticle(int index);
        HepMC::ThreeVector getMomentum(double p);
        int getPdgid(const std::string& particle_name);
        double getMass(const std::string& particle_name);

    private:
        std::vector<std::shared_ptr<GRDMSelector>> grdm_selectors;

    private:

        std::vector<std::string> m_particleNames;
        std::vector<int> m_particlePDG;
        std::vector<double> m_particleMass; // MeV.
        std::vector<int> m_particleStatus; // status
        // By default, the status is 1.
        // Followings are the detailed:
        // * 0x1000: need radioactivity decay by G4.

        std::vector<double> m_particleMomentums;
        /*
         * Momentum Interpretation
         *  * Momentum
         *  * TotalEnergy
         *  * KineticEnergy
         */
        std::string m_momOrKEOrTE;
        /*
         * Momentum Mode
         *  * Fix
         *  * Uniform (mean, half width)
         *  * Range [min, max]
         *  * Gaus (mean, sigma)
         */
        std::string m_particleMomentumMode;
        std::vector<double> m_particleMomentumParams;

        /*
         * Direction Mode:
         *  * Random
         *  * Fix
         *    If Fix mode is choosen, the user should set the directions.
         */
        std::string m_direction_mode;
        std::vector< std::vector<double> > m_particleDirections;

        /*  
         * Position Mode:
         *  * Omit: Set the positions in other tools
         *  * FixOne: Only one vertex 
         *  * FixMany: every particle will have its own vertex
         *
         */
        std::string m_position_mode;
        std::vector< std::vector<double> > m_particlePositions;


        /*
         * Time Mode:
         *  * Omit: skip this
         *  * FixOne: Only one vertex
         *  * FixMany: each particle will have its own vertex/time
         *
         */
        std::string m_time_mode;
        std::vector<double> m_particleTimes;
};
#endif
