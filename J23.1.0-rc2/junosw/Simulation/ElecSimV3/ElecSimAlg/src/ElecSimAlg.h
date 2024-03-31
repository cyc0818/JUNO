#ifndef ElecSimAlg_h
#define ElecSimAlg_h


/*
 * About the design of our electronics simulation.
 *
 *             A N A L O G    /---- DELAY -------> (FADC) -_  _--- D I G I T A L
 *              P U L S E    |                       ^      -_-     S I G N A L
 *  ~~~> PMT -> -.  .---  ---|                       |                  |
 *               ` ,         |                       | SWITCH           |
 *                `          |   |x                  | ON/OFF           v
 *                           ____|  x                |              B U F F E R
 *                   OTHERS      |    x -------------o                  |
 *                           ----|  x                |                  v
 *                               |x  T R I G G E R   ---------->--------o
 *                                                    T I M E           |
 * See JUNO-Doc-516                                  S T A M P          v
 *                                                                    D A Q
 *
 * This is an simplified example to show how SNiPER Task and Incident components
 * are used in our real simulation.
 *
 */


#include "SniperKernel/AlgBase.h"

#include "ElecSimSvc.h"

class ElecSimAlg: public AlgBase {
 public:
    ElecSimAlg(const std::string& name);
    ~ElecSimAlg();

    bool initialize();
    bool execute();
    bool finalize();

 private:
    ElecSimSvc* m_elecsvc;

    std::map<TriggerType, ITriggerHandler*>  m_trig_handler;

 private:
    // flag TriggerSPMTbyLPMT is used to enable/disable the trigger of SPMT by LPMT
    bool m_triggerSPMTbyLPMT;
    bool m_Gen;
};


#endif
