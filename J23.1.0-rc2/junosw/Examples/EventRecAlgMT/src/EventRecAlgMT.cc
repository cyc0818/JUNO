#include <boost/python.hpp>
#include "EventRecAlgMT.h"

#include "SniperKernel/SniperLog.h"

#include "EvtNavigator/EvtNavigator.h"
#include "Event/CalibHeader.h"
#include "Event/RecHeader.h"

DECLARE_ALGORITHM(EventRecAlgMT);

EventRecAlgMT::EventRecAlgMT(const std::string& name)
    : AlgBase(name) {
    Total_num_PMT = 17612; //new 17612
    PMT_R = 19.434;
    Ball_R = 19.18;
    LS_R = 17.7;
    pmt_r = 0.254;

    m_Pdf_Value = 0;

}

EventRecAlgMT::~EventRecAlgMT() {

}

bool EventRecAlgMT::initialize() {
    m_gbuf = GlobalBuffer::FromStream("GEvtStream");

    // access global geom
    SniperPtr<RecGeomSvcMT> geomSvcMT(getParent(), "RecGeomSvcMT");
    if (geomSvcMT.invalid()) {
        LogError << "Failed to get RecGeomSvcMT." << std::endl;
        return false;
    }

    RecGeomSvc* geom = geomSvcMT->geom();
    if (!geom) {
        LogError << "Failed to get RecGeomSvc." << std::endl;
        return false;
    }
    m_cdGeom = geom->getCdGeom(); 

    Total_num_PMT = m_cdGeom->findPmt20inchNum();
    LogInfo << "Total PMT: " << Total_num_PMT << std::endl;
    //Get PMT position
    for(int i = 0; i<Total_num_PMT; i++){
        unsigned int all_pmt_id = (unsigned int)i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
            continue;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_PMT_pos.push_back(Ball_R/PMT_R*all_pmtCenter);

        // TODO
        TimeSpread[i] = 1.0 * 2.354; //sigma to FWHM
        DarkRate[i]   = 50 * 1000; //kHz
    }

    //Load the hit time likelihood function
    Load_LikeFun();

    return true;
}

bool EventRecAlgMT::execute() {
    // =======================================================================
    // access event
    // =======================================================================
    GlobalBuffer::Elem* elem_in_gbuf = m_gbuf->next();
    if (!elem_in_gbuf) {
        LogError << "Can't find GlobalBuffer::Elem" << std::endl;
        return false;
    }
    EvtNavMT* nav = static_cast<EvtNavMT*>(elem_in_gbuf->dptr);
    if (!nav) {
        LogError << "Can't find EvtNavMT." << std::endl;
        return false;
    }

    JM::CalibHeader* hdr = dynamic_cast<JM::CalibHeader*>(nav->evtnav->getHeader("/Event/Calib"));
    if (!hdr) {
        LogError << "Can't find CalibHeader." << std::endl;
        return false;
    }

    JM::CalibEvent* evt = dynamic_cast<JM::CalibEvent*>(hdr->event());

    const std::list<JM::CalibPMTChannel*>& chhlist = evt->calibPMTCol();

    LogInfo << "size of calib PMT collection: " << chhlist.size() << std::endl;;

    std::list<JM::CalibPMTChannel*>::const_iterator chit = chhlist.begin();

    while (chit!=chhlist.end()) {
        JM::CalibPMTChannel  *calib = *chit++;

        if (!calib) {
            continue;
        }

        unsigned int pmtId = calib->pmtId();
        Identifier id = Identifier(pmtId);

        //only use 20inch pmt
        if (not CdID::is20inch(id)) {
            continue;
        }

        double nPE = calib->nPE();
        double firstHitTime = calib->firstHitTime();

        PMT_HIT[CdID::module(id)] = nPE;
        PMT_HITTIME[CdID::module(id)] = firstHitTime;
        PMT_HITTIMETTS[CdID::module(id)] = TimeSpread[CdID::module(id)];
        PMT_DarkRate[CdID::module(id)] = DarkRate[CdID::module(id)];

    }

    VectorPush();

    // =======================================================================
    // dummy rec
    // =======================================================================
    std::vector<double>::iterator Min_hit_time = min_element(Readout_hit_time.begin(),Readout_hit_time.end());
    min_hit_time = *Min_hit_time;
    LogDebug << "minimum hit time = " << min_hit_time << std::endl;

    num_PMT = PMT_pos.size();
    LogDebug << "numer of Fired PMT = " << num_PMT << std::endl;   

    //Charge Center Reconstruction
    ChargeCenterRecCorr();

    // =======================================================================
    // mark done
    // =======================================================================

    JM::RecHeader* aDataHdr = new JM::RecHeader(); //unit: mm,  MeV, ...
    JM::CDRecEvent* aData = new JM::CDRecEvent();
    aData->setX(m_ccr_x); 
    aData->setY(m_ccr_y); 
    aData->setZ(m_ccr_z); 
    // aData->setX(m_rec_x); 
    // aData->setY(m_rec_y); 
    // aData->setZ(m_rec_z); 
    // aData->setEnergy(m_rec_n);
    // aData->setEprec(m_rec_E); 
    // aData->setPESum(m_pe_sum); 
    // aData->setPx(m_rec_t); 
    // aData->setPy(m_time); 
    // aData->setPz(m_like_vtx); 
    aDataHdr->setCDEvent(aData);
    nav->evtnav->addHeader("/Event/Rec", aDataHdr); 


    m_gbuf->setDone(elem_in_gbuf); 

    VectorClear();

    return true;
}

bool EventRecAlgMT::finalize() {

    return true;
}

bool EventRecAlgMT::Load_LikeFun() {
    if (!getenv("RECTIMELIKEALGROOT")) {
        LogError << "Failed to find envvar $RECTIMELIKEALGROOT. " << std::endl;
        return false;
    }
    std::string base = getenv("RECTIMELIKEALGROOT");
    std::string file_path = base+"/share/";

    return true;
}

void EventRecAlgMT::VectorPush() {  
    m_pe_sum = 0;
    for (int i=0; i<Total_num_PMT; i++){
        if (PMT_HIT[i] > 0) {
            Readout_PE.push_back(PMT_HIT[i]);
            Readout_hit_time.push_back(PMT_HITTIME[i]);
            PMT_TTS.push_back(PMT_HITTIMETTS[i]);

            unsigned int pmtId = (unsigned int)i;
            Identifier id = Identifier(CdID::id(pmtId,0));
            PmtGeom *pmt = m_cdGeom->getPmt(id);
            if ( !pmt ) {
                LogError << "Wrong Pmt Id " << id << std::endl;
            }
            TVector3 pmtCenter = pmt->getCenter();
            PMT_pos.push_back(Ball_R/PMT_R*pmtCenter);
            PMT_ID.push_back(CdID::module(id));

            FirstHitTime_NPE_PMTPos.push_back(
                                              std::make_tuple(PMT_HITTIME[i],
                                                              PMT_HIT[i],
                                                              Ball_R/PMT_R*pmtCenter.x(),
                                                              Ball_R/PMT_R*pmtCenter.y(),
                                                              Ball_R/PMT_R*pmtCenter.z()));

            m_pe_sum += PMT_HIT[i];
        }
    }

}

void EventRecAlgMT::VectorClear() {  
    Readout_PE.clear();
    Readout_hit_time.clear();
    PMT_pos.clear();
    PMT_ID.clear();
    PMT_TTS.clear();
    FirstHitTime_NPE_PMTPos.clear();
}

//////////////////////////////////////////////////////////////////////////////
// Charge Center Reconstruction
//////////////////////////////////////////////////////////////////////////////

bool EventRecAlgMT::ChargeCenterRec()
{
    double x_sum = 0;
    double y_sum = 0;
    double z_sum = 0; 
    double PE_sum = 0;

    for(int i = 0; i < Total_num_PMT; i++){
        if (PMT_HIT[i] <= 0) continue;
        if (PMT_isDNHit[i]) continue;
        x_sum = x_sum + ALL_PMT_pos.at(i).X() * PMT_HIT[i];
        y_sum = y_sum + ALL_PMT_pos.at(i).Y() * PMT_HIT[i];
        z_sum = z_sum + ALL_PMT_pos.at(i).Z() * PMT_HIT[i];
        PE_sum = PE_sum + PMT_HIT[i];
    }
    ChaCenRec_x = x_sum/PE_sum;
    ChaCenRec_y = y_sum/PE_sum;
    ChaCenRec_z = z_sum/PE_sum;

    m_ccr_x = ChaCenRec_x;
    m_ccr_y = ChaCenRec_y;
    m_ccr_z = ChaCenRec_z;

    return true;
}

//Use first 30% hit for Charge Center Rec
bool EventRecAlgMT::ChargeCenterFitter(double percent = 0.3)
{
    double x_sum = 0;
    double y_sum = 0;
    double z_sum = 0;
    double PE_sum = 0;

    sort(FirstHitTime_NPE_PMTPos.begin(), FirstHitTime_NPE_PMTPos.end());

    for(int i = 0; i< num_PMT; i++){
        if (PE_sum > m_pe_sum * percent) break;
        x_sum = x_sum + std::get<2>(FirstHitTime_NPE_PMTPos[i])*std::get<1>(FirstHitTime_NPE_PMTPos[i]);
        y_sum = y_sum + std::get<3>(FirstHitTime_NPE_PMTPos[i])*std::get<1>(FirstHitTime_NPE_PMTPos[i]);
        z_sum = z_sum + std::get<4>(FirstHitTime_NPE_PMTPos[i])*std::get<1>(FirstHitTime_NPE_PMTPos[i]);
        PE_sum = PE_sum + std::get<1>(FirstHitTime_NPE_PMTPos[i]);
    }
    ChaCenRec_x = x_sum/PE_sum;
    ChaCenRec_y = y_sum/PE_sum;
    ChaCenRec_z = z_sum/PE_sum;

    return true;
}

//Corrected Charge Center Rec
bool EventRecAlgMT::ChargeCenterRecCorr()
{
    ChargeCenterRec();

    ChaCenRec_r = sqrt(ChaCenRec_x*ChaCenRec_x+ChaCenRec_y*ChaCenRec_y+ChaCenRec_z*ChaCenRec_z);
    TVector3 vr_ChaCenRec;
    double ChaCenRec_theta;
    double ChaCenRec_phi;
    if (ChaCenRec_r <= 20000){
        if ( m_Pdf_Value ){
            ChaCenRec_r *= 1.45;
        }
        else { 
            ChaCenRec_r *= 1.25;
        }
        vr_ChaCenRec.SetXYZ(ChaCenRec_x,ChaCenRec_y,ChaCenRec_z);
        ChaCenRec_theta = vr_ChaCenRec.Theta();
        ChaCenRec_phi = vr_ChaCenRec.Phi();
    } else {
        ChargeCenterFitter(0.3);
        ChaCenRec_r = sqrt(ChaCenRec_x*ChaCenRec_x+ChaCenRec_y*ChaCenRec_y+ChaCenRec_z*ChaCenRec_z);
        vr_ChaCenRec.SetXYZ(ChaCenRec_x,ChaCenRec_y,ChaCenRec_z);
        ChaCenRec_theta = vr_ChaCenRec.Theta();
        ChaCenRec_phi = vr_ChaCenRec.Phi();
        ChaCenRec_r = (1.24042e+04-5.90834e+03*log((2.01546e+04-ChaCenRec_r)/1.70063e+03)); //10000 - 19500
        ChaCenRec_r *= 1.07143;
    }
    ChaCenRec_x = ChaCenRec_r * TMath::Sin(ChaCenRec_theta) * TMath::Cos(ChaCenRec_phi);
    ChaCenRec_y = ChaCenRec_r * TMath::Sin(ChaCenRec_theta) * TMath::Sin(ChaCenRec_phi);
    ChaCenRec_z = ChaCenRec_r * TMath::Cos(ChaCenRec_theta);

    return true;
}

