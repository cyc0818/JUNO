# Install script for directory: /junofs/users/yuchincheng/J23.1.0-rc2/junosw

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
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/XmlObjDesc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Detector/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RootIO/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Database/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Calibration/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/EventDisplay/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Validation/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Analysis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/IADAnalysis/cmake_install.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/junoswConfig.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw/junoswTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw/junoswTargets.cmake"
         "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles/Export/c5bb5771d1d699ffa60a5d765900bd99/junoswTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw/junoswTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw/junoswTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles/Export/c5bb5771d1d699ffa60a5d765900bd99/junoswTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw" TYPE FILE FILES "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles/Export/c5bb5771d1d699ffa60a5d765900bd99/junoswTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/InstallArea/setup.sh;/junofs/users/yuchincheng/J23.1.0-rc2/junosw/InstallArea/setup.csh")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/InstallArea" TYPE FILE FILES
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/setup.sh"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/setup.csh"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/junosw" TYPE FILE FILES
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/Modules/PKG.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/Modules/EDM.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/Modules/XOD.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/Modules/RootDict.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/JUNODependencies.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/JUNOOnlineDependencies.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/JUNOVersionDefinition.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/JUNOSetupScripts.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/JUNOCommonDefinitions.cmake"
    "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/cmake/legacy/JUNOInstallOfflineSetup.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
