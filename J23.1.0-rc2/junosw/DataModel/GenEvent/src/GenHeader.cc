#include "Event/GenHeader.h"

ClassImp(JM::GenHeader)

namespace JM
{
    GenHeader::GenHeader() {

    }

    GenHeader::~GenHeader() {

    }

    void GenHeader::setEventEntry(const std::string& eventName, Long64_t& value) {
        if (eventName == "JM::GenEvt") {
            m_event.setEntry(value);
        }
    }

    EventObject* GenHeader::event(const std::string& eventName) {
        if (eventName == "JM::GenEvt") {
            return m_event.GetObject();
        }
        return 0;
    }

    bool GenHeader::hasEvent() {
        return m_event.HasObject();
    }
}
