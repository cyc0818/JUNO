PKG(QCtrRecAlg
    DEPENDS
    EvtNavigator
    Geometry
    CalibEvent
    RecEvent
    RecTools
    # IPMTSimParamSvc
    PMTCalibSvc

    $<$<BOOL:${WITH_OEC}>:EvtStore>
    $<$<BOOL:${WITH_OEC}>:EvtDataTypes>
    $<$<BOOL:${WITH_OEC}>:OecEvent>
    $<$<BOOL:${WITH_OEC}>:ElecEvent>

    JunoTimer
)
