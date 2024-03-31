#ifndef SimTrackSvc_hh
#define SimTrackSvc_hh

#include "DetSimAlg/ISimTrackSvc.h"
#include "SniperKernel/SvcBase.h"

#include "Event/SimTrack.h"
#include "Event/SimVertex.h"

class SimTrackSvc: public ISimTrackSvc, public SvcBase {
public:

    SimTrackSvc(const std::string& name);
    ~SimTrackSvc();

    bool initialize();
    bool finalize();

    bool reset();

    int put(JM::SimTrack*);
    std::vector<JM::SimTrack*>& all();

    JM::SimTrack* get(int trackid);
    int getTrackIdx(int trackid);

    int putVertex(JM::SimVertex*);
    std::vector<JM::SimVertex*>& allVertices();

private:

    std::vector<JM::SimTrack*> m_tracks;

    std::vector<JM::SimVertex*> m_vertices;
};

#endif
