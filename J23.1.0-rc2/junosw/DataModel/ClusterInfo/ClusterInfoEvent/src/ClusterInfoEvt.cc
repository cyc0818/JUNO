#include"../Event/ClusterInfoEvt.h"
#include<utility>

ClassImp(JM::ClusterInfoEvt)

namespace JM
{

void ClusterInfoEvt::AddCluster()
{
    m_clusterVec.push_back(new JM::ClusterHitCol);
}


void ClusterInfoEvt::AddHitToCluster(UInt_t ChannelIdx, UInt_t HitIdx, UInt_t ClusterIdx, double hittime)
{
    if( m_clusterVec.size() >= ClusterIdx+1 ){
        (m_clusterVec[ClusterIdx])->AddHit(ChannelIdx, HitIdx, hittime); 
    }
    else{
        this->AddCluster();
        this->AddHitToCluster(ChannelIdx,HitIdx,ClusterIdx,hittime);
    }
}

ClusterInfoEvt::ClusterInfoEvt()
{
    m_eventid = 0;
}

ClusterInfoEvt::ClusterInfoEvt(Int_t evtid)
{
    m_eventid = evtid;
}

ClusterInfoEvt::~ClusterInfoEvt()
{

    for(std::vector< JM::ClusterHitCol*>::iterator it = m_clusterVec.begin();
        it != m_clusterVec.end();++it) {
        delete (*it);
    }
    m_clusterVec.clear();
}
}
