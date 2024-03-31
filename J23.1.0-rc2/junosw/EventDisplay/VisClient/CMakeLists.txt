PKG(VisClient
        src/VisClient.cc
        src/VisHelp.cc
    NOAUX
    APP serena
    FILES
        src/jvis.C
    DICTS
        VisClient
    DEPENDS
        JVisLib
        Gui Eve EG TreePlayer Geom Ged RGL
)

#FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src)
#
#include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/VisClient)
#
## == generate root dict ==
#foreach (_f VisClient)
#    ROOT_GENERATE_DICTIONARY(${CMAKE_CURRENT_BINARY_DIR}/src/${_f}Dict
#        ${CMAKE_CURRENT_SOURCE_DIR}/VisClient/${_f}.h 
#        LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/src/${_f}LinkDef.h)
#endforeach(_f)
#
#add_library(VisClient SHARED
#    src/VisClient.cc
#    src/VisHelp.cc
#    ${CMAKE_CURRENT_BINARY_DIR}/src/VisClientDict.cxx
#)
#
#target_link_libraries(VisClient
#    JVisLib
#    EDMUtil
#    BaseEvent
#    SniperKernel
#    ${ROOT_LIBRARIES}
#    Gui Eve EG TreePlayer Geom Ged RGL
#    ${CLHEP_LIBRARIES}
#)
#
#add_executable(serena
#    src/jvis.C
#)
#target_link_libraries(serena
#    VisClient
#)
