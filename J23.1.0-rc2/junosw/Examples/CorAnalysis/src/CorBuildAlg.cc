#include "CorBuildAlg.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Task.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "InputReviser/InputReviser.h"
#include "Event/CdVertexRecHeader.h"
#include "Event/CdLpmtCalibHeader.h"
#include "Event/CdWaveformHeader.h"
#include "Event/SimHeader.h"
#include <boost/shared_ptr.hpp>
#include <TRandom.h>
DECLARE_ALGORITHM(CorBuildAlg);

CorBuildAlg::CorBuildAlg(const std::string& name)
    : AlgBase(name),
      m_buf(0)
{
    declProp("taskMap", m_taskmap);
    declProp("randomMode", m_task2random);
    // {IBD: mode} random mode selected
    // mode description:
    // - 0: don't choose event randomly
    // - 1: first time is randomly selected
    // - 2: randomly every time
    // default is 0.
}

bool
CorBuildAlg::initialize() {
    LogDebug << "initializing" << std::endl;

    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    Task* toptask = dynamic_cast<Task*>(getRoot());

    for(std::map<std::string,std::string>::iterator it=m_taskmap.begin();it!=m_taskmap.end();it++){
        bool m_loop = true;
        InputReviser* aInci = new InputReviser(*toptask, it->second, m_loop);  //first is task nam 
      
        m_incidentMap.insert(make_pair(it->first, aInci));//first is sample name，second is the InputReviser which related to the sample name. In test I use sample as task name.

    }


    return true;
}

bool
CorBuildAlg::execute() {

    // Load SimEvents from different tasks
    std::vector< std::shared_ptr<JM::EvtNavigator> > evtnavs;
    for(std::map<std::string,InputReviser*>::iterator it=m_incidentMap.begin();
        it!=m_incidentMap.end();it++){

        int entries = it->second->getEntries();

        if (m_task2random[it->first]==1) {
            int num = gRandom->Integer(entries-1);
            LogInfo << " reset to: " << num << "/" << entries << std::endl;
            it->second->reset(num);
            // just set mode=1 to mode=0, so the next time won't randomly select
            m_task2random[it->first] = 0;
            LogInfo << "Only random for the first time." << std::endl;
        }


        it->second->fire(*dynamic_cast<Task*>(getRoot()));


        int num = it->second->getEntry();
        LogInfo<<"-> selected sample: "<<it->first
               <<", sample entry: "
               <<num << "/" <<entries
               <<std::endl;


        std::string edm_path = m_taskmap[it->first] + ":/Event";
        SniperDataPtr<JM::NavBuffer> navBuf(*getRoot(), edm_path); 
        if (navBuf.invalid()) {
            LogError << "Can't locate data: " << edm_path << std::endl;
            return false;
        }
        evtnavs.push_back(*navBuf->current());
    }

    // Create ElecEvents with related to SimEvents
    auto nav = new JM::EvtNavigator();
    // FIXME: use the real time stamp
    static TTimeStamp time(2014, 7, 29, 10, 10, 2, 111);
    time.Add(TTimeStamp(0, 10000));
    nav->setTimeStamp(time);

    SniperPtr<IDataMemMgr> mMgr(getParent(), "BufferMemMgr");
    mMgr->adopt(nav, "/Event");

    // ElecEvent
    auto elec_hdr = new JM::CdWaveformHeader;
    auto elec_evt = new JM::CdWaveformEvt;
    elec_hdr->setEvent(elec_evt);
    JM::addHeaderObject(nav, elec_hdr);

    for (std::vector< std::shared_ptr<JM::EvtNavigator> >::iterator it = evtnavs.begin(); it != evtnavs.end(); ++it) {
        nav->copyHeader(it->get(), "/Event/Sim", "/Event/Sim");
    }

    return true;
}

bool
CorBuildAlg::finalize() {
    return true;
}

