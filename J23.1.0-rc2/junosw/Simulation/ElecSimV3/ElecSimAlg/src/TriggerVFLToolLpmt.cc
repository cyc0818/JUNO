#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include <boost/filesystem.hpp>
#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Pulse.h"
#include "ElecSimSvc.h"
#include "EventKeeper.h"

#include "TSystem.h"
#include "TStopwatch.h"
#include "TRandom.h"

#include "Geometry/PMT.h"

#include <map>
#include <deque>
#include <string>

class TriggerVFLToolLpmt : public ToolBase, public ITriggerTool {
    public:
        TriggerVFLToolLpmt(const std::string& name) : ToolBase(name),
        m_elecsvc(0) {
            declProp("StdTrigger_FiredPmtNum",m_StdTrigger_FiredPmtNum=300);// 
            declProp("StdTrigger_window",m_StdTrigger_window=300);// ns
            declProp("StdTrigger_slip_window",m_StdTrigger_slip_window=16);// ns

            declProp("VFLTrigger_FiredPmtNum",m_VFLTrigger_FiredPmtNum=100);// 
            declProp("VFLTrigger_window",m_VFLTrigger_window=48);// ns
            declProp("VFLTrigger_slip_window",m_VFLTrigger_slip_window=16);// ns
            declProp("VFLTrigger_ConfFile",m_VFLTrigger_ConfFile="default");// ns
            declProp("Total_volume", tot_volume=180);
            declProp("StdLatency", m_Stdlatency=180);
            declProp("VFLLatency", m_VFLlatency=120);
            declProp("Trigger_Mode", m_Trigger_Mode="OR-ed");

            declProp("doForceTrigger", m_doForceTrigger=false);
            declProp("doLaserTrigger", m_doLaserTrigger=false);
            declProp("doTimeOffsetCorrection", m_doTimeOffsetCorrection=false);
        }

        ~TriggerVFLToolLpmt() {;}

        std::string m_Trigger_Mode;

        float m_StdTrigger_FiredPmtNum;
        float m_StdTrigger_window;
        float m_StdTrigger_slip_window;

        float m_VFLTrigger_FiredPmtNum;
        float m_VFLTrigger_window;
        float m_VFLTrigger_slip_window;
        int tot_volume;
        float m_Stdlatency;  // ns
        float m_VFLlatency; // ns

        bool m_doForceTrigger;  // force trigger mode
        bool m_doLaserTrigger;  // Laser trigger mode
        bool m_doTimeOffsetCorrection;

	struct ThresholdWindowPair
	{
		ThresholdWindowPair():threshold(-1),window(-1){};
		ThresholdWindowPair(int Threshold,int Window){threshold = Threshold;window = Window;};
		int threshold;
		int window;
	};

        //TimeStamp m_globalRefTime;    // absolute time stamp, for each grid the global clock should be the same 

        TimeStamp m_startTime;      // absolute time stamp, for each grid the global clock should be the same

        bool corrpulsecmp(Pulse* x, Pulse* y) {
            return (x->corrpulseHitTime)<(y->corrpulseHitTime);
        }

        bool triggercmp(Trigger x, Trigger y) {
            return (x.time) < (y.time);
        }

        bool has_trigger() {

            TStopwatch timer;
            timer.Start();

            if (triggers.size() != 0) {
                return true;
            }

            std::deque<Pulse>& LPMT_buffer = m_elecsvc->get_CDLPMT_pulse_buffer();
            int buffersize =  LPMT_buffer.size();
            if (buffersize == 0)
                return false;

            std::deque<Trigger>& CD_event_trigger_buffer = m_elecsvc->get_event_trigger_buffer();
            CD_event_trigger_buffer.clear();

            int counter = 0;
            bool all_found = false;  // for all sub-volumes, whether a trigger found
            bool single_found = false;

            EventKeeper& keeper = EventKeeper::Instance();
            const EventKeeper::Entry& entry = keeper.current_entry();
            LogDebug << "EventKeeper: entry.tag = " << entry.tag << ", entry.filename = " << entry.filename << ", event time = " << m_elecsvc->get_cur_evttime() << std::endl;

            if (m_doForceTrigger) {
                // generate a trigger randomly in current buffer : 
                TimeStamp beginTime = LPMT_buffer.front().pulseHitTime;
                TimeStamp backTime  = LPMT_buffer.back().pulseHitTime;
                double deltaTime    = backTime - beginTime;

                TimeStamp forceTriggerTime;
                while (true) {
                    double genTime = gRandom->Uniform(0, deltaTime);
                    forceTriggerTime = beginTime;
                    forceTriggerTime.Add(genTime);
                    if( (m_elecsvc->get_marker_time() - forceTriggerTime).GetSeconds()*1e9 < m_elecsvc->getSafeTimeTolerance()) {
                        continue;
                    }

                    LogInfo << "A force trigger is assigned at " << forceTriggerTime << std::endl;

                    TimeStamp cur_triggerT = forceTriggerTime;

                    // if found, create an trigger object.
                    Trigger t;
                    t.type = kTriggerForce;
                    t.time = cur_triggerT;
                    t.nFiredPmt = pmtfired.size();
                    t.volume_id = -1;
                    CD_event_trigger_buffer.push_back(t);

                    counter++;
                    single_found = true;
                    all_found = true;
                    break;
                }
            } else if (m_doLaserTrigger) {
                TimeStamp beginTime = LPMT_buffer.front().pulseHitTime;
                TimeStamp backTime  = LPMT_buffer.back().pulseHitTime;

                TimeStamp LaserTriggerTime;
                bool btrigger = false;
                TimeStamp LaserTime = m_elecsvc->get_Laser_evttime();
                TimeStamp MarkerTime = m_elecsvc->get_marker_time();
                if ((strstr(entry.tag.c_str(), "Laser") != NULL)&&(LaserTime>beginTime)) {
                    btrigger = true;
                    LaserTriggerTime = m_elecsvc->get_cur_evttime();
                } else if ((LaserTime<MarkerTime)&&(LaserTime>beginTime)) {
                    btrigger = true;
                    LaserTriggerTime = LaserTime;
                }
                
                if (btrigger) {
                    LogInfo << "A Laser trigger is assigned at " << LaserTriggerTime << std::endl;

                    TimeStamp cur_triggerT = LaserTriggerTime;

                    // if found, create an trigger object.
                    Trigger t;
                    t.type = kTriggerLaser;
                    t.time = cur_triggerT;
                    t.nFiredPmt = pmtfired.size();
                    t.volume_id = -1; 
                    CD_event_trigger_buffer.push_back(t);

                    counter++;
                    single_found = true;
                    all_found = true;
                } else {
                    LogDebug << "*** First pulse time in buffer (before removing): " << LPMT_buffer.front().pulseHitTime << std::endl;
                    for (std::deque<Pulse>::iterator it = LPMT_buffer.begin(); it != LPMT_buffer.end(); ) {
                        //        //if (it->pulseHitTime >= m_elecsvc->get_marker_time()) break;
                        if ((m_elecsvc->get_marker_time() - it->pulseHitTime).GetSeconds()*1e9 >= m_elecsvc->getSafeTimeTolerance()) {
                            it = LPMT_buffer.erase(it);
                        }
                        else {
                            if(it->type == kDarkPulse || it->type == kDNAfterPulse) it = LPMT_buffer.erase(it);
                            else it++;
                        }
                    }
                    if(LPMT_buffer.size() > 0) {
                        m_elecsvc->setFirstPulseTimeInBuffer(LPMT_buffer.front().pulseHitTime);
                        LogDebug << "*** First pulse time in buffer (after removing): " << LPMT_buffer.front().pulseHitTime << std::endl;
                    }
                    return false;
                }
            } else {
                auto bound_corrpulsecmp = boost::bind(&TriggerVFLToolLpmt::corrpulsecmp, this, boost::placeholders::_1, boost::placeholders::_2);
		std::deque<Pulse*> LPMT_Pulses_pointer;
		for(auto& p:LPMT_buffer)
		{
			LPMT_Pulses_pointer.push_back(&p);
		}

                // Loop in each subvolume... 180th volume is standard trigger
                for (int volume_id=0; volume_id<tot_volume; volume_id++) {    // loop in all 179 grids for trigger searching...

                    if (m_Trigger_Mode == "onlyStd" and volume_id < 179 ) 
                        continue;

                    if (m_Trigger_Mode == "onlyVFL" and volume_id == 179 ) 
                        continue;

                    //std::cout << "Loop in volume " << volume_id << std::endl;

                    // do tof correction for all pulses in buffer...
                    for (int i=0; i<buffersize; i++) {
                        Pulse* p = LPMT_Pulses_pointer[i];
                        int pmt_id = p->pmtID;
                        TimeStamp corrtimestamp = p->pulseHitTime;
                        //double tof = hist_map->GetBinContent(pmt_id+1, volume_id+1);
                        double tof = m_elecsvc->getTof(pmt_id, volume_id);
                        corrtimestamp.Subtract(tof*1e-9);
                        
                        if (m_doTimeOffsetCorrection) {  // do calib time offset correction
                            corrtimestamp.Subtract(m_elecsvc->getCalibOffset(pmt_id)*1e-9); 
                        }

                        p->corrpulseHitTime = corrtimestamp;
                        //if (volume_id == 0) std::cout << "pptt" << p.corrpulseHitTime << std::endl;
                    }

                    sort(LPMT_Pulses_pointer.begin(), LPMT_Pulses_pointer.end(), bound_corrpulsecmp);

                    // npmt trigger
                    TimeStamp beginTime = LPMT_Pulses_pointer.front()->corrpulseHitTime;
                    TimeStamp backTime  = LPMT_Pulses_pointer.back()->corrpulseHitTime;
                    double deltaTime    = backTime - beginTime;

                    // global clock ...
                    /*if ( m_Trigger_Mode == "onlyVFL" or m_Trigger_Mode=="OR-ed") {
                        if(volume_id == 0) {
                            m_globalRefTime = beginTime;
                            m_globalRefTime.Subtract(1e-6); // move 1us earlier
                        }
                    }

                    else if ( m_Trigger_Mode == "onlyStd" and volume_id == 179) {
                        m_globalRefTime = beginTime;
                        m_globalRefTime.Subtract(1e-6); // move 1us earlier
                    }*/
		    m_startTime = m_elecsvc->get_start_time(); 

                    int beginIdx = 0;
                    float triggerWindow = BlockID_Conf[volume_id].window;
                    float slipWindow    = m_VFLTrigger_slip_window;
                    int firedPmtNum     = BlockID_Conf[volume_id].threshold;

                    if (volume_id == 179) {
                        triggerWindow = m_StdTrigger_window;
                        slipWindow = m_StdTrigger_slip_window;
                        firedPmtNum = m_StdTrigger_FiredPmtNum;
                    }

                    pmtfired.clear();

		    LogDebug << "VolumeId = " << volume_id << " triggerWindow = " << triggerWindow << " TriggerThreshold = " << firedPmtNum << std::endl;

                    while(true) {
                        if (beginIdx >= buffersize || 
                                (m_elecsvc->get_marker_time() - LPMT_Pulses_pointer[beginIdx]->pulseHitTime).GetSeconds()*1e9 < m_elecsvc->getSafeTimeTolerance()) {
                            break;
                        }

                        TimeStamp endTime = beginTime;
                        endTime.Add(triggerWindow*1e-9);

                        pmtfired.clear();

                        for (int i=beginIdx; i<buffersize; ++i) {
                            const Pulse* p = LPMT_Pulses_pointer[i];
                            if (not PMT::IsCD(p->pmtID) or not PMT::Is20inch(p->pmtID)) {
                                continue;
                            }

                            //std::cout << "wwtt" << p.corrpulseHitTime << std::endl;

                            if (p->corrpulseHitTime > endTime) {
                                break;
                            }

                            ++pmtfired[p->pmtID];
                        }

                        if (pmtfired.size() >= firedPmtNum ) {
                            LogDebug << "Fired PMT Number = " << pmtfired.size() << " in volume " << volume_id << std::endl;

                            TimeStamp cur_triggerT = endTime;

                            // move onto global clock ...
                            TimeStamp deltaT = cur_triggerT - m_startTime;
                            long long int delta_refT  = deltaT.GetSec()*1e9 + deltaT.GetNanoSec() ;
                            //long long int delta_refT  = (cur_triggerT - m_globalRefTime).GetSeconds()*1e9 ;
                            long long int delta_cycle = delta_refT / int(m_VFLTrigger_slip_window);
                            cur_triggerT = m_startTime;
                            //cur_triggerT = m_globalRefTime;
                            cur_triggerT.Add(delta_cycle * int(m_VFLTrigger_slip_window) * 1e-9);

                            // do lantency corrections...
                            if (volume_id < 179)
                                cur_triggerT.Add(m_VFLlatency*1e-9);
                            else if (volume_id == 179)
                                cur_triggerT.Add(m_Stdlatency*1e-9);

                            if(cur_triggerT >= m_elecsvc->GetEarliestCDTriggerTime())
                            {
                                // if found, create an trigger object.
                                Trigger t;
                                t.type = kTriggerLpmtVFL;
                                t.time = cur_triggerT;
                                t.nFiredPmt = pmtfired.size();
                                t.volume_id = volume_id;
                                if (volume_id == 179) {
                                    t.volume_id = -1;
                                    t.type = kTriggerLpmtStd;
                                }
                                CD_event_trigger_buffer.push_back(t);

                                counter++;
                                single_found = true;
                                all_found = true;
                                break;
                            }
                        }

                        // slip window
                        endTime = beginTime;
                        endTime.Add(slipWindow*1e-9);

                        while (++beginIdx < buffersize) {
                            Pulse* p = LPMT_Pulses_pointer[beginIdx];

                            if (not PMT::IsCD(p->pmtID) or not PMT::Is20inch(p->pmtID)) {
                                continue;
                            }

                            if (p->corrpulseHitTime > endTime ) {
                                beginTime = p->corrpulseHitTime;
                                break;
                            }
                        }
                    }

                }
                if (not all_found) {
                    LogDebug << "*** First pulse time in buffer (before removing): " << LPMT_buffer.front().pulseHitTime << std::endl;
                    for (std::deque<Pulse>::iterator it = LPMT_buffer.begin(); it != LPMT_buffer.end(); ) {
                        //        //if (it->pulseHitTime >= m_elecsvc->get_marker_time()) break;
                        if ((m_elecsvc->get_marker_time() - it->pulseHitTime).GetSeconds()*1e9 >= m_elecsvc->getSafeTimeTolerance() + m_elecsvc->GetPreTimeTolerance()) {
                            it = LPMT_buffer.erase(it);
                        }
                        else {
                            if(it->type == kDarkPulse || it->type == kDNAfterPulse) it = LPMT_buffer.erase(it);
                            else it++;
                        }
                    }
                    if(LPMT_buffer.size() > 0) {
                        m_elecsvc->setFirstPulseTimeInBuffer(LPMT_buffer.front().pulseHitTime);
                        LogDebug << "*** First pulse time in buffer (after removing): " << LPMT_buffer.front().pulseHitTime << std::endl;
                    }
                    return false;
                }

            }

            auto bound_triggercmp = boost::bind(&TriggerVFLToolLpmt::triggercmp, this, boost::placeholders::_1, boost::placeholders::_2);
            sort(CD_event_trigger_buffer.begin(), CD_event_trigger_buffer.end(), bound_triggercmp);

            triggers.push_back(CD_event_trigger_buffer.front());
            LogInfo << "Total Fired Volume number: " << CD_event_trigger_buffer.size() << " " << counter << std::endl;
            LogInfo <<"Trigger Time:"<< CD_event_trigger_buffer.front().time << " with volume ID : " << CD_event_trigger_buffer.front().volume_id <<std::endl;

            Trigger t_spmt;
            t_spmt.type = kTriggerSpmt;
            t_spmt.time = CD_event_trigger_buffer.front().time;
            LogInfo <<"SPMT Trigger Time:"<< t_spmt.time <<std::endl;
            triggers.push_back(t_spmt);

            timer.Stop();
            double m_time = timer.RealTime();
            LogInfo << "Time Consumed in Trigger Search -> " << m_time << " s" << std::endl;

            return true;
        }

        Trigger get_trigger() {
            Trigger t = triggers.front();
            triggers.pop_front();

            return t;
        }


        bool init() {

            SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
            if (svc.invalid()) {
                LogError << "can't find service ElecSimSvc" << std::endl;
                return false;
            }
            m_elecsvc = svc.data();

            m_startTime = m_elecsvc->get_start_time();


            for(int i = 0; i < 179; i++)
            {
		ThresholdWindowPair conf(m_VFLTrigger_FiredPmtNum,m_VFLTrigger_window);
		BlockID_Conf[i] = conf;
            }

	    if(m_VFLTrigger_ConfFile != "default")
	    {

	    	ifstream fin(m_VFLTrigger_ConfFile.data());
	    	if(!fin)
	    	{
			LogError << "Can't find " + m_VFLTrigger_ConfFile << " Please check..." << std::endl;
			return false;
	    	}
	    	else
	    	{
			int id, FiredPmtNum, Windows;
			while(fin >> id >> FiredPmtNum >> Windows)
			{
				if(id < 0 || id > 178)
				{
					LogError << "Wrong volumeid: " << id  << " Please check..." << std::endl;
					return false;
				}
				BlockID_Conf[id].threshold = FiredPmtNum;
				BlockID_Conf[id].window = Windows;
			}
		
	    	}

	    }

            LogInfo << "init trigger tool (LPMT). " << std::endl;

            return true;
        }

        std::string map_DirPath;

        std::deque<Trigger> triggers;

        ElecSimSvc* m_elecsvc;

        std::map<int, int> pmtfired; // key: pmtid, value: nhits
	
    private:
	std::map<int, ThresholdWindowPair> BlockID_Conf;
	std::string m_VFLTrigger_ConfFile;


};

DECLARE_TOOL(TriggerVFLToolLpmt);
