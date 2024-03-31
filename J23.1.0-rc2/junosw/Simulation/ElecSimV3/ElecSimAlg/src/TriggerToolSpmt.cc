#include <boost/python.hpp>
#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"

#include "ElecSimSvc.h"

#include "Geometry/PMT.h"

#include <map>
#include <deque>

class TriggerToolSpmt: public ToolBase, public ITriggerTool {
public:
    TriggerToolSpmt(const std::string& name): ToolBase(name) {

    }

    ~TriggerToolSpmt() {

    }

    bool has_trigger() {
        // check if there is any existing trigger.
        if (triggers.size()!=0) {
            return true;
        }

        // try to find a trigger.
        // note, in trigger tool, we don't modify pulse buffer
        std::deque<Pulse>& buffer = m_elecsvc->get_CDSPMT_pulse_buffer();

        // if buffer is empty, we can't find any trigger.
        if (buffer.size()==0) {
            return false;
        }

        // TODO: trigger scheme
        // here, we will use a separate trigger for SPMT.
        // use the same implemantion.

        // npmt trigger
        TimeStamp beginTime = buffer.front().pulseHitTime;
        int beginIdx = 0;
        int buffersize = buffer.size();

        double triggerWindow = 80; // ns
        double slipWindow = 16; //
        int firedPmtNum = 1; // TODO

        bool found = false;

        pmtfired.clear();
        while(true) {
            if(beginIdx >= buffersize || 
               (m_elecsvc->get_marker_time() - buffer[beginIdx].pulseHitTime).GetSeconds()*1e9 < m_elecsvc->getSafeTimeTolerance()) {
                break;
            }


            TimeStamp endTime = beginTime;
            endTime.Add(triggerWindow*1e-9); // trigger window

            pmtfired.clear();

            for (int i = beginIdx; i < buffersize; ++i) {
                const Pulse& p = buffer[i];

                if (not PMT::Is3inch(p.pmtID)) { // SPMT
                    continue;
                }

                // if the pulse is beyond the time window,
                // we will stop
                if (p.pulseHitTime > endTime) {
                    break;
                }

                // LogInfo << "pulse info: "
                //         << p.pmtID << " -> "
                //         << p.pulseHitTime << std::endl;

                ++pmtfired[p.pmtID];
            }

            LogInfo << "fired pmt numbers: " << pmtfired.size() << std::endl;

            if (pmtfired.size()>= firedPmtNum) {
                // found
                found = true;
                break;
            }

            // slip trigger window
            endTime = beginTime;
            endTime.Add(slipWindow*1e-9);

            while (++beginIdx < buffersize) {
                const Pulse& p = buffer[beginIdx];

                if (not PMT::Is3inch(p.pmtID)) { // SPMT
                    continue;
                }

                // found the next trigger window
                if (p.pulseHitTime > endTime) {
                    beginTime = p.pulseHitTime;
                    break;
                }

            }
            LogInfo << "slip window: beginIdx -> "
                    << beginIdx
                    << std::endl;
        }

        if (not found) {
            for (std::deque<Pulse>::iterator it = buffer.begin();
               it != buffer.end(); ) {
             if ((m_elecsvc->get_marker_time() - it->pulseHitTime).GetSeconds()*1e9 < m_elecsvc->getSafeTimeTolerance()) {
                break;
              }
             it = buffer.erase(it);
         }
            return false;
        }

        // if found, create an trigger object.
        Trigger t;
        t.type = kTriggerSpmt;
        t.time = beginTime;

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

        LogInfo << "init trigger tool (SPMT). " << std::endl;
        return true;
    }

    std::deque<Trigger> triggers;

    ElecSimSvc* m_elecsvc;

    std::map<int, int> pmtfired; // key: pmtid, value: nhits


};

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerToolSpmt);
