#ifndef GtHepMCDumper_h
#define GtHepMCDumper_h

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

class GtHepMCDumper: public ToolBase,
                     public IGenTool
{
    public:
        GtHepMCDumper(const std::string& name);
        ~GtHepMCDumper();

        bool configure();
        bool mutate(HepMC::GenEvent& event);
};

#endif
