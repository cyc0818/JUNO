#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "IADSvc/DataExchangeSvc.h"
#include <unistd.h>

DECLARE_SERVICE(DataExchangeSvc);

DataExchangeSvc::DataExchangeSvc(const std::string& name): SvcBase(name), m_isEmpty(true), m_incident("FilterTask")
{

}

bool DataExchangeSvc::initialize(){
    m_topTask = m_par->getRoot();
    return true;
}

void DataExchangeSvc::setEvt(const IADEvtBox& evt){
    if(!m_isEmpty){
        LogError<<"One evt remain"<<std::endl;
    }

    m_isEmpty = false;
    
    m_evt = evt;
}

const IADEvtBox& DataExchangeSvc::getEvt(){
    if(m_isEmpty){
        LogError<<"No evt to get"<<std::endl;
    }

    m_isEmpty = true;
    return m_evt;
}

bool DataExchangeSvc::filt(){
    
    while(this->isEmpty()){
        m_incident.fire(*m_topTask);
    }
    return true;
}