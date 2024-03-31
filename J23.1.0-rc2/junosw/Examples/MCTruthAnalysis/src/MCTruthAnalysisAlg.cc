#include "MCTruthAnalysisAlg.h"
#include "SniperKernel/AlgFactory.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdWaveformHeader.h"
#include "Event/SimHeader.h"
#include "Event/GenHeader.h"

DECLARE_ALGORITHM(MCTruthAnalysisAlg);

MCTruthAnalysisAlg::MCTruthAnalysisAlg(const std::string& name)
    : AlgBase(name), m_buf(0) {

}

bool MCTruthAnalysisAlg::initialize() {
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return true;
    }
    m_buf = navBuf.data();

    return true;
}

bool MCTruthAnalysisAlg::execute() {

    // access current event
    auto nav = m_buf->curEvt();

    if (!nav) {
        LogError << "Can't load Event Navigator. " << std::endl;
        return false;
    }

    // Rec
    auto rechdr = JM::getHeaderObject<JM::CdVertexRecHeader>(nav);

    // Calib
    auto calibhdr = JM::getHeaderObject<JM::CdLpmtCalibHeader>(nav);

    // Elec+ElecTruth
    auto elechdr = JM::getHeaderObject<JM::CdWaveformHeader>(nav);

    // Merged Sim
    auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);

    // Merged Gen
    auto genhdr = JM::getHeaderObject<JM::GenHeader>(nav);

    // summary output
    LogInfo << "The current evtnav " << nav << " contains "
            << " rechdr: " << rechdr
            << " calibhdr: " << calibhdr
            << " elechdr: " << elechdr
            << " simhdr: " << simhdr
            << " genhdr: " << genhdr
            << std::endl;
    
    // 
    if (genhdr) {
        HepMC::GenEvent* hepmc_evt = 0;

        auto genevt = dynamic_cast<JM::GenEvt*>(genhdr->event());        
        if (genevt) {
            hepmc_evt = genevt->getEvent();
        }

        if (hepmc_evt) {
            // loop all the vertex and particles

            LogInfo << "--> Loop HepMC event: " 
                    << std::endl;

            // loop vertex
            HepMC::GenEvent::vertex_const_iterator
                iVtx = (*hepmc_evt).vertices_begin(),
                doneVtx = (*hepmc_evt).vertices_end();

            for (/*nop*/; doneVtx != iVtx; ++iVtx) {

                const HepMC::FourVector& v = (*iVtx)->position();

                LogInfo << "----> vertex: "
                        << v.x() << "," << v.y() << "," << v.z() << "," << v.t()
                        << std::endl;


                // loop particles
                HepMC::GenVertex::particles_out_const_iterator 
                    iPart = (*iVtx)->particles_out_const_begin(),
                    donePart = (*iVtx)->particles_out_const_end();
                for (/*nop*/; donePart != iPart; ++iPart) {
                    const HepMC::FourVector& p = (*iPart)->momentum();

                    LogInfo << "--------> particle: "
                            << (*iPart)->status() << " "
                            << (*iPart)->pdg_id() << " "
                            << "(" << p.px() << "," << p.py() << "," << p.pz() << ") "
                            << std::endl;

                }

            }
        }
    }

    return true;
}

bool MCTruthAnalysisAlg::finalize() {

    return true;
}
