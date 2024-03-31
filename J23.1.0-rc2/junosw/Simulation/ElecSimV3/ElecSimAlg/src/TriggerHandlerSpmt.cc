#include "Trigger.h"
#include "SniperKernel/ToolBase.h"
#include "SniperKernel/ToolFactory.h"
#include "SniperKernel/SniperPtr.h"
#include "Event/CdSpmtElecTruthHeader.h"
#include "Event/CdSpmtElecHeader.h"

#include "Geometry/PMT.h"

#include "ElecSimSvc.h"
#include "AbcBoardSvc.h"

class TriggerHandlerSpmt: public ToolBase, public ITriggerHandler {
public:
    TriggerHandlerSpmt(const std::string& name): ToolBase(name) {
        declProp("preTimeTolerance", m_preTimeTolerance=300); // ns
        declProp("postTimeTolerance", m_postTimeTolerance=700); // ns

        declProp("storeElecTruth", m_enableStoreElecTruth=true);

    }

    ~TriggerHandlerSpmt() {

    }

    bool handle(const Trigger& trigger, JM::EvtNavigator* evtnav) {

        // generate waveforms first, then remove pulses.
        // if the pulse is earlier than the trigger time window,
        // we will also remove it.

        std::deque<Pulse>& buffer = m_elecsvc->get_CDSPMT_pulse_buffer();
        int sz = buffer.size();
        // empty buffer, so we just return.
        if (sz==0) {
            return false;
        }


        // we only select pulses in a time window

        TimeStamp simTimeEarliest = trigger.time;
        simTimeEarliest.Subtract(m_preTimeTolerance*1e-9);
        TimeStamp simTimeLatest = trigger.time;
        simTimeLatest.Add(m_postTimeTolerance*1e-9);

        // group by pmt id
        std::map<int, std::vector<Pulse*> > pulseMap;

        for (int idx=0; idx<sz; ++idx) {
            Pulse& pulse = buffer[idx];
            // only consider SPMT in CD.
            // omit other PMTs.
            if (not PMT::Is3inch(pulse.pmtID)) {
                continue;
            }

            // only select in time window
            if (pulse.pulseHitTime < simTimeEarliest) {
                continue;
            } else if (pulse.pulseHitTime >= simTimeLatest) {
                continue;
            }
	    pulse.evtTimeStamp=simTimeEarliest; //true and dark noise have same time stamp
	    if(pulse.type==kDarkPulse)
	      {
		if(pulse.truth)
		{
			pulse.truth->hitTime = (pulse.pulseHitTime- simTimeEarliest).GetSeconds()*1e9;
		}
	      }
            pulseMap[pulse.pmtID].push_back(&pulse);
        }

        // generate waveform
        std::map<int, std::vector<Pulse*> >::iterator itPulseMap;
        for (itPulseMap=pulseMap.begin(); itPulseMap!=pulseMap.end(); ++itPulseMap)
          m_abcboardsvc->pushPulses(itPulseMap->first,itPulseMap->second);
        m_abcboardsvc->addElecDataModel(evtnav, m_enableStoreElecTruth);
        m_abcboardsvc->clearVectors();

        // remove pulse
        // TODO: is that better to remove pulses in the main algorithm?
        
        for (std::deque<Pulse>::iterator it = buffer.begin();
             it != buffer.end(); ) {
            if ((it->pulseHitTime>=simTimeLatest)) {
                break;
            }
            bool flag = PMT::Is3inch(it->pmtID);
            if (flag) {
                it = buffer.erase(it);
            } else {
                ++it;
            }
        }


        return true;
    }

    bool init() {

        SniperPtr<ElecSimSvc> svc(*getRoot(), "ElecSimSvc");
        if (svc.invalid()) {
            LogError << "can't find service ElecSimSvc" << std::endl;
            return false;
        }
        m_elecsvc = svc.data();

        SniperPtr<AbcBoardSvc> abcb_svc(*getRoot(), "AbcBoardSvc");
        if (abcb_svc.invalid()) {
            LogError << "can't find service AbcBoardSvc" << std::endl;
            return false;
        }
        m_abcboardsvc = abcb_svc.data();


        LogInfo << "init trigger tool (SPMT). " << std::endl;

        return true;
    }

    ElecSimSvc* m_elecsvc;
    AbcBoardSvc* m_abcboardsvc;


    double m_preTimeTolerance;
    double m_postTimeTolerance;

    // By default, the Elec Truth will be created and stored into EvtNavigator.
    // In order to reduce the memory usage, user can disable this part. 
    bool m_enableStoreElecTruth;

};

// Please declare this tool, otherwise we can't locate it.
DECLARE_TOOL(TriggerHandlerSpmt);
