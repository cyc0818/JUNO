#include "MuSteering.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/AlgFactory.h"

DECLARE_ALGORITHM(MuSteering); 

MuSteering::MuSteering(const std::string& name)
    :AlgBase(name),
     m_buf(nullptr),
     m_recMuSingle(nullptr),
     m_recMuBundle(nullptr),
     m_task(nullptr)
{
    declProp("SteeringMap", m_taskMap);
    declProp("IsInTopTask", m_isInTopTask=false);
}

MuSteering::~MuSteering()
{
}

bool MuSteering::initialize()
{
    //Event navigator
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if ( navBuf.invalid() ) {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    //create incidents for SubTasks
    if (m_taskMap.empty()) {
        std::map<std::string, std::string> defaultMap{
            {"CD_MuSingleLike", "RecMuSingle"},
            {"CD_MuBundleLike", "RecMuBundle"},
        };
        m_taskMap.swap(defaultMap);
    }
    m_recMuSingle = new Incident(m_taskMap.at("CD_MuSingleLike"));
    m_recMuBundle = new Incident(m_taskMap.at("CD_MuBundleLike"));

    // get the top or parent task
    if (m_isInTopTask) {
        m_task = this->getRoot();
    } else {
        m_task = this->getParent();
    }

    return true; 
}

bool MuSteering::execute()
{
    JM::EvtNavigator* evt = m_buf->curEvt();

    int count = 0;

    switch(evt->getMuonType()) {
        case(JM::EvtNavigator::MuonType::single):
            count = m_recMuSingle->fire(*m_task);
            LogInfo << "Invoke RecMuSingle sub-task. Return count: " << count << std::endl;
            break;
        case(JM::EvtNavigator::MuonType::bundle):
            count = m_recMuBundle->fire(*m_task);
            LogInfo << "Invoke RecMuBundle sub-task. Return count: " << count << std::endl;
            break;
        default:
            LogError << "Unknown CD Muon Type" << std::endl;
            return true;
    }

    return true;
}

bool MuSteering::finalize()
{
    delete m_recMuSingle;
    delete m_recMuBundle;

    return true; 
}
