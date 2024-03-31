#ifndef IAD_BLOCK_CONTEXT_H
#define IAD_BLOCK_CONTEXT_H

#include "EvtNavigator/EvtNavigator.h"
#include "SniperMuster/GlobalStream.h"
#include <memory>
#include "IADFormat/Buffer.h"

struct IADBlockContext
{
    IADBlockContext(IAD::BufferPtr buf)
        : buffer(buf)
    {
    }

    IAD::BufferPtr buffer;
};

typedef GlobalBuffer<IADBlockContext> IADGlobalBuffer;
typedef GlobalStream<IADBlockContext> IADGlobalStream;

#endif