# Install script for directory: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/ElecEvent

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdLpmtElecHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdSpmtElecHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdWaveformHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/WpElecHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/WpWaveformHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/TtElecHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdLpmtElecEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdSpmtElecEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdWaveformEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/WpElecEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/WpWaveformEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/TtElecEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/SpmtElecAbcBlock.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/SpmtElecDiscrWord.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/SpmtElecSpecialWord.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/ElecWaveform.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/ElecChannel.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdTriggerHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/CdTriggerEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/WpTriggerHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/WpTriggerEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/TtTriggerHeader.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/Event" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/Event/TtTriggerEvt.h")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libElecEvent.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libElecEvent.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libElecEvent.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE SHARED_LIBRARY FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/lib/libElecEvent.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libElecEvent.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libElecEvent.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/binutils/2.36/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/libElecEvent.so")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/lib/libElecEvent.rootmap")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "shlib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE FILE FILES
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdLpmtElecHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdSpmtElecHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdWaveformHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/WpElecHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/WpWaveformHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/TtElecHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdLpmtElecEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdSpmtElecEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdWaveformEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/WpElecEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/WpWaveformEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/TtElecEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/SpmtElecAbcBlockDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/SpmtElecDiscrWordDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/SpmtElecSpecialWordDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/ElecWaveformDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/ElecChannelDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdTriggerHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/CdTriggerEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/WpTriggerHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/WpTriggerEvtDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/TtTriggerHeaderDict_rdict.pcm"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/ElecEvent/src/TtTriggerEvtDict_rdict.pcm"
    )
endif()

