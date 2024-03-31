/*
 * Calculate fit quality based on *all* hits and not just the fitted hits.
 *
 * Author: J. P. A. M. de Andre <jpandre@iphc.cnrs.fr>
 */

#include "Event/SimHeader.h"
#include "Event/SimTrack.h"
#include "Event/TtCalibHeader.h"
#include "Event/TtRecHeader.h"

#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "BufferMemMgr/IDataMemMgr.h"

#include "Geometry/RecGeomSvc.h"
#include "Identifier/Identifier.h"
#include "Identifier/TtID.h"

#include "TtCheckResolution.h"
#include "TtPostFitChecksAlg.h"
#include "TtLikelihood.h"

#include "Geometry/RecGeomSvc.h"
#include "Geometry/TtGeom.h"

#include "SniperKernel/AlgFactory.h"
#include "RootWriter/RootWriter.h"

#include <sstream>

DECLARE_ALGORITHM(TtPostFitChecksAlg);

TtPostFitChecksAlg::TtPostFitChecksAlg(const std::string& name)
    :AlgBase(name)
{
    // Set Default Values of Some Parameters

    m_evt_id = -1;
    m_compute_resolution = false;

    declProp("ComputeResolution", m_compute_resolution);

}

TtPostFitChecksAlg::~TtPostFitChecksAlg()
{

}

bool TtPostFitChecksAlg::initialize()
{
    // Get Data Navigator
    SniperDataPtr<JM::NavBuffer> navBuf(getParent(), "/Event");
    if( navBuf.invalid() )
    {
        LogError << "cannot get the NavBuffer @ /Event" << std::endl;
        return false;
    }
    m_buf = navBuf.data();

    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc");
    if(rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    m_ttGeom = rgSvc->getTtGeom();

    m_tt_height = m_ttGeom->getBoundingDz();
    m_wp_height = rgSvc->getWpGeom()->getWpDz();

    SniperPtr<RootWriter> svc(getParent(), "RootWriter");
    if (svc.invalid()) {
         LogError << "Can't Locate RootWriter. If you want to use it, please "
                  << "enable it in your job option file."
                  << std::endl;
         return false;
    }
    m_tree_TTQuality = svc->bookTree(*m_par, "USER_OUTPUT/TT_Quality", "TT Postfit");
    m_tree_TTQuality->Branch("evtID", &m_eventID, "evtID/I");
    m_tree_TTQuality->Branch("NTracks",&m_NTracks,"NTracks/I");
    m_tree_TTQuality->Branch("Rank_MissTime",&m_Rank_MissTime,"Rank_MissTime[NTracks]/I");
    m_tree_TTQuality->Branch("Rank_TimeOnly",&m_Rank_TimeOnly,"Rank_TimeOnly[NTracks]/I");
    m_tree_TTQuality->Branch("NPtsInTrack"  ,&m_PtsInTrack   ,"NPtsInTrack[NTracks]/I"  );
    m_tree_TTQuality->Branch("EvtTopoType"  ,&m_evt_topo     ,"EvtTopoType[NTracks]/I"  );
    m_tree_TTQuality->Branch("NHitStrips"   ,&m_NHitStrips   ,"NHitStrips[NTracks]/I"   );
    m_tree_TTQuality->Branch("NMissedStrips",&m_NMissedStrips,"NMissedStrips[NTracks]/I");
    m_tree_TTQuality->Branch("NMiss1SStrips",&m_NMiss1SStrips,"NMiss1SStrips[NTracks]/I");
    m_tree_TTQuality->Branch("NNoiseStrips" ,&m_NNoiseStrips ,"NNoiseStrips[NTracks]/I" );
    m_tree_TTQuality->Branch("CorrTimeRMS"  ,&m_TimeRMS      ,"CorrTimeRMS[NTracks]/F" );
    if(m_compute_resolution){
        m_tree_TTQuality->Branch("AngularRes",&m_AngularRes  ,"AngularRes[NTracks]/F" );
        m_tree_TTQuality->Branch("DistWPRes" ,&m_DistWPRes   ,"DistWPRes[NTracks]/F"  );
        m_tree_TTQuality->Branch("TrueEvtTopoType",&m_true_evt_topo,"TrueEvtTopoType/I");
    }

    m_tree_TTMissedStrips = svc->bookTree(*m_par, "USER_OUTPUT/TT_MissedStrips", "TT missed strips info");
    m_tree_TTMissedStrips->Branch("evtID", &m_eventID, "evtID/I");
    m_tree_TTMissedStrips->Branch("trackNum", &m_trackNum,"trackNum/I");
    m_tree_TTMissedStrips->Branch("NMissed", &m_NMissed,"NMissed/I");
    m_tree_TTMissedStrips->Branch("chID", &m_missed_ChID,"chID[NMissed]/i");

    m_tree_evaluate_distance = svc->bookTree(*m_par, "USER_OUTPUT/TT_DistCross", "TT Distance hit - pmt");
    m_tree_evaluate_distance->Branch("evtID", &m_eventID, "evtID/I");
    m_tree_evaluate_distance->Branch("trackNum", &m_trackNum,"trackNum/I");
    m_tree_evaluate_distance->Branch("NCrossed", &m_nCrossed, "NCrossed/I");
    m_tree_evaluate_distance->Branch("chID", &m_ChID,"chID[NCrossed]/i");
    m_tree_evaluate_distance->Branch("type", &m_type,"type[NCrossed]/i");
    m_tree_evaluate_distance->Branch("dist", &m_dist,"dist[NCrossed]/F");

    m_tree_timeinfo = svc->bookTree(*m_par, "USER_OUTPUT/TT_TimeInfo", "TT Time info for crossed hits");
    m_tree_timeinfo->Branch("evtID", &m_eventID, "evtID/I");
    m_tree_timeinfo->Branch("trackNum", &m_trackNum,"trackNum/I");
    m_tree_timeinfo->Branch("NHitCrossed", &m_nHitCrossed, "NHitCrossed/I");
    m_tree_timeinfo->Branch("chID", &m_ChID_HX,"chID[NHitCrossed]/i");
    m_tree_timeinfo->Branch("distInStrip", &m_distInStrip,"distInStrip[NHitCrossed]/F");
    m_tree_timeinfo->Branch("distToStrip", &m_distToStrip,"distToStrip[NHitCrossed]/F");
    m_tree_timeinfo->Branch("timeProp", &m_timeProp,"timeProp[NHitCrossed]/F");
    m_tree_timeinfo->Branch("hitTime", &m_hitTime,"hitTime[NHitCrossed]/F");
    m_tree_timeinfo->Branch("corrTime", &m_corrTime,"corrTime[NHitCrossed]/F");

    LogInfo << objName() << " initialized successfully." << std::endl;
    return true;
}

bool TtPostFitChecksAlg::execute()
{
    m_evt_id++;

    LogDebug << "---------------------------------------" << std::endl;
    LogDebug << "Processing event " << m_evt_id << std::endl;

    // Get Input Data

    auto nav = m_buf->curEvt();

    auto calib_header = JM::getHeaderObject<JM::TtCalibHeader>(nav);
    if (not calib_header) return true;

    auto calib_evt  = calib_header->event();
    if (not calib_evt) return true;

    const auto& calib_ttch = calib_evt->calibTtCol();

    auto rec_header = JM::getHeaderObject<JM::TtRecHeader>(nav);
    if (not rec_header) return true;

    auto rec_event = rec_header->event();
    if (not rec_event) return true;

    std::vector<JM::SimTrack*> true_tracks;
    if(m_compute_resolution){
        auto sim_header = dynamic_cast<JM::SimHeader*>(nav->getHeader("/Event/SimOrig"));
        if(!sim_header){
            LogError << "/Event/SimOrig not available. Cannot get resolution! Abort!" << std::endl;
            throw;
        }
        auto sim_event = dynamic_cast<JM::SimEvt*>(sim_header->event());
        if (!sim_event) return false;
        true_tracks = sim_event->getTracksVec();
    }

    // Process data

    TtLH_HitNoHit check_HitNoHit(&calib_ttch);
    check_HitNoHit.SetGeometry(m_ttGeom);

    TtLH_TimeCheck check_time(&calib_ttch);
    check_time.SetGeometry(m_ttGeom);

    TtCheckResolution check_resolution(&true_tracks);
    check_resolution.SetBottomWP(-m_wp_height);
    check_resolution.SetHitNoHitLH(&check_HitNoHit);
    if(m_compute_resolution){
        m_true_evt_topo = check_resolution.getTopoType();
    }

    // std::cout << "Event " << m_evt_id << std::endl;
    // for(std::list<JM::CalibTTChannel*>::const_iterator ittch = calib_ttch.begin(); ittch!=calib_ttch.end(); ittch++){
        // std::cout << "Ch " << (*ittch)->pmtId() << std::endl;
    // }
    m_eventID = m_evt_id;
    m_NTracks = rec_event->nTracks();
    for(int i=0; i< m_NTracks; i++){
        LogDebug << "Fit" <<
            " " << rec_event->Coeff0()[i] <<
            " " << rec_event->Coeff1()[i] <<
            " " << rec_event->Coeff2()[i] <<
            " " << rec_event->Coeff3()[i] <<
            " " << rec_event->Coeff4()[i] <<
            " " << rec_event->Coeff5()[i] <<
            std::endl;
        const int npar = 6;
        double params[npar];
        // Need to do a coordinate change as fitting in TTTracking done in TT coordinates
        // rather than using JUNO coordinates.
        const double z_global_shift = m_wp_height + m_tt_height;
        params[0] = rec_event->Coeff0()[i];
        params[1] = rec_event->Coeff1()[i];
        params[2] = rec_event->Coeff2()[i] + z_global_shift;
        params[3] = rec_event->Coeff3()[i];
        params[4] = rec_event->Coeff4()[i];
        params[5] = rec_event->Coeff5()[i];
        m_PtsInTrack[i] = rec_event->nPoints()[i];

        if(m_compute_resolution){
            check_resolution.check_fit(npar, params);
            m_AngularRes[i] = check_resolution.GetOpeningAngle();
            m_DistWPRes [i] = check_resolution.GetDistanceBottomWP();
            LogDebug << "True-Reco: angle " << m_AngularRes[i] << " distWP " << m_DistWPRes[i] << std::endl;
        }

        check_HitNoHit.check_fit(npar,params);
        std::map<std::string,float> * extra = check_HitNoHit.extra_info();
        for(std::map<std::string,float>::iterator ie = extra->begin(); ie != extra->end(); ie++){
            LogDebug << ie->first << " => " << ie->second << std::endl;
        }
        m_NHitStrips   [i] = (*extra)["NHit"];
        m_NMissedStrips[i] = (*extra)["NMissed"];
        m_NMiss1SStrips[i] = (*extra)["NMiss1S"];
        m_NNoiseStrips [i] = (*extra)["NNoise"];
        delete extra;

        m_evt_topo     [i] = check_HitNoHit.getTopoType();

        m_trackNum = i;
        if(m_NMissedStrips[i]>0){
            assert(m_NMissedStrips[i] < MAX_NMISSED);
            m_NMissed = m_NMissedStrips[i];
            std::vector<unsigned int> * missedStrips = check_HitNoHit.getMissedStrips();
            for(int j = 0; j < m_NMissed; j++){
                m_missed_ChID[j] = (*missedStrips)[j];
            }
            m_tree_TTMissedStrips->Fill();
        }
        m_nCrossed = m_NMissedStrips[i] + m_NHitStrips[i] + m_NMiss1SStrips[i];
        if(m_nCrossed>0){
            int j=0;
            std::map<unsigned int, float> * dist_map = check_HitNoHit.getDistHit2PMT();

            std::vector<unsigned int> * strips = check_HitNoHit.getHitStrips();
            for(int k=0; k<m_NHitStrips[i]; k++){
                m_ChID[j] = (*strips)[k];
                m_type[j] = 0;
                m_dist[j] = (*dist_map)[m_ChID[j]];
                j++;
            }

            strips = check_HitNoHit.getMiss1SStrips();
            for(int k=0; k<m_NMiss1SStrips[i]; k++){
                m_ChID[j] = (*strips)[k];
                m_type[j] = 1;
                m_dist[j] = (*dist_map)[m_ChID[j]];
                j++;
            }

            strips = check_HitNoHit.getMissedStrips();
            for(int k=0; k<m_NMissedStrips[i]; k++){
                m_ChID[j] = (*strips)[k];
                m_type[j] = 2;
                m_dist[j] = (*dist_map)[m_ChID[j]];
                j++;
            }

            m_tree_evaluate_distance->Fill();
        }


        m_TimeRMS[i] = check_time.check_fit(npar,params);
        LogDebug << "Time RMS => " << m_TimeRMS[i] << std::endl;

        m_nHitCrossed = 0;
        auto * time_info_map = check_time.getTimeInfo();
        for(auto iTIM = time_info_map->begin(); iTIM != time_info_map->end(); iTIM++){
            m_ChID_HX    [m_nHitCrossed] = iTIM->first;
            m_distInStrip[m_nHitCrossed] = iTIM->second.distance_in_strip;
            m_distToStrip[m_nHitCrossed] = iTIM->second.distance_muon_to_strip;
            m_timeProp   [m_nHitCrossed] = iTIM->second.time_propagation;
            m_hitTime    [m_nHitCrossed] = iTIM->second.hit_time;
            m_corrTime   [m_nHitCrossed] = iTIM->second.corr_time;
            m_nHitCrossed++;
        }
        m_tree_timeinfo->Fill();


    }

    for(int iTrk_1=0; iTrk_1 < m_NTracks; iTrk_1++){
        m_Rank_MissTime[iTrk_1] = iTrk_1;
        for(int iTrk_2=0; iTrk_2 < iTrk_1; iTrk_2++){
            bool earlier = false;
            if(m_PtsInTrack[iTrk_1] >= 3 && m_PtsInTrack[iTrk_2] < 3) earlier = true;
            if(m_NMissedStrips[iTrk_2] > m_NMissedStrips[iTrk_1]) earlier = true;
            if(m_NMissedStrips[iTrk_2] == m_NMissedStrips[iTrk_1] && m_TimeRMS[iTrk_2] > m_TimeRMS[iTrk_1]) earlier = true;
            if(earlier && m_Rank_MissTime[iTrk_1] > m_Rank_MissTime[iTrk_2]){
                int swp = m_Rank_MissTime[iTrk_1];
                m_Rank_MissTime[iTrk_1] = m_Rank_MissTime[iTrk_2];
                m_Rank_MissTime[iTrk_2] = swp;
            }
            else if(!earlier && m_Rank_MissTime[iTrk_1] < m_Rank_MissTime[iTrk_2]){
                int swp = m_Rank_MissTime[iTrk_1];
                m_Rank_MissTime[iTrk_1] = m_Rank_MissTime[iTrk_2];
                m_Rank_MissTime[iTrk_2] = swp;
            }
        }
    }

    for(int iTrk_1=0; iTrk_1 < m_NTracks; iTrk_1++){
        m_Rank_TimeOnly[iTrk_1] = iTrk_1;
        for(int iTrk_2=0; iTrk_2 < iTrk_1; iTrk_2++){
            bool earlier = false;
            if(m_PtsInTrack[iTrk_1] >= 3 && m_PtsInTrack[iTrk_2] < 3) earlier = true;
            if(m_TimeRMS[iTrk_2] > m_TimeRMS[iTrk_1]) earlier = true;
            if(earlier && m_Rank_TimeOnly[iTrk_1] > m_Rank_TimeOnly[iTrk_2]){
                int swp = m_Rank_TimeOnly[iTrk_1];
                m_Rank_TimeOnly[iTrk_1] = m_Rank_TimeOnly[iTrk_2];
                m_Rank_TimeOnly[iTrk_2] = swp;
            }
            else if(!earlier && m_Rank_TimeOnly[iTrk_1] < m_Rank_TimeOnly[iTrk_2]){
                int swp = m_Rank_TimeOnly[iTrk_1];
                m_Rank_TimeOnly[iTrk_1] = m_Rank_TimeOnly[iTrk_2];
                m_Rank_TimeOnly[iTrk_2] = swp;
            }
        }
    }

    if(m_NTracks > 1){
        std::ostringstream oss1, oss2;
        oss1 << "Track order (Miss/Time)";
        oss2 << "Track order (Time only)";
        for(int iTrk=0; iTrk < m_NTracks; iTrk++){
            oss1 << " " << m_Rank_MissTime[iTrk] << " (" << m_DistWPRes[iTrk]/1000 << "/" << m_AngularRes[iTrk]*180/TMath::Pi() << ")";
            oss2 << " " << m_Rank_TimeOnly[iTrk] << " (" << m_DistWPRes[iTrk]/1000 << "/" << m_AngularRes[iTrk]*180/TMath::Pi() << ")";
        }
        LogDebug << oss1.str() << std::endl;
        LogDebug << oss2.str() << std::endl;
    }


    m_tree_TTQuality->Fill();

    return true;
}



bool TtPostFitChecksAlg::finalize()
{
    LogInfo << objName() << " finalized successfully." << std::endl;
    return true;
}

// vim: expandtab shiftwidth=4 softtabstop=4 tabstop=4
