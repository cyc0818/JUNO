#pragma once

#include <string>
#include <sstream>

enum PMT_CATEGORY {
  kPMT_Unknown=-1,
  kPMT_NNVT,
  kPMT_Hamamatsu,
  kPMT_HZC,
  kPMT_NNVT_HighQE
};


struct PMTCategory
{
   static constexpr const char* Unknown     = "kPMT_Unknown" ;   
   static constexpr const char* NNVT        = "kPMT_NNVT" ;   
   static constexpr const char* Hamamatsu   = "kPMT_Hamamatsu" ;   
   static constexpr const char* HZC         = "kPMT_HZC" ;   
   static constexpr const char* NNVT_HighQE = "kPMT_NNVT_HighQE" ; 

   static bool IsValid(int pmtcat); 
   static bool IsLPMT(int pmtcat); 
   static const char* Name(int pmtcat); 
   static std::string List(); 
};

inline bool PMTCategory::IsValid(int pmtcat)
{
    return  pmtcat >= (int)kPMT_Unknown && pmtcat <= (int)kPMT_NNVT_HighQE ; 
}
inline bool PMTCategory::IsLPMT(int pmtcat)
{
    return pmtcat == kPMT_NNVT || pmtcat == kPMT_NNVT_HighQE || pmtcat == kPMT_Hamamatsu ;  
}

inline const char* PMTCategory::Name(int pmtcat)
{
    const char* catname = NULL ; 
    switch(pmtcat)
    {   
       case kPMT_Unknown:      catname = Unknown     ; break ; 
       case kPMT_NNVT:         catname = NNVT        ; break ; 
       case kPMT_Hamamatsu:    catname = Hamamatsu   ; break ; 
       case kPMT_HZC:          catname = HZC         ; break ; 
       case kPMT_NNVT_HighQE:  catname = NNVT_HighQE ; break ; 
    }   
    return catname ; 
}

inline std::string PMTCategory::List() 
{
    std::stringstream ss ; 
    ss << kPMT_Unknown << ":" << Unknown ; 
    ss << "," ; 
    ss << kPMT_NNVT << ":" << NNVT ; 
    ss << "," ; 
    ss << kPMT_Hamamatsu << ":" << Hamamatsu ; 
    ss << "," ; 
    ss << kPMT_HZC << ":" << HZC ; 
    ss << "," ; 
    ss << kPMT_NNVT_HighQE << ":" << NNVT_HighQE ; 
    return ss.str(); 
}




