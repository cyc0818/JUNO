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
 *   Collect the vertex collections. Update on 21 March 2023
 *
 *     Assume 'O' is a daughter track in secondaries. Some of them may point to
 *     the same vertex and creator process. 
 *
 *                 /----> Proc <----\
 *                 |        ^       |
 *                 |        |       |
 *          O      O        O       O
 *
 *     When loop the tracks, we need to check whether the vertex of the track
 *     already created in the vertex collection.
 *
 *     If the vertex does not exist yet, need to create a new vertex and register
 *     in the vertex collection.
 *
 *     Finally, need to associate the track id and the vertex idx
 *
 *
 * Tao Lin, July 29, 2020
 */
#include <vector>
namespace JM {
    class SimTrack;
    class SimVertex;
}

class ISimTrackSvc {
public:
    // reset all
    virtual bool reset() = 0;

    // put the Track (the ownership is transferred to this service)
    // the idx will be returned. 
    virtual int put(JM::SimTrack*) = 0;

    // get all Tracks
    virtual std::vector<JM::SimTrack*>& all() = 0;

    // get the Track via track id
    virtual JM::SimTrack* get(int trackid) = 0;
    virtual int getTrackIdx(int trackid) = 0;

    // put a vertex (the ownership is transferred to this service)
    // note: the association between input/output track and vertex need to 
    // set correctly. 
    virtual int putVertex(JM::SimVertex*) = 0; // return the idx in collection
    virtual std::vector<JM::SimVertex*>& allVertices() = 0;

};

#endif
