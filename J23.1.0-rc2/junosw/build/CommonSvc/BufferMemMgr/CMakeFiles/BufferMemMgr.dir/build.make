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
include CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/compiler_depend.make

# Include the progress variables for this target.
include CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/progress.make

# Include the compile flags for this target's objects.
include CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/flags.make

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/flags.make
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BeginEvtHdl.cc
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o -MF CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o.d -o CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BeginEvtHdl.cc

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BeginEvtHdl.cc > CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.i

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BeginEvtHdl.cc -o CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.s

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/flags.make
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BufferMemMgr.cc
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o -MF CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o.d -o CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BufferMemMgr.cc

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BufferMemMgr.cc > CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.i

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/BufferMemMgr.cc -o CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.s

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/flags.make
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/EndEvtHdl.cc
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o -MF CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o.d -o CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/EndEvtHdl.cc

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/EndEvtHdl.cc > CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.i

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/EndEvtHdl.cc -o CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.s

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/flags.make
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/FullStateNavBuf.cc
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o -MF CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o.d -o CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/FullStateNavBuf.cc

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/FullStateNavBuf.cc > CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.i

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr/src/FullStateNavBuf.cc -o CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.s

# Object files for target BufferMemMgr
BufferMemMgr_OBJECTS = \
"CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o" \
"CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o" \
"CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o" \
"CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o"

# External object files for target BufferMemMgr
BufferMemMgr_EXTERNAL_OBJECTS =

lib/libBufferMemMgr.so: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BeginEvtHdl.cc.o
lib/libBufferMemMgr.so: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/BufferMemMgr.cc.o
lib/libBufferMemMgr.so: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/EndEvtHdl.cc.o
lib/libBufferMemMgr.so: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/src/FullStateNavBuf.cc.o
lib/libBufferMemMgr.so: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/build.make
lib/libBufferMemMgr.so: lib/libRootIOSvc.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libBufferMemMgr.so: lib/libRootIOUtil.so
lib/libBufferMemMgr.so: lib/libEvtNavigator.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libBufferMemMgr.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libBufferMemMgr.so: lib/libEDMUtil.so
lib/libBufferMemMgr.so: lib/libBaseEvent.so
lib/libBufferMemMgr.so: CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library ../../lib/libBufferMemMgr.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BufferMemMgr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/build: lib/libBufferMemMgr.so
.PHONY : CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/build

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr && $(CMAKE_COMMAND) -P CMakeFiles/BufferMemMgr.dir/cmake_clean.cmake
.PHONY : CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/clean

CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/CommonSvc/BufferMemMgr /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CommonSvc/BufferMemMgr/CMakeFiles/BufferMemMgr.dir/depend
