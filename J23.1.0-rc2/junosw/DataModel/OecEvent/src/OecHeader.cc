#include "Event/OecHeader.h"

ClassImp(JM::OecHeader)

namespace JM{
    OecHeader::OecHeader(){
    }

    OecHeader::~OecHeader(){
    }

    EventObject* OecHeader::event(const string &value){
        if (value=="JM::OecEvt"){
            return m_event.GetObject();
        }
        return NULL;
    }
    void OecHeader::setEventEntry(const string &eventName, Long64_t &value){
        if (eventName=="JM::OecEvt"){
            m_event.setEntry(value);
        }
    }
}
