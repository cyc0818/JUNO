#ifndef IOPSimulator_h
#define IOPSimulator_h

/*
 * IOPSimulator:
 *   base class for the simulator of optical photons.
 *
 * The detailed implementation should derive from this interface.
 *
 * Relationship with the simulation framework
 *
 *     +-------------+                         +------------+
 *     | Begin Event |                         |            |
 *     +-------------+                   .---->|  Optical   |
 *     |    NonOP    | ->-              /      |  Photon    |
 *     +-------------+    \            /  .--<-|  Simulator |
 *     |  End Event  | ->- gensteps ->-  /     |            |
 *     +-------------+ -<--- hits ---<---      +------------+
 *
 * In order to support different simulators, the gensteps/hits objects 
 * are defined as simple as possible.
 *
 * For simplicity, the OPSimulator need to maintain its own steps and hits.
 */

#include "OPSimulator/IGenStep.h"
#include "OPSimulator/IHit.h"

namespace opsimulator {

class IOPSimulator {
public:
    // fill the genstep
    virtual IGenStep* create_genstep() = 0;
    virtual bool clear_gensteps() = 0;

    // simulate an event or multiple events
    virtual bool simulate() = 0;

    // get the hit
    virtual int get_nhits() = 0;
    virtual IHit* get_hit(int idx) = 0;
    virtual bool clear_hits() = 0;
};

} // namespace opsimulator

#endif
