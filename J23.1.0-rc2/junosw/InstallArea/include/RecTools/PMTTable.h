#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "TTimeStamp.h"
#include "TVector3.h"

namespace JM {
    class CalibPmtChannel;
}

enum PMTTYPE{
    NONE,
    LPMT_DYNODE,
    LPMT_MCP,
    LPMT_MCP_HIGHQE,
    SPMT
};

struct PMTProp{
    //------static members, the same for all events
    unsigned int id;
    PMTTYPE type;
    TVector3 pos;//unit: mm
    double dnrate;//unit: Hz
    double rpde;//unit: 1
    double gain;//unit: p.e.
    double tts;//unit: ns

    //------non-static members, vary between events
    //PMT level
    double fht;//first hit time
    double totq;//total charge
    //reference to PMTCalibChannel
    JM::CalibPmtChannel* pmtcalib=nullptr;
    bool isused;

    PMTProp(unsigned int id=-1u){
        reset(id);
    }

    void reset(unsigned int id=-1u) {
        id = id;
        type = PMTTYPE::NONE;
        pos = TVector3(0, 0, 0);
        dnrate = 0;
        rpde = 0;
        gain = 0;
        tts = 0;
        fht = 0;
        totq = 0;
        pmtcalib = nullptr;
        isused = false;
    }

    void set_unused() {
        fht = 0;
        totq = 0;
        pmtcalib = nullptr;
        isused = false;
    }
};
typedef std::vector<PMTProp*> PMTTable;
typedef std::vector<PMTProp*> HitPMTVector;

struct RecInfo{
    int runID;
    int evtID;
    TTimeStamp evttime;
    TVector3 vtx;
    double energy;

    RecInfo(){
        runID = -1;
        evtID = -1;
        evttime = TTimeStamp(0, 0);
        vtx = TVector3(0, 0, 0);
        energy = 0;
    }
};
