#include <boost/python.hpp>
#include <boost/filesystem.hpp>
#include "ElecSimSvc.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "TFile.h"
#include "TTree.h"
#include <TRandom.h>
#include <fstream>
#include <sstream>
#include <cstring>

DECLARE_SERVICE(ElecSimSvc);


ElecSimSvc::ElecSimSvc(const std::string& name)
    : SvcBase(name)
{
      declProp("PmtTotal", m_PmtTotal=43213);//CD 17746; veto PMT 30000-32400 CD+WP
      declProp("start", m_start_Time="1970-01-01T00:00:00Z");
      declProp("end", m_stop_Time="2038-01-19T00:00:00Z");
      declProp("safeTimeTolerance", m_safeTimeTolerance=2000); //in ns
      declProp("RunID", m_runID=0);
      declProp("EventID", m_eventID=0);
      declProp("TofByPmtFile", m_TofbyPmtFile="pmt_tof_map.root");
      declProp("TofByGroupFile", m_TofbyGroupFile="group_tof_map.root");
      declProp("PmtGroupFile", m_pmtGroupFile="groupId.txt");
      declProp("TriggerByGroup", m_triggerByGroup=true);
      declProp("CalibOffsetFile", m_calibOffsetFile="tOffCalib.root");

      declProp("preTimeTolerance", m_preTimeTolerance=300); // ns
      declProp("postTimeTolerance", m_postTimeTolerance=700); // ns
}

ElecSimSvc::~ElecSimSvc()
{

}

bool
ElecSimSvc::initialize()
{

    double angle[9] = {-80,-60,-40,-20,0,20,40,60,80};
    double ratio[9] = {0.177,0.0479,0.04835,0.0445,0.0433,0.0552,0.0403,0.0372,0.1849};
    g1 = new TGraph(9,angle,ratio);
    TTimeStamp StartTime = str2time(m_start_Time);
    LogInfo<<"current_evt_time(TTimeStamp): "  << StartTime.AsString("c") <<std::endl;
    TimeStamp m_Starttime(StartTime);
    m_current_evttime = m_Starttime;
    m_StartTime = m_Starttime;
    m_firstPulseTimeInBuffer = m_Starttime;
    LogInfo<<"current_evt_time: "  << m_current_evttime.AsString("c") <<std::endl;

    TTimeStamp StopTime = str2time(m_stop_Time);
    TimeStamp m_Stoptime(StopTime.GetSec(), StopTime.GetNanoSec() );
    m_StopTime = m_Stoptime;

    // Run / Event ID
    if (m_runID>0) m_runID = -m_runID;
    LogInfo << "ElecSim Starting with "
            << " Run Number: " << m_runID
            << " Event Number: " << m_eventID
            << std::endl;

    // calib toffset loading :
    std::string s = getenv("ELECSIMROOT") ;
    s += "/share/";
    s += m_calibOffsetFile;
    if (!boost::filesystem::exists(s)) {
        LogError << "Can not find tof map file " << s << std::endl;
        return false;
    } else {
        LogInfo << "Loading timeoffset calib table from " << s << std::endl;
    }
    TFile* ff = new TFile(s.c_str(), "read");
    TTree* tt = (TTree*) ff->Get("calib");
    double pmtOffset;
    tt ->SetBranchAddress("tOffCalib", &pmtOffset);
   for(int i=0; i<tt->GetEntries(); i++){
        tt->GetEntry(i);
        m_pmtCalibOffset[i] = pmtOffset;
   }
   delete tt;
   delete ff;

    // trigger tof_map loading
    s = getenv("ELECSIMROOT");
    s += "/share/";
    if (m_triggerByGroup) {
        s += m_TofbyGroupFile;
    } else {
        s += m_TofbyPmtFile;
    }
    if (!boost::filesystem::exists(s)) {
        LogError << "Can not find tof map file " << s << std::endl;
        return false;
    } else {
        LogInfo << "Loading tof map from " << s << std::endl;
    }
    tofFile = new TFile(s.c_str(), "read");
    tofHist = (TH2D*)tofFile->Get("tof_map");
    if(!tofHist) { LogError << "Can not load top map histogram" << std::endl; return false;}

    s = getenv("ELECSIMROOT");
    s += "/share/";
    s += m_pmtGroupFile;
    std::string line; int pmtid, groupid;
    std::ifstream in; in.open(s.c_str());
    while(getline(in, line)) {
        std::istringstream ss(line);
        ss >> pmtid >> groupid;
        m_pmtGroupId[pmtid] = groupid;
    }
    in.close();

    m_readout_window_length = m_preTimeTolerance + m_postTimeTolerance;
    m_Earliest_CD_TriggerTime = m_StartTime;

    return true;
}

bool
ElecSimSvc::finalize()
{
    delete tofHist;
    delete tofFile;

    return true;
}

TTimeStamp ElecSimSvc::str2time(std::string str_time){
    struct tm tm_time;
    memset(&tm_time, 0, sizeof(struct tm)); // reset is very important
    
    if (str_time.find("T") != std::string::npos) {
        // ISO 8601 Data Format yyyy-mm-ddTHH:MM:SSZ
        strptime(str_time.c_str(),"%FT%T%z", &tm_time);
    } else {
        // Customized Data Format
        strptime(str_time.c_str(),"%Y-%m-%d %H:%M:%S", &tm_time);
    }

    auto offset = tm_time.tm_gmtoff;
    LogInfo << "Parse time zone offset: " << offset << std::endl;

    // We don't need to set the time zone as ISO 8601 already consists timezone info
    // setenv("TZ", "", 1);
    // tzset();
    
    time_t tTime=mktime(&tm_time) - timezone; // apply the local timezone first
    tTime -= offset;
    //LogInfo<<"tTime (number of seconds since standard time 1970-1-1 00:00:00): " << tTime<<std::endl;

    LogInfo << "Parse datetime string '"<< str_time << "'" << std::endl;
    LogInfo << " -> local time: " << ctime(&tTime) << std::endl;

    TTimeStamp answer(tTime,0);
    LogInfo << " -> UTC time: " << answer.AsString("c") << std::endl;
    return answer;
}

TimeStamp& ElecSimSvc::get_start_time(){
    return m_StartTime;
}
TimeStamp& ElecSimSvc::get_stop_time(){
    return m_StopTime;
}

void
ElecSimSvc::set_start_time(TimeStamp ts)
{
    m_StartTime = ts;
}

void
ElecSimSvc::set_stop_time(TimeStamp ts)
{
    m_StopTime = ts;
}

void
ElecSimSvc::iniCDDrakPulseStartTime(int nanosec)
{
	TimeStamp starttime = m_StartTime;
	starttime.Subtract(nanosec*1e-9);
	m_cddarkPulseStartTime = starttime;
}

void 
ElecSimSvc::setCDDrakPulseStartTime(TimeStamp t)
{
	m_cddarkPulseStartTime = t;
}

const TimeStamp& 
ElecSimSvc::getCDDarkPulseStartTime()
{
	return m_cddarkPulseStartTime;
}

bool
ElecSimSvc::has_trigger()
{
    return m_trigger_buffer.size()!=0;
}

Trigger
ElecSimSvc::get_trigger()
{
    Trigger t = m_trigger_buffer.front();
    m_trigger_buffer.pop_front();
    return t;
}

void
ElecSimSvc::put_trigger(const Trigger& t)
{
    LogInfo << "put a trigger: " << t.type << " " << t.time << std::endl;
    m_trigger_buffer.push_back(t);
}

std::deque<Trigger>&
ElecSimSvc::get_trigger_buffer()
{
    return m_trigger_buffer;
}

const std::deque<Trigger>&
ElecSimSvc::get_trigger_buffer() const
{
    return m_trigger_buffer;
}


bool
ElecSimSvc::has_event_trigger()
{
    return m_event_trigger_buffer.size()!=0;
}

Trigger
ElecSimSvc::get_event_trigger()
{
    Trigger t = m_event_trigger_buffer.front();
    m_event_trigger_buffer.pop_front();
    return t;
}

void
ElecSimSvc::put_event_trigger(const Trigger& t)
{
    LogInfo << "put a event_trigger." << std::endl;
    m_event_trigger_buffer.push_back(t);
}

std::deque<Trigger>&
ElecSimSvc::get_event_trigger_buffer()
{
    return m_event_trigger_buffer;
}

const std::deque<Trigger>&
ElecSimSvc::get_event_trigger_buffer() const
{
    return m_event_trigger_buffer;
}


std::deque<Pulse>&
ElecSimSvc::get_CDSPMT_pulse_buffer()
{
    return m_CDSPMT_pulse_buffer;
}

const std::deque<Pulse>&
ElecSimSvc::get_CDSPMT_pulse_buffer() const
{
    return m_CDSPMT_pulse_buffer;
}

std::deque<Pulse>&
ElecSimSvc::get_CDLPMT_pulse_buffer()
{
    return m_CDLPMT_pulse_buffer;
}

const std::deque<Pulse>&
ElecSimSvc::get_CDLPMT_pulse_buffer() const
{
    return m_CDLPMT_pulse_buffer;
}



void
ElecSimSvc::set_CDLPMT_pulse_buffer(const std::deque<Pulse>& buffer ) 
{
    m_CDLPMT_pulse_buffer = buffer;
}
/////////////WP//////////////
std::deque<Pulse>&
ElecSimSvc::get_WPLPMT_pulse_buffer()
{
    return m_WPLPMT_pulse_buffer;
}

const std::deque<Pulse>&
ElecSimSvc::get_WPLPMT_pulse_buffer() const
{
    return m_WPLPMT_pulse_buffer;
}

void
ElecSimSvc::set_WPLPMT_pulse_buffer(const std::deque<Pulse>& buffer )
{
    m_WPLPMT_pulse_buffer = buffer;
}

/////////////TT//////////////
std::deque<Pulse>&
ElecSimSvc::get_TT_pulse_buffer()
{
    return m_TT_pulse_buffer;
}

const std::deque<Pulse>&
ElecSimSvc::get_TT_pulse_buffer() const
{
    return m_TT_pulse_buffer;
}

void
ElecSimSvc::set_TT_pulse_buffer(const std::deque<Pulse>& buffer )
{
    m_TT_pulse_buffer = buffer;
}


void
ElecSimSvc::set_cur_evttime(const TimeStamp& ts)
{
    m_current_evttime = ts;
}

const TimeStamp&
ElecSimSvc::get_cur_evttime()
{
    return m_current_evttime;
}

void
ElecSimSvc::set_Laser_evttime(const TimeStamp& ts)
{
    m_Laser_evttime = ts;
}

const TimeStamp&
ElecSimSvc::get_Laser_evttime()
{
    return m_Laser_evttime;
}

void
ElecSimSvc::set_marker_time(TimeStamp ts)
{
    m_marker_time = ts;
}

TimeStamp
ElecSimSvc::get_marker_time()
{
    return m_marker_time;
}

void
ElecSimSvc::setFirstPulseTimeInBuffer(TimeStamp ts)
{
    m_firstPulseTimeInBuffer = ts;
}

TimeStamp
ElecSimSvc::getFirstPulseTimeInBuffer()
{
    return m_firstPulseTimeInBuffer;
}

void ElecSimSvc::setSafeTimeTolerance(double t)
{
   m_safeTimeTolerance = t;
}

double ElecSimSvc::getSafeTimeTolerance()
{
   return m_safeTimeTolerance;
}

void ElecSimSvc::setCurrentRunID(int runid) {
    m_runID = runid;
}

int ElecSimSvc::getCurrentRunID() const {
    return m_runID;
}

void ElecSimSvc::setCurrentEventID(int evtid) {
    m_eventID = evtid;
}

int ElecSimSvc::getCurrentEventID() const {
    return m_eventID;
}


TGraph* ElecSimSvc::get_graph(){
   return g1;
}

bool ElecSimSvc::has_enough_pulse() {
    std::deque<Pulse>& pulse_buffer = get_CDLPMT_pulse_buffer();
    std::deque<Pulse>& wp_pulse_buffer = get_WPLPMT_pulse_buffer();
    std::deque<Pulse>& sPMT_pulse_buffer = get_CDSPMT_pulse_buffer();

    LogDebug << "check if enough pulses exists in buffer."<<std::endl;
    if (pulse_buffer.empty()&&wp_pulse_buffer.empty()&&sPMT_pulse_buffer.empty()) {
        return false;
    }

    // TODO:


    // TimeStamp deltaTime = pulse_buffer.back().pulseHitTime - pulse_buffer.front().pulseHitTime;

    const TimeStamp& curevt = get_cur_evttime();
    bool flag= false;

    LogDebug << "start to check LPMT buffer."<<std::endl;
    
    if(!pulse_buffer.empty() ) { 
        const Pulse& front_pulse = pulse_buffer.front();
        if((get_marker_time() - front_pulse.pulseHitTime).GetSeconds()*1e9 < m_safeTimeTolerance) {
          for(std::deque<Pulse>::iterator it = pulse_buffer.begin(); it != pulse_buffer.end();) {
            if(it->type == kDarkPulse || it->type == kDNAfterPulse) it = pulse_buffer.erase(it);
            else it++;
          }
        }
        else flag = true;
        if(!pulse_buffer.empty()) m_firstPulseTimeInBuffer = (pulse_buffer.front()).pulseHitTime;
    }

    LogDebug << "First pulse time in buffer: " << m_firstPulseTimeInBuffer << std::endl;
    LogDebug << "Flag of LPMT buffer: " << flag << "; buffer size: " << pulse_buffer.size() << "; marker T: " << get_marker_time() <<std::endl;

    LogDebug << "start to check sPMT buffer."<<std::endl;

    if(!sPMT_pulse_buffer.empty()){
        if(flag == false) {
          for(std::deque<Pulse>::iterator it = sPMT_pulse_buffer.begin(); it != sPMT_pulse_buffer.end();) {
            if(it->pulseHitTime >= get_marker_time()) break;
            if(it->pulseHitTime < get_marker_time() && it->type == kDarkPulse) it = sPMT_pulse_buffer.erase(it);
            else it++;
          }
        }
    }

    LogDebug << "Flag of sPMT buffer: " << flag << "; buffer size: " << sPMT_pulse_buffer.size() << "; marker T: " << get_marker_time() <<std::endl;

    LogDebug << "start to check WP PMT buffer."<<std::endl;

    if(!wp_pulse_buffer.empty()){
        const Pulse& wp_front_pulse = wp_pulse_buffer.front();
        if((get_marker_time() - wp_front_pulse.pulseHitTime).GetSeconds()*1e9 < m_safeTimeTolerance) {
          for(std::deque<Pulse>::iterator it = wp_pulse_buffer.begin(); it != wp_pulse_buffer.end();) {
            if(it->pulseHitTime >= get_marker_time()) break;
            if(it->pulseHitTime < get_marker_time() && it->type == kDarkPulse) it = wp_pulse_buffer.erase(it);
            else it++;
          }
        }
        else flag = true;
    }

    LogDebug << "Flag of WP buffer: " << flag << "; buffer size: " << wp_pulse_buffer.size() << "; marker T: " << get_marker_time() <<std::endl;

    return (flag);

}

//offline/Simulation/ElecSimV3/ElecSimAlg/src/TriggerVFLToolLpmt.cc
double ElecSimSvc::getTof(int pmtId, int cubicId) 
{
    if (m_triggerByGroup) {
        int Id = m_pmtGroupId[pmtId];
        return tofHist->GetBinContent(Id+1, cubicId+1);
    } else {
        return tofHist->GetBinContent(pmtId+1, cubicId+1);
    }
}

int ElecSimSvc::getGroupId(int pmtId)
{
    return m_pmtGroupId[pmtId];
}

void ElecSimSvc::SetEarliestCDTriggerTime(TimeStamp triggr_time)
{
	m_Earliest_CD_TriggerTime = triggr_time;
	m_Earliest_CD_TriggerTime.Add(m_readout_window_length*1.0e-9);
}

const TimeStamp& ElecSimSvc::GetEarliestCDTriggerTime()
{
	return m_Earliest_CD_TriggerTime;
}

const int& ElecSimSvc::GetPreTimeTolerance()
{
	return m_preTimeTolerance;
}

//offline/Simulation/ElecSimV3/ElecSimAlg/src/TriggerVFLToolLpmt.cc
double ElecSimSvc::getCalibOffset(int pmtId) {
    return m_pmtCalibOffset[pmtId];
}
