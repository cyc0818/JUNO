#include"../Event/ClusterHitCol.h"
#include<Rtypes.h>
#include<vector>

ClassImp(JM::ClusterHitCol)

namespace JM
{

ClusterHitCol::ClusterHitCol()
{
    m_clusterCol = std::vector<JM::ClusterHitIdx>();
    m_start  = 1.e10;
    m_end  = -1.e10;
}

ClusterHitCol::~ClusterHitCol()
{

}

void ClusterHitCol::AddHit(UInt_t ChannelIdx, UInt_t HitIdx, double hittime)
{
    ClusterHitIdx myhit;
    myhit.channel = ChannelIdx;
    myhit.hit = HitIdx;
    m_clusterCol.push_back(myhit);
 
    if(hittime < m_start){
        m_start = hittime;
    }
    else if(hittime > m_end){
        m_end = hittime;
    }
}

UInt_t ClusterHitCol::getSize() const
{
    return m_clusterCol.size();
}

UInt_t ClusterHitCol::getChannel(UInt_t hitIdx) const
{
    return m_clusterCol.at(hitIdx).channel;
}

UInt_t ClusterHitCol::getHit(UInt_t hitIdx) const
{
    return m_clusterCol.at(hitIdx).hit;
}

double ClusterHitCol::getStartTime() const
{   
    return m_start;
}

double ClusterHitCol::getEndTime() const
{
    return m_end;
}

double ClusterHitCol::getLength() const
{
    if(m_end > m_start){
        return m_end-m_start;
    }
    else
    {
        return -1.;
    }
}

}
