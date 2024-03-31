#cmake_minimum_required(VERSION 3.15)

#project(TagDataModel)
#set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

#find_package(junosw)
#include(PKG)

add_subdirectory(PSDPreSelectAlg)
add_subdirectory(EventRecordSvc)

#include(JUNOSetupScripts)
#include(JUNOInstallOfflineSetup)
