include(PKG)
PKG(AtmNuGen
	DEPENDS
		GenDeex
		EG
        # == FIXME ==


    APPONLY
        AtmNuGen
    FILES
        src/AtmNuGen.cc
		src/AtmEvent.cc
		src/PreSelect.cc
		src/Data.cc
)
