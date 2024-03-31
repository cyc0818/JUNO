# Install script for directory: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/G4Svc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/DetSimAlg/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/SimUtil/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCParamsSvc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/PMTSim/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/PhysiSim/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/OPSimulator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/VoxelMethod/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/CalibUnit/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/CentralDetector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/Chimney/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/TopTracker/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/AnalysisCode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/DetSimOptions/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/DetSimMT/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/DetSimMTUtil/cmake_install.cmake")
endif()

