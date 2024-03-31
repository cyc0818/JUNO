#include <boost/python.hpp>
#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"

#include "ElecSimSvc.h"

#include "TriggerTT_L1.h"
#include "Identifier/TtID.h"

#include <map>
#include <deque>
#include <set>

/******************************************************************************
 *                                WARNING
 * current version of this code does not fully implement TT behaviour.
 *  - In TT the L1 trigger is used to reject events. In the current
 *    implementation, no pulses are dropped as long as the event is valid.
 *  - The TT L2 trigger will require aligment between the triggered walls. In
 *    the current implementation only 3 layers are required.
 *  - The timewindow involving the L2 triggers is not really verified. A
 *    larger than expected number was put for grouping as a reference
 * Note also that no configuration parameters are yet available for the L1
 * trigger, even if the TriggerTT_L1 class do accept some configuration.
 ******************************************************************************/

class TriggerToolTT: public ToolBase, public ITriggerTool {
    public:
        TriggerToolTT(const std::string& name): ToolBase(name) {

        }

        ~TriggerToolTT() {

        }

        bool has_trigger() {
            // check if there is any existing trigger.
            if (triggers.size()!=0) {
                return true;
            }

            // try to find a trigger.
            std::deque<Pulse>& buffer = m_elecsvc->get_TT_pulse_buffer();

            // if buffer is empty, we can't find any trigger.
            if (buffer.size()==0) {
                return false;
            }

            TimeStamp beginTime = buffer.front().pulseHitTime;

            double triggerWindow = 500; // ns
            TimeStamp endTime = beginTime;
            endTime.Add(triggerWindow*1e-9); // trigger window

            std::map<int, std::deque<std::deque<Pulse>::iterator> > list_refs_L1;
            std::map<int, TriggerTT_L1* > trig_L1;

            for(auto iPulse = buffer.begin() ; iPulse != buffer.end(); iPulse++){
                const Pulse& p = *iPulse;

                // if the pulse is beyond the time window,
                // we will stop
                if (p.pulseHitTime > endTime) {
                    break;
                }

                int wall_id = TtID::wall_id(Identifier(p.pmtID));
                int pmt_id  = TtID::pmt(Identifier(p.pmtID));

                if(trig_L1.find(wall_id) == trig_L1.end()){
                    trig_L1[wall_id] = new TriggerTT_L1();
                    // TODO: Set up options
                }
                bool hit_added;
                while(true){
                    // Try adding a hit to L1 coincidence. If hit is beyond TW it cannot be
                    // added unless I check the L1 didn't trigger before that. If that was
                    // not the case I can drop the first hit and try again.
                    hit_added = trig_L1[wall_id]->AddHit(pmt_id, p.pulseHitTime);
                    if(hit_added){
                        list_refs_L1[wall_id].push_back(iPulse);
                        break;
                    }
                    if(trig_L1[wall_id]->HasTrigger()) break;
                    trig_L1[wall_id]->RemoveFirst();
                    buffer.erase(list_refs_L1[wall_id].front());
                    list_refs_L1[wall_id].pop_front();
                }
            }

            // Currently require 3 walls hits, without alignment.
            int trigger_time;
            int this_trigger_time;
            std::set<int> trigs_per_layer;
            int n_walls = 0;
            for(auto trig_L1_iter = trig_L1.begin(); trig_L1_iter != trig_L1.end(); trig_L1_iter++){
                int wall_id = trig_L1_iter->first;
                TriggerTT_L1 * this_L1 = trig_L1_iter->second;
                if(this_L1->HasTrigger(&this_trigger_time)){
                    // Note: Trigger time will correspond to 1st L1 coincidence,
                    // not to when L2 accepted
                    if(trigger_time > this_trigger_time) trigger_time = this_trigger_time;
                    trigs_per_layer.insert(TtID::layer(TtID::id(wall_id,0)));
                    n_walls++;
                }
                delete this_L1;
            }
            bool found = false;
            if(trigs_per_layer.size() >= 3) found = true;
            LogInfo << "TT Triggered in " << n_walls << " walls in " << trigs_per_layer.size() << " layers => " << found << std::endl;

            if (not found) {
                // Currently remove only first hit and then redo it.
                // TODO: Should at least remove the wall.
                if(trigs_per_layer.size() > 0 && buffer.size() > 0)
                    buffer.erase(buffer.begin());
                // for(auto list_refs_to_del = list_refs_L1.begin(); list_refs_to_del != list_refs_L1.end(); list_refs_to_del++){
                    // while(list_refs_to_del->second.size() > 0){
                        // buffer.erase(list_refs_to_del->second.front());
                        // list_refs_to_del->second.pop_front();
                    // }
                // }
                return false;
            }

            // if found, create an trigger object.
            Trigger t;
            t.type = kTriggerTT;
            t.time = trigger_time;

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

            LogInfo << "init trigger tool (TT). " << std::endl;
            return true;
        }

        std::deque<Trigger> triggers;

        ElecSimSvc* m_elecsvc;

};

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerToolTT);
// vim: et:sw=4:sts=4:ts=4
