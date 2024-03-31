/*
 * Description:
 *   Create dummy SimEvent
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn>
 *   May 31, 2022
 */

#include "SniperKernel/AlgBase.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"

#include "Event/SimHeader.h"
#include "Event/CdWaveformHeader.h"
#include "Event/CdLpmtElecHeader.h"
#include "Event/CdSpmtElecHeader.h"

#include <vector>

class DummyMCTruthCreatorAlg: public AlgBase {
public:
    DummyMCTruthCreatorAlg(const std::string& name)
        : AlgBase(name) {
        declProp("PDGs", m_pdgs={13, -13, 11, -11});
        declProp("SubDets", m_subdets={0,1,2});
    }

    ~DummyMCTruthCreatorAlg() {

    }

    bool initialize() {

        m_current_pdg_idx = 0;
        m_current_subdet_idx = 0;

        return true;
    }

    bool execute() {
        int pdgid = m_pdgs[m_current_pdg_idx];
        m_current_pdg_idx = (++m_current_pdg_idx)%m_pdgs.size();

        JM::EvtNavigator* nav = new JM::EvtNavigator();

        // == get the BufferMemMgr of I/O task ==
        SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
        mMgr->adopt(nav, "/Event");

        // == Detector Simulation ==
        auto simhdr = new JM::SimHeader();
        auto simevt = new JM::SimEvt();

        // generate multiple tracks
        int ntracks = std::max(1, m_current_pdg_idx);
        for (int itrack = 0; itrack<ntracks; ++itrack) {
            auto trk = simevt->addTrack();
            trk->setPDGID(pdgid);
        }

        simhdr->setEvent(simevt);
        JM::addHeaderObject(nav, simhdr);

        // == Electronics Simulation ==
        int subdet = m_subdets[m_current_subdet_idx];
        m_current_subdet_idx = (++m_current_subdet_idx)%m_subdets.size();
        
        if (subdet == 0) {
            auto wavehdr = new JM::CdWaveformHeader();
            JM::addHeaderObject(nav, wavehdr);
        } else if (subdet = 1) {
            auto cdlpmthdr = new JM::CdLpmtElecHeader();
            JM::addHeaderObject(nav, cdlpmthdr);
        } else if (subdet = 2) {
            auto cdspmthdr = new JM::CdSpmtElecHeader();
            JM::addHeaderObject(nav, cdspmthdr);
        }

        return true;
    }

    bool finalize() {
        return true;
    }

private:
    std::vector<int> m_pdgs;
    std::vector<int> m_subdets;

    int m_current_pdg_idx;
    int m_current_subdet_idx;
};

DECLARE_ALGORITHM(DummyMCTruthCreatorAlg);

