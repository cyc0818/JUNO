###############################################################################
# The cmake files inside this file will be moved to commonsw
###############################################################################

###############################################################################
# Project Version
###############################################################################
# if it is not tag, the use 'dev' rule
include(cmake/legacy/JUNOVersionDefinition.cmake)

###############################################################################
# C++ Standard: 17
###############################################################################
#   it could be overriden by -DCMAKE_CXX_STANDARD=17
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

# Use the GNU Install Directories
include(GNUInstallDirs)
# setup Module PATH
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/legacy/Modules/")

include (cmake/legacy/JUNOCommonDefinitions.cmake)
include (cmake/legacy/JUNOOptionDefinitions.cmake)

if(BUILD_ONLINE)
  include (cmake/legacy/JUNOOnlineDependencies.cmake)
else()
  include (cmake/legacy/JUNODependencies.cmake)
endif()

include (cmake/legacy/JUNOSetupGraphVizOptions.cmake)


# add macros
include (PKG)
include (EDM)
