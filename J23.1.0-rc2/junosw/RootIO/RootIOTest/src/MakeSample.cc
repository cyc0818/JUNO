#include "MakeSample.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperDataPtr.h"
#include "Event/SimHeader.h"
#include "Event/DummyEvent.h"
#include "Event/DummyHeader.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "RootIOSvc/RootOutputSvc.h"
#include <sstream>

DECLARE_ALGORITHM(MakeSample);

MakeSample::MakeSample(const std::string& name)
    : AlgBase(name)
    , m_iEvt(0)
    , m_curEvt(0)
    , m_header(0)
    , m_event(0)
{
    declProp("mode", m_mode=1);
    declProp("hitNum", m_hitNum);
    declProp("outputFile", m_outputFile);
    declProp("EventPerFile", m_eventPerFile);
}

MakeSample::~MakeSample()
{
}

bool MakeSample::initialize()
{
    if (0 == m_mode) {
        // Plain format
        m_file = TFile::Open(m_outputFile.c_str(), "recreate");
        m_headerTree = new TTree("DummyHeader", "Tree for Plain Header");
        m_eventTree = new TTree("DummyEvent", "Tree for Plain Event");
        m_headerTree->Branch("Header", "DummyHeader", &m_header);
        m_eventTree->Branch("Event", "DummyEvent", &m_event);
    }

    else if (1 == m_mode) {
        // EDM format
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

bool MakeSample::execute()
{
    ++m_iEvt;
    ++m_curEvt;
    int hitNum = (int)m_ran.Gaus(m_hitNum,20);
    LogDebug << "Executing: " << m_iEvt << std::endl;

    if (0 == m_mode) {
        m_header = new DummyHeader();
        m_event = new DummyEvent();

        for (int i = 0; i < hitNum; ++i) {
            DummyPMTHit* aHit = m_event->addCDHit();
            aHit->setPMTID(i);
            aHit->setNPE((int)m_ran.Gaus(3000,400));
            aHit->setHitTime(m_ran.Gaus(100,25));
            aHit->setTimeWindow(m_ran.Gaus(8,2));
        }

        m_headerTree->Fill();
        m_eventTree->Fill();
        delete m_header;
        delete m_event;
    }

    if (1 == m_mode) {
        if (m_curEvt == m_eventPerFile+1) {
            m_curEvt = 0;
            std::map<std::string,std::string> newMap;
            std::stringstream ss;
            ss << "output" << m_iEvt/m_eventPerFile << ".root";
            std::string filename = ss.str();
            newMap["/Event/Sim"] = filename;
            SniperPtr<RootOutputSvc> ptr_out_svc(*getParent(), "OutputSvc");
            ptr_out_svc->newFile(newMap);
        }
        JM::EvtNavigator* nav = new JM::EvtNavigator();
        static TTimeStamp time(2015, 9, 22, 12, 15, 2, 111);
        time.Add(TTimeStamp(0, abs((int)m_ran.Gaus(200000, 200000/5))));
        nav->setTimeStamp(time);

        JM::SimEvt* event = new JM::SimEvt();
        JM::SimHeader* header = new JM::SimHeader();
        header->setEvent(event);

        for (int i = 0; i < hitNum; ++i) {
            JM::SimPMTHit* aHit = event->addCDHit();
            aHit->setPMTID(i);
            aHit->setNPE((int)m_ran.Gaus(3000,400));
            aHit->setHitTime(m_ran.Gaus(100,25));
            aHit->setTimeWindow(m_ran.Gaus(8,2));
        }

        nav->addHeader("/Event/Sim", header);

        SniperPtr<IDataMemMgr> mMgr(*getParent(), "BufferMemMgr");
        mMgr->adopt(nav, "/Event"); 
    }

    return true;
}

bool MakeSample::finalize()
{
    if (0 == m_mode) {
        m_headerTree->Write(NULL,TObject::kOverwrite);
        m_eventTree->Write(NULL,TObject::kOverwrite);
        m_file->Close();
        delete m_file;
    }
    return true;
}

