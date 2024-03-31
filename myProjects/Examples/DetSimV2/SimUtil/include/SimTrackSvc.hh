#ifndef SimTrackSvc_hh
#define SimTrackSvc_hh

#include "DetSimAlg/ISimTrackSvc.h"
#include "SniperKernel/SvcBase.h"

#include "Event/SimTrack.h"

class SimTrackSvc: public ISimTrackSvc, public SvcBase {
public:

    SimTrackSvc(const std::string& name);
    ~SimTrackSvc();

    bool initialize();
    bool finalize();

    bool reset();

    bool put(JM::SimTrack*);
    std::vector<JM::SimTrack*>& all();

    JM::SimTrack* get(int trackid);

private:

    std::vector<JM::SimTrack*> m_tracks;
};

#endif
