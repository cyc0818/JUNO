PKG(RecCdMuonAlg
    DEPENDS
    RecEvent
    CalibEvent
    Identifier
    Geometry
    EvtNavigator
    
    BufferMemMgr
    RecTools
    SNiPER::RootWriterLib
    $<$<BOOL:${WITH_OEC}>:EvtStore>
    $<$<BOOL:${WITH_OEC}>:EvtDataTypes>
    $<$<BOOL:${WITH_OEC}>:OecEvent>
)
