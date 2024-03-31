#include "TtLikelihood.h"

#include "Identifier/Identifier.h"
#include "Identifier/TtID.h"

#include <map>
#include <string>
#include <iostream>

TtLikelihoodBase::TtLikelihoodBase(const std::list<JM::CalibTtChannel*> * data){
    m_data = data;
    m_ttGeom = NULL;
}

TtLikelihoodBase::~TtLikelihoodBase() {;}

void TtLikelihoodBase::SetGeometry(TtGeom * ttGeom){
    m_ttGeom = ttGeom;
}

TtLH_HitNoHit::TtLH_HitNoHit(const std::list<JM::CalibTtChannel*> * data) : TtLikelihoodBase(data){
    n_hit_strips=0;
    n_missed_strips=0;
    n_missed1side_strips=0;
    n_noise_strips=0;

    hit_strips   .clear();
    miss1s_strips.clear();
    missed_strips.clear();
    noise_strips .clear();
}

float TtLH_HitNoHit::check_fit(int npar, double * pars){
    n_hit_strips=0;
    n_missed_strips=0;
    n_missed1side_strips=0;
    n_noise_strips=0;

    hit_strips   .clear();
    miss1s_strips.clear();
    missed_strips.clear();
    noise_strips .clear();
    dist_hit_to_pmt.clear();

    TVector3 pos(pars[0], pars[1], pars[2]);
    // As TTTracking sets the z-coordinate of the fit to the center of the TT,
    // by looking for a crossing point at pos + a * dir with a positive will only
    // cross half of the strips. We need therefore to check with pos + a * dir and
    // pos - a * dir, and for that 2 directions are defined for checks.
    TVector3 dir1( pars[3],  pars[4],  pars[5]);
    TVector3 dir2(-pars[3], -pars[4], -pars[5]);

    for(TtGeom::ChannelMapCIt ich = m_ttGeom->cbegin(); ich != m_ttGeom->cend(); ich++){
        bool crossed1 = ich->second->isCrossed(pos,dir1);
        bool crossed2 = ich->second->isCrossed(pos,dir2);
        bool crossed = crossed1 || crossed2;
        if(crossed){
            float distance_in_strip = TtGeom::distanceInStrip(ich, pos, dir1);
            dist_hit_to_pmt[ich->first.getValue()] = distance_in_strip;
        }
        bool ch_has_hit = false;
        double npe = 0;
        std::list<JM::CalibTtChannel*>::const_iterator ichlist;
        for(ichlist = m_data->begin() ; ichlist != m_data->end(); ichlist++){
            if((*ichlist)->pmtId() == ich->first.getValue()){
                npe = (*ichlist)->nPE();
                if(npe > 1./3)
                    ch_has_hit = true;
                break;
            }
        }
        if     ( crossed &&  ch_has_hit){
            // std::cout << "Hit strip " << ich->first.getValue() << " -- " << npe << std::endl;
            hit_strips.push_back(ich->first.getValue());
            n_hit_strips++;
        }
        else if(!crossed &&  ch_has_hit){
            // std::cout << "Noise strip " << ich->first.getValue() << " -- " << npe << std::endl;
            noise_strips.push_back(ich->first.getValue());
            n_noise_strips++;
        }
        else if( crossed && !ch_has_hit){
            unsigned int id_other_end = TtID::id_other_end(ich->first).getValue();
            for(ichlist = m_data->begin() ; ichlist != m_data->end(); ichlist++){
                if((*ichlist)->pmtId() == id_other_end){
                    npe = (*ichlist)->nPE();
                    if(npe > 1./3)
                        ch_has_hit = true;
                    break;
                }
            }
            if(ch_has_hit){
                // std::cout << "Missed strip w/ hit other side " << ich->first.getValue() << std::endl;
                miss1s_strips.push_back(ich->first.getValue());
                n_missed1side_strips++;
            }
            else{
                // TODO: account for more special cases
                // - we get our dimensions from the coating volume, not the bar volume
                // - how to treat 'corner clipping'
                // std::cout << "Missed strip " << ich->first.getValue() << std::endl;
                missed_strips.push_back(ich->first.getValue());
                n_missed_strips++;
            }
        }
    }

    // Note: strip is not truly a missed strip unless a perpendicular strip is
    //       also missed (otherwise hit would be rejected at L1...)
    std::vector<unsigned int>::iterator iMS = missed_strips.begin();
    while(iMS != missed_strips.end()){
        bool has_perp = false;
        int wall= TtID::wall_id(TtID::id(*iMS));
        int pmt = TtID::pmt(TtID::id(*iMS));
        const int * pmt_perp = TtID::pmts_perp(pmt);
        for(int i_pmt_perp = 0; i_pmt_perp < 8; i_pmt_perp++){
            unsigned int this_perp_id = TtID::getIntID(wall, pmt_perp[i_pmt_perp], 0);
            // perp hit can be among existing hits
            std::vector<unsigned int>::iterator iHS;
            for(iHS = hit_strips.begin(); iHS != hit_strips.end(); iHS++){
                int hit_wall= TtID::wall_id(TtID::id(*iHS));
                int hit_pmt = TtID::pmt(TtID::id(*iHS));
                unsigned int hit_id  = TtID::getIntID(hit_wall, hit_pmt, 0);
                if(hit_id == this_perp_id){
                    has_perp = true;
                    break;
                }
            }
            if(has_perp) break;
            // perp hit can be among existing noise
            for(iHS = noise_strips.begin(); iHS != noise_strips.end(); iHS++){
                int hit_wall= TtID::wall_id(TtID::id(*iHS));
                int hit_pmt = TtID::pmt(TtID::id(*iHS));
                unsigned int hit_id  = TtID::getIntID(hit_wall, hit_pmt, 0);
                if(hit_id == this_perp_id){
                    has_perp = true;
                    break;
                }
            }
            if(has_perp) break;
            // perp hit can be among missing hits also
            for(iHS = missed_strips.begin(); iHS != missed_strips.end(); iHS++){
                int hit_wall= TtID::wall_id(TtID::id(*iHS));
                int hit_pmt = TtID::pmt(TtID::id(*iHS));
                unsigned int hit_id  = TtID::getIntID(hit_wall, hit_pmt, 0);
                if(hit_id == this_perp_id){
                    has_perp = true;
                    break;
                }
            }
            if(has_perp) break;
        }
        if(!has_perp){
            // std::cout << "Missed strip " << *iMS << " has no perp counterpart. Removing." << std::endl;
            iMS = missed_strips.erase(iMS);
            n_missed_strips--;
        }
        else{
            iMS++;
        }

    }

    return n_missed_strips;
}

int TtLH_HitNoHit::getTopoType(){
    int topoType = kUnknown;
    std::vector<unsigned int>::iterator iStrip;
    for(iStrip = hit_strips.begin(); iStrip!=hit_strips.end(); iStrip++){
        int column = TtID::column(TtID::id(*iStrip));
        int row    = TtID::row   (TtID::id(*iStrip));
        if(column==1 && row==3) topoType = topoType | kChimney;
        else                    topoType = topoType | kRegular;
    }
    for(iStrip = missed_strips.begin(); iStrip!=missed_strips.end(); iStrip++){
        int column = TtID::column(TtID::id(*iStrip));
        int row    = TtID::row   (TtID::id(*iStrip));
        if(column==1 && row==3) topoType = topoType | kChimney;
        else                    topoType = topoType | kRegular;
    }
    return topoType;
}

std::map<std::string,float> * TtLH_HitNoHit::extra_info(){
    std::map<std::string,float> * results = new std::map<std::string,float>();
    if(results == NULL) return results;
    (*results)["NHit"]    = n_hit_strips;
    (*results)["NMissed"] = n_missed_strips;
    (*results)["NMiss1S"] = n_missed1side_strips;
    (*results)["NNoise"]  = n_noise_strips;
    return results;
}

TtLH_TimeCheck::TtLH_TimeCheck(const std::list<JM::CalibTtChannel*> * data) : TtLikelihoodBase(data){
}

float TtLH_TimeCheck::check_fit(int npar, double * pars){
    time_info.clear();

    TVector3 pos(pars[0], pars[1], pars[2]);
    // As TTTracking sets the z-coordinate of the fit to the center of the TT,
    // by looking for a crossing point at pos + a * dir with a positive will only
    // cross half of the strips. We need therefore to check with pos + a * dir and
    // pos - a * dir, and for that 2 directions are defined for checks.
    TVector3 dir1( pars[3],  pars[4],  pars[5]);
    TVector3 dir2(-pars[3], -pars[4], -pars[5]);

    std::vector<float> red_time;
    for(std::list<JM::CalibTtChannel*>::const_iterator ichlist = m_data->begin() ; ichlist != m_data->end(); ichlist++){
        float npe = (*ichlist)->nPE();
        if(npe <= 1./3) continue;

        unsigned int chID = (*ichlist)->pmtId();
        TtGeom::ChannelMapCIt ich = m_ttGeom->cfind(TtID::id(chID));
        PmtGeom * strip = ich->second;
        bool crossed1 = strip->isCrossed(pos,dir1);
        bool crossed2 = strip->isCrossed(pos,dir2);
        bool crossed = crossed1 || crossed2;
        if(!crossed) continue;

        struct hit_time_info this_time_info;

        this_time_info.hit_time = (*ichlist)->time();

        this_time_info.distance_in_strip = TtGeom::distanceInStrip(ich, pos, dir1);

        this_time_info.distance_muon_to_strip = TtGeom::distanceToStripZ(ich, pos, dir1);

        const float speed_light = 299.792458; // mm/ns
        const float n_index_strip = 1.6; // from simulation
        this_time_info.time_propagation = (this_time_info.distance_in_strip * n_index_strip + this_time_info.distance_muon_to_strip) / speed_light;
        this_time_info.corr_time = this_time_info.hit_time - this_time_info.time_propagation;

        time_info[chID] = this_time_info;
        red_time.push_back(this_time_info.corr_time);
    }

    return TMath::StdDev(red_time.begin(), red_time.end());
}

std::map<std::string,float> * TtLH_TimeCheck::extra_info(){
    std::map<std::string,float> * results = new std::map<std::string,float>();
    if(results == NULL) return results;
    // (*results)["NNoise"]  = n_noise_strips;
    return results;
}

// vim: expandtab shiftwidth=4 softtabstop=4 tabstop=4
