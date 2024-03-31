#ifndef IAD_EVT_BOX_H
#define IAD_EVT_BOX_H

#include "IADFormat/Block.h"

struct IADEvtBox{
    IAD::EventHeader* evtHeader;
    uint32_t fileNumber;
    uint32_t tag;
};

#endif