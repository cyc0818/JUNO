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
include Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/compiler_depend.make

# Include the progress variables for this target.
include Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/progress.make

# Include the compile flags for this target's objects.
include Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/flags.make

Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.o: Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/flags.make
Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Generator/RadioActivity/AmC/src/AmC.cc
Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.o: Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.o -MF CMakeFiles/AmC.dir/src/AmC.cc.o.d -o CMakeFiles/AmC.dir/src/AmC.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Generator/RadioActivity/AmC/src/AmC.cc

Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AmC.dir/src/AmC.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Generator/RadioActivity/AmC/src/AmC.cc > CMakeFiles/AmC.dir/src/AmC.cc.i

Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AmC.dir/src/AmC.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Generator/RadioActivity/AmC/src/AmC.cc -o CMakeFiles/AmC.dir/src/AmC.cc.s

# Object files for target AmC
AmC_OBJECTS = \
"CMakeFiles/AmC.dir/src/AmC.cc.o"

# External object files for target AmC
AmC_EXTERNAL_OBJECTS =

bin/AmC.exe: Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/src/AmC.cc.o
bin/AmC.exe: Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/build.make
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
bin/AmC.exe: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/CLHEP/2.4.1.0/lib/libCLHEP.so
bin/AmC.exe: Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/AmC.exe"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AmC.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/build: bin/AmC.exe
.PHONY : Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/build

Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC && $(CMAKE_COMMAND) -P CMakeFiles/AmC.dir/cmake_clean.cmake
.PHONY : Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/clean

Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Generator/RadioActivity/AmC /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Generator/RadioActivity/AmC/CMakeFiles/AmC.dir/depend

