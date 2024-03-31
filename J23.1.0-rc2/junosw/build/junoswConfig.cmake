
set(junosw_VERSION_MAJOR "23")
set(junosw_VERSION_MINOR "1")
set(junosw_VERSION_PATCH "20231013")
set(junosw_VERSION "J23.1.20231013-g27d86b9")

include(GNUInstallDirs)

##############################################################################
# setup commonsw
##############################################################################
find_package(commonsw)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}")

if (commonsw_FOUND)
  # if there is commonsw, don't load any thing. 
else()
  # if there is no commonsw, setup the legacy cmake
  include(JUNOVersionDefinition)
  include(JUNOCommonDefinitions)
  if(BUILD_ONLINE)
    include(JUNOOnlineDependencies)
  else()
    include(JUNODependencies)
  endif()
  include(PKG)
  include(EDM)
endif()


# Include the targets, so other projects could use offline
include("${CMAKE_CURRENT_LIST_DIR}/junoswTargets.cmake")

