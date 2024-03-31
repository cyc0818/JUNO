#ifndef Pulse_h
#define Pulse_h

#include <memory>

#include "Context/TimeStamp.h"
#include "EventKeeper.h"

using namespace std;

enum PulseType {
    kNormalPulse,
    kDarkPulse,
    kAfterPulse,
    kDNAfterPulse,
    kUnknownPulse
};

struct PulseTruth {

    int npe;
    double hitTime;
    double timeWindow;
    double TTS;
    double timeoffset;
    shared_ptr<EventKeeper::Entry> m_entry_pointer;
    PulseTruth()
     : TTS(0.0){
    }
};



struct Pulse {
    int pmtID;
    double amplitude;
    TimeStamp pulseHitTime;
    TimeStamp evtTimeStamp;

    PulseType type;
    TimeStamp corrpulseHitTime;//pulseTime after correct tof (for vertex fit trigger).
    double tof;

    shared_ptr<PulseTruth> truth;

    Pulse()
    : pmtID(-1), amplitude(0.0), truth(0), type(kNormalPulse) {
        
    }

    // sort by hit time
    bool operator < (const Pulse& rhs) const {
       if(this->pulseHitTime < rhs.pulseHitTime ||( this->pulseHitTime == rhs.pulseHitTime && this->pmtID < rhs.pmtID)){
         return true;
         }
       return false; 
    }
    bool operator == (const Pulse& rhs) const {
        return this->pmtID == rhs.pmtID && this->pulseHitTime == rhs.pulseHitTime;
    }
    bool operator != (const Pulse& rhs) const {
        return this->pmtID != rhs.pmtID || this->pulseHitTime != rhs.pulseHitTime;
    }
    Pulse& operator += (Pulse& p){
        (*this).amplitude = (*this).amplitude + p.amplitude;
        return *this;
      }

    //about truth
    void AddTruth()
    {
	shared_ptr<PulseTruth> Pointer{new PulseTruth};
	truth = Pointer;
    }

};


#endif
