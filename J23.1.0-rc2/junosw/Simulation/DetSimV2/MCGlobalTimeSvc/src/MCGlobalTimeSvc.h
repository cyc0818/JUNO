/*
 * =====================================================================================
 *
 *       Filename:  MCGlobalTimeSvc.h
 *
 *    Description:  The real implementation.
 *
 *
 *         Author:  Tao Lin (lintao@ihep.ac.cn), 
 *   Organization:  IHEP
 *
 * =====================================================================================
 */
#ifndef MCGlobalTimeSvc_h
#define MCGlobalTimeSvc_h

#include "MCGlobalTimeSvc/IMCGlobalTimeSvc.h"
#include "SniperKernel/SvcBase.h"

class MCGlobalTimeSvc: public IMCGlobalTimeSvc, public SvcBase
{
public:
    MCGlobalTimeSvc(const std::string& name);
    ~MCGlobalTimeSvc();

    bool initialize();
    bool finalize();

    TTimeStamp get_base_time();
    bool set_base_time(const TTimeStamp& stamp);

    bool update_base_time();

private:
    // helper
    bool cnv_str2tm(const std::string& str, struct tm* t, 
                    const std::string& label);

private:
    TTimeStamp m_base_time;
    // begin and end time
    TTimeStamp m_begin_time;
    TTimeStamp m_end_time;

    double m_tau;

    // we need some strings to represent the time.
    // SQL format is used here:
    // * "%Y-%m-%d %H:%M:%S"
    // REF: https://dev.mysql.com/doc/refman/5.0/en/datetime.html
    // But pay attention to TIMESTAMP data type, its range is
    // * '1970-01-01 00:00:01' UTC to '2038-01-19 03:14:07' UTC
    std::string m_str_begin_time;
    std::string m_str_end_time;
    // event rate
    double m_event_rate;
};

#endif
