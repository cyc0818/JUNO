#include "OECProcessor/OECBuffer.h"
#include "TTimeStamp.h" 
#include <float.h>

#include <iostream>
using JM::EvtNavigator;

OECBuffer::OECBuffer(double floor, double ceiling): d_floor(floor), d_ceiling(ceiling){
}

OECBuffer::~OECBuffer(){
}

//OECBuffer::rIterator OECBuffer::rbegin(){
//    return m_dBuf.rbegin();
//}
//
//OECBuffer::rIterator OECBuffer::rend(){
//    return m_dBuf.rend();
//}
//
//OECBuffer::rIterator OECBuffer::rcurrent(){
//    std::deque<std::shared_ptr<JM::EvtNavigator>>::reverse_iterator it(m_dBuf.begin()+m_iCur);
//    return it;
//}

bool OECBuffer::next(){
    if (++m_iCur>=int(m_dBuf.size())){
        std::cout<<"WARN:current event exceeds the buffer range!"<<std::endl;
        return false;
    }
    if (!updateFloor()) return false;
    return true;
}

bool OECBuffer::adopt(ElementPtr& nav){
    m_dBuf.push_back(nav);
    //if (!updateFloor()) return false;
    return true;
}

double OECBuffer::t2Floor(int no){
    int cur=m_iCur+no;
    if (cur<=0) return 0;
    if (cur>int(m_dBuf.size()-1)) return DBL_MAX;
    JM::EvtNavigator *fevt=m_dBuf.front().get();
    const TTimeStamp& t0=m_dBuf[cur]->TimeStamp();
    TTimeStamp ft=fevt->TimeStamp();
    ft.Add(TTimeStamp(-t0.GetSec(),-t0.GetNanoSec()));

    return -1*ft.AsDouble();
}

double OECBuffer::t2Ceiling(int no){
    int cur=m_iCur+no;
    if (cur<0) return DBL_MAX;
    if (cur>=int(m_dBuf.size()-1)) return 0;
    JM::EvtNavigator *cevt=m_dBuf.back().get();
    const TTimeStamp& t0=m_dBuf[cur]->TimeStamp();
    TTimeStamp ct=cevt->TimeStamp();
    ct.Add(TTimeStamp(-t0.GetSec(),-t0.GetNanoSec()));

    return ct.AsDouble();
}

bool OECBuffer::updateFloor(){
    JM::EvtNavigator *fevt=m_dBuf.front().get();
    while (fevt!=m_dBuf[m_iCur].get()){
        const TTimeStamp& t0=m_dBuf[m_iCur]->TimeStamp();
        TTimeStamp ft=fevt->TimeStamp();
        ft.Add(TTimeStamp(-t0.GetSec(),-t0.GetNanoSec()));
        double dt=-1*ft.AsDouble();
        if (d_floor<0.0 && dt<-1*d_floor){
            break;
        }
        m_tmpBuf.push_back(m_dBuf.front());
        m_dBuf.pop_front();
        m_iCur--;
        fevt=m_dBuf.front().get();
    }
    return true;
}

std::shared_ptr<JM::EvtNavigator> OECBuffer::realPop(){
    std::shared_ptr<JM::EvtNavigator> tmp;
    if (m_tmpBuf.size()!=0){
        tmp=m_tmpBuf.front();
        m_tmpBuf.pop_front();
    }
    return tmp;
}

void OECBuffer::popAll(){
    while (m_iCur>0){
        m_tmpBuf.push_back(m_dBuf.front());
        m_dBuf.pop_front();
        m_iCur--;
    }
}
