# Install script for directory: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/InstallArea")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/binutils/2.36/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimVertex.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTrack.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimPMTHit.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimTTHit.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/Event/SimHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libSimEvent.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libSimEvent.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libSimEvent.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE SHARED_LIBRARY FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/lib/libSimEvent.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libSimEvent.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libSimEvent.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/binutils/2.36/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libSimEvent.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/lib/libSimEvent.rootmap")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE FILE FILES
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/src/SimVertexDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/src/SimTrackDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/src/SimPMTHitDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/src/SimTTHitDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/src/SimEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent/src/SimHeaderDict_rdict.pcm"
    )
endif()

