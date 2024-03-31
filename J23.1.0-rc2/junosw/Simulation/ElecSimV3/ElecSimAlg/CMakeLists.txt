PKG(ElecSimAlg
    DEPENDS
        GenEvent
        SimEvent 
        ElecEvent 
        ElecTruth
        EvtNavigator 
        Identifier 
        BufferMemMgr 
        Context 
         
       
        InputReviser 
        Geometry
        IPMTSimParamSvc
        SpmtElecConfigSvc
    SETENV
        ELECSIMROOT="$ENV{JUNOTOP}/data/Simulation/ElecSim"
)
