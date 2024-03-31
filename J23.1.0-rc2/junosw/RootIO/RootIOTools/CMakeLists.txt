include (PKG)
PKG(RootIOTools
    DEPENDS
        EDMUtil
        RootIOUtil
        RootIOSvc
        GenEvent
        SimEvent # used for the merger
        BufferMemMgr
)
