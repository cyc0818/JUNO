# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Cmake/3.24.1/bin/cmake

# The command to remove a file.
RM = /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Cmake/3.24.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /junofs/users/yuchincheng/J23.1.0-rc2/junosw

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build

# Include any dependencies generated for this target.
include Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/compiler_depend.make

# Include the progress variables for this target.
include Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/progress.make

# Include the compile flags for this target's objects.
include Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/flags.make

Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o: Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/flags.make
Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Reconstruction/RecMuon/SpmtMuonRecTool/src/SpmtMuonRecTool.cc
Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o: Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o -MF CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o.d -o CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Reconstruction/RecMuon/SpmtMuonRecTool/src/SpmtMuonRecTool.cc

Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Reconstruction/RecMuon/SpmtMuonRecTool/src/SpmtMuonRecTool.cc > CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.i

Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Reconstruction/RecMuon/SpmtMuonRecTool/src/SpmtMuonRecTool.cc -o CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.s

# Object files for target SpmtMuonRecTool
SpmtMuonRecTool_OBJECTS = \
"CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o"

# External object files for target SpmtMuonRecTool
SpmtMuonRecTool_EXTERNAL_OBJECTS =

lib/libSpmtMuonRecTool.so: Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/src/SpmtMuonRecTool.cc.o
lib/libSpmtMuonRecTool.so: Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/build.make
lib/libSpmtMuonRecTool.so: lib/libRecTools.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libSpmtMuonRecTool.so: lib/libPMTCalibSvc.so
lib/libSpmtMuonRecTool.so: lib/libGeometry.so
lib/libSpmtMuonRecTool.so: lib/libIdentifier.so
lib/libSpmtMuonRecTool.so: lib/libParameter.so
lib/libSpmtMuonRecTool.so: lib/libRest.so
lib/libSpmtMuonRecTool.so: lib/libcJSON.so
lib/libSpmtMuonRecTool.so: /usr/lib64/libcurl.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/lib/libboost_python39.so.1.78.0
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/Python/3.9.14/lib/libpython3.9.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGeom.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libSpmtMuonRecTool.so: lib/libCondObj.so
lib/libSpmtMuonRecTool.so: lib/libCondDB.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/lib/libboost_system.so.1.78.0
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/lib/libboost_filesystem.so.1.78.0
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/lib/libboost_atomic.so.1.78.0
lib/libSpmtMuonRecTool.so: lib/libDBISvc.so
lib/libSpmtMuonRecTool.so: lib/libdbi.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/mysql-connector-cpp/1.1.12/lib/libmysqlcppconn.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/mysql-connector-c/6.1.9/lib/libmysqlclient.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/sqlite3/3.38.5/lib/libsqlite3.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libSpmtMuonRecTool.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libSpmtMuonRecTool.so: Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../../lib/libSpmtMuonRecTool.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SpmtMuonRecTool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/build: lib/libSpmtMuonRecTool.so
.PHONY : Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/build

Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool && $(CMAKE_COMMAND) -P CMakeFiles/SpmtMuonRecTool.dir/cmake_clean.cmake
.PHONY : Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/clean

Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Reconstruction/RecMuon/SpmtMuonRecTool /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Reconstruction/RecMuon/SpmtMuonRecTool/CMakeFiles/SpmtMuonRecTool.dir/depend

