#ifndef GenEvent_h
#define GenEvent_h

#include "Event/EventObject.h"
#include "HepMC/GenEvent.h"

namespace JM
{
    class GenEvt: public EventObject
    {
        private:
            HepMC::GenEvent* evt;

        public:

            GenEvt();
            ~GenEvt();

            void setEvent(HepMC::GenEvent* val) {
                evt = val;
            }
            HepMC::GenEvent* getEvent() {
                return evt;
            }

            ClassDef(GenEvt,2)

    };
}

#endif
