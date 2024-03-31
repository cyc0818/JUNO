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
include Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/compiler_depend.make

# Include the progress variables for this target.
include Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/flags.make

Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o: Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/flags.make
Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/PSDTag/PSDPreSelectAlg/src/PSDPreSelectAlg.cc
Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o: Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o -MF CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o.d -o CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/PSDTag/PSDPreSelectAlg/src/PSDPreSelectAlg.cc

Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/PSDTag/PSDPreSelectAlg/src/PSDPreSelectAlg.cc > CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.i

Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/PSDTag/PSDPreSelectAlg/src/PSDPreSelectAlg.cc -o CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.s

# Object files for target PSDPreSelectAlg
PSDPreSelectAlg_OBJECTS = \
"CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o"

# External object files for target PSDPreSelectAlg
PSDPreSelectAlg_EXTERNAL_OBJECTS =

lib/libPSDPreSelectAlg.so: Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/src/PSDPreSelectAlg.cc.o
lib/libPSDPreSelectAlg.so: Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/build.make
lib/libPSDPreSelectAlg.so: lib/libPSDClassification.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/lib/libboost_numpy39.so.1.78.0
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libPSDPreSelectAlg.so: lib/libSimEvent.so
lib/libPSDPreSelectAlg.so: lib/libRecEvent.so
lib/libPSDPreSelectAlg.so: lib/libCLHEPDict.so
lib/libPSDPreSelectAlg.so: lib/libEventRecordSvc.so
lib/libPSDPreSelectAlg.so: lib/libClassificationTool.so
lib/libPSDPreSelectAlg.so: lib/libEvtNavigator.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libPSDPreSelectAlg.so: lib/libElecEvent.so
lib/libPSDPreSelectAlg.so: lib/libContext.so
lib/libPSDPreSelectAlg.so: lib/libEDMUtil.so
lib/libPSDPreSelectAlg.so: lib/libBaseEvent.so
lib/libPSDPreSelectAlg.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J22.2.x/ExternalLibs/Boost/1.78.0/lib/libboost_python39.so.1.78.0
lib/libPSDPreSelectAlg.so: Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../../lib/libPSDPreSelectAlg.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PSDPreSelectAlg.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/build: lib/libPSDPreSelectAlg.so
.PHONY : Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/build

Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg && $(CMAKE_COMMAND) -P CMakeFiles/PSDPreSelectAlg.dir/cmake_clean.cmake
.PHONY : Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/clean

Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/PSDTag/PSDPreSelectAlg /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/PSDTag/PSDPreSelectAlg/CMakeFiles/PSDPreSelectAlg.dir/depend
