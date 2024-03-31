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

    for (auto vertex: m_vertices) {
        delete vertex;
    }
    m_vertices.clear();
    return true;
}

int SimTrackSvc::put(JM::SimTrack* track) {
    // check
    int exists = -1;

    int cnt = 0;
    for (auto trk: m_tracks) {
        if (track->getTrackID() == trk->getTrackID()) {
            exists = cnt;
            break;
        }
        ++cnt;
    }

    // put
    if (exists != -1) {
        LogWarn<< "track with track ID " << track->getTrackID()
               << " is already exists in the SimTrackSvc."
               << " The track idx is " << cnt
               << std::endl;
    } else {
        // take the ownership
        m_tracks.push_back(track);
        exists = m_tracks.size()-1;
    }

    return exists;
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

int SimTrackSvc::getTrackIdx(int trackid) {
    int idx = -1;

    int cnt = 0;
    for (auto trk: m_tracks) {
        if (trackid == trk->getTrackID()) {
            idx = cnt;
            break;
        }
        ++cnt;
    }

    return idx;
}

int SimTrackSvc::putVertex(JM::SimVertex* vertex) {
    m_vertices.push_back(vertex);
    return m_vertices.size()-1;
}

std::vector<JM::SimVertex*>& SimTrackSvc::allVertices() {
    return m_vertices;
}
