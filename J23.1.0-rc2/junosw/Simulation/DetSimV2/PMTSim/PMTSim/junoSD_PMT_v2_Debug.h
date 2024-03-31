#pragma once
/**
junoSD_PMT_v2_Debug.h : ProcessHits debugging and metadata collection
=======================================================================

This headeronly impl is included into the below within WITH_G4CXOPTICKS blocks::

    junoSD_PMT_v2.cc
    junoSD_PMT_v2_Opticks.cc

**/

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#if defined(WITH_G4CXOPTICKS) || defined(WITH_NP)
#include "NP.hh"
#endif



namespace EPH
{
    // enumerate ProcessHits return
    enum { 
       UNSET,        // 0
       NDIS,         // 1
       NOPT,         // 2
       NEDEP,        // 3
       NBOUND,       // 4
       NPROC,        // 5
       NDETECT,      // 6
       NDECULL,      // 7
       YMERGE,       // 8
       YSAVE         // 9
    };

    static constexpr const char* UNSET_   = "EPH_UNSET" ;   // 0
    static constexpr const char* NDIS_    = "EPH_NDIS" ;    // 1
    static constexpr const char* NOPT_    = "EPH_NOPT" ;    // 2
    static constexpr const char* NEDEP_   = "EPH_NEDEP" ;   // 3 
    static constexpr const char* NBOUND_  = "EPH_NBOUND" ;  // 4
    static constexpr const char* NPROC_   = "EPH_NPROC" ;   // 5
    static constexpr const char* NDETECT_ = "EPH_NDETECT" ; // 6 
    static constexpr const char* NDECULL_ = "EPH_NDECULL" ; // 7 
    static constexpr const char* YMERGE_  = "EPH_YMERGE" ;  // 8
    static constexpr const char* YSAVE_   = "EPH_YSAVE" ;   // 9

    static const char* Name(int eph)
    {
        const char* sn = nullptr ; 
        switch(eph)
        {
            case UNSET:   sn = UNSET_    ; break ;  // 0
            case NDIS:    sn = NDIS_     ; break ;  // 1 
            case NOPT:    sn = NOPT_     ; break ;  // 2
            case NEDEP:   sn = NEDEP_    ; break ;  // 3
            case NBOUND:  sn = NBOUND_   ; break ;  // 4
            case NPROC:   sn = NPROC_    ; break ;  // 5
            case NDETECT: sn = NDETECT_  ; break ;  // 6
            case NDECULL: sn = NDECULL_  ; break ;  // 7
            case YMERGE:  sn = YMERGE_   ; break ;  // 8 
            case YSAVE:   sn = YSAVE_    ; break ;  // 9 
        }
        return sn ; 
    }

    static void GetNames(std::vector<std::string>& names)
    {
        names.push_back(Name(UNSET));   // 0
        names.push_back(Name(NDIS));    // 1 
        names.push_back(Name(NOPT));    // 2
        names.push_back(Name(NEDEP));   // 3 
        names.push_back(Name(NBOUND));  // 4 
        names.push_back(Name(NPROC));   // 5
        names.push_back(Name(NDETECT)); // 6 
        names.push_back(Name(NDECULL)); // 7 
        names.push_back(Name(YMERGE));  // 8
        names.push_back(Name(YSAVE));   // 9
    }
}




struct junoSD_PMT_v2_Debug
{
    int ProcessHits_count ;  
    int ProcessHits_true  ;  
    int ProcessHits_false ;  
    int SaveNormHit_count ; 
    int SaveMuonHit_count ; 

    int UNSET ;         // 0
    int NDIS ;          // 1
    int NOPT ;          // 2
    int NEDEP ;         // 3
    int NBOUND ;        // 4
    int NPROC ;         // 5
    int NDETECT ;       // 6
    int NDECULL ;       // 7
    int YMERGE ;        // 8
    int YSAVE ;         // 9

    junoSD_PMT_v2_Debug(); 
    void zero() ; 
    std::string desc() const ; 
    void add(int eph, bool processHits); 
    void get_kvs( std::vector<std::pair<std::string, int>>& kv ) const ; 

#if defined(WITH_G4CXOPTICKS) || defined(WITH_NP)
    NP* get_meta_array( int opticksMode, int eventID ) const ;  // dummy array to carry metadata
#endif

};

inline junoSD_PMT_v2_Debug::junoSD_PMT_v2_Debug()
{
    zero(); 
}
inline void junoSD_PMT_v2_Debug::zero()
{
    ProcessHits_count = 0 ; 
    ProcessHits_true = 0 ;
    ProcessHits_false = 0 ; 
    SaveNormHit_count = 0 ;
    SaveMuonHit_count = 0 ;
    UNSET = 0 ;
    NDIS = 0 ;
    NOPT = 0 ;
    NEDEP = 0 ;
    NBOUND = 0 ;
    NPROC = 0 ;
    NDETECT = 0 ;
    NDECULL = 0 ;
    YMERGE = 0 ;
    YSAVE = 0 ;
}
inline std::string junoSD_PMT_v2_Debug::desc() const 
{
    std::stringstream ss ; 
    ss << "junoSD_PMT_v2_Debug::desc" << std::endl 
       << " ProcessHits_count  " << std::setw(8) << ProcessHits_count << std::endl 
       << " ProcessHits_true   " << std::setw(8) << ProcessHits_true  << std::endl 
       << " ProcessHits_false  " << std::setw(8) << ProcessHits_false << std::endl 
       << " SaveNormHit_count  " << std::setw(8) << SaveNormHit_count << std::endl 
       << " SaveMuonHit_count  " << std::setw(8) << SaveMuonHit_count << std::endl 
       << std::setw(20) << EPH::UNSET_   << std::setw(8) << UNSET << std::endl   // 0 
       << std::setw(20) << EPH::NDIS_    << std::setw(8) << NDIS << std::endl    // 1
       << std::setw(20) << EPH::NOPT_    << std::setw(8) << NOPT << std::endl    // 2
       << std::setw(20) << EPH::NEDEP_   << std::setw(8) << NEDEP << std::endl   // 3
       << std::setw(20) << EPH::NBOUND_  << std::setw(8) << NBOUND << std::endl  // 4
       << std::setw(20) << EPH::NPROC_   << std::setw(8) << NPROC << std::endl   // 5
       << std::setw(20) << EPH::NDETECT_ << std::setw(8) << NDETECT << std::endl // 6
       << std::setw(20) << EPH::NDECULL_ << std::setw(8) << NDECULL << std::endl // 7
       << std::setw(20) << EPH::YMERGE_  << std::setw(8) << YMERGE << std::endl  // 8
       << std::setw(20) << EPH::YSAVE_   << std::setw(8) << YSAVE << std::endl   // 9
       ; 
    std::string str = ss.str(); 
    return str ; 
}
inline void junoSD_PMT_v2_Debug::add(int eph, bool processHits)
{
    ProcessHits_count  += 1 ;  
    ProcessHits_true   += int(processHits) ; 
    ProcessHits_false  += int(!processHits) ; 

    switch(eph)
    {
        case EPH::UNSET:   UNSET   += 1 ; break ;  // 0 
        case EPH::NDIS:    NDIS    += 1 ; break ;  // 1
        case EPH::NOPT:    NOPT    += 1 ; break ;  // 2
        case EPH::NEDEP:   NEDEP   += 1 ; break ;  // 3
        case EPH::NBOUND:  NBOUND  += 1 ; break ;  // 4
        case EPH::NPROC:   NPROC   += 1 ; break ;  // 5
        case EPH::NDETECT: NDETECT += 1 ; break ;  // 6
        case EPH::NDECULL: NDECULL += 1 ; break ;  // 7
        case EPH::YMERGE:  YMERGE  += 1 ; break ;  // 8
        case EPH::YSAVE:   YSAVE   += 1 ; break ;  // 9
    }
}

inline void junoSD_PMT_v2_Debug::get_kvs( std::vector<std::pair<std::string, int>>& kvs ) const 
{
    typedef std::pair<std::string, int> KV ; 
    kvs.push_back(KV("ProcessHits_count", ProcessHits_count)); 
    kvs.push_back(KV("ProcessHits_true", ProcessHits_true)); 
    kvs.push_back(KV("ProcessHits_false", ProcessHits_false)); 
    kvs.push_back(KV("SaveNormHit_count", SaveNormHit_count)); 
    kvs.push_back(KV("SaveMuonHit_count", SaveMuonHit_count)); 
    kvs.push_back(KV("UNSET", UNSET)); 
    kvs.push_back(KV("NDIS", NDIS)); 
    kvs.push_back(KV("NOPT", NOPT)); 
    kvs.push_back(KV("NEDEP", NEDEP)); 
    kvs.push_back(KV("NBOUND", NBOUND)); 
    kvs.push_back(KV("NPROC", NPROC)); 
    kvs.push_back(KV("NDETECT", NDETECT)); 
    kvs.push_back(KV("NDECULL", NDECULL)); 
    kvs.push_back(KV("YMERGE", YMERGE)); 
    kvs.push_back(KV("YSAVE", YSAVE)); 
}



#if defined(WITH_G4CXOPTICKS) || defined(WITH_NP)
inline NP* junoSD_PMT_v2_Debug::get_meta_array( int opticksMode, int eventID ) const 
{
    NP* meta = NP::Make<int>(1) ;  
    EPH::GetNames(meta->names) ; 

    std::vector<std::pair<std::string, int>> kvs ; 
    get_kvs(kvs); 
    meta->set_meta_kv(kvs); 
    meta->set_meta<int>("opticksMode", opticksMode ); 
    meta->set_meta<int>("eventID", eventID ); 
    return meta ; 
}
#endif


