#pragma once
#if defined(WITH_NP) || defined(WITH_G4CXOPTICKS)

#include "schrono.h"
#include "NPFold.h"
#include "S4MaterialPropertyVector.h"
#include "PMTSimParamSvc/PMTSimParamData.h"
#include <CLHEP/Units/SystemOfUnits.h>

struct _PMTSimParamData
{
    static int Load( PMTSimParamData& data, const char* fold ); 

    _PMTSimParamData( PMTSimParamData& data );  
    PMTSimParamData& data ; 

    void populate_LPMT(); 
    void populate_WP(); 
    void populate_SPMT(); 
    void populate_MPT(); 
    void populate_CONST(); 
    void populate_QEshape(); 
    void populate();

    void save(const char* fold) const ; 
    int  load(const char* fold);

    NPFold* serialize() const ; 
    int import(const NPFold* f ); 
    NPFold* serialize_QEshape() const ; 
    void import_QEshape( const NPFold* QEshape ); 
    NP* serialize_pmtTotal() const ; 
    void import_pmtTotal(const NP* kv ); 

    std::string desc() const ; 

    template<typename T>
    static NP* Scan_pmtid_qe(const T* provider, unsigned num_energy, bool dump); 

}; 


inline int _PMTSimParamData::Load( PMTSimParamData& data, const char* fold )  // static 
{
    _PMTSimParamData _pspd(data);
    int rc = _pspd.load(fold) ; 
    return rc ; 
}

inline _PMTSimParamData::_PMTSimParamData( PMTSimParamData& data_ )
    :
    data(data_)
{
}

inline void _PMTSimParamData::populate_LPMT()
{
    data.m_PmtTotal_ALL = 0 ; 
    data.m_PmtTotal = 100 ; 
    data.m_PmtTotal_ALL += data.m_PmtTotal ;

    for(int i=0 ; i < data.m_PmtTotal ; i++)
    {
        int id = kOFFSET_CD_LPMT  + i ; 

        double pde = 0.4 ; 

        double gain = 0.4 ; 
        double sigmaGain = 0.4 ; 
        double afterPulseProb = 0.4 ; 
        double prePulseProb = 0.4 ; 
        double darkRate = 0.4 ; 
        double timeSpread = 0.4 ; 
        double timeOffset = 0.4 ; 

        PmtSimData_LPMT lpmt(id, pde, gain, sigmaGain, afterPulseProb, prePulseProb, darkRate, timeSpread, timeOffset); 
        data.pd_vector.push_back(lpmt); 

        data.m_all_pmtID.push_back(id) ; 
        data.m_all_pmtID_qe_scale.push_back(0.5) ; 

        int cat = id % 4 ;  

        data.m_map_pmt_category[id] = cat ; 
        data.m_all_pmt_category[id] = cat ;
        data.m_all_pmt_catvec.push_back(cat) ;  
    }
}
inline void _PMTSimParamData::populate_WP()
{
    data.m_PmtTotal_WP = 200 ; 
    data.m_PmtTotal_ALL += data.m_PmtTotal_WP ;

    for(int i=0 ; i < data.m_PmtTotal_WP ; i++)
    {
        int id = kOFFSET_WP_PMT + i ; 
        int cat = id % 4 ;  

        data.m_all_pmtID.push_back(id) ; 
        data.m_all_pmtID_qe_scale.push_back(0.5) ; 
        data.m_all_pmt_category[id] = cat ; 
        data.m_all_pmt_catvec.push_back(cat) ;  
    }
}

inline void _PMTSimParamData::populate_SPMT()
{
    data.m_PmtTotal_SPMT = 300 ; 
    data.m_PmtTotal_ALL += data.m_PmtTotal_SPMT ;
 
    for(int i=0 ; i < data.m_PmtTotal_SPMT ; i++)
    {
        int id = kOFFSET_CD_SPMT + i ; 

        double QE = 0.5 ; 
        double efficiency = 0.5 ; 

        double gain = 0.5 ; 
        double sigmaGain = 0.5 ; 
        double afterPulseProb = 0.5 ; 
        double prePulseProb = 0.5 ; 
        double darkRate = 0.5 ; 
        double timeSpread = 0.5 ; 
        double timeOffset = 0.5 ; 
        PmtSimData_SPMT spmt(id, QE, efficiency, gain, sigmaGain, afterPulseProb, prePulseProb, darkRate, timeSpread, timeOffset) ; 
        data.pd_map_SPMT[id] = spmt ; 

        int cat = id % 4 ;  
        data.m_all_pmtID.push_back(id) ; 
        data.m_all_pmtID_qe_scale.push_back(0.5) ; 
        data.m_all_pmt_category[id] = cat ; 
        data.m_all_pmt_catvec.push_back(cat) ;
    }
}

inline void _PMTSimParamData::populate_MPT()
{
    std::map<int, std::map<std::string, G4MaterialPropertyVector*>>& MPT = data.m_PMT_MPT;
    MPT[0]["ARC_RINDEX"] = S4MaterialPropertyVector::Make_V(0.5) ; 
    MPT[0]["ARC_KINDEX"] = S4MaterialPropertyVector::Make_V(0.6) ; 

    MPT[1]["ARC_RINDEX"] = S4MaterialPropertyVector::Make_V(1.5) ; 
    MPT[1]["ARC_KINDEX"] = S4MaterialPropertyVector::Make_V(1.6) ; 
}

inline void _PMTSimParamData::populate_CONST()
{
    std::map<int, std::map<std::string, G4double>>& CONST = data.m_PMT_CONST ;

    CONST[0]["HELLO"] = 10. ; 
    CONST[0]["WORLD"] = 100. ; 

    CONST[1]["HELLO"] = 20. ; 
    CONST[1]["WORLD"] = 200. ; 
}

inline void _PMTSimParamData::populate_QEshape()
{
    std::vector<std::string> names ; 
    PMTSimParamData::Get_QEshape_Names(names); 
    for(unsigned i=0 ; i < names.size() ; i++)
    {
        const char* key = names[i].c_str(); 
        G4MaterialPropertyVector* mpv = S4MaterialPropertyVector::Make_V(0.5) ; 
        data.setQEshapeMPV(key, mpv); 
    }
}

inline void _PMTSimParamData::populate()
{
    populate_LPMT(); 
    populate_WP(); 
    populate_SPMT(); 
    populate_MPT(); 
    populate_CONST(); 
    populate_QEshape(); 
}

inline void _PMTSimParamData::save(const char* fold ) const 
{
    NPFold* f = serialize(); 
    f->save(fold);  
}
inline int _PMTSimParamData::load(const char* fold )
{
    NPFold* f = NPFold::Load(fold) ; 
    int rc = import(f);

    if( rc != 0 ) std::cerr 
        << "_PMTSimParamData::load"
        << " ERROR WHILE LOADING FROM " << ( fold ? fold : "-" )
        << std::endl 
        ; 

    return rc ;  
}

inline NPFold* _PMTSimParamData::serialize() const 
{
    NP* pmtID = NPX::ArrayFromVec<int, int>(data.m_all_pmtID) ; 
    NP* qeScale = NPX::ArrayFromVec<double,double>(data.m_all_pmtID_qe_scale) ; 
    NP* lpmtCat = NPX::ArrayFromMap<int, int>(data.m_map_pmt_category) ; 
    NP* pmtCat = NPX::ArrayFromDiscoMap<int>(data.m_all_pmt_category) ; 
    NP* pmtCatVec = NPX::ArrayFromVec<int, int>(data.m_all_pmt_catvec) ; 

    NP* spmtData = NPX::ArrayFromMap<double, PmtSimData_SPMT>(data.pd_map_SPMT) ; 
    NP* lpmtData = NPX::ArrayFromVec<double, PmtSimData_LPMT>(data.pd_vector) ; 
    NP* pmtTotal = serialize_pmtTotal(); 

    NPFold* MPT = S4MaterialPropertyVector::Serialize_MIMSV(data.m_PMT_MPT); 
    NPFold* CONST = NPFold::Serialize_MIMSD(data.m_PMT_CONST); 
    NPFold* QEshape = serialize_QEshape() ; 
    

    NPFold* f = new NPFold ; 

    f->add("pmtID", pmtID ); 
    f->add("qeScale", qeScale ); 
    f->add("lpmtCat", lpmtCat ); 
    f->add("pmtCat", pmtCat ); 
    f->add("pmtCatVec", pmtCatVec ); 
    f->add("spmtData", spmtData ); 
    f->add("lpmtData", lpmtData ); 
    f->add("pmtTotal", pmtTotal ); 

    f->add_subfold("MPT", MPT) ; 
    f->add_subfold("CONST", CONST); 
    f->add_subfold("QEshape", QEshape ); 

    return f ; 
}

inline int _PMTSimParamData::import(const NPFold* f )
{
    if( f == nullptr) return 2 ; 

    const NP* pmtID = f->get("pmtID"); 
    const NP* qeScale = f->get("qeScale"); 
    const NP* lpmtCat = f->get("lpmtCat"); 
    const NP* pmtCat = f->get("pmtCat"); 
    const NP* pmtCatVec = f->get("pmtCatVec"); 
    const NP* spmtData = f->get("spmtData"); 
    const NP* lpmtData = f->get("lpmtData"); 
    const NP* pmtTotal = f->get("pmtTotal"); 


    NPFold* MPT = f->get_subfold("MPT") ; 
    NPFold* CONST = f->get_subfold("CONST") ; 
    NPFold* QEshape = f->get_subfold("QEshape") ; 


    bool incomplete = pmtID == nullptr || 
                      qeScale == nullptr || 
                      lpmtCat == nullptr || 
                      pmtCat == nullptr || 
                      pmtCatVec == nullptr || 
                      spmtData == nullptr || 
                      lpmtData == nullptr || 
                      pmtTotal == nullptr ||
                      MPT == nullptr ||
                      CONST == nullptr ||
                      QEshape == nullptr 
                      ;


    if( incomplete ) std::cerr 
         << "_PMTSimParamData::import"
         << " ERROR INCOMPLETE "
         << " probably are trying to load wrong NPFold directory "
         << std::endl 
         ;

    if(incomplete) return 1 ; 

    NPX::VecFromArray<int>(data.m_all_pmtID, pmtID ); 
    NPX::VecFromArray<double>(data.m_all_pmtID_qe_scale, qeScale ); 
    NPX::MapFromArray<int>(data.m_map_pmt_category, lpmtCat ); 
    NPX::DiscoMapFromArray<int>(data.m_all_pmt_category, pmtCat ); 
    NPX::VecFromArray<int>(data.m_all_pmt_catvec, pmtCatVec ); 
    NPX::MapFromArray<PmtSimData_SPMT>(data.pd_map_SPMT, spmtData); 
    NPX::VecFromArray<PmtSimData_LPMT>(data.pd_vector, lpmtData); 
    import_pmtTotal(pmtTotal); 
      
    S4MaterialPropertyVector::Import_MIMSV( data.m_PMT_MPT, MPT ); 
    NPFold::Import_MIMSD( data.m_PMT_CONST, CONST ); 
    import_QEshape(QEshape); 

    return 0 ; 
}

inline NPFold* _PMTSimParamData::serialize_QEshape() const 
{
    NPFold* QEshape = new NPFold ; 

    std::vector<std::string> names ; 
    PMTSimParamData::Get_QEshape_Names(names); 
    for(unsigned i=0 ; i < names.size() ; i++)
    {
        const char* key = names[i].c_str(); 
        G4MaterialPropertyVector* mpv = data.getQEshapeMPV(key);  
        if(mpv == nullptr) std::cerr 
            << "_PMTSimParamData::serialize_QEshape"
            << " FATAL : mpv null "
            << " key: " << key 
            << std::endl 
            ;
        assert(mpv); 
        QEshape->add(key, S4MaterialPropertyVector::ConvertToArray(mpv));  
    }
    return QEshape ; 
}
inline void _PMTSimParamData::import_QEshape( const NPFold* QEshape )
{
    std::vector<std::string> names ; 
    PMTSimParamData::Get_QEshape_Names(names); 
    for(unsigned i=0 ; i < names.size() ; i++)
    {
        const char* key = names[i].c_str(); 
        const NP* a = QEshape->get(key); 
        G4MaterialPropertyVector* mpv = S4MaterialPropertyVector::FromArray(a) ; 
        data.setQEshapeMPV(key, mpv); 
    }
}

inline NP* _PMTSimParamData::serialize_pmtTotal() const
{
    NPX::KV<int> kv ; 
    kv.add("PmtTotal",      data.m_PmtTotal); 
    kv.add("PmtTotal_SPMT", data.m_PmtTotal_SPMT );  
    kv.add("PmtTotal_WP",   data.m_PmtTotal_WP );  
    kv.add("PmtTotal_ALL",  data.m_PmtTotal_ALL );  
    return kv.values() ; 
}
inline void _PMTSimParamData::import_pmtTotal(const NP* pmtTotal )
{
    data.m_PmtTotal      = pmtTotal->get_named_value<int>("PmtTotal",      -1) ; 
    data.m_PmtTotal_SPMT = pmtTotal->get_named_value<int>("PmtTotal_SPMT", -1) ; 
    data.m_PmtTotal_WP   = pmtTotal->get_named_value<int>("PmtTotal_WP",   -1) ; 
    data.m_PmtTotal_ALL  = pmtTotal->get_named_value<int>("PmtTotal_ALL",  -1) ; 
}






inline std::string _PMTSimParamData::desc() const
{
    std::stringstream ss ; 
    ss << "_PMTSimParamData::desc" << std::endl ; 
    ss << S4MaterialPropertyVector::Desc_MIMSV(data.m_PMT_MPT)   << std::endl ; 
    ss << NPFold::Desc_MIMSD(data.m_PMT_CONST) << std::endl ; 
    std::string str = ss.str(); 
    return str ; 
}


template<typename T>
inline NP* _PMTSimParamData::Scan_pmtid_qe(const T* provider, unsigned num_energy, bool dump) // static 
{
    schrono::TP t0 = schrono::stamp();

    const std::vector<int>& all_pmtID = provider->get_all_pmtID(); 
    unsigned num_pmt = all_pmtID.size(); 

    double en0 = 1.55*CLHEP::eV ; 
    double en1 = 15.5*CLHEP::eV ; 
    if(dump) std::cerr << "_PMTSimParamData::Scan_pmtid_qe num_pmt " << num_pmt << std::endl ; 


    NP* a = NP::Make<double>( num_pmt, num_energy ); 
    double* aa = a->values<double>() ; 
    for(unsigned i=0 ; i < num_pmt ; i++)
    {
        int pmtid = all_pmtID[i] ; 

        if(dump) std::cerr << "_PMTSimParamData::Scan_pmtid_qe pmtid " << pmtid << std::endl ; 

        for(unsigned j=0 ; j < num_energy ; j++)
        {
            double fr = double(j)/double(num_energy-1) ; 
            double en = en0*(1.-fr) + en1*fr ; 
            int idx = i*num_energy + j ;  
            double qe = provider->get_pmtid_qe( pmtid, en ) ; 

            if(dump && j % 10 == 0) std::cerr << "_PMTSimParamData::Scan_pmtid_qe "
                   << " fr " << std::setw(10) << std::scientific << fr
                   << " en " << std::setw(10) << std::scientific << en 
                   << " qe " << std::setw(10) << std::scientific << qe
                   << std::endl 
                   ;  

            aa[idx] = qe ;   
        }
    }
    schrono::TP t1 = schrono::stamp();
    double dt = schrono::duration(t0, t1 ); 
    a->set_meta<double>("scantime", dt ); 

    return a ; 
}


#endif

