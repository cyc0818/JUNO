PKG(RootIOTest
        src/MakeSample.cc
        src/SelectEventData.cc
        src/TestRecEDM.cc
    NOAUX
    DICTS
        DummyHeader
        DummyEvent
        DummyPMTHit
        DummyTTHit
        DummyTrack
    DEPENDS
        BufferMemMgr
        RootIOUtil
        SimEvent
        CalibEvent
        RecEvent
        TestEvent
)

