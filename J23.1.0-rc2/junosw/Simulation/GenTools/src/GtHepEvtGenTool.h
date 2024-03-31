#ifndef GtHepEvtGenTool_h
#define GtHepEvtGenTool_h

/* This is stolen from NuWa
 * See: Nuwa: dybgaudi/Simulation/GenTools/
 */

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

class HepEvt2HepMC;
class GtHepEvtGenTool: public ToolBase,
                       public IGenTool
{
    public:
        GtHepEvtGenTool(const std::string& name);
        ~GtHepEvtGenTool();

        bool configure();
        bool mutate(HepMC::GenEvent& event);

    private:
        std::string m_source;
        HepEvt2HepMC* m_parser;
};

#endif
