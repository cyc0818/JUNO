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
include Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/compiler_depend.make

# Include the progress variables for this target.
include Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/progress.make

# Include the compile flags for this target's objects.
include Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/flags.make

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/flags.make
Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorAnaAlg.cc
Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o -MF CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o.d -o CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorAnaAlg.cc

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorAnaAlg.cc > CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.i

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorAnaAlg.cc -o CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.s

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/flags.make
Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorBuildAlg.cc
Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o -MF CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o.d -o CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorBuildAlg.cc

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorBuildAlg.cc > CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.i

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis/src/CorBuildAlg.cc -o CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.s

# Object files for target CorAnalysis
CorAnalysis_OBJECTS = \
"CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o" \
"CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o"

# External object files for target CorAnalysis
CorAnalysis_EXTERNAL_OBJECTS =

lib/libCorAnalysis.so: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorAnaAlg.cc.o
lib/libCorAnalysis.so: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/src/CorBuildAlg.cc.o
lib/libCorAnalysis.so: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/build.make
lib/libCorAnalysis.so: lib/libSimEvent.so
lib/libCorAnalysis.so: lib/libElecEvent.so
lib/libCorAnalysis.so: lib/libCalibEvent.so
lib/libCorAnalysis.so: lib/libRecEvent.so
lib/libCorAnalysis.so: lib/libInputReviser.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libCorAnalysis.so: lib/libContext.so
lib/libCorAnalysis.so: lib/libCLHEPDict.so
lib/libCorAnalysis.so: lib/libBufferMemMgr.so
lib/libCorAnalysis.so: lib/libRootIOSvc.so
lib/libCorAnalysis.so: lib/libRootIOUtil.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libCorAnalysis.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libCorAnalysis.so: lib/libEvtNavigator.so
lib/libCorAnalysis.so: lib/libEDMUtil.so
lib/libCorAnalysis.so: lib/libBaseEvent.so
lib/libCorAnalysis.so: Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../../lib/libCorAnalysis.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CorAnalysis.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/build: lib/libCorAnalysis.so
.PHONY : Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/build

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis && $(CMAKE_COMMAND) -P CMakeFiles/CorAnalysis.dir/cmake_clean.cmake
.PHONY : Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/clean

Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Examples/CorAnalysis /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Examples/CorAnalysis/CMakeFiles/CorAnalysis.dir/depend

