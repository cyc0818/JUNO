include (EDM)
# generate source code
EDM(RecEvent
    FILES
        RecTrack
        RecVertex
        CdVertexRecEvt
        CdTrackRecEvt
        WpRecEvt
        TtRecEvt
        CdVertexRecHeader
        CdTrackRecHeader
        WpRecHeader
        TtRecHeader
    DEPENDS
        CLHEPDict
        CLHEP
)
