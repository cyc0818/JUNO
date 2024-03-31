#include "libnuwro.h"
#include "NuWroEvGen.h"
#include "event1.h"

DECLARE_TOOL(NuWroEvGen);

NuWroEvGen::NuWroEvGen(const std::string& name) : ToolBase(name) {
    declProp("NuWroConfigFile", gOptNuWroConfigFile);
}

bool NuWroEvGen::configure() {
    // LogInfo << "configure NuWro..." << std::endl;
    // std::cerr << "configure NuWro..." << std::endl;
    if (!evg_driver){
        try{
            Initialize();
        }
        catch(...){
            // std::cerr << "NuWroEvGen::configure() failed." << std::endl;
            return false;
        }
    }
    return true;
}

void NuWroEvGen::Initialize(void){
    evg_driver = std::make_shared<nuwro_interface>(gOptNuWroConfigFile.c_str());
}

bool NuWroEvGen::mutate(HepMC::GenEvent& event){
    event2HepMC(evg_driver->gen_event(), event);
    return true;
}

template <typename T>
bool event2HepMC(T&& e, HepMC::GenEvent& hepmc_event){
    HepMC::GenVertex* hepmc_vertex = hepmc_event.signal_process_vertex();
    if (not hepmc_vertex) {
        double brVtxX = 0.0;
        double brVtxY = 0.0;
        double brVtxZ = 0.0;
        double brVtxT = 0.0;

        const double m_scale_xyz = 1.0;
        const double m_scale_t = 1.0;

        hepmc_vertex = new HepMC::GenVertex(HepMC::FourVector(brVtxX*m_scale_xyz,
                                                              brVtxY*m_scale_xyz,
                                                              brVtxZ*m_scale_xyz,
                                                              brVtxT*m_scale_t));
        hepmc_event.set_signal_process_vertex(hepmc_vertex);
    }
    for (auto && particle : e.post)
    {
        auto hepmc_particle = new HepMC::GenParticle( 
                                                HepMC::FourVector(
                                                                particle.p4().x, 
                                                                particle.p4().y, 
                                                                particle.p4().z, 
                                                                particle.p4().t),
                                                particle.pdg,
                                                1 /* status */
                ); 
        hepmc_vertex->add_particle_out(hepmc_particle); 
    }
    // for (auto && particle : e.in)
    // {
    //     auto hepmc_particle = new HepMC::GenParticle( 
    //                                             HepMC::FourVector(
    //                                                             particle.p4().x, 
    //                                                             particle.p4().y, 
    //                                                             particle.p4().z, 
    //                                                             particle.p4().t),
    //                                             particle.pdg,
    //                                             0 /* status */
    //             ); 
    //     hepmc_vertex->add_particle_in(hepmc_particle); 
    // }
    return 0;
}
