#include "Event/GenEvt.h"

ClassImp(JM::GenEvt)

namespace JM
{
    GenEvt::GenEvt() {
        evt = 0;
    }

    GenEvt::~GenEvt() {
        if (evt) {
            delete evt;
        }
    }
}
