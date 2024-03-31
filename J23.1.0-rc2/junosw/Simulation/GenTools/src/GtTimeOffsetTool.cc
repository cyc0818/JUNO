#include <boost/python.hpp>
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "GtTimeOffsetTool.h"
#include <limits>

DECLARE_TOOL(GtTimeOffsetTool);

GtTimeOffsetTool::GtTimeOffsetTool(const std::string& name)
    : ToolBase(name)
{

}

GtTimeOffsetTool::~GtTimeOffsetTool()
{

}

bool
GtTimeOffsetTool::configure()
{

    return true;
}

bool
GtTimeOffsetTool::mutate(HepMC::GenEvent& event)
{
    // copy from dyb
    double dt = std::numeric_limits<double>::max();

    for (HepMC::GenEvent::vertex_const_iterator it = event.vertices_begin(); 
                                                it != event.vertices_end();
                                                ++it) {
        double t = (*it)->position().t();
        if (t < dt) dt = t;
    }

    double toffset = dt; // unit: ns

    // we should remove dt from all vertices
    for (HepMC::GenEvent::vertex_iterator it = event.vertices_begin(); 
                                          it != event.vertices_end();
                                          ++it) {
        HepMC::FourVector vtx = (*it)->position();
        double t = vtx.t() - toffset;
        vtx.setT(t);
        (*it)->set_position(vtx);
    }

    return true;
}
