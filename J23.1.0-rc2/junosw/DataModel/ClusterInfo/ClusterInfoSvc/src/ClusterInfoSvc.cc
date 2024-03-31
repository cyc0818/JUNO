#include "SniperKernel/SvcFactory.h"
//#include "SniperKernel/AlgBase.h"
#include "ClusterInfoSvc.h"
#include "Event/ClusterHitCol.h"
#include "Event/ClusterHitIdx.h"
#include <list>
#include "Event/CalibHeader.h"
#include "Event/CalibPMTChannel.h"

DECLARE_SERVICE(ClusterInfoSvc);

ClusterInfoSvc::ClusterInfoSvc(const std::string &name)
    :SvcBase(name)
    , m_calib(NULL)
    , m_cluster(NULL)
{
    // no properties to declare

}

ClusterInfoSvc::~ClusterInfoSvc()
{

}

bool ClusterInfoSvc::initialize()
{
    return true;
}

bool ClusterInfoSvc::finalize()
{
    return true;
}

bool ClusterInfoSvc::initEvent(JM::EvtNavigator* nav)
{
    m_nav = nav;
    return (readCalib() && readCluster());
}

UInt_t ClusterInfoSvc::getNCluster() const
{
    return m_cluster->getNCluster();
}

UInt_t ClusterInfoSvc::getClusterSize(UInt_t clusterIdx) const
{
    if(this->getNCluster()>clusterIdx){
        return m_cluster->getCluster(clusterIdx)->getSize();
    }
    else{
        //LogError << "Cluster number "<<clusterIdx<<" does not exist!\n";
        return -1;
    }
}

double ClusterInfoSvc::getCharge(UInt_t clusterIdx, UInt_t hitIdx)
{
    //Get channel and hit from the ClusterInfoEvt
    UInt_t channelIdx    = m_cluster->getCluster(clusterIdx)->getChannel(hitIdx);
    UInt_t channelHitIdx = m_cluster->getCluster(clusterIdx)->getHit(hitIdx);

    UInt_t chcnt = 0;
    //Now loop through CalibEvent
    const std::list<JM::CalibPMTChannel*>& chhlist = m_calib->event()->calibPMTCol();
    for(std::list<JM::CalibPMTChannel*>::const_iterator it = chhlist.begin();it!=chhlist.end();it++){
        if(chcnt == channelIdx){
            return (*it)->charge(channelHitIdx);
            break;
        }
        else{
            chcnt++;
        }
    }
    // not found, should not happen
    LogError << "Channel not found! \n";
    return -1;

}

double ClusterInfoSvc::getTime(UInt_t clusterIdx, UInt_t hitIdx){
    //Get channel and hit from the ClusterInfoEvt
    UInt_t channelIdx    = m_cluster->getCluster(clusterIdx)->getChannel(hitIdx);
    UInt_t channelHitIdx = m_cluster->getCluster(clusterIdx)->getHit(hitIdx);

    UInt_t chcnt = 0;
    //Now loop through CalibEvent
    const std::list<JM::CalibPMTChannel*>& chhlist = m_calib->event()->calibPMTCol();
    for(std::list<JM::CalibPMTChannel*>::const_iterator it = chhlist.begin();it!=chhlist.end();it++){
        if(chcnt == channelIdx){
            return (*it)->time(channelHitIdx);
            break;
        }
        else{
            chcnt++;
        }
    }
    // not found, should not happen
    LogError << "Channel not found! \n";
    return -1;

}


UInt_t ClusterInfoSvc::getPMTID(UInt_t clusterIdx, UInt_t hitIdx){

    //Get channel and hit from the ClusterInfoEvt
    UInt_t channelIdx    = m_cluster->getCluster(clusterIdx)->getChannel(hitIdx);

    UInt_t chcnt = 0;
    //Now loop through CalibEvent
    const std::list<JM::CalibPMTChannel*>& chhlist = m_calib->event()->calibPMTCol();
    for(std::list<JM::CalibPMTChannel*>::const_iterator it = chhlist.begin();it!=chhlist.end();it++){
        if(chcnt == channelIdx){
            return (*it)->pmtId();
            break;
        }
        else{
            chcnt++;
        }
    } 
    // not found, should not happen
    LogError << "Channel not found! \n";
    return -1;
}


bool ClusterInfoSvc::readCalib(){
    bool retval = true;
    m_calib = dynamic_cast<JM::CalibHeader*>(m_nav->getHeader("/Event/Calib"));
    LogDebug << "calibhdr: " << m_calib << std::endl;
    if (not m_calib) {
        retval = false;
    }
    return retval;
}

bool ClusterInfoSvc::readCluster(){
    bool retval = true;
    JM::ClusterInfoHeader* tmpclusterhead = NULL; 
    tmpclusterhead = dynamic_cast<JM::ClusterInfoHeader*>(m_nav->getHeader("/Event/Cluster"));
    LogDebug << "clusterinfohdr: " << tmpclusterhead << std::endl;
    if (not tmpclusterhead) {
        retval = false;
    }
    m_cluster = dynamic_cast<JM::ClusterInfoEvt*>(tmpclusterhead->event());
    LogDebug << "clusterinfoevent: " << m_cluster << std::endl;
    if (not m_cluster) {
        retval = false;
    }

    return retval;
}

double ClusterInfoSvc::getClusterStart(UInt_t clusterIdx)
{
    double clusterStart = m_cluster->getCluster(clusterIdx)->getStartTime();
    double clusterEnd   = m_cluster->getCluster(clusterIdx)->getEndTime();
    if(clusterEnd>clusterStart){
        return clusterStart;
    }
    else{
        LogWarn << "Cluster Start and End are not properly filled!\n";
        return -1.;
    }
}

double ClusterInfoSvc::getClusterEnd(UInt_t clusterIdx)
{
    double clusterStart = m_cluster->getCluster(clusterIdx)->getStartTime();
    double clusterEnd   = m_cluster->getCluster(clusterIdx)->getEndTime();
    if(clusterEnd>clusterStart){
        return clusterEnd;
    }
    else{
        LogWarn << "Cluster Start and End are not properly filled!\n";
        return -1.;
    }
    
}

double ClusterInfoSvc::getClusterLength(UInt_t clusterIdx)
{
    double clusterLength = m_cluster->getCluster(clusterIdx)->getLength();
    if(clusterLength>0.){
        return clusterLength;
    }
    else{
        LogWarn << "Cluster Start and End are not properly filled!\n";
        return -1.;
    }
}
