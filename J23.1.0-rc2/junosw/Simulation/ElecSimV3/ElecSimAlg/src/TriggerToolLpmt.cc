#include <boost/bind/bind.hpp>
#include <boost/python.hpp>
#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Pulse.h"
#include "ElecSimSvc.h"

#include <map>
#include <deque>

#include "Geometry/PMT.h"

class TriggerToolLpmt: public ToolBase, public ITriggerTool {
public:
    TriggerToolLpmt(const std::string& name): ToolBase(name),
                                              m_elecsvc(0) {

    declProp("Trigger_FiredPmtNum",m_Trigger_FiredPmtNum=300);// 
    declProp("Trigger_window",m_Trigger_window=80);// ns
    declProp("Trigger_slip_window",m_Trigger_slip_window=16);// ns
    }

    ~TriggerToolLpmt() {

    }

    float m_Trigger_FiredPmtNum;
    float m_Trigger_window;
    float m_Trigger_slip_window;
   // std::deque<Pulse> buffer;
    bool cmp(Pulse* x,Pulse* y)
    {
      return (x->corrpulseHitTime)<(y->corrpulseHitTime);
    }  
    bool has_trigger() {
        // check if there is any existing trigger.
        if (triggers.size()!=0) {
            return true;
        }

        // try to find a trigger.
        // note, in trigger tool, we don't modify pulse buffer
        std::deque<Pulse>& LPMT_buffer = m_elecsvc->get_CDLPMT_pulse_buffer();

        // if buffer is empty, we can't find any trigger.
        if (LPMT_buffer.size()==0) {
            return false;
        }

	std::deque<Pulse*> LPMT_Pulses_pointer;
	for(auto& p:LPMT_buffer)
	{
		LPMT_Pulses_pointer.push_back(&p);
	}
        
        auto bound_cmp=boost::bind(&TriggerToolLpmt::cmp, this, boost::placeholders::_1, boost::placeholders::_2);
        sort(LPMT_Pulses_pointer.begin(),LPMT_Pulses_pointer.end(),bound_cmp);

        // npmt trigger
        TimeStamp beginTime = LPMT_Pulses_pointer.front()->corrpulseHitTime;
        TimeStamp backTime = LPMT_Pulses_pointer.back()->corrpulseHitTime;
        double deltaTime = backTime - beginTime;

        double m_tof = LPMT_Pulses_pointer.front()->tof;// tof of the triggering hit.
        
        int beginIdx = 0;
        int buffersize = LPMT_Pulses_pointer.size();

        float triggerWindow = m_Trigger_window; // ns
        float slipWindow = m_Trigger_slip_window; //
        int firedPmtNum = m_Trigger_FiredPmtNum;

        bool found = false;
        TimeStamp Trigger_time;

        pmtfired.clear();


        while(true) {
            if (beginIdx >= buffersize || 
                (m_elecsvc->get_marker_time() - LPMT_Pulses_pointer[beginIdx]->pulseHitTime).GetSeconds()*1e9 < m_elecsvc->getSafeTimeTolerance()) {
                break;
            }


            TimeStamp endTime = beginTime;
            endTime.Add(triggerWindow*1e-9); // trigger window

            pmtfired.clear();

            for (int i = beginIdx; i < buffersize; ++i) {
                const Pulse* p = LPMT_Pulses_pointer[i];

                if (not PMT::IsCD(p->pmtID) or not PMT::Is20inch(p->pmtID)) { // not CD LPMT
		   LogWarn << "Find other PMT pulse in CD LPMT pulse buffer, please check..." << std::endl;
		   continue;
                } 

                // if the pulse is beyond the time window,
                // we will stop
                if (p->corrpulseHitTime > endTime) {
                    break;
                }

               //  LogInfo << "pulse info: "
               //          << p.pmtID << " -> "
               //          << p.corrpulseHitTime << std::endl;

                ++pmtfired[p->pmtID];
            }


            if (pmtfired.size()>= firedPmtNum) {

                beginTime.Add(m_tof*1e-9);
                TimeStamp endTime = beginTime;
                endTime.Add(triggerWindow*1e-9); // trigger window

	        // move onto global clock ...
	        TimeStamp start_time = m_elecsvc->get_start_time();
	        Trigger_time = start_time;
	        TimeStamp deltaT = endTime - start_time;
	        long long int delta_refT  = deltaT.GetSec()*1e9 + deltaT.GetNanoSec() ;
	        long long int delta_cycle = delta_refT / int(m_Trigger_slip_window);
	        Trigger_time.Add(delta_cycle * int(m_Trigger_slip_window) * 1e-9);
		if(Trigger_time >= m_elecsvc->GetEarliestCDTriggerTime())
                // found
                {
                        LogInfo << "fired pmt numbers: " << pmtfired.size() << std::endl;
                        found = true;
                        break;
		}
            }

            // slip trigger window
            endTime = beginTime;
            endTime.Add(slipWindow*1e-9);

            while (++beginIdx < buffersize) {
                const Pulse* p = LPMT_Pulses_pointer[beginIdx];

                if (not PMT::IsCD(p->pmtID) or not PMT::Is20inch(p->pmtID)) { // LPMT
			LogWarn << "Find other PMT pulse in CD LPMT pulse buffer, please check..." << std::endl;
			continue;
                }

                // found the next trigger window
                if (p->corrpulseHitTime > endTime) {
                    beginTime = p->corrpulseHitTime;
                    m_tof = p->tof;
                    break;
                }

            }
            //LogInfo << "slip window: beginIdx -> "
            //        << beginIdx
            //        << std::endl;
        }

        if (not found) {
           //clear pulses vector.
           LogDebug << "*** No trigger found, remove pulses in buffer before marker time and dark pulses in whole buffer." << std::endl;
           LogDebug << "*** First pulse time in buffer (before removing): " << LPMT_buffer.front().pulseHitTime << std::endl;
           for (std::deque<Pulse>::iterator it = LPMT_buffer.begin(); it != LPMT_buffer.end(); ) {
             //if (it->pulseHitTime >= m_elecsvc->get_marker_time()) break;
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

        //beginTime.Add(m_tof*1e-9);
        //TimeStamp endTime = beginTime;
        //endTime.Add(triggerWindow*1e-9); // trigger window

	//// move onto global clock ...
	//TimeStamp start_time = m_elecsvc->get_start_time();
	//TimeStamp Trigger_time = start_time;
	//TimeStamp deltaT = endTime - start_time;
	//long long int delta_refT  = deltaT.GetSec()*1e9 + deltaT.GetNanoSec() ;
	//long long int delta_cycle = delta_refT / int(m_Trigger_slip_window);
	//Trigger_time.Add(delta_cycle * int(m_Trigger_slip_window) * 1e-9);

        // if found, create an trigger object.
        Trigger t;
        t.type = kTriggerLpmt;
        //t.time = beginTime;
        t.time = Trigger_time;
        LogInfo <<"Trigger Time:"<<beginTime<<std::endl;
        triggers.push_back(t);

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

        LogInfo << "init trigger tool (LPMT). " << std::endl;

        return true;
    }

    std::deque<Trigger> triggers;

    ElecSimSvc* m_elecsvc;

    std::map<int, int> pmtfired; // key: pmtid, value: nhits

};

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerToolLpmt);
