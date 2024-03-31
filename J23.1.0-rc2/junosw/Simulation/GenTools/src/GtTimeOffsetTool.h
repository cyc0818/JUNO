#ifndef GtTimeOffsetTool_h
#define GtTimeOffsetTool_h

/*
 * Set the time offset.
 *
 * Especially for the event from GenDecay.
 */

#include "SniperKernel/ToolBase.h"
#include "GenTools/IGenTool.h"

#include "HepMC/SimpleVector.h"
#include "HepMC/GenParticle.h"

class GtTimeOffsetTool: public ToolBase,
                        public IGenTool
{
    public:

        GtTimeOffsetTool(const std::string& name);
        ~GtTimeOffsetTool();

        bool configure();
        bool mutate(HepMC::GenEvent& event);
};

#endif
