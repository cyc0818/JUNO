/*
 * =====================================================================================
 *
 *       Filename:  GtOpScintTool.h
 *
 *    Description:  This tool is generated scintillation light at fixed positions.
 *                  The energy spectrum is emission spectrum of LS.
 *
 *                  The old version:
 *                  * offline/Simulation/DetSimV2/GenSim/src/OpticalPhotonGun.cc
 *                  * offline/Simulation/DetSimV2/GenSim/src/OpticalPhotonGunRan.cc
 *
 *                  Due to the time of every photon is different, we need one vertex
 *                  for one optical photon.
 *
 *         Author:  Tao Lin (lintao@ihep.ac.cn), 
 *   Organization:  IHEP
 *
 * =====================================================================================
 */
#ifndef GtOpScintTool_h
#define GtOpScintTool_h

#include <string>

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

class G4PhysicsOrderedFreeVector;
namespace HepMC {
    class GenEvent;
}

class GtOpScintTool: public ToolBase,
                     public IGenTool
{
public:
    GtOpScintTool(const std::string& name);
    ~GtOpScintTool();

    bool configure();
    bool mutate(HepMC::GenEvent& event);

private:
    // initialize the spectrum
    bool init_spec();
    bool add_optical_photon(HepMC::GenEvent& event);


private:
    G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector;
    int m_total_numbers;

    std::string m_energy_mode;
    std::string m_time_mode;

    bool m_dfball;
    double m_dfball_x;
    double m_dfball_y;
    double m_dfball_z;

    double fastTimeConstant;
    double yieldRatio;           // fast/(fast+slow+slower)
    double slowTimeConstant;     // 
    double slowerTimeConstant;   //
    double slowerRatio;          // slower/(slow+slower)

    double fixed_energy;

    // cos(theta) range [ct1, ct2]
    double ct1;
    double ct2;

};

#endif
