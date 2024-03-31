#ifndef ElecSimSvc_h
#define ElecSimSvc_h

/*
 * This is a general service or helper to manage:
 *   - trigger buffer
 *   - pulse buffer
 *   -pmt parameters
 * It would be easy to split this service into several services.
 *
 */

#include "SniperKernel/SvcBase.h"
#include "Context/TimeStamp.h"

#include <deque>
#include <iostream>
#include <TGraph.h>
#include <TTimeStamp.h>
#include <string>
#include <map>

#include "Trigger.h"
#include "Pulse.h"

#include "TFile.h"
#include "TH2.h"
/*
 * general service
 */

class ElecSimSvc: public SvcBase {
 public:
    ElecSimSvc(const std::string& name);
    ~ElecSimSvc();

    bool initialize();
    bool finalize();


 public:


    // trigger buffer
    bool has_trigger();
    Trigger get_trigger();
    void put_trigger(const Trigger&);
    std::deque<Trigger>& get_trigger_buffer();
    const std::deque<Trigger>& get_trigger_buffer() const;
    // event trigger buffer for LPMT
    bool has_event_trigger();
    Trigger get_event_trigger();
    void put_event_trigger(const Trigger& );
    std::deque<Trigger>& get_event_trigger_buffer();
    const std::deque<Trigger>& get_event_trigger_buffer() const;

    // pulse buffer
    bool has_enough_pulse();

    std::deque<Pulse>& get_CDSPMT_pulse_buffer();
    const std::deque<Pulse>& get_CDSPMT_pulse_buffer() const;
    std::deque<Pulse>& get_CDLPMT_pulse_buffer();
    const std::deque<Pulse>& get_CDLPMT_pulse_buffer() const;
    void  set_CDLPMT_pulse_buffer(const std::deque<Pulse>& );
    std::deque<Pulse>& get_WPLPMT_pulse_buffer();
    const std::deque<Pulse>& get_WPLPMT_pulse_buffer() const;
    void  set_WPLPMT_pulse_buffer(const std::deque<Pulse>& );
    std::deque<Pulse>& get_TT_pulse_buffer();
    const std::deque<Pulse>& get_TT_pulse_buffer() const;
    void  set_TT_pulse_buffer(const std::deque<Pulse>& );

    // timestamp
    void set_cur_evttime(const TimeStamp&);
    const TimeStamp& get_cur_evttime();
    
    // timestamp
    void set_Laser_evttime(const TimeStamp&);
    const TimeStamp& get_Laser_evttime();

    // timestamp
    void set_marker_time(TimeStamp);
    TimeStamp get_marker_time();

    // timestamp
    void setFirstPulseTimeInBuffer(TimeStamp);
    TimeStamp getFirstPulseTimeInBuffer();

    // timestamp
    void setSafeTimeTolerance(double t);
    double getSafeTimeTolerance();

    //timestamp
    void setCDDrakPulseStartTime(TimeStamp);
    void iniCDDrakPulseStartTime(int);
    const TimeStamp& getCDDarkPulseStartTime();

    void SetEarliestCDTriggerTime(TimeStamp);
    const TimeStamp& GetEarliestCDTriggerTime();
    const int& GetPreTimeTolerance();

    TimeStamp& get_start_time();    
    TimeStamp& get_stop_time();

    void set_start_time(TimeStamp ts);
    void set_stop_time(TimeStamp ts);

    // Run Number and Event Number 
    void setCurrentRunID(int runid);
    int getCurrentRunID() const;

    void setCurrentEventID(int evtid);
    int getCurrentEventID() const;
    // ---- END ----

    TGraph* get_graph();

 public:
    double getTof(int pmtId, int cubicId);
    int getGroupId(int pmtId);

    double getCalibOffset(int pmtId);

 private:
    std::deque<Trigger> m_trigger_buffer;
    std::deque<Trigger> m_event_trigger_buffer;
    std::deque<Pulse> m_CDSPMT_pulse_buffer;
    std::deque<Pulse> m_CDLPMT_pulse_buffer;
    std::deque<Pulse> m_WPLPMT_pulse_buffer;
    std::deque<Pulse> m_TT_pulse_buffer;

    TimeStamp m_current_evttime;
    TimeStamp m_Laser_evttime;
    TimeStamp m_marker_time;
    TimeStamp m_firstPulseTimeInBuffer;
    TimeStamp m_StartTime;
    TimeStamp m_StopTime;
    TimeStamp m_cddarkPulseStartTime;

    std::string m_pmtdata_file;
    //std::vector<PmtData> pd_vector;
    int m_PmtTotal;
    TGraph* g1;
//    int m_init_seed;

    std::string m_start_Time;
    std::string m_stop_Time;
    TTimeStamp str2time(std::string str_time);

    double m_safeTimeTolerance;

    // For MC simulation, the runID should be always negative.
    // the eventID >= 0.
    int m_runID;
    int m_eventID;


    // trigger tof_map loading
    std::string m_TofbyPmtFile;
    std::string m_TofbyGroupFile;
    std::string m_pmtGroupFile;
    TFile* tofFile;
    TH2D*  tofHist;

    // calib timeoffset
    std::string m_calibOffsetFile;
    double m_pmtCalibOffset[20000];

    bool m_triggerByGroup;

    int m_pmtGroupId[20000];

    int m_preTimeTolerance;
    int m_postTimeTolerance;
    int m_readout_window_length ;
    TimeStamp m_Earliest_CD_TriggerTime;
};

#endif
