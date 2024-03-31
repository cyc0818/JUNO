#pragma once
/**
PMTAccessor.h : low dependency access to PMT data  
===================================================

This is the full data equivalent to the standalone JPMT.h 
that is based on top of the PMTSimParamData struct that 
can be obtained from PMTSimParamSvc. 

Fortunately due to PMTSimParamData persisting from _PMTSimParamData 
this struct which is intended to be a monolith resident used together
with a CustomG4OpBoundaryProcess can be compared with JPMT.h 
outside the monolith using "PMTAccessor::Create()" without data argument, 
which will cause the data to be loaded from the directory pointed to 
by envvar PMTSimParamData_BASE

See also:

* j/PMTFastSim/tests/PMTAccessorTest.sh 
* u4/tests/U4PMTAccessorTest.sh 

Note:

* G4MaterialPropertiesTable::GetProperty is not const-correct

**/

#include <string>
#include <sstream>
#include <cstdlib>
#include <cassert>

#include "G4Material.hh"

#include "Geometry/PMT.h"
#include "Geometry/PMTCategory.h"
#include "PMTSimParamSvc/_PMTSimParamData.h"
#include "PMTSimParamSvc/PMTSimParamData.h"

#include "C4IPMTAccessor.h"

struct PMTAccessor : public C4IPMTAccessor
{
    static constexpr const char* TypeName = "PMTAccessor" ; 

    const PMTSimParamData* data ; 

    const G4Material*          Pyrex  ; 
    const G4Material*          Vacuum ; 
    G4MaterialPropertiesTable* PyrexMPT ; 
    G4MaterialPropertiesTable* VacuumMPT ; 
    G4MaterialPropertyVector*  PyrexRINDEX ;  
    G4MaterialPropertyVector*  VacuumRINDEX ; 

    static std::string Desc(); 

    static const PMTSimParamData* LoadData(const char* base); 
    static const PMTAccessor* Load(const char* base ); 
    static const PMTAccessor* Create(const PMTSimParamData* data ); 

    PMTAccessor(const PMTSimParamData* data); 
    std::string desc() const ; 


    // C4IPMTAccessor interface
    int    get_num_lpmt() const ; 
    double get_pmtid_qe( int pmtid, double energy ) const ; 
    double get_qescale(  int pmtid ) const ; 
    int    get_pmtcat( int pmtid  ) const ; 
    void   get_stackspec( std::array<double, 16>& ss, int pmtcat, double energy_eV ) const ; 
    const char* get_typename() const ; 

    // HMM: is the argument really energy_eV or energy (factor of 1e-6 between them)
};

inline std::string PMTAccessor::Desc()  // static  
{
    std::stringstream ss ; 
    ss 
        << "PMTAccessor::Desc" 
        << std::endl 
        << "PMTCategory::List() " << PMTCategory::List() 
        << std::endl 
        ; 
    for(int i=kPMT_Unknown ; i <= kPMT_NNVT_HighQE ; i++) ss
        << " PMTCategory::Name(" << std::setw(2) <<  i << ") " 
        << PMTCategory::Name(i) 
        << std::endl 
        ; 
    std::string str = ss.str(); 
    return str ; 
}


inline const PMTAccessor* PMTAccessor::Create(const PMTSimParamData* data )  // static 
{ 
    assert( data ); 
    return new PMTAccessor(data) ; 
}

inline const PMTSimParamData* PMTAccessor::LoadData(const char* base)  // static 
{
    PMTSimParamData* data = nullptr ; 
#if defined(WITH_NP) || defined(WITH_G4CXOPTICKS)
    data = new PMTSimParamData ; 
    if(base) _PMTSimParamData::Load(*data, base) ; 
#else
    std::cerr 
        << "PMTAccessor::Load ERROR"
        << " cannot load persisted PMTSimParamData from base " << ( base ? base : "-" ) 
        << " unless junosw is compiled WITH_NP or WITH_G4CXOPTICKS " 
        << std::endl 
        ;  
#endif
    return data ; 
}

inline const PMTAccessor* PMTAccessor::Load(const char* base )
{
    const PMTSimParamData* data = LoadData(base) ; 
    assert( data ); 
    return Create(data);  
}



inline PMTAccessor::PMTAccessor(const PMTSimParamData* data_ )
    :
    data(data_),
    Pyrex(G4Material::GetMaterial("Pyrex")),
    Vacuum(G4Material::GetMaterial("Vacuum")),
    PyrexMPT(Pyrex ? Pyrex->GetMaterialPropertiesTable() : nullptr),
    VacuumMPT(Vacuum ? Vacuum->GetMaterialPropertiesTable() : nullptr),
    PyrexRINDEX(PyrexMPT ? PyrexMPT->GetProperty("RINDEX") : nullptr),
    VacuumRINDEX(VacuumMPT ? VacuumMPT->GetProperty("RINDEX") : nullptr)
{
}

inline std::string PMTAccessor::desc() const 
{
    std::stringstream ss ; 
    ss << "PMTAccessor::desc"
       << " data " << data 
       << " Pyrex " << ( Pyrex ? "YES" : "NO" ) 
       << " Vacuum " << ( Vacuum ? "YES" : "NO" )  
       << " TypeName " << get_typename()
       ; 
    std::string str = ss.str(); 
    return str ; 
}

inline int PMTAccessor::get_num_lpmt() const 
{
    return data->get_num_lpmt() ; 
}
inline double PMTAccessor::get_pmtid_qe( int pmtid, double energy ) const
{
    return data->get_pmtid_qe(pmtid, energy) ; 
}
inline double PMTAccessor::get_qescale( int pmtid ) const
{
    return data->get_qescale(pmtid) ; 
}

inline int PMTAccessor::get_pmtcat( int pmtid  ) const
{
    return data->get_pmtcat(pmtid) ; 
}
inline void PMTAccessor::get_stackspec( std::array<double, 16>& ss, int pmtcat, double energy_eV ) const
{
    double energy = energy_eV*CLHEP::eV ;  

    ss.fill(0.); 

    ss[4*0+0] = PyrexRINDEX->Value(energy) ; 

    ss[4*1+0] = data->get_pmtcat_prop(       pmtcat, "ARC_RINDEX" , energy );  
    ss[4*1+1] = data->get_pmtcat_prop(       pmtcat, "ARC_KINDEX" , energy );  
    ss[4*1+2] = data->get_pmtcat_const_prop( pmtcat, "ARC_THICKNESS" )/CLHEP::nm ; 

    ss[4*2+0] = data->get_pmtcat_prop(       pmtcat, "PHC_RINDEX" , energy );  
    ss[4*2+1] = data->get_pmtcat_prop(       pmtcat, "PHC_KINDEX" , energy );  
    ss[4*2+2] = data->get_pmtcat_const_prop( pmtcat, "PHC_THICKNESS" )/CLHEP::nm ; 

    ss[4*3+0] = VacuumRINDEX->Value(energy);
}

inline const char* PMTAccessor::get_typename() const 
{ 
    return TypeName ; 
}


