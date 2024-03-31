#include "TtMake3D.h"

#include <map>
#include <set>
#include <vector>
#include <algorithm>

#include "Identifier/Identifier.h"
#include "Identifier/TtID.h"

TtMake3D::TtMake3D(TTGeomSvc* tt_geom_svc){
    m_tt_geom_svc = tt_geom_svc;
}

TtMake3D::~TtMake3D(){
}

struct points_3D * TtMake3D::makePoint(const std::list<JM::CalibTtChannel*> & tmp_calibttcol,std::set<int> * CTFlag){
    struct points_3D * pts3D = new struct points_3D;
    pts3D->n3D = 0;
    pts3D->ntouchedwalls = 0;
    map_wall_pos * hitWall = new map_wall_pos;
    Identifier channelID;
    //loop on hits from data model and set threshold if not done already

    for (auto ithit = tmp_calibttcol.begin(); ithit != tmp_calibttcol.end(); ++ithit){
        auto tthit = *ithit;

        channelID=TtID::id(tthit->pmtId());

        Int_t WallID  = TtID::wall_id(channelID);
        Int_t PMTID   = TtID::pmt   (channelID);
        Int_t stripID = TtID::strip (channelID);

        Int_t le_PMTID = TtID::lower_pmt_index(PMTID);

        Int_t chID_lePMT    = TtID::getIntID(WallID, le_PMTID, stripID);

        if(CTFlag->find(chID_lePMT) != CTFlag->end()) continue;

        double tthit_x = m_tt_geom_svc->getChannelPos(channelID,0);
        double tthit_y = m_tt_geom_svc->getChannelPos(channelID,1);
        double tthit_z = m_tt_geom_svc->getChannelPos(channelID,2);

        if(tthit_x<1e9){
            (*hitWall)[WallID].xHitPos.push_back(tthit_x);
            (*hitWall)[WallID].zHitPosX = tthit_z;
        }
        else if(tthit_y<1e9){
            (*hitWall)[WallID].yHitPos.push_back(tthit_y);
            (*hitWall)[WallID].zHitPosY = tthit_z;
        }

    }

    //second loop to merge neighbouring strips
    vec_wall_pos * hitWallClean = new vec_wall_pos;

    for(map_wall_pos::iterator iMWP = hitWall->begin(); iMWP != hitWall->end() ; iMWP++){
        if(iMWP->second.xHitPos.empty()) continue;
        if(iMWP->second.yHitPos.empty()) continue;

        struct wall_positions cleanWall;

        cleanWall.zHitPosX = iMWP->second.zHitPosX;
        cleanWall.zHitPosY = iMWP->second.zHitPosY;

        std::sort(iMWP->second.xHitPos.begin(),iMWP->second.xHitPos.end());
        std::sort(iMWP->second.yHitPos.begin(),iMWP->second.yHitPos.end());

        int n = 0;
        float coord = 0;

        for(std::vector<float>::iterator iPos = iMWP->second.xHitPos.begin(); iPos != iMWP->second.xHitPos.end(); iPos++){
            coord += *iPos;
            n++;
            std::vector<float>::iterator next_iPos = iPos + 1;
            if(next_iPos == iMWP->second.xHitPos.end() || (*next_iPos) - (*iPos) > 30){
                cleanWall.xHitPos.push_back(coord/n);
                coord=0;
                n=0;
            }
        }

        coord=0;
        n=0;

        for(std::vector<float>::iterator iPos = iMWP->second.yHitPos.begin(); iPos != iMWP->second.yHitPos.end(); iPos++){
            coord += *iPos;
            n++;
            std::vector<float>::iterator next_iPos = iPos + 1;
            if(next_iPos == iMWP->second.yHitPos.end() || (*next_iPos) - (*iPos) > 30){
                cleanWall.yHitPos.push_back(coord/n);
                coord=0;
                n=0;
            }
        }
        hitWallClean->push_back(cleanWall);

    }
    delete hitWall;


    //3rd loop to build the 3D points  and do the linear fit

    for(vec_wall_pos::iterator iMWP = hitWallClean->begin(); iMWP != hitWallClean->end() ; iMWP++){

        pts3D->ntouchedwalls++;

        float posZ = (iMWP->zHitPosX + iMWP->zHitPosY)/2.;

        for(std::vector<float>::iterator iX = iMWP->xHitPos.begin(); iX != iMWP->xHitPos.end(); iX++){
            for(std::vector<float>::iterator iY = iMWP->yHitPos.begin(); iY != iMWP->yHitPos.end(); iY++){
                pts3D->x3D.push_back(*iX);
                pts3D->y3D.push_back(*iY);
                pts3D->z3D.push_back(posZ);
                pts3D->n3D++;

                //avoid showers
                if(pts3D->n3D>80)
                {
                    delete hitWallClean;
                    delete pts3D;
                    return NULL;
                }

            }
        }
    }

    delete hitWallClean;

    return pts3D;
}

TtMake3DTime::TtMake3DTime(TTGeomSvc* tt_geom_svc, TtGeom* tt_geom, float time_thresh) : TtMake3D(tt_geom_svc){
    m_tt_geom = tt_geom;
    m_time_threshold = time_thresh;
}

TtMake3DTime::~TtMake3DTime(){
}

void TtMake3DTime::SetTimeThreshold(float time){
    m_time_threshold = time;
}

struct points_3D * TtMake3DTime::makePoint(const std::list<JM::CalibTtChannel*> & tmp_calibttcol,std::set<int> * CTFlag){
    struct points_3D * pts3D = new struct points_3D;
    pts3D->n3D = 0;
    pts3D->ntouchedwalls = 0;
    map_wall_hits * hitWall = new map_wall_hits;
    Identifier channelID;
    //loop on hits from data model and set threshold if not done already

    for (auto ithit = tmp_calibttcol.begin(); ithit != tmp_calibttcol.end(); ++ithit){
        auto tthit = *ithit;

        channelID=TtID::id(tthit->pmtId());
        float hitTime = tthit->time();

        Int_t WallID  = TtID::wall_id(channelID);
        Int_t PMTID   = TtID::pmt   (channelID);
        Int_t stripID = TtID::strip (channelID);

        Int_t le_PMTID = TtID::lower_pmt_index(PMTID);

        Int_t chID_lePMT    = TtID::getIntID(WallID, le_PMTID, stripID);

        if(CTFlag->find(chID_lePMT) != CTFlag->end()) continue;

        if(le_PMTID<4){
            (*hitWall)[WallID].chid_y.push_back(channelID);
            (*hitWall)[WallID].chle_y.push_back(chID_lePMT);
            (*hitWall)[WallID].time_y.push_back(hitTime);
        }
        else{
            (*hitWall)[WallID].chid_x.push_back(channelID);
            (*hitWall)[WallID].chle_x.push_back(chID_lePMT);
            (*hitWall)[WallID].time_x.push_back(hitTime);
        }

    }

    //make 3D points

    for(map_wall_hits::iterator iMWP = hitWall->begin(); iMWP != hitWall->end() ; iMWP++){
        if(iMWP->second.chid_x.empty()) continue;
        if(iMWP->second.chid_y.empty()) continue;


        float posZ = (m_tt_geom_svc->getChannelPos(iMWP->second.chid_x[0],2) + m_tt_geom_svc->getChannelPos(iMWP->second.chid_y[0],2))/2.;

        std::map<int, std::set<int> > accepted_pairs;
        bool first = true;
        for(long unsigned int iX = 0; iX < iMWP->second.chid_x.size(); iX++){
            int ch_x  = iMWP->second.chid_x[iX];
            float t_x = iMWP->second.time_x[iX];
            float posX = m_tt_geom_svc->getChannelPos(ch_x,0);

            TtGeom::ChannelMapCIt ich_x = m_tt_geom->cfind(TtID::id(ch_x));

            for(long unsigned int iY = 0; iY < iMWP->second.chid_y.size(); iY++){
                if(accepted_pairs[iMWP->second.chle_x[iX]].size() > 0){
                    if(accepted_pairs[iMWP->second.chle_x[iX]].find(iMWP->second.chle_y[iY]) != accepted_pairs[iMWP->second.chle_x[iX]].end())
                        continue;
                }
                int ch_y  = iMWP->second.chid_y[iY];
                float t_y = iMWP->second.time_y[iY];
                float posY = m_tt_geom_svc->getChannelPos(ch_y,1);

                TtGeom::ChannelMapCIt ich_y = m_tt_geom->cfind(TtID::id(ch_y));

                TVector3 pos(ich_x->second->getCenter().x(), ich_y->second->getCenter().y(), 0);
                const TVector3 dir(0,0,1);

                float dx = TtGeom::distanceInStrip(ich_x, pos, dir);
                float dy = TtGeom::distanceInStrip(ich_y, pos, dir);

                const float speed_light = 299.792458; // mm/ns
                const float n_index_strip = 1.6; // from simulation
                float t_x_corr = t_x - dx * n_index_strip/speed_light;
                float t_y_corr = t_y - dy * n_index_strip/speed_light;

                // std::cout << "Corrected times : " << t_x_corr << ", " << t_y_corr << " => " << fabs(t_x_corr - t_y_corr) << std::endl;
                if(fabs(t_x_corr - t_y_corr) < m_time_threshold){
                    accepted_pairs[iMWP->second.chle_x[iX]].insert(iMWP->second.chle_y[iY]);

                    pts3D->x3D.push_back(posX);
                    pts3D->y3D.push_back(posY);
                    pts3D->z3D.push_back(posZ);
                    pts3D->n3D++;
                    if(first){
                        first = false;
                        pts3D->ntouchedwalls++;
                    }
                }

                //avoid showers
                if(pts3D->n3D>80)
                {
                    delete hitWall;
                    delete pts3D;
                    return NULL;
                }

            }
        }
    }

    delete hitWall;

    return pts3D;
}

// vim: expandtab shiftwidth=4 softtabstop=4
