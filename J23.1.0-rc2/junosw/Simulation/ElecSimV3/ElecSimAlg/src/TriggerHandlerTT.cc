#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Event/TtElecTruthHeader.h"
#include "Event/TtElecHeader.h"

#include "EvtNavigator/EvtNavHelper.h"

#include "ElecSimSvc.h"

class TriggerHandlerTT: public ToolBase, public ITriggerHandler {
    public:
        TriggerHandlerTT(const std::string& name): ToolBase(name) {
            declProp("preTimeTolerance", m_preTimeTolerance=100); // ns
            declProp("postTimeTolerance", m_postTimeTolerance=500); // ns

        }

        ~TriggerHandlerTT() {

        }

        bool handle(const Trigger& trigger, JM::EvtNavigator* evtnav) {
            // Trigger was found, now we need to save the data in the EDM format
            std::deque<Pulse>& buffer = m_elecsvc->get_TT_pulse_buffer();
            int sz = buffer.size();
            // empty buffer, so we just return.
            if (sz==0) {
                return false;
            }

            auto elec_header = JM::getHeaderObject<JM::TtElecHeader>(evtnav);
            if(!elec_header){
                elec_header = new JM::TtElecHeader();
                JM::addHeaderObject(evtnav,elec_header);
            }
            // JM::ElecTruthHeader* elec_truth_header = dynamic_cast<JM::ElecTruthHeader*>(evtnav->getHeader("/Event/Sim/Truth"));
            auto tt_elec_evt = new JM::TtElecEvt;

            // TODO: Get from TriggerToolTT which pulses need to be saved
            //       For now just save everything around trigger time...
            //       (as done for other systems)
            // TODO: use trigger time rather than pulse time?

            TimeStamp simTimeEarliest = buffer.begin()->pulseHitTime;
            simTimeEarliest.Subtract(m_preTimeTolerance*1e-9);
            TimeStamp simTimeLatest = buffer.begin()->pulseHitTime;
            simTimeLatest.Add(m_postTimeTolerance*1e-9);
	    TimeStamp FirstHitTime = buffer.begin()->pulseHitTime;

            
            for (auto iPulse = buffer.begin(); iPulse != buffer.end();) {
                Pulse& pulse = *iPulse;

                // only select in time window
                if (pulse.pulseHitTime < simTimeEarliest) {
                    iPulse = buffer.erase(iPulse);
                    continue;
                } else if (pulse.pulseHitTime >= simTimeLatest) {
                    iPulse++;
                    continue;
                }

                JM::ElecChannel *this_ch = tt_elec_evt->addChannel(pulse.pmtID);
                
								// If the pulse time difference is greater than one second, this implementation would fail

                this_ch->setCharge({static_cast<unsigned int>(pulse.amplitude)});
                this_ch->setTime({static_cast<unsigned int>((pulse.pulseHitTime-FirstHitTime).GetNanoSec())});

                iPulse = buffer.erase(iPulse);
            }

            elec_header-> setEvent(tt_elec_evt);
            return true;
        }

        bool init() {

            SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
            if (svc.invalid()) {
                LogError << "can't find service ElecSimSvc" << std::endl;
                return false;
            }
            m_elecsvc = svc.data();

            // SniperPtr<AbcBoardSvc> abcb_svc(*getRoot(), "AbcBoardSvc");
            // if (abcb_svc.invalid()) {
                // LogError << "can't find service AbcBoardSvc" << std::endl;
                // return false;
            // }
            // m_abcboardsvc = abcb_svc.data();


            LogInfo << "init trigger tool (TT). " << std::endl;

            return true;
        }

        ElecSimSvc* m_elecsvc;
        // AbcBoardSvc* m_abcboardsvc;


        double m_preTimeTolerance;
        double m_postTimeTolerance;

};

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerHandlerTT);
// vim: et:sw=4:sts=4:ts=4
