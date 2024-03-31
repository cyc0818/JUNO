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
include Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/compiler_depend.make

# Include the progress variables for this target.
include Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/progress.make

# Include the compile flags for this target's objects.
include Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/flags.make

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/flags.make
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthMuonBundleClassifier.cc
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o -MF CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o.d -o CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthMuonBundleClassifier.cc

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthMuonBundleClassifier.cc > CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.i

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthMuonBundleClassifier.cc -o CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.s

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/flags.make
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthPointLikeClassifier.cc
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o -MF CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o.d -o CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthPointLikeClassifier.cc

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthPointLikeClassifier.cc > CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.i

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthPointLikeClassifier.cc -o CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.s

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/flags.make
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o: /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthTrackLikeClassifier.cc
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o -MF CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o.d -o CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o -c /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthTrackLikeClassifier.cc

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.i"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthTrackLikeClassifier.cc > CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.i

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.s"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/contrib/gcc/11.2.0/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification/src/MCTruthTrackLikeClassifier.cc -o CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.s

# Object files for target MCTruthClassification
MCTruthClassification_OBJECTS = \
"CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o" \
"CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o" \
"CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o"

# External object files for target MCTruthClassification
MCTruthClassification_EXTERNAL_OBJECTS =

lib/libMCTruthClassification.so: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthMuonBundleClassifier.cc.o
lib/libMCTruthClassification.so: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthPointLikeClassifier.cc.o
lib/libMCTruthClassification.so: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/src/MCTruthTrackLikeClassifier.cc.o
lib/libMCTruthClassification.so: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/build.make
lib/libMCTruthClassification.so: lib/libClassificationTool.so
lib/libMCTruthClassification.so: lib/libEvtNavigator.so
lib/libMCTruthClassification.so: lib/libSimEvent.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/sniper/InstallArea/lib64/libSniperKernel.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libCore.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libImt.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRIO.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libNet.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libHist.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGraf3d.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libGpad.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTDataFrame.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTree.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libTreePlayer.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libRint.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPostscript.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMatrix.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libPhysics.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMathCore.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libThread.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libMultiProc.so
lib/libMCTruthClassification.so: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc1120/Pre-Release/J23.1.0-rc2/ExternalLibs/ROOT/6.26.06/lib/libROOTVecOps.so
lib/libMCTruthClassification.so: lib/libEDMUtil.so
lib/libMCTruthClassification.so: lib/libBaseEvent.so
lib/libMCTruthClassification.so: Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ../../lib/libMCTruthClassification.so"
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MCTruthClassification.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/build: lib/libMCTruthClassification.so
.PHONY : Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/build

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/clean:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification && $(CMAKE_COMMAND) -P CMakeFiles/MCTruthClassification.dir/cmake_clean.cmake
.PHONY : Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/clean

Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/depend:
	cd /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /junofs/users/yuchincheng/J23.1.0-rc2/junosw /junofs/users/yuchincheng/J23.1.0-rc2/junosw/Classification/MCTruthClassification /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification /junofs/users/yuchincheng/J23.1.0-rc2/junosw/build/Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Classification/MCTruthClassification/CMakeFiles/MCTruthClassification.dir/depend

