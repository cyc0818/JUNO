#ifndef TriggerSimAlg_h
#define TriggerSimAlg_h

#include "SniperKernel/AlgBase.h"
#include "ElecSimSvc.h"


class TriggerSimAlg: public AlgBase {
 public:
    TriggerSimAlg(const std::string& name);
    ~TriggerSimAlg();

    bool initialize();
    bool execute();
    bool finalize();

 private:
    ElecSimSvc* m_elecsvc;
    float m_PulseBufferLength;
    std::string m_LpmtTrigger_alg;
    std::string m_WPLpmtTrigger_alg;
    // to identify the trigger tool, we use trigger type as key
    std::map<TriggerType, ITriggerTool*>  m_trig_tool;

 private:
    // flag TriggerSPMTbyLPMT is used to enable/disable the trigger of SPMT by LPMT
    bool m_triggerSPMTbyLPMT;
};

#endif
