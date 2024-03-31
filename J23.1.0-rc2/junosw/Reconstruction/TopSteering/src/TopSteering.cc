#include "TopSteering.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/AlgFactory.h"

DECLARE_ALGORITHM(TopSteering); 

TopSteering::TopSteering(const std::string& name)
    :AlgBase(name),
     m_buf(nullptr),
     m_recPoint(nullptr),
     m_recTrack(nullptr),
     m_recWP(nullptr),
     m_recTT(nullptr)
{
    declProp("SteeringMap", m_taskMap);
}

TopSteering::~TopSteering()
{
}

bool TopSteering::initialize()
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
            {"CD_PointLike", "RecPoint"},
            {"CD_TrackLike", "RecTrack"},
            {"WP", "RecWP"},
            {"TT", "RecTT"},
        };
        m_taskMap.swap(defaultMap);
    }
    m_recPoint = new Incident(m_taskMap.at("CD_PointLike"));
    m_recTrack = new Incident(m_taskMap.at("CD_TrackLike"));
    m_recWP = new Incident(m_taskMap.at("WP"));
    m_recTT = new Incident(m_taskMap.at("TT"));

    return true; 
}

bool TopSteering::execute()
{
    JM::EvtNavigator* evt = m_buf->curEvt();

    switch(evt->getDetectorType()) {
        case(JM::EvtNavigator::DetectorType::CD):
            if (evt->isPointLike()) {
                m_recPoint->fire(*(this->getParent()));
            }
            else if (evt->isTrackLike()) {
                m_recTrack->fire(*(this->getParent()));
            }
            else {
                LogError << "Unknown event type: PointLike/TrackLike" << std::endl;
                return false;
            }
            break;
        case(JM::EvtNavigator::DetectorType::WP):
            m_recWP->fire(*(this->getParent()));
            break;
        case(JM::EvtNavigator::DetectorType::TT):
            m_recTT->fire(*(this->getParent()));
            break;
        default:
            LogError << "Unknown DetectorType" << std::endl;
            return false;
    }

    return true;
}

bool TopSteering::finalize()
{
    delete m_recPoint;
    delete m_recTrack;
    delete m_recWP;
    delete m_recTT;

    return true; 
}
