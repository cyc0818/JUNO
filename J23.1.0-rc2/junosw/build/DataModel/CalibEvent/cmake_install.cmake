# Install script for directory: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/CalibEvent

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CdLpmtCalibHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CdSpmtCalibHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/WpCalibHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/TtCalibHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CdLpmtCalibEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CdSpmtCalibEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/WpCalibEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/TtCalibEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CalibPmtChannel.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/Event/CalibTtChannel.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libCalibEvent.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libCalibEvent.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libCalibEvent.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE SHARED_LIBRARY FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/lib/libCalibEvent.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libCalibEvent.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libCalibEvent.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/binutils/2.36/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libCalibEvent.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/lib/libCalibEvent.rootmap")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE FILE FILES
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/CdLpmtCalibHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/CdSpmtCalibHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/WpCalibHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/TtCalibHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/CdLpmtCalibEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/CdSpmtCalibEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/WpCalibEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/TtCalibEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/CalibPmtChannelDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/CalibEvent/src/CalibTtChannelDict_rdict.pcm"
    )
endif()

