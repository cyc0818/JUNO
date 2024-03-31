
include (PKG)
PKG (PMTSim
    DEPENDS
        SimUtil
        IPMTSimParamSvc
        Geometry
        MultiFilmSimSvc
        MCParamsSvc
        GenTools
        EGet
        $<$<BOOL:${Opticks_FOUND}>:${Opticks_TARGET}>  
)
