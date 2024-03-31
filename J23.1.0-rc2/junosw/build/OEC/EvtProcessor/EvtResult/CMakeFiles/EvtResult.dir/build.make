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
include OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/compiler_depend.make

# Include the progress variables for this target.
include OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/progress.make

# Include the compile flags for this target's objects.
include OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/flags.make

OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o: OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/flags.make
OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/OEC/EvtProcessor/EvtResult/src/EvtResultSvc.cc
OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o: OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o -MF CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o.d -o CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/OEC/EvtProcessor/EvtResult/src/EvtResultSvc.cc

OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/OEC/EvtProcessor/EvtResult/src/EvtResultSvc.cc > CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.i

OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/OEC/EvtProcessor/EvtResult/src/EvtResultSvc.cc -o CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.s

# Object files for target EvtResult
EvtResult_OBJECTS = \
"CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o"

# External object files for target EvtResult
EvtResult_EXTERNAL_OBJECTS =

lib/libEvtResult.so: OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/src/EvtResultSvc.cc.o
lib/libEvtResult.so: OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/build.make
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libEvtResult.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libEvtResult.so: OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library ../../../lib/libEvtResult.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EvtResult.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/build: lib/libEvtResult.so
.PHONY : OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/build

OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult && $(CMAKE_COMMAND) -P CMakeFiles/EvtResult.dir/cmake_clean.cmake
.PHONY : OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/clean

OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/OEC/EvtProcessor/EvtResult /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : OEC/EvtProcessor/EvtResult/CMakeFiles/EvtResult.dir/depend

