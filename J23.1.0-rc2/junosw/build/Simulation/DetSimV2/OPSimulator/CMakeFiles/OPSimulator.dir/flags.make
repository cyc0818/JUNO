# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# compile CXX with /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++
CXX_DEFINES = -DBOOST_ATOMIC_DYN_LINK -DBOOST_ATOMIC_NO_LIB -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_PYTHON_DYN_LINK -DBOOST_PYTHON_NO_LIB -DBOOST_SYSTEM_DYN_LINK -DBOOST_SYSTEM_NO_LIB -DOPSimulator_EXPORTS -DSNIPER_VERSION_2 -DWITH_CUSTOM4 -DWITH_OEC

CXX_INCLUDES = -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Boost/1.78.0 -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/log4cpp/1.1.3/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/CLHEP/2.4.1.0/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/CLHEP/2.4.1.0/lib/CLHEP-2.4.1.0/../../include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Geant4/10.04.p02.juno/include/geant4 -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/HepMC/2.06.11/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/genie/3.02.00/include/GENIE -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/frontier/2.9.1/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/mysql-connector-c/6.1.9/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/mysql-connector-cpp/1.1.12/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/libyaml/0.2.4/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/libonnxruntime/1.11.1/include/onnxruntime -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/nuwro/21.09.2/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/OPSimulator -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/OPSimulator/src -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/OPSimulator/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/DetSimAlg -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/DetSimAlg/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/G4Svc -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/G4Svc/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/PhysiSim -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/PhysiSim/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCParamsSvc -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCParamsSvc/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/cJSON -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/cJSON/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Database/ParaDB -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Database/ParaDB/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Database/DBISvc -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Database/DBISvc/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Database/dbi -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Database/dbi/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/SimSvc/IPMTSimParamSvc -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/SimSvc/IPMTSimParamSvc/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/SimSvc/PMTSimParamSvc -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/SimSvc/PMTSimParamSvc/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Detector/Geometry -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Detector/Geometry/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Detector/Identifier -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Detector/Identifier/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Detector/Parameter -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Detector/Parameter/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/EGet -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/EGet/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/SimUtil -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/SimUtil/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/SimEvent -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/SimEvent/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/EDMUtil -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/EDMUtil/include -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/DataModel/BaseEvent -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/BaseEvent -I/junofs/users/yuchincheng/J23.1.0-rc2/junosw/DataModel/BaseEvent/include -isystem /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Python/3.9.14/include/python3.9 -isystem /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/include -isystem /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/include -isystem /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/include

CXX_FLAGS =  -Werror=return-type -O3 -DNDEBUG -fPIC   -DG4INTY_USE_XT -DG4VIS_USE_OPENGL -DG4UI_USE_TCSH -DG4VIS_USE_RAYTRACERX -DG4VIS_USE_OPENGLX -W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-variadic-macros -Wshadow -pipe -DG4USE_STD11 -pthread -ftls-model=global-dynamic  -DG4MULTITHREADED -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Geant4/10.04.p02.juno/bin/../include/Geant4 -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/CLHEP/2.4.1.0/include -I/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/custom4/0.1.4/include/Custom4 -std=c++17

