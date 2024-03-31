#pragma once
/**
PMTSimParamData.h : Providing self contained access to PMT data
================================================================

NB the idea is to provide an API for the PMT data as needed by junoPMTOpticalModel 
NOT to duplicate the entire PMTSimParamSvc API. 


This PMTSimParamData.h struct is used as the data member of PMTSimParamSvc which 
gets populated at instanciation of PMTSimParamSvc from file or database or frontier etc..
The data is accessible from the Svc with::

    PMTSimParamData* PMTSimParamSvc::getPMTSimParamData() 

This enables decoupled and self contained access to the same PMT data 
that PMTSimParamSvc provides but without needing to include PMTSimParamSvc.h 
and a "kitchensink" of headers which that requires including many Sniper 
and ROOT headers which are entirely irrelevant to accessing the PMT data.   

Note that the mishmash of different member names used here is done 
in order to match the PMTSimParamSvc references to precisely the 
same data. 

Note this is an example of separation of concerns : many 
headers that are relevant to populating the PMT data are 
not relevant to accessing that data. 

**/

#include <cassert>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

#include "Geometry/PMT.h"
#include "Geometry/PMTCategory.h"
#include "PMTSimParamSvc/PmtSimData_LPMT.h"
#include "PMTSimParamSvc/PmtSimData_SPMT.h"
#include "G4MaterialPropertyVector.hh"


struct PMTSimParamData
{
    std::vector<int>               m_all_pmtID;
    std::vector<double>            m_all_pmtID_qe_scale ; 
    std::map<int, int>             m_map_pmt_category ; // NB not all, only LPMT
    std::map<int, int>             m_all_pmt_category ;  
    std::vector<int>               m_all_pmt_catvec ;  

    int                            m_PmtTotal_SPMT ; 
    std::map<int, PmtSimData_SPMT> pd_map_SPMT;

    int                            m_PmtTotal ;  // _LPMT
    std::vector<PmtSimData_LPMT>   pd_vector;

    int                            m_PmtTotal_ALL ; 
    int                            m_PmtTotal_WP ; 



    G4MaterialPropertyVector* m_QEshape_HZC;
    G4MaterialPropertyVector* m_QEshape_NNVT;
    G4MaterialPropertyVector* m_QEshape_NNVT_HiQE;
    G4MaterialPropertyVector* m_QEshape_R12860;
    G4MaterialPropertyVector* m_QEshape_WP_PMT;

    static constexpr const char* QEshape_Names = "QEshape_HZC QEshape_NNVT QEshape_NNVT_HiQE QEshape_R12860 QEshape_WP_PMT" ; 

    std::map<int, std::map<std::string, G4MaterialPropertyVector*>> m_PMT_MPT;
    std::map<int, std::map<std::string, double>> m_PMT_CONST;

    // primary 
    const std::vector<int>&   get_all_pmtID() const ;  
    double                    get_pmtcat_const_prop(int pmtcat, const std::string& prop_name) const ; 
    G4MaterialPropertyVector* get_pmtcat_mpv(   int pmtcat, const std::string& prop_name ) const ; 
    double                    get_pmtcat_prop(  int pmtcat, const std::string& prop_name, double val) const ; 

    // methods needed for qe 
    int                       get_pmtcat_slowly(int pmtid) const ; 
    int                       get_pmtcat(int pmtid) const ; 

    int                       get_num_lpmt() const ; 
    int                       get_num_spmt() const ; 
    int                       get_num_wpmt() const ; 
    int                       get_num_pmt() const ; 

    double                    get_pmtid_qe(int pmtid, double energy) const ; 

    double                    get_pmtcat_qe(int pmtcat, double energy) const ; 
    double                    get_qescale(int pmtid) const ; 
    int                       get_pmtContiguousIndex_slowly(int pmtid) const ; 
    int                       get_pmtContiguousIndex(int pmtid) const ; 

    G4MaterialPropertyVector* get_pmtcat_qe_vs_energy(int pmtcat) const ; 

    // machinery used for serialize/import from _PMTSimParamData
    static void Get_QEshape_Names(std::vector<std::string>& names); 
    G4MaterialPropertyVector* getQEshapeMPV(const char* key) const ; 
    void setQEshapeMPV( const char* key, G4MaterialPropertyVector* mpv ); 
    std::string desc() const ;
    std::string descTotal() const ; 

};

inline const std::vector<int>& PMTSimParamData::get_all_pmtID() const
{
    return m_all_pmtID ;
}

inline double PMTSimParamData::get_pmtcat_const_prop(int pmtcat, const std::string& prop_name) const 
{
    return m_PMT_CONST.at(pmtcat).at(prop_name) ; 
}
inline G4MaterialPropertyVector* PMTSimParamData::get_pmtcat_mpv( int pmtcat, const std::string& prop_name ) const
{
    return m_PMT_MPT.at(pmtcat).at(prop_name) ;
}
inline double PMTSimParamData::get_pmtcat_prop( int pmtcat, const std::string& prop_name, double val) const
{
    const G4MaterialPropertyVector* mpv = get_pmtcat_mpv(pmtcat, prop_name) ; 
    return mpv ? mpv->Value(val) : -1. ; 
}

// methods needed for qe 

inline int PMTSimParamData::get_pmtcat_slowly(int pmtid) const 
{
    return m_all_pmt_category.at(pmtid); 
}
inline int PMTSimParamData::get_pmtcat(int pmtid) const 
{
    int idx = get_pmtContiguousIndex(pmtid);  
    return m_all_pmt_catvec[idx]; 
}




inline int PMTSimParamData::get_num_lpmt() const 
{
    return m_PmtTotal ; 
}
inline int PMTSimParamData::get_num_spmt() const 
{
    return m_PmtTotal_SPMT ; 
}
inline int PMTSimParamData::get_num_wpmt() const 
{
    return m_PmtTotal_WP ; 
}
inline int PMTSimParamData::get_num_pmt() const 
{
    return m_PmtTotal_ALL ; 
}


/**
PMTSimParamData::get_pmtid_qe
------------------------------

Switching from using m_all_pmt_category to m_all_pmt_catvec
improves speedup factor of the all PMT scan from 3.43 to 25.91::

          .               IPMTSimParamSvc :       7.84 s 
                          PMTSimParamData :       2.28 s 
          IPMTSimParamSvc/PMTSimParamData :       3.43 

                          IPMTSimParamSvc :       7.68 s 
                          PMTSimParamData :       0.30 s 
          IPMTSimParamSvc/PMTSimParamData :      25.91 
**/

inline double PMTSimParamData::get_pmtid_qe(int pmtid, double energy) const 
{
    int idx = get_pmtContiguousIndex(pmtid) ; 
    int cat = m_all_pmt_catvec[idx] ; 
    double qe = get_pmtcat_qe(cat, energy);
    double qe_scale = m_all_pmtID_qe_scale[idx] ; 
    qe *= qe_scale ;
    assert(qe > 0 && qe < 1);
    return qe ; 
}

inline double PMTSimParamData::get_pmtcat_qe(int cat, double energy) const 
{
    G4MaterialPropertyVector* vec = get_pmtcat_qe_vs_energy(cat);
    double qe = vec->Value(energy);
    return qe;
}

inline double PMTSimParamData::get_qescale(int pmtid) const
{
    int idx = get_pmtContiguousIndex(pmtid) ; 
    return m_all_pmtID_qe_scale[idx] ;  
}

inline int PMTSimParamData::get_pmtContiguousIndex_slowly(int pmtid) const
{
    const std::vector<int>& a = m_all_pmtID ;
    size_t idx = std::distance( a.begin(), std::find( a.begin(), a.end(), pmtid )) ;
    return idx < a.size() ? int(idx) : -1 ;
}

/**
PMTSimParamData::get_pmtContiguousIndex
---------------------------------------

::

    pmtid
    |
    |                          SPMT 
    |
    + kOFFSET_CD_SPMT=300000
    |
    |                          WPMT    
    |
    + kOFFSET_WP_PMT=30000
    |  
    |                          LPMT 
    |
    + kOFFSET_CD_LPMT=0 


**/
inline int PMTSimParamData::get_pmtContiguousIndex(int pmtid) const
{
    int idx = -1 ;  
    if(PMT::IsCD(pmtid) and PMT::Is20inch(pmtid)) {
        idx = pmtid ;    // LPMT 
    } else if(PMT::IsWP(pmtid)) {
        idx = pmtid - kOFFSET_WP_PMT  + m_PmtTotal ;  // WPMT 
    } else if (PMT::IsCD(pmtid) and PMT::Is3inch(pmtid)){
        idx = pmtid - kOFFSET_CD_SPMT + m_PmtTotal + m_PmtTotal_WP ;  // SPMT  
    }

    return idx ;     
}


inline G4MaterialPropertyVector* PMTSimParamData::get_pmtcat_qe_vs_energy(int pmtcat) const 
{
    G4MaterialPropertyVector * vec = 0 ;
    switch(pmtcat)
    {   
        case kPMT_Unknown:     vec = m_QEshape_WP_PMT    ; break ;
        case kPMT_NNVT:        vec = m_QEshape_NNVT      ; break ;
        case kPMT_Hamamatsu:   vec = m_QEshape_R12860    ; break ;
        case kPMT_HZC:         vec = m_QEshape_HZC       ; break ;
        case kPMT_NNVT_HighQE: vec = m_QEshape_NNVT_HiQE ; break ;
    }
    if(vec == nullptr) std::cerr 
        << "PMTSimParamData::get_pmtcat_qe_vs_energy"
        << " INVALID pmtcat "
        << " pmtcat " << pmtcat 
        << " PMTCategory::Name(pmtcat) " <<  ( PMTCategory::Name(pmtcat) ? PMTCategory::Name(pmtcat) : "-" )
        << std::endl 
        ; 

    assert(vec);
    return vec;
}

inline void PMTSimParamData::Get_QEshape_Names(std::vector<std::string>& names) // static
{
    std::stringstream ss(QEshape_Names); 
    std::string str;
    while (std::getline(ss, str, ' ')) names.push_back(str) ; 
}

inline G4MaterialPropertyVector* PMTSimParamData::getQEshapeMPV(const char* key) const 
{
    G4MaterialPropertyVector* mpv = nullptr ; 
    if(     strcmp(key, "QEshape_HZC")==0)       mpv = m_QEshape_HZC ; 
    else if(strcmp(key, "QEshape_NNVT")==0)      mpv = m_QEshape_NNVT ; 
    else if(strcmp(key, "QEshape_NNVT_HiQE")==0) mpv = m_QEshape_NNVT_HiQE ; 
    else if(strcmp(key, "QEshape_R12860")==0)    mpv = m_QEshape_R12860 ; 
    else if(strcmp(key, "QEshape_WP_PMT")==0)    mpv = m_QEshape_WP_PMT ; 
    return mpv ; 
}
inline void PMTSimParamData::setQEshapeMPV(const char* key, G4MaterialPropertyVector* mpv )
{
    if(     strcmp(key, "QEshape_HZC")==0)       m_QEshape_HZC = mpv ; 
    else if(strcmp(key, "QEshape_NNVT")==0)      m_QEshape_NNVT = mpv ; 
    else if(strcmp(key, "QEshape_NNVT_HiQE")==0) m_QEshape_NNVT_HiQE = mpv ; 
    else if(strcmp(key, "QEshape_R12860")==0)    m_QEshape_R12860 = mpv ; 
    else if(strcmp(key, "QEshape_WP_PMT")==0)    m_QEshape_WP_PMT = mpv ; 
}

inline std::string PMTSimParamData::desc() const 
{
    std::stringstream ss ; 
    ss << "PMTSimParamData::desc" 
       << std::endl 
       << std::setw(30) 
       << " m_all_pmtID.size " << m_all_pmtID.size()
       << std::endl 
       << std::setw(30) 
       << " m_all_pmtID_qe_scale.size " << m_all_pmtID_qe_scale.size()
       << std::endl 
       << std::setw(30) 
       << " m_map_pmt_category.size " << m_map_pmt_category.size()
       << std::endl 
       << std::setw(30) 
       << " m_all_pmt_category.size " << m_all_pmt_category.size() 
       << std::endl 
       << std::setw(30) 
       << " pd_map_SPMT.size " << pd_map_SPMT.size()
       << std::endl 
       << std::setw(30) 
       << " pd_vector.size " << pd_vector.size()
       << std::endl 
       ;

    std::string str = ss.str(); 
    return str ; 
}


inline std::string PMTSimParamData::descTotal() const 
{
    std::stringstream ss ; 
    ss << "PMTSimParamData::descTotal" 
       << std::endl
       << std::setw(30) << " m_PmtTotal        " << m_PmtTotal << std::endl
       << std::setw(30) << " pd_vector.size    " << pd_vector.size() << std::endl
       << std::setw(30) << " m_PmtTotal_SPMT   " << m_PmtTotal_SPMT << std::endl 
       << std::setw(30) << " pd_map_SPMT.size  " << pd_map_SPMT.size() << std::endl
       << std::setw(30) << " m_PmtTotal_WP     " << m_PmtTotal_WP << std::endl 
       << std::setw(30) << " m_PmtTotal_ALL    " << m_PmtTotal_ALL << std::endl 
       << std::setw(30) << " m_all_pmtID.size  " << m_all_pmtID.size() << std::endl
       << std::setw(30) << " pd_vector[0].pmtId() " << pd_vector[0].pmtId() << std::endl 
       << std::endl
       ;    

    std::string str = ss.str(); 
    return str ; 
}




// NB : not a member function 
inline std::ostream& operator<<(std::ostream& os, const PMTSimParamData& v )
{
    os << std::endl << v.desc() ; 
    int edge = 10 ; 
    int num_lpmt = v.pd_vector.size() ;  

    os << "pd_vector num_lpmt " << num_lpmt << std::endl ; 
    for(int i=0 ; i < num_lpmt ; i++)
    {
        const PmtSimData_LPMT& lpmt = v.pd_vector[i] ;  
        if(i < edge || (i > num_lpmt - edge) ) os << lpmt << std::endl ; 
    } 

    typedef std::map<int,PmtSimData_SPMT> ISPMT ; 
    const ISPMT& mp = v.pd_map_SPMT ; 
    typename ISPMT::const_iterator it = mp.begin()  ;
    int num_spmt = mp.size() ;  

    os << "pd_map_SPMT num_spmt " << num_spmt << std::endl ; 
    for(int i=0 ; i < num_spmt ; i++)
    {
        int key = it->first ; 
        const PmtSimData_SPMT& spmt = it->second ; 
        if(i < edge || i > num_spmt - edge ) 
        {
            os 
                << std::setw(10) << i 
                << " : "
                << std::setw(10) << key 
                << " : " 
                << spmt 
                << std::endl 
                ; 
        }
        std::advance(it, 1); 
    } 

    int num_all_pmtID = v.m_all_pmtID.size() ; 
    os << "num_all_pmtID " << num_all_pmtID  << std::endl ; 

    for(int i=0 ; i < num_all_pmtID ; i++)
    {
        if( i < edge || i > num_all_pmtID - edge )
        {
            os << std::setw(10) << i << ":" << std::setw(10) << v.m_all_pmtID[i]  << std::endl ; 
        }
    }


    typedef std::map<int, int> MII ; 
    const MII& mcat = v.m_map_pmt_category ;
    typename MII::const_iterator jt = mcat.begin()  ;

    int num_pmt_category = mcat.size() ; 

    os << "num_pmt_category " << num_pmt_category << std::endl ; 
     
    for(int i=0 ; i < num_pmt_category ; i++)
    {
        if( i < edge || i > num_pmt_category - edge )
        {
            os << std::setw(10) << i 
               << " : " 
               << std::setw(10) << jt->first
               << " : " 
               << std::setw(10) << jt->second
               << std::endl
               ; 
        }
        std::advance(jt, 1); 
    }

    return os;  
}

