#ifndef Trigger_h
#define Trigger_h

#include "Context/TimeStamp.h"
#include "EvtNavigator/EvtNavigator.h"

#include <iostream>


// the trigger type should be also moved to Context.
enum TriggerType {
    kTriggerLpmt,    // LPMT only
    kTriggerLpmtStd, // LPMT Std
    kTriggerLpmtVFL, // LPMT VFL
    kTriggerSpmt,    // SPMT only
    kTriggerCD,      // both LPMT and SPMT
    kTriggerWP,      // WP trigger
    kTriggerTT,      // TT trigger
    kTriggerForce,   // force trigger
    kTriggerLaser,   // Laser trigger
    kTriggerUnknown  // unknown type
};

std::ostream& operator<<(std::ostream& out, const TriggerType& value);


struct Trigger {
    TriggerType type;
    TimeStamp time;
    int volume_id = -1; // 0-178 for VFL, -1 for non-VFL
    int nFiredPmt = -1; // Fired pmt number
};

/*
 * trigger sim tool
 *   produce a trigger according to the trigger scheme.
 * interfaces:
 *   - has_trigger(): return true/false
 *   - get_trigger(): return trigger if exists
 *   - set_elecsvc(): initialize 
 */
struct ITriggerTool {
    virtual bool has_trigger() = 0;
    virtual Trigger get_trigger() = 0;
    //virtual bool erase_buffer() = 0;

    virtual bool init() = 0;
};

/*
 * trigger handler
 *   we need to implement it as a tool
 * about the interface:
 *   - input: trigger
 *   - in/out: event header (todo)
 */
struct ITriggerHandler {
    virtual bool handle(const Trigger&, JM::EvtNavigator*) = 0;

    virtual bool init() = 0;

};

#endif
