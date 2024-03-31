#ifndef ISimTrackSvc_h
#define ISimTrackSvc_h

/*
 * Description:
 *
 *   ISimTrackSvc stores the tracks, which are related to 
 *   specific physics process, into event data model.
 *
 *   This service will be invoked in Geant4's User Actions:
 *   - In Tracking/Stepping action, filter the interested physics process
 *   - Fill the track into this service
 *   - End of event action, dump the info and reset.
 *
 *   A more complicated application is that calculate the edep of a track
 *   and its descendant.
 *   - In stepping action, get the track IDs of all the ancestors.
 *   - Check the track is stored in the track service.
 *   - If found, accumulate them.
 *
 * Tao Lin, July 29, 2020
 */
#include <vector>
namespace JM {
    class SimTrack;
}

class ISimTrackSvc {
public:
    // reset all
    virtual bool reset() = 0;

    // put the Track (the ownership is transfered to this service)
    virtual bool put(JM::SimTrack*) = 0;

    // get all Tracks
    virtual std::vector<JM::SimTrack*>& all() = 0;

    // get the Track via track id
    virtual JM::SimTrack* get(int trackid) = 0;
};

#endif
