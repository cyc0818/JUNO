#include <boost/python.hpp>
#include "PostGenTools.h"
#include "GenTools/GenEventBuffer.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"

#include "RootWriter/RootWriter.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "Event/GenHeader.h"
#include "Event/GenEvt.h"

#include "HepMC/GenEvent.h"

DECLARE_ALGORITHM(PostGenTools);

PostGenTools::PostGenTools(const std::string& name)
    : AlgBase(name)
{
}

PostGenTools::~PostGenTools()
{
}

bool
PostGenTools::initialize()
{
    SniperPtr<RootWriter> svc(*getParent(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enalbe it in your job option file."
                 << std::endl;
        return false;
    }
    m_evt_tree = svc->bookTree(*m_par, "GENEVT/geninfo", "Generator Info");
    m_evt_tree->Branch("evtID", &m_eventID, "evtID/I");
    m_evt_tree->Branch("nInitParticles", &m_init_nparticles, "nInitParticles/I");
    m_evt_tree->Branch("InitPDGID", m_init_pdgid, "InitPDGID[nInitParticles]/I");
    m_evt_tree->Branch("InitX", m_init_x, "InitX[nInitParticles]/F");
    m_evt_tree->Branch("InitY", m_init_y, "InitY[nInitParticles]/F");
    m_evt_tree->Branch("InitZ", m_init_z, "InitZ[nInitParticles]/F");
    m_evt_tree->Branch("InitPX", m_init_px, "InitPX[nInitParticles]/F");
    m_evt_tree->Branch("InitPY", m_init_py, "InitPY[nInitParticles]/F");
    m_evt_tree->Branch("InitPZ", m_init_pz, "InitPZ[nInitParticles]/F");
    m_evt_tree->Branch("InitKine", m_init_kine, "InitKine[nInitParticles]/F");
    m_evt_tree->Branch("InitMass", m_init_mass, "InitMass[nInitParticles]/F");
    m_evt_tree->Branch("InitTime", m_init_time, "InitTime[nInitParticles]/F");
    return true;
}

bool
PostGenTools::execute()
{
    LogInfo << "PRINT GEN EVENT DATA" << std::endl;

    SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
    if (navBuf.invalid()) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    auto evt_nav = navBuf->curEvt();
    if (not evt_nav) {
        LogError << "cannot get the EvtNavigator @ /Event" << std::endl;
        return false;
    }

    auto gen_header = JM::getHeaderObject<JM::GenHeader>(evt_nav);
    if (not gen_header) {
        LogError << "cannot get the GenHeader @ /Event" << std::endl;
        return false;
    }
    auto gen_event = dynamic_cast<JM::GenEvt*>(gen_header->event());
    if (not gen_event) {
        LogError << "cannot get the GenEvent @ /Event" << std::endl;
        return false;
    }

    LogInfo << "get the GenEvent @ /Event" << std::endl;
    gen_event->getEvent()->print();

    // save the info into Root File.
    // reset the value first
    m_init_nparticles = 0;
    // Loop over vertex first
    //     Loop over particles in vertex
      
    // Loop over vertices in the event
    HepMC::GenEvent* gep = gen_event->getEvent();
    HepMC::GenEvent::vertex_const_iterator
        iVtx = (*gep).vertices_begin(),
        doneVtx = (*gep).vertices_end();
    for (/*nop*/; doneVtx != iVtx; ++iVtx) {
        const HepMC::FourVector& v = (*iVtx)->position();
        // Loop over particles in the vertex
        HepMC::GenVertex::particles_out_const_iterator 
            iPart = (*iVtx)->particles_out_const_begin(),
            donePart = (*iVtx)->particles_out_const_end();
        for (/*nop*/; donePart != iPart; ++iPart) {
            // Only keep particles that are important for tracking
            if ((*iPart)->status() != 1) continue;
            int pdgcode= (*iPart)-> pdg_id();
            const HepMC::FourVector& p = (*iPart)->momentum();

            // pdgid
            m_init_pdgid[m_init_nparticles] = pdgcode;
            // position and time
            m_init_x[m_init_nparticles] = v.x();
            m_init_y[m_init_nparticles] = v.y();
            m_init_z[m_init_nparticles] = v.z();
            m_init_time[m_init_nparticles] = v.t();
            // momentum
            m_init_px[m_init_nparticles] = p.px();
            m_init_py[m_init_nparticles] = p.py();
            m_init_pz[m_init_nparticles] = p.pz();
            // mass
            m_init_mass[m_init_nparticles] = p.m();
            m_init_kine[m_init_nparticles] = p.e()-p.m();

            ++m_init_nparticles;

            if (m_init_nparticles >= 50000) {
                LogWarn << "Skip the particles" << std::endl;
                return false;
            }


        }
    }

    // Loop over all particles
    LogInfo << "Loop over all particles" << std::endl;
    HepMC::GenEvent::particle_const_iterator
        iParticle = (*gep).particles_begin(),
        doneParticle = (*gep).particles_end();
    for ( ; doneParticle != iParticle; ++iParticle) {
        HepMC::GenParticle* part = (*iParticle);
        LogInfo << "Status: " << part->status()
                << std::endl;
    }
    // end loop

    m_evt_tree->Fill();
    //
    return true;
}

bool
PostGenTools::finalize()
{
    return true;
}
