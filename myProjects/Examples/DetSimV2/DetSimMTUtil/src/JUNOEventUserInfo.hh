#ifndef JUNOEventUserInfo_hh
#define JUNOEventUserInfo_hh

#include "G4VUserEventInformation.hh"

#include "SniperMuster/GlobalBuffer.h"

/* A temp solution to access the event in the global buffer.
 * This event level user info is used to cache the pointer to the element in the global buffer.
 *
 * Tao Lin <lintao@ihep.ac.cn>, 04 Nov 2020
 */

struct JUNOEventUserInfo: public G4VUserEventInformation {

    JUNOEventUserInfo() {
        elem = 0;
    }

    ~JUNOEventUserInfo() {
        elem = 0;
    }

    void Print ()const {

    }

    // cache current event (between primary generator and user actions)
    GlobalBuffer::Elem* elem;
};

#endif

