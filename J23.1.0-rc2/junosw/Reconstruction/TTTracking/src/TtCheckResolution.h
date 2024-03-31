#ifndef TtCheckResolution_h
#define TtCheckResolution_h

#include "TtLikelihood.h"

#include "Event/SimTrack.h"

#include<list>
#include<string>
#include<map>

class TtCheckResolution {
    public:
        TtCheckResolution(const std::vector<JM::SimTrack*> * true_tracks);
        ~TtCheckResolution();

        void check_fit(int npar, double * pars);

        void SetBottomWP(float z_bottom_WP);
        void SetHitNoHitLH(TtLH_HitNoHit * check_HitNoHit);

        int getTopoType();

        float GetOpeningAngle() { return m_opening_angle; }
        float GetDistanceBottomWP(){ return m_distance_bottom_WP; }

    protected:
        const std::vector<JM::SimTrack*> * m_true_tracks;
        TtLH_HitNoHit * m_check_HitNoHit;

        float m_z_bottom_wp;

        float m_opening_angle;
        float m_distance_bottom_WP;
};

#endif  // TtCheckResolution_h

// vim: expandtab shiftwidth=4 softtabstop=4 tabstop=4
