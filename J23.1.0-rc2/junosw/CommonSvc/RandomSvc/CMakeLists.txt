include (PKG)
PKG(RandomSvc
    FILES
        binding/RandomSvcBinding.cc
    DEPENDS
        CLHEP
)
#include_directories (${CMAKE_CURRENT_SOURCE_DIR})
#include_directories (src)
#
#add_library (RandomSvc SHARED
#                src/IRandomSvc.cc
#                src/RandomSvc.cc 
#                src/RandomSeedRecorder.cc
#                binding/RandomSvcBinding.cc
#            )
#
#target_link_libraries(RandomSvc 
#    ${SNIPERKERNEL_LIBRARIES}
#    ${ROOT_LIBRARIES}
#    CLHEP)
#
#target_include_directories(RandomSvc
#    PUBLIC
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#)
#
#install(TARGETS RandomSvc DESTINATION lib)
#install(DIRECTORY python/RandomSvc
#        DESTINATION python)
#install(FILES share/load_random_svc_with_new.py
#        DESTINATION share/RandomSvc)
#
## copy python to build tree
#file(COPY
#    python/RandomSvc
#    DESTINATION ${CMAKE_BINARY_DIR}/python
#)
## copy share to build tree
#file(COPY
#    share/cmp_seed_and_seed_status.sh
#    share/load_random_svc_with_new.py
#    DESTINATION ${CMAKE_BINARY_DIR}/share/RandomSvc
#)
#
## copy header
#file(COPY
#    RandomSvc
#    DESTINATION ${CMAKE_BINARY_DIR}/include
#)
