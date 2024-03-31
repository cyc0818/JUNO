#include "JVisLib/JVisEvtMgr.h"
#include "JVisLib/JVisOpMgr.h"
#include "Identifier/Identifier.h"
#include "Event/SimEvt.h"
#include "Event/CdLpmtCalibEvt.h"
#include "Event/CdVertexRecHeader.h"

// ROOT 
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

// C/C++
#include <iostream>

using namespace std;

JVisEvtMgr::JVisEvtMgr()
    : m_hasSim(false)
    , m_hasCalib(false)
    , m_hasRec(false)
    , m_hasSimus(false)
    , m_verb(99)
    , totalEvent(0)
{

}

JVisEvtMgr::~JVisEvtMgr()
{

}

bool JVisEvtMgr::initialize(TString simFileName, TString calibFileName, TString recFileName, TString simusFileName)
{
    if ( m_verb > 1 ) std::cout << "  init JVisEvtMgr " << std::endl;

    // SimEvent
    if ( m_verb > 1 ) std::cout << "name " << simFileName << std::endl;

    if ( !gSystem->AccessPathName(simFileName.Data()) ) {
        m_hasSim = true;
        m_simFile = new TFile(simFileName);
        if ( m_verb > 0 ) std::cout << "Open m_simFile " << m_simFile->GetName() << std::endl;
    }
    else {
        std::cout << "Can not open m_simFile " << simFileName << std::endl;
    }

    if ( m_hasSim ) {
        m_simEventTree = (TTree*)m_simFile->Get("Event/Sim/SimEvt");
        if ( m_simEventTree ) {
            if ( m_verb > 1 ) {
                std::cout << "Open m_simEventTree " << m_simEventTree->GetName()
                    << " entries: " << m_simEventTree->GetEntries() << std::endl;
            }
            m_se = new JM::SimEvt();
            m_simEventTree->SetBranchAddress("SimEvt", &m_se);
            m_simEventTree->GetBranch("SimEvt")->SetAutoDelete(true);
        }
        else {
            m_hasSim = false;
            std::cout << "Can not open m_simEventTree " << std::endl;
        }
    }

    // CalibEvent
    if ( m_verb > 1 ) std::cout << "name " << calibFileName << std::endl;

    if ( !gSystem->AccessPathName(calibFileName.Data()) ) {
        m_hasCalib = true;
        m_calibFile = new TFile(calibFileName);
        if ( m_verb > 0 ) std::cout << "Open m_calibFile " << m_calibFile->GetName() << std::endl;
    }
    else {
        std::cout << "Can not open m_calibFile " << calibFileName << std::endl;
    }

    if ( m_hasCalib ) {
        m_calibEventTree = (TTree*)m_calibFile->Get("Event/CdLpmtCalib/CdLpmtCalibEvt");
        if ( m_calibEventTree ) {
            if ( m_verb > 1 ) {
                std::cout << "Open m_calibEventTree " << m_calibEventTree->GetName()
                    << " entries: " << m_calibEventTree->GetEntries() << std::endl;
            }
            m_ce = new JM::CdLpmtCalibEvt();
            m_calibEventTree->SetBranchAddress("CdLpmtCalibEvt", &m_ce);
        }
        else {
            m_hasCalib = false;
            std::cout << "Can not open m_calibEventTree " << std::endl;
        }
    }

    // CDRecEvent
    if ( m_verb > 1 ) std::cout << "name " << recFileName << std::endl;

    if ( !gSystem->AccessPathName(recFileName.Data()) ) {
        m_hasRec = true;
        m_recFile = new TFile(recFileName);
        if ( m_verb > 0 ) std::cout << "Open m_recFile " << m_recFile->GetName() << std::endl;
    }
    else {
        std::cout << "Can not open m_recFile " << recFileName << std::endl;
    }

    if ( m_hasRec ) {
        m_recEventTree = (TTree*)m_recFile->Get("Event/CdVertexRec/CdVertexRecEvt");
        m_re = new JM::CdVertexRecEvt();
        m_recEventTree->SetBranchAddress("CdVertexRecEvt", &m_re);
        if ( m_recEventTree ) {
            if ( m_verb > 1 ) {
                std::cout << "Open m_recEventTree " << m_recEventTree->GetName() 
                    << " entries: " << m_recEventTree->GetEntries() << std::endl;
                totalEvent = m_recEventTree->GetEntries();
            }
        }
        else {
            std::cout << "Can not open m_recEventTree " << std::endl;
        }
    }

    // SimUserEvent
    if ( m_verb > 1 ) std::cout << "name " << simusFileName << std::endl;
    
    if ( !gSystem->AccessPathName(simusFileName.Data()) ) {
        m_hasSimus = true;
        m_simusFile = new TFile(simusFileName);
        if ( m_verb > 0 ) std::cout << "Open m_simusFile " << m_simusFile->GetName() << std::endl;
    }
    else {
        std::cout << "Can not open m_simusFile " << simusFileName << std::endl;
    }
    
    if ( m_hasSimus ) {
        m_simusOpStepsTree = (TTree*)m_simusFile->Get("opsteps");
        if ( m_simusOpStepsTree ) {
            if ( m_verb > 1 ) {
                std::cout << "Open m_simusOpStepsTree " << m_simusOpStepsTree->GetName()
                    << " entries: " << m_simusOpStepsTree->GetEntries() << std::endl;
            }
            m_opMgr = new JVisOpMgr();
            m_opMgr->setOpStepsTree( m_simusOpStepsTree );
            m_opMgr->init();
        }
        else {
            m_hasSimus = false;
            std::cout << __func__ << "m_simusOpStepsTree does not exist" << std::endl;
        }
    }


    if ( m_verb > 0 ) {
        std::cout << "hasSim   " << hasSim()   << std::endl;
        std::cout << "hasCalib " << hasCalib() << std::endl;
        std::cout << "hasRec   " << hasRec()   << std::endl;
        std::cout << "hasSimus " << hasSimus() << std::endl;
    }

    if ( m_verb > 1 ) std::cout << __func__ << " JVisEvtMgr successfully " << std::endl;

    return true;
}

bool JVisEvtMgr::checkEvt(int iEvt)
{
    if ( iEvt < 0 ) return false;

    if ( m_hasSim && iEvt >= m_simEventTree->GetEntries() ) {
        std::cout << "checkEvt " << iEvt << " out of range of SimTree " << m_simEventTree->GetName() << std::endl;
        return false;
    }

    if ( m_hasCalib && iEvt >= m_calibEventTree->GetEntries() ) {
        std::cout << "checkEvt " << iEvt << " out of range of CalibTree " << m_calibEventTree->GetName() << std::endl;
        return false;
    }

    if ( m_hasRec && iEvt >= m_recEventTree->GetEntries() ) {
        std::cout << "checkEvt " << iEvt << " out of range of RecTree " << m_recEventTree->GetName() << std::endl;
        return false;
    }

    if ( m_hasSimus && iEvt >= m_simusOpStepsTree->GetEntries() ) {
        std::cout << "checkEvt " << iEvt << " out of range of SimusOpStepsTree " << m_simusOpStepsTree->GetName() << std::endl;
        return false;
    }

    return true; 
}

bool JVisEvtMgr::readEvt(int iEvt)
{
    bool status = checkEvt(iEvt);
    if ( !status ) {
        return false;
    }
    if ( m_verb > 1 ) std::cout << "readEvt " << iEvt << std::endl;

    //read SimEvent data
    if ( m_hasSim ) {
        m_simEventTree->GetEntry(iEvt);
        if ( 0 == m_se ) {
            std::cout << "Failed to get SimEvent" << std::endl;
            return false;
        }
        if ( m_verb > 1 ) {
            std::cout << "  SimEvent SimTrack  " << m_se->getTracksVec().size() << std::endl;
            std::cout << "  SimEvent SimPMTHit " << m_se->getCDHitsVec().size() << std::endl;
        }
    }

    //read CalibEvent data
    if ( m_hasCalib ) {
        m_calibEventTree->GetEntry(iEvt);
        if (0 == m_ce) {
            std::cout << "Failed to get CalibEvent" << std::endl;
            return false;
        }
        if ( m_verb > 1 ) {
            std::cout << "  CalibEvent calibPMT " << m_ce->calibPMTCol().size() << std::endl;
        }
    }

    //read CDRecEvent data
    if ( m_hasRec ) {
        m_recEventTree->GetEntry(iEvt);
        if (0 == m_re) {
            std::cout << "Failed to get CDRecEvent" << std::endl;
            return false;
        }
        auto rec_vertex = m_re->getVertex(0); // get the first vertex
        if ( m_verb > 1 ) {
            std::cout << "  CDRecEvent peSum " << rec_vertex->peSum() << " energy " << rec_vertex->energy() << std::endl;
            std::cout << "  CDRecEvent vtx (" << rec_vertex->x() << ", " << rec_vertex->y() << ", " << rec_vertex->z() << ")" << std::endl;
        }
    }

    //read SimUserEvent data
    if ( m_hasSimus ) {
        if ( m_verb > 1 ) {
            std::cout << "  OpMgr readEvt(" << iEvt << std::endl;
        }
        bool opMgrStatus = m_opMgr->readEvt(iEvt);
        if ( opMgrStatus ) {
            if ( m_verb > 1 ) {
                m_opMgr->printCurEvt();
            }
        }
        else {
            std::cout << "  Not Entry " << iEvt << " in m_opMgr" << std::endl;
        }
    }

    return true;
}

TString JVisEvtMgr::getEvtInfoText()
{
    if (m_verb > 2) std::cout << __func__ << std::endl;

    TString t("");
    if (0 != m_se) {
        const int simEventID = m_se->getEventID();
        const int simNTracks = m_se->getTracksVec().size();
        const int simNCDHits = m_se->getCDHitsVec().size();
        t += Form("SimEvent:%d \nnTracks=%d nCDHits=%d \n", simEventID, simNTracks, simNCDHits);

        const auto& stc = m_se->getTracksVec();
        for (int ist = 0; ist < simNTracks; ist++ ) {
            auto st = stc[ist];
            t += Form("Trk%d pdg=%d Edep=%5.3fMeV \n", ist, st->getPDGID(), st->getEdep());
        }
    }

    if (0 != m_re) {
        auto rec_vertex = m_re->getVertex(0);
        t += Form("CDRecEvent: \npeSum=%3.0f energy=%5.1fMeV \n", rec_vertex->peSum(), rec_vertex->energy());
        t += Form("RecVertex (%3.0f, %3.0f, %3.0f)mm \n", rec_vertex->x(), rec_vertex->y(), rec_vertex->z());
    }

    return t;
}

void JVisEvtMgr::closeRootFile()
{
    if (m_simFile != NULL) m_simFile->Close();
    if (m_calibFile != NULL) m_calibFile->Close();
    if (m_recFile != NULL) m_recFile->Close();
    if (m_simusFile != NULL) m_simFile->Close();
}

bool JVisEvtMgr::finalize()
{
    std::cout << " finalized successfully" << std::endl;

    return true;
}

