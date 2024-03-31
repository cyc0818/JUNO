
cmake_minimum_required (VERSION 3.15)

# add this options before PROJECT keyword
# disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

##############################################################################
# Set the default CMAKE_BUILD_TYPE to Release (copied from ROOT)
if(NOT GENERATOR_IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
  if(NOT CMAKE_C_FLAGS AND NOT CMAKE_CXX_FLAGS AND NOT CMAKE_Fortran_FLAGS)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
      "Specifies the build type on single-configuration generators" FORCE)
  endif()
endif()

##############################################################################
# Set compiler flags
# set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")



##############################################################################
project (junosw)


# follow the rule of cmake self 
# (link: https://cmake.org/cmake/help/latest/variable/CMAKE_VERSION.html)
# - rel: MAJOR.MINOR.PATCH-rc<n>
# - dev: MAJOR.MINOR.DATE-<id>
#   - <id>
#     - for svn, prefix is r, following the revision number
#     - for git, prefix is g, following the sha1 abbrev 
set (offline_VERSION_MAJOR 23)
set (offline_VERSION_MINOR 1)
set (offline_VERSION_PATCH 0)
set (offline_VERSION "J${offline_VERSION_MAJOR}.${offline_VERSION_MINOR}.${offline_VERSION_PATCH}")

##############################################################################

find_package(commonsw)
if (commonsw_FOUND)
  message(STATUS "CommonSW is found.")
else()
  message(STATUS "CommonSW is not found. Use legacy cmake modules.")
  include(cmake/legacy.cmake)
endif()

# ================================
# switch user or default packages
# ================================
# 
# If user creates a user defined package lists, then we will build the 
# user specified packages
# 
# User could create from the template
# 

if(BUILD_ONLINE)
  message(STATUS "Using online OEC packages lists")
  include (${CMAKE_SOURCE_DIR}/CMakeLists.online.txt) 
elseif (EXISTS "${CMAKE_SOURCE_DIR}/CMakeLists.user.txt")
  message(STATUS "Using user customized package lists")
  find_package(junosw)
  include ("${CMAKE_SOURCE_DIR}/CMakeLists.user.txt")
else()
  message(STATUS "Using default package lists")
  include ("${CMAKE_SOURCE_DIR}/CMakeLists.default.txt")
endif()

# At last, generate setup scripts and install all the necessary files
if (commonsw_FOUND)
  message(STATUS "CommonSW is found. Creating setup scripts and Installing")
  include (JUNOSetupScripts)
  include (JUNOInstallOffline)
else()
  message(STATUS "CommonSW is not found. Creating setup scripts and Installing using legacy cmake")
  include (cmake/legacy/JUNOSetupScripts.cmake)
  include (cmake/legacy/JUNOInstallOffline.cmake)
endif()
