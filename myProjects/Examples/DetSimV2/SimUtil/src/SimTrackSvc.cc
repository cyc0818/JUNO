#include <boost/python.hpp>

#include "SimTrackSvc.hh"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/Task.h"

DECLARE_SERVICE(SimTrackSvc);

SimTrackSvc::SimTrackSvc(const std::string& name)
    : SvcBase(name) {

}

SimTrackSvc::~SimTrackSvc() {

}

bool SimTrackSvc::initialize() {
    return true;
}

bool SimTrackSvc::finalize() {
    return true;
}

bool SimTrackSvc::reset() {
    for (auto trk: m_tracks) {
        delete trk;
    }
    m_tracks.clear();
    return true;
}

bool SimTrackSvc::put(JM::SimTrack* track) {
    // check
    bool exists = false;
    for (auto trk: m_tracks) {
        if (track->getTrackID() == trk->getTrackID()) {
            exists = true;
            break;
        }
    }

    // put
    if (exists) {
        LogWarn<< "track with track ID " << track->getTrackID()
               << " is already exists in the SimTrackSvc."
               << std::endl;
    } else {
        // take the ownership
        m_tracks.push_back(track);
    }

    return true;
}

std::vector<JM::SimTrack*>& SimTrackSvc::all() {
    return m_tracks;
}

JM::SimTrack* SimTrackSvc::get(int trackid) {

    JM::SimTrack* track = NULL;
    for (auto trk: m_tracks) {
        if (trackid == trk->getTrackID()) {
            track = trk;
            break;
        }
    }

    return track;
}
