include (PKG)

PKG(DetSimOptions
    DEPENDS
        AnalysisCode

        CentralDetector
        TopTracker
        Chimney
        CalibUnit

        PhysiSim
        PMTSim
        SimUtil
        MCParamsSvc
        PMTSimParamSvc

        DetSimAlg

        ${GEANT4_LIBRARIES}


        $<$<BOOL:${Opticks_FOUND}>:${Opticks_TARGET}>
)
