#include"Event/ClusterInfoHeader.h"

ClassImp(JM::ClusterInfoHeader)

namespace JM
{

    ClusterInfoHeader::ClusterInfoHeader() {

    }

    ClusterInfoHeader::~ClusterInfoHeader(){

    }


    void ClusterInfoHeader::setEventEntry(const std::string& eventName, Long64_t& value) {
        if (eventName == "JM::ClusterInfoEvt") {
            m_event.setEntry(value);
        }
    }

    EventObject* ClusterInfoHeader::event(const std::string& eventName) {
        if (eventName == "JM::ClusterInfoEvt") {
            return m_event.GetObject();
        }
        return 0;
    }

    
    bool ClusterInfoHeader::hasEvent(){
        return m_event.HasObject();
    }    
}
