
PKG(GenTools
    DEPENDS
        GenEvent
        BufferMemMgr
        MCGlobalTimeSvc
        SNiPER::RootWriterLib
        EG
        Parameter
        # == FIXME ==
        G4geometry
        Boost::regex

        $<$<BOOL:${Opticks_FOUND}>:${Opticks_TARGET}> 
)

