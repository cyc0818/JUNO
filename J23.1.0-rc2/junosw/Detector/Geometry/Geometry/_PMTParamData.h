#pragma once

#include "PMTParamData.h"
#include "PMT.h"

#if defined(WITH_NP) || defined(WITH_G4CXOPTICKS)
#include "NPFold.h"

struct _PMTParamData
{
    _PMTParamData( PMTParamData& data ); 

    PMTParamData& data ; 

    void populate();  
    void save(const char* fold) const ;
    void load(const char* fold);
    NPFold* serialize() const ;
    void import(const NPFold* f );
};

_PMTParamData::_PMTParamData( PMTParamData& data_ )
    :
    data(data_)
{
}

inline void _PMTParamData::populate()
{
    // dummy testing values
    for(int i=0 ; i < 17612 ; i++) data.m_pmt_categories[i] = i % 4 ; 
    for(int i=kOFFSET_CD_SPMT ; i < kOFFSET_CD_SPMT_END ; i++) data.m_pmt_categories[i] = 5 ; 
}
inline void _PMTParamData::save(const char* fold) const 
{
    NPFold* f = serialize();
    f->save(fold);
}
inline void _PMTParamData::load(const char* fold) 
{
    NPFold* f = NPFold::Load(fold) ;
    import(f);
}
inline NPFold* _PMTParamData::serialize() const 
{
    NPFold* f = new NPFold ;
    f->add("pmtCat", NPX::ArrayFromDiscoMap<int>(data.m_pmt_categories));
    return f ; 
}
inline void _PMTParamData::import( const NPFold* f )
{
    const NP* pmtCat = f->get("pmtCat");
    NPX::DiscoMapFromArray<int>(data.m_pmt_categories, pmtCat );
}
#endif

