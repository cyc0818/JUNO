
#include <vector>
#include <algorithm>

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"

// EDM related
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

// GenEvt
#include "Event/GenHeader.h"
// SimEvt
#include "Event/SimHeader.h"

class DumpEDMAlg: public AlgBase {

public:
    DumpEDMAlg(const std::string& name)
        : AlgBase(name) {
        declProp("Entries", m_entries);
        declProp("ShowSimVertex", m_show_simvertex=true);
    }

    ~DumpEDMAlg() {

    }

    bool initialize() {

        if (m_entries.size()) {
            std::sort(m_entries.begin(), m_entries.end());
        }

        return true;
    }

    bool execute() {

        SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event"); 
        if (navBuf.invalid()) {
            LogError << "Can't locate data: /Event" << std::endl;
            return false;
        }
        if (!navBuf->size()) {
            LogWarn << "Can't load more events. " << std::endl;
            return false;
        }

        if (!navBuf->curEvt()) {
            LogWarn << "Can't load more events. "  << std::endl;
            return false;
        }

        bool need_to_print = false;

        if (m_entries.size() == 0) {
            need_to_print = true;
        } else if (std::find(m_entries.begin(), m_entries.end(), m_cur_entry) != m_entries.end()) {
            need_to_print = true;
        }

        // before return, need to update the current entry
        int entry = m_cur_entry;
        ++m_cur_entry;

        if (not need_to_print) {
            return true;
        }

        auto nav = navBuf->curEvt();
        const auto& paths = nav->getPath();

        // ===================================================================
        // COMMON BLOCK
        // ===================================================================
        LogInfo << std::endl;
        LogInfo << "========================================" << std::endl;
        LogInfo << "Entry: " << entry << " contains following paths:" << std::endl;
        for (auto& p: paths) {
            // skip if no header
            if (not nav->getHeader(p)) {
                continue;
            }
            LogInfo << " - path: " << p << " " << nav->getHeader(p) << std::endl;
        }
        LogInfo << "========================================" << std::endl;

        // ===================================================================
        // GENEVT
        // ===================================================================
        auto genhdr = JM::getHeaderObject<JM::GenHeader>(nav);
        if (genhdr) {
            auto genevt = dynamic_cast<JM::GenEvt*>(genhdr->event());
            if (genevt) helper_genevt(genevt);
        }

        // ===================================================================
        // SIMEVT
        // ===================================================================
        auto simhdr = JM::getHeaderObject<JM::SimHeader>(nav);
        if (simhdr) {
            auto simevt = dynamic_cast<JM::SimEvt*>(simhdr->event());
            helper_simevt(simevt);
        }

        LogInfo << std::endl;

        return true;
    }

    bool finalize() {
        return true;
    }

private:

    // -----------------------------------------------------------------------
    // HELPERS
    // -----------------------------------------------------------------------

    void helper_genevt(JM::GenEvt* genevt) {
        auto hepmc_genevt = genevt->getEvent();
        if (not hepmc_genevt) {
            return;
        }

        // loop all vertices
        for (auto iVtx = hepmc_genevt->vertices_begin();
             iVtx != hepmc_genevt->vertices_end();
             ++iVtx) {

            const auto& vertex_pos = (*iVtx)->position();
            LogInfo << " - GenVertex: "
                    << " @ ("
                    << vertex_pos.x() << ","
                    << vertex_pos.y() << ","
                    << vertex_pos.z() << ","
                    << vertex_pos.t() << ")"
                    << std::endl;

            // loop all particles
            for (auto iPart = (*iVtx)->particles_in_const_begin(); 
                 iPart != (*iVtx)->particles_in_const_end();
                 ++iPart) {
                const auto& part_momentum = (*iPart)->momentum();

                LogInfo << "  - [IN] GenParticle: "
                        << " status: " << (*iPart)->status()
                        << " pdg: " << (*iPart)->pdg_id()
                        << " momentum: (" 
                        << part_momentum.px() << ","
                        << part_momentum.py() << ","
                        << part_momentum.pz() << ","
                        << part_momentum.e() << ")"
                        << std::endl;
            }


            for (auto iPart = (*iVtx)->particles_out_const_begin(); 
                 iPart != (*iVtx)->particles_out_const_end();
                 ++iPart) {
                const auto& part_momentum = (*iPart)->momentum();

                LogInfo << "  - [OUT] GenParticle: "
                        << " status: " << (*iPart)->status()
                        << " pdg: " << (*iPart)->pdg_id()
                        << " momentum: (" 
                        << part_momentum.px() << ","
                        << part_momentum.py() << ","
                        << part_momentum.pz() << ","
                        << part_momentum.e() << ")"
                        << std::endl;
            }


        }
    }

    void helper_simevt(JM::SimEvt* simevt) {
        auto& tracks = simevt->getTracksVec();
        auto& vertices = simevt->getVerticesVec();

        // TODO:
        // part I: tracks

        // part II: vertices
        // if don't need to show vertex, just return
        if (not m_show_simvertex) {
            return;
        }

        for (auto vertex: vertices) {
            auto& idx_trks_in = vertex->getTrackIdxIn();
            auto& idx_trks_out = vertex->getTrackIdxOut();
            LogInfo << " - Vertex : "
                    << vertex->getProcName()
                    << " @ (" 
                    << vertex->getX() << "," 
                    << vertex->getY() << "," 
                    << vertex->getZ() << ","
                    << vertex->getT() << ")"
                    << std::endl;
            // input
            for (auto idx: idx_trks_in) {
                LogInfo << "   - input track: " 
                        << " idx:" <<  idx << " " 
                        << " pdgid:" << tracks[idx]->getPDGID() << " "
                        << " trackid:" << tracks[idx]->getTrackID() << " "
                        << " parentid:" << tracks[idx]->getParentID() << " "
                        << " ancestorid:" << tracks[idx]->getAncestorID() << " "
                        << " Qedep:" << tracks[idx]->getQEdep() << " "
                        << std::endl;
            }
            // output
            for (auto idx: idx_trks_out) {
                LogInfo << "   - output track: " 
                        << " idx:" <<  idx << " " 
                        << " pdgid:" << tracks[idx]->getPDGID() << " "
                        << " trackid:" << tracks[idx]->getTrackID() << " "
                        << " parentid:" << tracks[idx]->getParentID() << " "
                        << " ancestorid:" << tracks[idx]->getAncestorID() << " "
                        << " Qedep:" << tracks[idx]->getQEdep() << " "
                        << std::endl;
            }
        }


    }

private:

    std::vector<int> m_entries;

    bool m_show_simvertex;

private:
    int m_cur_entry = 0;

};

DECLARE_ALGORITHM(DumpEDMAlg);
