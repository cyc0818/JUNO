#include "Event/DummyTTHit.h"

ClassImp(DummyTTHit);

    DummyTTHit::DummyTTHit() {
        barid = -1;
        peL = 0.0;
        peR = 0.0;
        timeL = 0.0;
        timeR = 0.0;
        ADCL = 0.0;
        ADCR = 0.0;
        x = 0.0;
        y = 0.0;
        z = 0.0;

    }

    DummyTTHit::~DummyTTHit() {

    }
