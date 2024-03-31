find_package(Boost COMPONENTS numpy)

PKG(PSDPreSelectAlg
    DEPENDS
        SimEvent
		RecEvent
		EventRecordSvc
		PSDClassification
		EvtNavigator
        Boost::numpy
)
