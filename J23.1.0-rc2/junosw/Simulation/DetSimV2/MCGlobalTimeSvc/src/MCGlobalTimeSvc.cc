#include <boost/python.hpp>
#include "MCGlobalTimeSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Task.h"

#include "CLHEP/Random/RandExponential.h"

#include <ctime>

DECLARE_SERVICE(MCGlobalTimeSvc);

MCGlobalTimeSvc::MCGlobalTimeSvc(const std::string& name)
    : SvcBase(name)
{
    // FIXME: Make sure the time range is valid
    declProp("BeginTime", m_str_begin_time="1970-01-01 00:00:01");
    declProp("EndTime", m_str_end_time="2038-01-19 03:14:07");

    declProp("EventRate", m_event_rate = 1.0); // Hz
    m_tau = 1./m_event_rate;
}

MCGlobalTimeSvc::~MCGlobalTimeSvc()
{

}

bool
MCGlobalTimeSvc::initialize() {
    // convert begin and end time

    struct tm tm_begin;
    struct tm tm_end;
    bool status = true;

    // convert string to struct tm
    status = cnv_str2tm(m_str_begin_time, &tm_begin,
                        "begin time");
    if (not status) { return status; }

    status = cnv_str2tm(m_str_end_time, &tm_end,
                        "end time");
    if (not status) { return status; }

    // convert struct tm to Timestamp
    m_begin_time = TTimeStamp(TTimeStamp::MktimeFromUTC(&tm_begin), 0);
    m_end_time   = TTimeStamp(TTimeStamp::MktimeFromUTC(&tm_end),   0);

    LogDebug << "Begin Time: '" << m_begin_time << "'" << std::endl;
    LogDebug << "  End Time: '" << m_end_time << "'" << std::endl;

    // check begin < end
    if (not (m_begin_time<=m_end_time)) {
        LogError << "Begin Time should be smaller than End Time"
                 << std::endl;
        return false;
    }

    // let base = begin
    m_base_time = m_begin_time;

    // event rate
    if (m_event_rate <= 0.0) {
        LogError << "Event rate should be greater than zero. "
                 << std::endl;
        return false;
    }
    m_tau = 1./m_event_rate; // unit: s

    return true;
}

bool
MCGlobalTimeSvc::finalize() {
    return true;
}

TTimeStamp
MCGlobalTimeSvc::get_base_time() {
    return m_base_time;
}

bool
MCGlobalTimeSvc::set_base_time(const TTimeStamp& stamp) {
    // check the range
    if (m_begin_time<=stamp && stamp<m_end_time) {
        m_base_time = stamp;
    } else {
        LogError << "The new base time is not in [begin, end)"
                 << std::endl;
        return false;
    }
    return true;
}

bool
MCGlobalTimeSvc::update_base_time() {
    double deltatime = CLHEP::RandExponential::shoot(m_tau); // unit: s
    // need to get second and nanosecond
    // Ref: $CONTEXTROOT/Context/TimeStamp.h
    int Sec = (int)deltatime; 
    int NanoSec = (int)((deltatime-Sec)*1.0e9);

    m_base_time.Add(TTimeStamp(Sec, NanoSec));

    // check the time validation
    if (m_begin_time<=m_base_time && m_base_time<m_end_time) {
        // ok
    } else {
        // not valid now, how to do?
        // TODO
        LogWarn << "base time is not in the range." << std::endl;
        LogWarn << "Begin: " << m_begin_time << std::endl;
        LogWarn << " Base: " << m_base_time << std::endl;
        LogWarn << "  End: " << m_end_time << std::endl;
        return false;
    }
    return true;
}

// helper
bool
MCGlobalTimeSvc::cnv_str2tm(const std::string& str, struct tm* t, 
                            const std::string& label) {
    static const Char_t *kSQL = "%Y-%m-%d %H:%M:%S";

    char* strst = NULL;
    // parse time first
    strst = strptime(str.c_str(), kSQL, t);
    if (!strst) {
        LogError << "can't convert " << label << ": '" 
                 << str << "'." << std::endl;
        return false;
    } else if (*strst == '\0') {
        LogInfo << "Parse " << label << ": '" 
                << str 
                << "' successfully."
                << std::endl;
        return true;
    } else {
        LogWarn << "Parse " << label << " with extra string: " 
                << strst << std::endl;
        return true;
    }
    return true;
}
