# look for more
include_directories(
    $ENV{JUNO_EXTLIB_libmore_HOME}/include
)
find_library(MORE_LIBRARIES more
    HINTS $ENV{JUNO_EXTLIB_libmore_HOME}/lib
)
link_directories($ENV{JUNO_EXTLIB_libmore_HOME}/lib)

include (PKG)

PKG(GenDecay
    DEPENDS
        GenTools
        RandomSvc
        more

    SETENV
        MORE_PHYS_FETCHER=${CMAKE_CURRENT_SOURCE_DIR}/share/lbl-ensdf-fetcher.py
)

