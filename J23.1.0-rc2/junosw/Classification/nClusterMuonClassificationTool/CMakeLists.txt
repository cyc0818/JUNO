PKG(nClusterMuonClassificationTool
    DEPENDS
    RecEvent
    CalibEvent
    Identifier
    Geometry
    EvtNavigator
	ClassificationTool
	RecTools
	Spectrum
    
    BufferMemMgr
    RecTools
    SNiPER::RootWriterLib
    $<$<BOOL:${WITH_OEC}>:EvtStore>
    $<$<BOOL:${WITH_OEC}>:EvtDataTypes>
    $<$<BOOL:${WITH_OEC}>:OecEvent>
)
