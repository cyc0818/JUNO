
/**
PMTQty
--------

Idea is to cache frequently needed properties into a quickly 
accessible array of G4MaterialPropertyVector pointers. 

**/

struct PMTQty
{
    enum { NUM_CAT = 4 }; 
    enum { ARC_RINDEX, ARC_KINDEX, PHC_RINDEX, PHC_KINDEX, NUM_QTY } ; 
    static constexpr const char* ARC_RINDEX_ = "ARC_RINDEX" ; 
    static constexpr const char* ARC_KINDEX_ = "ARC_KINDEX" ; 
    static constexpr const char* PHC_RINDEX_ = "PHC_RINDEX" ; 
    static constexpr const char* PHC_KINDEX_ = "PHC_KINDEX" ; 
    static const char* Name(int qty); 

    PMTQty(const PMTSimParamData& data); 

    G4MaterialPropertyVector** _ARC_RINDEX ; 
    G4MaterialPropertyVector** _ARC_KINDEX ; 
    G4MaterialPropertyVector** _PHC_RINDEX ; 
    G4MaterialPropertyVector** _PHC_KINDEX ; 

    G4MaterialPropertyVector** getPropArray(int qty) const ; 
    G4MaterialPropertyVector*  getProperty(int qty, int cat) const ; 
};

inline const char* PMTQty::Name(int qty)
{
    const char* str = nullptr ; 
    switch(qty)
    {
        case ARC_RINDEX: str = ARC_RINDEX_ ; break ;  
        case ARC_KINDEX: str = ARC_KINDEX_ ; break ;  
        case PHC_RINDEX: str = PHC_RINDEX_ ; break ;  
        case PHC_KINDEX: str = PHC_KINDEX_ ; break ;  
    }
    return str ; 
}

inline G4MaterialPropertyVector** PMTQty::getPropArray(int qty) const 
{
    G4MaterialPropertyVector** mpv = nullptr ; 
    switch(qty)
    {
        case ARC_RINDEX: mpv = _ARC_RINDEX ; break ;  
        case ARC_KINDEX: mpv = _ARC_KINDEX ; break ;  
        case PHC_RINDEX: mpv = _PHC_RINDEX ; break ;  
        case PHC_KINDEX: mpv = _PHC_KINDEX ; break ;  
    }
    return mpv ; 
}

inline G4MaterialPropertyVector* PMTQty::getProperty(int qty, int cat) const 
{
    G4MaterialPropertyVector** mpva = getPropArray(qty); 
    G4MaterialPropertyVector* mpv = mpva[cat] ; 
    return mpv ; 
}

inline PMTQty::PMTQty(const PMTSimParamData& data)
    :
    _ARC_RINDEX(new G4MaterialPropertyVector* [NUM_CAT]),
    _ARC_KINDEX(new G4MaterialPropertyVector* [NUM_CAT]),
    _PHC_RINDEX(new G4MaterialPropertyVector* [NUM_CAT]),
    _PHC_KINDEX(new G4MaterialPropertyVector* [NUM_CAT])
{
    int cats[NUM_CAT] = { kPMT_NNVT, kPMT_Hamamatsu, kPMT_HZC, kPMT_NNVT_HighQE }; 

    for(int qty=0 ; qty < NUM_QTY ; qty++)
    {
        const char* qname = Name(qty) ; 
        G4MaterialPropertyVector** mpva = getPropArray(qty); 

        for(int j=0 ; j < NUM_CAT ; j++)
        {
            int cat = cats[j]; 
            mpva[cat] = nullptr ;  

            if(data.m_PMT_MPT.count(cat) == 0)           continue ; 
            if(data.m_PMT_MPT.at(cat).count(qname) == 0) continue ; 

            mpva[cat] = data.m_PMT_MPT.at(cat).at(qname) ;  
        }
    }
}

