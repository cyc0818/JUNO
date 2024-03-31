# J20v2r0-Pre0 is released
## What's New?

Compared to the previous release J20v1r0-Pre2, there are following major changes:

- installation and 3rd party libraries
  - The default OS: CentOS 7.8.2003. We won't provide the official support for SL6 starting from this version.
  - GCC: 8.3.0 (Note: the _GLIBCXX_USE_CXX11_ABI is 1)
    - You could check the ABI with following command 
```bash
echo '#include <string>' | g++ -x c++ -E -dM - | fgrep --color=auto _GLIBCXX_USE_CXX11_ABI
```
  - Binutils: 2.28
  - Update the external libraries.
    - SNiPER v1.5 (fixed file access via xrootd protocol)
    - bazel 0.24.1 (this tool is used to build tensorflow)
    - TensorFlow C API 1.14.0 (experimental)
    - PyTorch C++ API 1.6.0 (experimental)
    - python3 (not enabled by default)

- event data model
  - A track level truth information is provided in electronics simulation.

- Simulation parameters
  - PMTSimParamSvc is added to manage the LPMT parameters for both detector simulation and electronics simulation
  - The testing data is added into simulation. See Miao's ticket #101.
- detector simulation
  - In previous simulation, only the primary tracks are saved into EDM data. In this version, the secondaries belong
    to some specific process could be also saved. See the new SimTrackSvc. This is necessary 
    because the electronics simulation needs such info.
  - The Sensitive Detector is updated to use the PMTSimParamSvc to generate the detector response.
  - Opticks integration is done by Simon C Blyth. To enable it, need to add opticks to the envvar CMTEXTRATAGS.
- electronics simulation
  - Optimize the simulation efficiency by only simulating the necessary hits. For the details, see Miao's ticket #101.

## Basic info


This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v2r0-Pre0
- offline data version: "trunk 63"
- external interface version: "trunk 30"
- sniper version: "v1.5" (GitHub)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre0/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.