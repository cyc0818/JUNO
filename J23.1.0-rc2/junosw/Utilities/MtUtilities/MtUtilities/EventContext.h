#ifndef JUNO_EVENT_CONTEXT_H
#define JUNO_EVENT_CONTEXT_H

#include "EvtNavigator/EvtNavigator.h"
#include "SniperMuster/GlobalStream.h"
#include "RootWriter/MtTTree.h"
#include <memory>

struct EventContext
{
    EventContext(std::shared_ptr<JM::EvtNavigator> &nav)
        : event(nav)
    {
    }

    std::shared_ptr<JM::EvtNavigator> event;
    std::vector<MtTTree*> trees;
};

typedef GlobalBuffer<EventContext> JunoGlobalBuffer;
typedef GlobalStream<EventContext> JunoGlobalStream;

#endif