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
include RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/compiler_depend.make

# Include the progress variables for this target.
include RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/progress.make

# Include the compile flags for this target's objects.
include RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/flags.make

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/flags.make
RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputStream.cc
RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o -MF CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o.d -o CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputStream.cc

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputStream.cc > CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.i

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputStream.cc -o CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.s

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/flags.make
RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputSvc.cc
RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o -MF CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o.d -o CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputSvc.cc

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputSvc.cc > CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.i

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc/src/RawDataInputSvc.cc -o CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.s

# Object files for target RawDataIOSvc
RawDataIOSvc_OBJECTS = \
"CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o" \
"CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o"

# External object files for target RawDataIOSvc
RawDataIOSvc_EXTERNAL_OBJECTS =

lib/libRawDataIOSvc.so: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputStream.cc.o
lib/libRawDataIOSvc.so: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/src/RawDataInputSvc.cc.o
lib/libRawDataIOSvc.so: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/build.make
lib/libRawDataIOSvc.so: lib/libRawEvent.so
lib/libRawDataIOSvc.so: lib/libRawFile.so
lib/libRawDataIOSvc.so: lib/libRootIOSvc.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libRawDataIOSvc.so: lib/libRootIOUtil.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libRawDataIOSvc.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libRawDataIOSvc.so: lib/libEvtNavigator.so
lib/libRawDataIOSvc.so: lib/libEDMUtil.so
lib/libRawDataIOSvc.so: lib/libBaseEvent.so
lib/libRawDataIOSvc.so: RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../../lib/libRawDataIOSvc.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RawDataIOSvc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/build: lib/libRawDataIOSvc.so
.PHONY : RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/build

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc && $(CMAKE_COMMAND) -P CMakeFiles/RawDataIOSvc.dir/cmake_clean.cmake
.PHONY : RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/clean

RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/RawDataIO/RawDataIOSvc /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : RawDataIO/RawDataIOSvc/CMakeFiles/RawDataIOSvc.dir/depend

