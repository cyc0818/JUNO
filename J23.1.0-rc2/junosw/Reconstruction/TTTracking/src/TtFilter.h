#ifndef __TT_FILTER__
#define __TT_FILTER__

#include <list>
#include <set>

#include "Event/TtCalibEvt.h"

std::set<int> * TtFilter(const std::list<JM::CalibTtChannel*> & calibTtHits);

#endif
// vim: expandtab shiftwidth=4 softtabstop=4
