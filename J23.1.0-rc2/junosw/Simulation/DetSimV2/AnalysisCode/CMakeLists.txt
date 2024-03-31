include (PKG)
PKG(AnalysisCode
    DEPENDS
        PMTSim
        SimUtil
        DetSimAlg
        OPSimulator
        TopTracker

        GenTools

        SimEvent
        EvtNavigator

        JunoTimer
		
		np
        

        Boost::filesystem Boost::system
        Geom

        $<$<BOOL:${Opticks_FOUND}>:${Opticks_TARGET}>
)
