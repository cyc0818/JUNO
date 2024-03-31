#include "SelectEventData.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Event/SimHeader.h"
#include "EvtNavigator/EvtNavigator.h"

#include "TFile.h"
#include "TTree.h"
#include "Event/DummyHeader.h"
#include "Event/DummyEvent.h"

#include <cstdlib>

DECLARE_ALGORITHM(SelectEventData);

SelectEventData::SelectEventData(const std::string& name)
    : AlgBase(name)
    , m_ratio(0.0)
    , m_seed(0)
    , m_limit(0)
    , m_iEvt(0)
    , m_buf(0)
    , m_header(0)
    , m_event(0)
    , m_nav(0)
    , m_headerTree(0)
    , m_eventTree(0)
    , m_navTree(0)
    , m_file(0)
    , m_plainheader(0)
    , m_plainevent(0)
{
    declProp("Ratio", m_ratio=1.0); 
    declProp("Seed", m_seed=42); 
    declProp("Mode", m_mode=1);
    declProp("InputFile", m_fileName);

}

SelectEventData::~SelectEventData()
{
}

bool SelectEventData::initialize()
{
    srand(m_seed);
    m_limit = m_ratio * 10000;

    if (m_mode == 0) {
        m_file = TFile::Open(m_fileName.c_str(), "read");
        m_headerTree = static_cast<TTree*>(m_file->Get("DummyHeader"));
        m_eventTree = static_cast<TTree*>(m_file->Get("DummyEvent"));
        m_headerTree->SetBranchAddress("Header", &m_plainheader);
        m_eventTree->SetBranchAddress("Event", &m_plainevent);
    }

    else if (m_mode == 1) {
        SniperDataPtr<JM::NavBuffer>  navBuf(*getParent(), "/Event");
        if ( navBuf.invalid() ) {
            LogError << "cannot get the NavBuffer @ /Event" << std::endl;
            return false;
        }
        m_buf = navBuf.data();
    }
    else {
        LogError << "Mode must be 0 (plain event data input) or 1 (event data model input)!" << std::endl;
        return false;
    }

    return true;
}

bool SelectEventData::execute()
{
    int random = rand() % 10000;

    if (0 == m_mode) {
        m_headerTree->GetEntry(m_iEvt);
        m_eventTree->GetEntry(m_iEvt);

        delete m_plainheader;
        delete m_plainevent;
        m_plainheader = 0;
        m_plainevent = 0;
        ++m_iEvt;
    }

    if (1 == m_mode) {
        m_nav = m_buf->curEvt();
        m_header = static_cast<JM::SimHeader*>(m_nav->getHeader("/Event/Sim"));

    
        if ( random < m_limit  ) {
            m_event = static_cast<JM::SimEvt*>(m_header->event());
        std::cout << m_event << std::endl;
        }
    }

    return true;
}

bool SelectEventData::finalize()
{
    return true;
}
