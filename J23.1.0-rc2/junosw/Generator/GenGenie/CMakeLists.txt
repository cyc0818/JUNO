
PKG(GenGenie
    DEPENDS
        GenTools
        GenEvent
        BufferMemMgr
        MCGlobalTimeSvc
        SNiPER::RootWriterLib
        ROOT::EG ROOT::EGPythia6 ROOT::Gui ROOT::Geom
        # == FIXME ==
        G4geometry
        Boost::regex
        ${GENIE_LIBRARIES}
        ${LOG4CPP_LIBRARIES}
        LibXml2::LibXml2
        GSL::gsl GSL::gslcblas
        $<$<BOOL:${Opticks_FOUND}>:${Opticks_TARGET}> 
)

