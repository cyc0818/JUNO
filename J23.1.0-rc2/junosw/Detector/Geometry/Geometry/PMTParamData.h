#pragma once
#include <map>
#include <ostream>

struct PMTParamData
{
    std::map<int, int> m_pmt_categories ;
};

inline std::ostream& operator<<(std::ostream& os, const PMTParamData& d )
{
    unsigned ni = d.m_pmt_categories.size() ;  
    os 
       << std::endl 
       << "PMTParamData " 
       << " m_pmt_categories.size " << ni
       << std::endl 
       ; 

    unsigned edge = 10 ; 
    typedef std::map<int,int> MII ; 
    MII::const_iterator it = d.m_pmt_categories.begin() ; 

    for(unsigned i=0 ; i < ni ; i++)
    {
        int id = it->first ; 
        int cat = it->second ; 

        if( i < edge || (i > (ni - edge)) )  os
            << " i:" << i  
            << " id:" << id
            << " cat:" << cat
            << std::endl 
            ;
   
        it++ ; 
    }
    return os ; 
}

