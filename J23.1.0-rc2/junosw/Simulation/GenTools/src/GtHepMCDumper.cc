#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "GtHepMCDumper.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

DECLARE_TOOL(GtHepMCDumper);

GtHepMCDumper::GtHepMCDumper(const std::string& name)
    : ToolBase(name)
{

}

GtHepMCDumper::~GtHepMCDumper()
{

}

bool
GtHepMCDumper::configure()
{
    return true;
}

bool
GtHepMCDumper::mutate(HepMC::GenEvent& event)
{
    LogInfo << "Event #" << event.event_number()
            << " signal_process_id=" << event.signal_process_id() << ", "
            << event.vertices_size() << " vertices" << std::endl;

    if (SniperLog::logLevel() <= 3 ) {
        event.print();
    }

    return true;
}
