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
include Utilities/Rest/CMakeFiles/Rest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Utilities/Rest/CMakeFiles/Rest.dir/compiler_depend.make

# Include the progress variables for this target.
include Utilities/Rest/CMakeFiles/Rest.dir/progress.make

# Include the compile flags for this target's objects.
include Utilities/Rest/CMakeFiles/Rest.dir/flags.make

Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.o: Utilities/Rest/CMakeFiles/Rest.dir/flags.make
Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/cURL.cc
Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.o: Utilities/Rest/CMakeFiles/Rest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.o -MF CMakeFiles/Rest.dir/src/cURL.cc.o.d -o CMakeFiles/Rest.dir/src/cURL.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/cURL.cc

Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Rest.dir/src/cURL.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/cURL.cc > CMakeFiles/Rest.dir/src/cURL.cc.i

Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Rest.dir/src/cURL.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/cURL.cc -o CMakeFiles/Rest.dir/src/cURL.cc.s

Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.o: Utilities/Rest/CMakeFiles/Rest.dir/flags.make
Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/object.cc
Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.o: Utilities/Rest/CMakeFiles/Rest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.o -MF CMakeFiles/Rest.dir/src/object.cc.o.d -o CMakeFiles/Rest.dir/src/object.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/object.cc

Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Rest.dir/src/object.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/object.cc > CMakeFiles/Rest.dir/src/object.cc.i

Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Rest.dir/src/object.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest/src/object.cc -o CMakeFiles/Rest.dir/src/object.cc.s

# Object files for target Rest
Rest_OBJECTS = \
"CMakeFiles/Rest.dir/src/cURL.cc.o" \
"CMakeFiles/Rest.dir/src/object.cc.o"

# External object files for target Rest
Rest_EXTERNAL_OBJECTS =

lib/libRest.so: Utilities/Rest/CMakeFiles/Rest.dir/src/cURL.cc.o
lib/libRest.so: Utilities/Rest/CMakeFiles/Rest.dir/src/object.cc.o
lib/libRest.so: Utilities/Rest/CMakeFiles/Rest.dir/build.make
lib/libRest.so: lib/libcJSON.so
lib/libRest.so: /usr/lib64/libcurl.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libRest.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libRest.so: Utilities/Rest/CMakeFiles/Rest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library ../../lib/libRest.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Rest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Utilities/Rest/CMakeFiles/Rest.dir/build: lib/libRest.so
.PHONY : Utilities/Rest/CMakeFiles/Rest.dir/build

Utilities/Rest/CMakeFiles/Rest.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest && $(CMAKE_COMMAND) -P CMakeFiles/Rest.dir/cmake_clean.cmake
.PHONY : Utilities/Rest/CMakeFiles/Rest.dir/clean

Utilities/Rest/CMakeFiles/Rest.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Utilities/Rest /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Utilities/Rest/CMakeFiles/Rest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Utilities/Rest/CMakeFiles/Rest.dir/depend

