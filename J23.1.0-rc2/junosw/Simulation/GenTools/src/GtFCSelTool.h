#ifndef GtFCSelTool_h
#define GtFCSelTool_h

// Pre-select the FC dataset of atmo neutrino events.

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

class GtFCSelTool: public ToolBase,
                   public IGenTool
{
public:
    GtFCSelTool(const std::string& name);
    ~GtFCSelTool();

    bool configure();
    bool mutate(HepMC::GenEvent& event);
private:
    float dElossdx_muon; // unit: MeV/mm. Since Edep has a bias with Eloss for muon, here we need to use dEloss/dx to estimate the kinetic energy loss.
    float rcut; // length unit: millimeter
};

#endif
