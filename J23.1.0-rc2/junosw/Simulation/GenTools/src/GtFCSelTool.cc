// This gentool is used to pre select the FC atmo neutrino events to save computing resource.

#include <boost/python.hpp>

#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperLog.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include "GtFCSelTool.h"

DECLARE_TOOL(GtFCSelTool);

GtFCSelTool::GtFCSelTool(const std::string& name)
    : ToolBase(name)
{
    declProp("dedx", dElossdx_muon);
    declProp("rcut", rcut);
}

GtFCSelTool::~GtFCSelTool()
{

}

bool
GtFCSelTool::configure()
{
    return true;
}

bool
GtFCSelTool::mutate(HepMC::GenEvent& event)
{
    // Set the status for selected particles
    // Loop over vertices in the event
    for(HepMC::GenEvent::vertex_iterator vtx = event.vertices_begin();
                                         vtx != event.vertices_end();
                                         ++vtx)
    {
        const HepMC::FourVector& v = (*vtx)->position();
        bool skipPC = false;
        // Loop over particles in the vertex to judge if this vertex need to be skip
        for(HepMC::GenVertex::particle_iterator part = (*vtx)->particles_begin();
                                                part != (*vtx)->particles_end(HepMC::relatives);
                                                ++part)
        {
            if((*part)->pdg_id()!=13&&(*part)->pdg_id()!=-13) continue;
            
            const HepMC::FourVector& p = (*part)->momentum();

            float px = p.px(), py = p.py(), pz = p.pz();
            float pv = sqrt(px*px + py*py + pz*pz);

            float kine = p.e() - p.m();
            float tracklength = kine / dElossdx_muon;

            float x = px/pv*tracklength + v.x();
            float y = py/pv*tracklength + v.y();
            float z = pz/pv*tracklength + v.z();
            // this is a very loose cut: to judge if the end of the muon track is outside the central detector
            if(x*x + y*y + z*z > rcut*rcut)
            {
                skipPC = true;
                break;
            }     
        }
    
        if(skipPC) {
            for(HepMC::GenVertex::particle_iterator part = (*vtx)->particles_begin(); part != (*vtx)->particles_end(HepMC::relatives); ++part) {
                (*part)->set_status(0x4000); // To avoid confusion, use 0x4000 to tag the partially contained atmospheric neutrino events
            }
        }
    }                                
    return true;
}
