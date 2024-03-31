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
include Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/compiler_depend.make

# Include the progress variables for this target.
include Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/progress.make

# Include the compile flags for this target's objects.
include Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/flags.make

Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o: Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/flags.make
Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCGlobalTimeSvc/src/MCGlobalTimeSvc.cc
Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o: Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o -MF CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o.d -o CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCGlobalTimeSvc/src/MCGlobalTimeSvc.cc

Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCGlobalTimeSvc/src/MCGlobalTimeSvc.cc > CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.i

Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCGlobalTimeSvc/src/MCGlobalTimeSvc.cc -o CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.s

# Object files for target MCGlobalTimeSvc
MCGlobalTimeSvc_OBJECTS = \
"CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o"

# External object files for target MCGlobalTimeSvc
MCGlobalTimeSvc_EXTERNAL_OBJECTS =

lib/libMCGlobalTimeSvc.so: Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/src/MCGlobalTimeSvc.cc.o
lib/libMCGlobalTimeSvc.so: Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/build.make
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libMCGlobalTimeSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libMCGlobalTimeSvc.so: Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../../lib/libMCGlobalTimeSvc.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MCGlobalTimeSvc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/build: lib/libMCGlobalTimeSvc.so
.PHONY : Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/build

Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc && $(CMAKE_COMMAND) -P CMakeFiles/MCGlobalTimeSvc.dir/cmake_clean.cmake
.PHONY : Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/clean

Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Simulation/DetSimV2/MCGlobalTimeSvc /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Simulation/DetSimV2/MCGlobalTimeSvc/CMakeFiles/MCGlobalTimeSvc.dir/depend
