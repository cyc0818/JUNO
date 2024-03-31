#include "TtCheckResolution.h"

#include <TVector3.h>

#include <cstdlib>
#include <cmath>
#include <string>
#include <iostream>

TtCheckResolution::TtCheckResolution(const std::vector<JM::SimTrack*> * true_tracks){
    m_true_tracks = true_tracks;
    m_z_bottom_wp = std::nanf("");
}

TtCheckResolution::~TtCheckResolution() {;}

void TtCheckResolution::SetBottomWP(float z_bottom_WP){
    m_z_bottom_wp = z_bottom_WP;
}

void TtCheckResolution::SetHitNoHitLH(TtLH_HitNoHit * check_HitNoHit){
    m_check_HitNoHit = check_HitNoHit;
}

void TtCheckResolution::check_fit(int npar, double * pars){
    TVector3 fit_pos(pars[0], pars[1], pars[2]);
    TVector3 fit_dir(pars[3], pars[4], pars[5]);

    double lambda_fit_bot_WP = (m_z_bottom_wp - fit_pos.z())/fit_dir.z();
    TVector3 fit_bot_WP = fit_pos + lambda_fit_bot_WP * fit_dir;

    float global_best_dist_at_TT;
    bool first = true;
    for(auto iTrack = m_true_tracks->begin(); iTrack != m_true_tracks->end(); iTrack++){
        if(fabs((*iTrack)->getPDGID()) != 13) continue;
        TVector3 true_pos((*iTrack)->getInitX (), (*iTrack)->getInitY (), (*iTrack)->getInitZ ());
        TVector3 true_dir((*iTrack)->getInitPx(), (*iTrack)->getInitPy(), (*iTrack)->getInitPz());

        double lambda_true_bot_WP = (m_z_bottom_wp - true_pos.z())/true_dir.z();
        TVector3 true_bot_WP = true_pos + lambda_true_bot_WP * true_dir;

        double lambda_true_at_TT = (fit_pos.z() - true_pos.z())/true_dir.z();
        TVector3 true_pos_at_TT = true_pos + lambda_true_at_TT * true_dir;

        float this_op_angle = true_dir.Angle(fit_dir);
        float this_dist_WP  = (true_bot_WP - fit_bot_WP).Mag();
        float this_dist_at_TT = (true_pos_at_TT - fit_pos).Mag();

        if(first){
            first=false;
            m_opening_angle = this_op_angle;
            m_distance_bottom_WP = this_dist_WP;
            global_best_dist_at_TT = this_dist_at_TT;
        }
        else if(global_best_dist_at_TT > this_dist_at_TT){
            // For more than one track try idenfifying with the one closest at center of TT
            m_opening_angle = this_op_angle;
            m_distance_bottom_WP = this_dist_WP;
            global_best_dist_at_TT = this_dist_at_TT;
        }
    }
}

int TtCheckResolution::getTopoType(){
    int topoType = TtLH_HitNoHit::kUnknown;
    const int npars = 6;
    double params[npars];
    int iMu=0;
    for(auto iTrack = m_true_tracks->begin(); iTrack != m_true_tracks->end(); iTrack++){
        if(fabs((*iTrack)->getPDGID()) != 13) continue;
        params[0] = (*iTrack)->getInitX ();
        params[1] = (*iTrack)->getInitY ();
        params[2] = (*iTrack)->getInitZ ();
        params[3] = (*iTrack)->getInitPx();
        params[4] = (*iTrack)->getInitPy();
        params[5] = (*iTrack)->getInitPz();

        m_check_HitNoHit->check_fit(npars,params);
        topoType = topoType | (m_check_HitNoHit->getTopoType() << (TtLH_HitNoHit::ktopoBits *iMu));
    }
    return topoType;
}


// vim: expandtab shiftwidth=4 softtabstop=4 tabstop=4
