#ifndef EvtNavMT_h
#define EvtNavMT_h

/*
 * Description:
 *   This struct holds the share_ptr<EvtNavigator> to avoid the object is released,
 *   especially when use ROOT I/O and buffer management in Input Thread.
 *
 *   The life cycle:
 *   - create in input thread, associate the evtnav with it
 *   - access in worker threads
 *   - release in output thread
 *
 *   Use following code to copy the evtnav
 *     m_current_event->evtnav = *navBuf->current();
 *
 * Author:
 *   Tao Lin <lintao@ihep.ac.cn>
 *
 */

#include <memory>
#include <EvtNavigator/EvtNavigator.h>

struct EvtNavMT {
    std::shared_ptr<JM::EvtNavigator> evtnav;

    EvtNavMT(std::shared_ptr<JM::EvtNavigator> _evtnav) {
        evtnav = _evtnav;
    }

};

#endif
