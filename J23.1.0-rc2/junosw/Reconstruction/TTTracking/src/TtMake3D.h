#ifndef __TT_MAKE_3D__
#define __TT_MAKE_3D__

#include <set>
#include <list>

#include <iostream>

#include "Geometry/TTGeomSvc.h"
#include "Geometry/TtGeom.h"

#include "Event/TtCalibEvt.h"

struct points_3D{
    int n3D;
    std::vector<float> x3D;
    std::vector<float> y3D;
    std::vector<float> z3D;
    int ntouchedwalls;
};

class TtMake3D{
    public:
        TtMake3D(TTGeomSvc* tt_geom_svc);
        ~TtMake3D();

        virtual struct points_3D * makePoint(const std::list<JM::CalibTtChannel*> & tmp_calibttcol, std::set<int> * CTFlag );

    private:
        struct wall_positions{
            std::vector<float> xHitPos;
            std::vector<float> yHitPos;
            float zHitPosX;
            float zHitPosY;
        };
        typedef std::map<int, struct wall_positions> map_wall_pos;
        typedef std::vector<struct wall_positions> vec_wall_pos;

    protected:
        TTGeomSvc* m_tt_geom_svc;
};

class TtMake3DTime : public TtMake3D{
    public:
        /// TODO: Change TTGeomSvc and/or TTGeom so that both can provide same info.
        ///       as it is I need the svc to have positions and TtGeom to deal with timing
        TtMake3DTime(TTGeomSvc* tt_geom_svc, TtGeom* tt_geom, float time_thresh=20);
        ~TtMake3DTime();

        void SetTimeThreshold(float time);

        struct points_3D * makePoint(const std::list<JM::CalibTtChannel*> & tmp_calibttcol, std::set<int> * CTFlag );

    private:
        struct wall_hits{
            std::vector<int>   chid_x;
            std::vector<int>   chle_x;
            std::vector<float> time_x;
            std::vector<int>   chid_y;
            std::vector<int>   chle_y;
            std::vector<float> time_y;
        };
        typedef std::map<int, struct wall_hits> map_wall_hits;

        TtGeom * m_tt_geom;

        float m_time_threshold;

};

#endif
// vim: expandtab shiftwidth=4 softtabstop=4
