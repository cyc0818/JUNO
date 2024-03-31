#include "RelationFinder.hh"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "EvtNavigator/NavBuffer.h"
#include "EvtNavigator/EvtNavHelper.h"
#include "RootWriter/RootWriter.h"
#include "BufferMemMgr/IDataMemMgr.h"
#include "Geometry/RecGeomSvc.h"
#include "Identifier/CdID.h"
#include "TMath.h"
#include "TTree.h"

#include "Event/SimHeader.h"
#include <fstream>

DECLARE_ALGORITHM(RelationFinder);

RelationFinder::RelationFinder(const std::string& name)
    : AlgBase(name)
    , m_simheader(NULL)
    , m_simevent(NULL)
    , m_evt_tree(NULL)
    , m_npe(NULL)
{
    Total_num_PMT = 17612;
    declProp("OutputGeom", m_output_geom_enable=false);

    declProp("PhotonsOnly", m_photons_only=false);

    declProp("CountingOnly", m_counting_only=false);
}

RelationFinder::~RelationFinder()
{

}

bool
RelationFinder::initialize()
{
    if (m_photons_only) {
        LogInfo << "Photons mode enabled." << std::endl;
    }
    // = load the geometry info of PMTs =
    // == Reconstruction Geometry service ==
    SniperPtr<RecGeomSvc> rgSvc(getParent(), "RecGeomSvc"); 
    if ( rgSvc.invalid()) {
        LogError << "Failed to get RecGeomSvc instance!" << std::endl;
        return false;
    }
    CdGeom* m_cdGeom = rgSvc->getCdGeom(); 

    Total_num_PMT = m_cdGeom->findPmt20inchNum();
    LogInfo << "Total 20inch PMT number: " << Total_num_PMT << std::endl;
    // Get the geometry service for all PMT;    
    for(int i = 0; i<Total_num_PMT; i++){
        unsigned int all_pmt_id = (unsigned int)i;
        Identifier all_id = Identifier(CdID::id(all_pmt_id,0));
     //   std::cout<<all_id<<std::endl;
        PmtGeom *all_pmt = m_cdGeom->getPmt(all_id);
        if ( !all_pmt ) {
            LogError << "Wrong Pmt Id " << i << std::endl;
        }
        TVector3 all_pmtCenter = all_pmt->getCenter();
        ALL_PMT_pos.push_back(all_pmtCenter);
        

    } 


    // = RootWriter to save the info = 
    SniperPtr<RootWriter> svc(getParent(), "RootWriter");
    if (svc.invalid()) {
        LogError << "Can't Locate RootWriter. If you want to use it, please "
                 << "enalbe it in your job option file."
                 << std::endl;
        return false;
    }
    m_evt_tree = svc->bookTree(*m_par, "SIMEVT/relation", "relation");
    m_evt_tree->Branch("r", &r_vertex, "r/F");
    m_evt_tree->Branch("theta", &theta, "theta/F");
    m_evt_tree->Branch("hittime", &hittime, "hittime/F");

    m_npe = svc->bookTree(*m_par, "SIMEVT/nperelation", "nperelation");
    m_npe->Branch("r", &r_vertex, "r/F");
    m_npe->Branch("theta", &theta, "theta/F");
    m_npe->Branch("nPE", &npe, "nPE/I");

    // = save the geom =
    if (m_output_geom_enable) {
        LogInfo << "save the geometry info into root file" << std::endl;
        m_geom = svc->bookTree(*m_par, "GEOM/geom", "position of PMT");
        m_geom->Branch("pmtid", &m_geom_id, "pmtid/I");
        m_geom->Branch("pmttype", &m_geom_type, "pmttype/I"); // 20 or 3
        m_geom->Branch("x", &m_geom_x, "x/F");
        m_geom->Branch("y", &m_geom_y, "y/F");
        m_geom->Branch("z", &m_geom_z, "z/F");
        
        for (int i = 0; i < Total_num_PMT; ++i) {
            m_geom_id = i;
            if (i < m_cdGeom->findPmt20inchNum()) {
                m_geom_type = 20;
            } else {
                m_geom_type = 3;
            }
            TVector3& all_pmtCenter = ALL_PMT_pos[i];
            m_geom_x = all_pmtCenter.x();
            m_geom_y = all_pmtCenter.y();
            m_geom_z = all_pmtCenter.z();

            m_geom->Fill();
        }
    }

    entry = 0;

    return true;
}

bool
RelationFinder::execute()
{
    ++entry;
    // = load the sim event = 
    if (not load_sim_event()) {
        LogWarn << "can't load any sim event" << std::endl;
        throw StopRunProcess();
    }
    // only counting, don't do any caculation.
    if (m_counting_only) {
        debug_counting();
        return true;
    }
    // = calculate the edep position =
    calculate_edep_pos();
    // = loop all hits, calculate the theta =
    // FIXME: is that necessary to caculate all pmts first?
    calculate_theta();
    calculate_theta_hits(); 
    return true;
}

bool
RelationFinder::finalize()
{
    LogInfo << "total events: " << entry << std::endl;
    return true;
}

bool
RelationFinder::load_sim_event()
{
    SniperDataPtr<JM::NavBuffer>  navBuf(getParent(), "/Event");
    if (navBuf.invalid()) {
        LogError << "Can't get navBuf." << std::endl;
        return false;
    }
    LogDebug << "navBuf: " << navBuf.data() << std::endl;
    auto evt_nav = navBuf->curEvt();
    LogDebug << "evt_nav: " << evt_nav << std::endl;
    if (not evt_nav) {
        LogError << "Can't get event navigator" << std::endl;
        return false;
    }
    const TTimeStamp& timestamp = evt_nav->TimeStamp();
    LogDebug << "Timestamp: " << timestamp << std::endl;
    m_simheader = JM::getHeaderObject<JM::SimHeader>(evt_nav);
    LogDebug << "simhdr: " << m_simheader << std::endl;
    if (not m_simheader) {
        LogError << "Can't get sim header" << std::endl;
        return false;
    }
    m_simevent = dynamic_cast<JM::SimEvt*>(m_simheader->event());
    LogDebug << "simevt: " << m_simevent << std::endl;
    if (not m_simevent) {
        LogError << "Can't get sim event" << std::endl;
        return false;
    }

    return true;
}

bool
RelationFinder::calculate_edep_pos()
{
    const auto& tracks = m_simevent->getTracksVec();
    float edep = 0;
    float edep_x = 0; 
    float edep_y = 0;
    float edep_z = 0;
    // magic here, try to determine the tracks are photons
    if (!m_photons_only) {
        std::vector<JM::SimTrack*>::const_iterator it = tracks.begin();

        if ((*it)->getPDGID() == 20022) {
            LogInfo << "photons detected, using photon mode!" << std::endl;
            m_photons_only = true;
        }

    }

    if (m_photons_only) {
        // optical photons
        std::vector<JM::SimTrack*>::const_iterator it = tracks.begin();
        m_edep_x = (*it)->getInitX();
        m_edep_y = (*it)->getInitY();
        m_edep_z = (*it)->getInitZ();
    } else {
        // gamma or electrons
        for (std::vector<JM::SimTrack*>::const_iterator it = tracks.begin();
                it != tracks.end(); ++it) {
            edep += (*it)->getEdep();
            edep_x += (*it)->getEdepX() * (*it)->getEdep();
            edep_y += (*it)->getEdepY() * (*it)->getEdep();
            edep_z += (*it)->getEdepZ() * (*it)->getEdep();
        }
        m_edep_x = edep_x / edep;
        m_edep_y = edep_y / edep;
        m_edep_z = edep_z / edep;
    } 

    m_vertex = TVector3(m_edep_x, m_edep_y, m_edep_z);
    r_vertex = TMath::Sqrt(m_edep_x*m_edep_x
                         + m_edep_y*m_edep_y
                         + m_edep_z*m_edep_z); 

    return true;
}

bool
RelationFinder::calculate_theta()
{
    ALL_PMT_theta.clear();
    ALL_PMT_npe.clear();

    for(int i = 0; i<Total_num_PMT; ++i){
        TVector3& all_pmtCenter = ALL_PMT_pos[i];
        float theta = all_pmtCenter.Angle(m_vertex);
        ALL_PMT_theta.push_back(theta);
        ALL_PMT_npe.push_back(0);
    }
    return true;
}

bool
RelationFinder::calculate_theta_hits()
{
    const auto& hits = m_simevent->getCDHitsVec();
    for (std::vector<JM::SimPMTHit*>::const_iterator it = hits.begin();
            it != hits.end(); ++it) {
        int pmtid = (*it)->getPMTID();
        // TODO: only calculate 20inch PMT
        if (pmtid > 30000) {
            continue;
        }
        hittime = (*it)->getHitTime();
        theta = ALL_PMT_theta[pmtid]; 
        m_evt_tree->Fill();

        assert(pmtid < Total_num_PMT);
        // count the nPE per PMT
        ++ALL_PMT_npe[pmtid];
    }

    for (int i = 0; i < Total_num_PMT; ++i) {
        theta = ALL_PMT_theta[i];
        npe = ALL_PMT_npe[i];
        m_npe->Fill();
    }

    return true;
}

bool
RelationFinder::debug_counting()
{
    const auto& tracks = m_simevent->getTracksVec();
    // magic here, try to determine the tracks are photons
    if (!m_photons_only) {
        std::vector<JM::SimTrack*>::const_iterator it = tracks.begin();

        if ((*it)->getPDGID() == 20022) {
            LogInfo << "photons detected, using photon mode!" << std::endl;
            m_photons_only = true;
        }

    }
    if (m_photons_only) {
        LogDebug << "generated photons " << tracks.size() << std::endl;
    }
    return true;
}
