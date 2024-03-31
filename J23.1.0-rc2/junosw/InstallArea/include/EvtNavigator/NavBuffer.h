#ifndef EVTNAVIGATOR_NAV_BUFFER_H
#define EVTNAVIGATOR_NAV_BUFFER_H

#include "EvtNavigator/EvtNavigator.h"

#ifdef SNIPER_VERSION_2
#include "SniperKernel/DataBuffer.h"
#else
#include "DataBuffer/DataBuffer.h"
#endif

namespace JM {

typedef DataBuffer<EvtNavigator> NavBuffer;

}

#endif
