PKG(WpPMTCalibAlg
    DEPENDS
        PMTCalibSvc
        BufferMemMgr
        Geometry
        $<$<NOT:$<BOOL:${BUILD_ONLINE}>>:ElecSimAlg>
)
