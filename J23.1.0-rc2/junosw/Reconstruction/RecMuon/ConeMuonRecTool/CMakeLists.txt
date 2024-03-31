PKG(ConeMuonRecTool
    DEPENDS
        RecTools
        
        BufferMemMgr
        Minuit2
        Minuit
    SETENV
        RECMU_CONE_DATA="$ENV{JUNOTOP}/data/Reconstruction/RecMuon/ConeMuonData"
)
