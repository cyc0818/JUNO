/*
 * =========================================================================
 *
 *       Filename:  IMCGlobalTimeSvc.h
 *
 *    Description:  Interface to manage time related info:
 *                  * get/set begin or end time (?)
 *                  * get_base_time()
 *                  * set_base_time()
 *                  * update_base_time()
 *
 *         Author:  Tao Lin (lintao@ihep.ac.cn), 
 *   Organization:  IHEP
 *
 * ==========================================================================
 */

#ifndef IMCGlobalTimeSvc_h
#define IMCGlobalTimeSvc_h

#include "TTimeStamp.h"

class IMCGlobalTimeSvc {
    public:
        // base time is the current event time.
        // Note: for IBD events, prompt and delay event have the *same* base time.
        // The developer needs to pay attention to this. He needs to 
        // calculate the current prompt and delay time such as:
        //     Time_prompt = Time_base 
        //     Time_delay = Time_base + delta time (capture time)
        virtual TTimeStamp get_base_time() = 0;
        virtual bool set_base_time(const TTimeStamp& stamp) = 0;

        // update_base_time is useful when the implementation will yield
        // a list of time. When a new time is needed, just update it.
        //
        // For some cases, if the current base time > m_end_time, 
        // we need to stop the event loop.
        virtual bool update_base_time() = 0;

        virtual ~IMCGlobalTimeSvc() {}
};

#endif
