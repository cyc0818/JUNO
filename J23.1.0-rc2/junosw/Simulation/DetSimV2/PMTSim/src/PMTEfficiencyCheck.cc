#ifdef WITH_G4OPTICKS

#include "PMTEfficiencyCheck.hh"
#include "junoSD_PMT_v2.hh"

#include "PMTEfficiency.hh"
#include "SPack.hh"
#include "SStr.hh"
#include "SSys.hh"
#include "NPY.hpp"
#include "PLOG.hh"


PMTEfficiencyCheck::PMTEfficiencyCheck(const junoSD_PMT_v2* jpmt)
    :
    m_jpmt(jpmt),
#ifdef SCB_KLUDGE_FIX
    m_basedir("$TMP/PMTEfficiencyCheck/SCB_KLUDGE_FIX/"),
#else
    m_basedir("$TMP/PMTEfficiencyCheck/ASIS/"), 
#endif
    m_dump(SSys::getenvbool("PMTEfficiencyCheck_dump")),
    m_assert_match(SSys::getenvbool("PMTEfficiencyCheck_assert_match")),
    m_record_count(0),
    m_record_limit(SSys::getenvint("PMTEfficiencyCheck_record_limit", 1000000)),
    m_eventID(-1)
  //  m_parcat_mismatch_count(0),
  //  m_simcat_mismatch_count(0)
{
    init(); 
}

PMTEfficiencyCheck::~PMTEfficiencyCheck()
{
}

void PMTEfficiencyCheck::init()
{
    m_volnames.push_back("HamamatsuR12860_PMT_20inch_body_phys"); 
    m_volnames.push_back("NNVTMCPPMT_PMT_20inch_body_phys"); 
    m_volnames.push_back("PMT_3inch_body_phys");
}

/**
PMTEfficiencyCheck::getVolnameIndex
-------------------------------------

returns 0,1,2 when the volname is on the list or one past the end (eg 3) when not found

**/

int PMTEfficiencyCheck::getVolnameIndex(const std::string& volname) const 
{
    const std::vector<std::string>& v = m_volnames;
    return std::distance(v.begin(), std::find(v.begin(), v.end(), volname));
}


/**
PMTEfficiencyCheck::addHitRecord
----------------------------------

Invoked from junoSD_PMT_v2::ProcessHits Simulation/DetSimV2/PMTSim/src/junoSD_PMT_v2.cc  

**/

void PMTEfficiencyCheck::addHitRecord(int pmtID, const G4ThreeVector& global_pos, const G4ThreeVector& local_pos, double qe, double ce, double de, const std::string& volname, int ce_cat )
{
    if(m_record_count == m_record_limit) 
    {
        LOG(info) 
            << " m_eventID " << m_eventID
            << " m_record_count " << m_record_count
            << " m_record_limit " << m_record_limit << " (change limit with envvar : PMTEfficiencyCheck_record_limit) "
            << " record_count reached record_limit " 
            << " invoke SaveRecords "
            ; 

        SaveRecords(m_eventID);  
    } 
    else if ( m_record_count > m_record_limit )
    {
        return ; 
    } 

    if( m_record_count % 10000 == 0 )
    {
        LOG(info) 
            << " m_eventID " << m_eventID
            << " m_record_count " << m_record_count
            ;
    }

    unsigned volIdx = getVolnameIndex(volname);  
    // NB volIdx uses the standard list of volname, not the one being collected below which is in an uncontrolled order 

    if(volIdx > m_volnames.size())
    {
        LOG(fatal) 
            << " UNEXPECTED hit onto an unlisted volume "
            << " volIdx " << volIdx 
            << " volname " << volname 
            ; 
    }


    std::vector<std::string>& u = m_uvolname ; 
    if(std::find(u.begin(), u.end(), volname ) == u.end()) u.push_back(volname); // collect unique volnames 

    double theta = local_pos.theta() ; 
    double qe2 = m_jpmt->getQuantumEfficiency(pmtID); 
    double ce2 = m_jpmt->getCollectionEfficiency(theta, pmtID);  
    double de2 = m_jpmt->getDetectionEfficiency(theta, pmtID); 

    double epsilon = 1e-10 ; 
    bool qe_match = std::abs(qe - qe2) < epsilon ; 
    bool ce_match = std::abs(ce - ce2) < epsilon ; 
    bool de_match = std::abs(de - de2) < epsilon ; 


    if(m_assert_match)
    {
        assert( qe_match );  
        assert( ce_match ); 
        assert( de_match ); 
    }

    // int parcat = m_jpmt->getParCategory(pmtID); 
    // int simcat = m_jpmt->getSimCategory(pmtID); 

    // int parcat_s = PMTEfficiency::ParToSim(parcat);    // translate parcat enum into simcat enum  
    // int simcat_p = PMTEfficiency::SimToPar(simcat);    // translate simcat enum into parcat enum

    // bool parcat_mismatch = parcat != simcat_p ; 
    // bool simcat_mismatch = simcat != parcat_s ; 

    // unsigned mismatch = 
    //     ( unsigned(!qe_match)       << 0 ) | 
    //     ( unsigned(!ce_match)       << 1 ) | 
    //     ( unsigned(!de_match)       << 2 ) |
    //     ( unsigned(parcat_mismatch) << 3 ) |
    //     ( unsigned(simcat_mismatch) << 4 ) 
    //     ;  

    // if(parcat_mismatch) m_parcat_mismatch_count += 1 ; 
    // if(simcat_mismatch) m_simcat_mismatch_count += 1 ; 

    // if(m_dump && (parcat_mismatch || simcat_mismatch))
    // {
    //     G4cout 
    //        << " pmtID " << pmtID
    //        << " parcat " << parcat
    //        << " simcat_p " << simcat_p 
    //        <<  ( parcat_mismatch ? "PARCAT MISMATCH" : "parcat match" )
    //        << " simcat " << simcat
    //        << " parcat_s " << parcat_s
    //        <<  ( simcat_mismatch ? "SIMCAT MISMATCH" : "simcat match" )
    //        << " parcat.name "   << m_jpmt->getParCategoryName(parcat)
    //        << " simcat_p.name " << m_jpmt->getParCategoryName(simcat_p) 
    //        << " simcat.name "   << m_jpmt->getSimCategoryName(simcat)
    //        << " parcat_s.name " << m_jpmt->getSimCategoryName(parcat_s) 
    //        << " mismatch " << std::hex << mismatch << std::dec
    //        << " volname " << volname
    //        << G4endl
    //        ;
    // }

    if(m_dump) std::cout 
        << __FILE__ << " +" << __LINE__ 
#ifdef SCB_KLUDGE_FIX
        << " with SCB_KLUDGE_FIX "
#else
        << " without fix "
#endif
        << " record_count " << m_record_count 
        << " qe " << qe
        << " qe2 " << qe2
        << ( qe_match ? " " : "QE_MISMATCH" )
        << " ce " << ce 
        << " ce2 " << ce2 
        << ( ce_match ? " " : "CE_MISMATCH" )
        << " de " << de 
        << " de2 " << de2 
        << ( de_match ? " " : "DE_MISMATCH" )
        // << " parcat " << parcat 
        // << " simcat " << simcat 
        << " volIdx " << volIdx 
        << " ce_cat " << ce_cat
        << " volname " << volname
        << " epsilon " << epsilon 
        << " theta " << theta
        << " pmtID " << pmtID
        // << " parcat.name " << m_jpmt->getParCategoryName(parcat)
        // << " simcat " << simcat
        // << " simcat.name " << m_jpmt->getSimCategoryName(simcat)
        << " volname " << volname
        << std::endl
        ;


    std::vector<std::string>& n = m_names ; 
    std::vector<double>& d = m_hitrecords ; 
    bool first = m_record_count == 0 ; 
    m_record_count += 1 ; 

    // see ~/j/PMTEfficiencyCheck.sh for analysis

    int parcat = 0 ; 

    d.push_back(SPack::unsigned_as_double(pmtID,parcat));    
    d.push_back(SPack::unsigned_as_double(ce_cat,volIdx));   
    d.push_back(qe);  
    d.push_back(qe2);  
    if(first)
    {
        n.push_back("pmtId_parcat"); 
        n.push_back("cecat_volIdx"); 
        n.push_back("qe"); 
        n.push_back("qe2"); 
    }

    d.push_back(ce);  
    d.push_back(ce2);  
    d.push_back(de);  
    d.push_back(de2);  
    if(first)
    {
        n.push_back("ce"); 
        n.push_back("ce2"); 
        n.push_back("de"); 
        n.push_back("de2"); 
    }

    d.push_back(local_pos.x());  
    d.push_back(local_pos.y());  
    d.push_back(local_pos.z());  
    d.push_back(theta);  
    if(first)
    {
        n.push_back("lposx"); 
        n.push_back("lposy"); 
        n.push_back("lposz"); 
        n.push_back("ltheta"); 
    }


    d.push_back(global_pos.x());  
    d.push_back(global_pos.y());  
    d.push_back(global_pos.z());  
    int simcat = 0 ; 
    int mismatch = 0 ; 
    d.push_back(SPack::unsigned_as_double(simcat,mismatch));    
    if(first)
    {
        n.push_back("gposx"); 
        n.push_back("gposy"); 
        n.push_back("gposz"); 
        n.push_back("simcat_mismatch"); 
    }
}

void PMTEfficiencyCheck::Initialize(int eventID) 
{
    m_eventID = eventID ; 
    m_record_saved = false ; 
}

void PMTEfficiencyCheck::EndOfEvent(int eventID) 
{
    SaveRecords(eventID) ; 
    m_record_saved = false ;  // so the next event records will get saved 
}

void PMTEfficiencyCheck::SaveRecords(int eventID)
{
    std::cout 
        << __FILE__ << " +" << __LINE__ 
        << " SaveRecords " 
        << " eventID " << eventID 
        << " m_eventID " << m_eventID 
        << " m_record_limit " << m_record_limit
        << " m_record_count " << m_record_count
        << " m_record_saved " << m_record_saved
        << " " << ( m_record_saved ? " SKIP AS RECORD SAVED ALREADY " : " PROCEED " ) 
        << std::endl
        ;

    if(m_record_saved) return ; 

    std::cout 
        << __FILE__ << " +" << __LINE__ 
        << " SaveRecords " 
        << " eventID " << eventID 
        << " m_uvolname.size " << m_uvolname.size() 
        << " all volnames hit : "
        << std::endl 
        ;    

 
    const std::vector<std::string>& u = m_uvolname ; 
    for(unsigned i=0 ; i < u.size() ; i++) std::cout << u[i] << std::endl ; 

    const std::vector<double>& d = m_hitrecords ; 
    unsigned entries = d.size() ; 
    assert( entries % 16 == 0 ); 
    unsigned items = entries/16 ; 

    std::cout 
        << __FILE__ << " +" << __LINE__ 
        << " SaveRecords " 
        << " eventID " << eventID 
        << " m_eventID " << m_eventID 
        << " m_hitrecords.size " << m_hitrecords.size()
        << " record_count " << m_record_count  
        << " items " << items 
        //<< " parcat_mismatch_count " << m_parcat_mismatch_count
        //<< " simcat_mismatch_count " << m_simcat_mismatch_count
        << std::endl 
        ;    

    NPY<double>* a = NPY<double>::make(items, 4, 4) ;  
    a->read( d.data() ); 

    const char* path = SStr::Concat(m_basedir, unsigned(eventID), ".npy"); 
    std::cout << __FILE__ << " +" << __LINE__ << " record_count " << m_record_count << " save to " << path << " shape " << a->getShapeString() << std::endl ; 
    a->save(path); 
    delete a ; 

    m_hitrecords.clear(); 

    const char* txtpath = SStr::Concat(m_basedir, unsigned(eventID), ".txt"); 
    const std::vector<std::string>& n = m_names ; 
    std::cout << __FILE__ << " +" << __LINE__ << " record_count " << m_record_count << " save to " << txtpath << " names " << n.size() << std::endl ; 
    SStr::Save(txtpath, n, '\n' );
    m_names.clear(); 

    m_record_count = 0 ; 
    m_record_saved = true ; 
}
#endif

