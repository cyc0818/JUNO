# J20v1r1-Pre0 is released
## What's New?

- offline data
  - pre-SN model added.

- installation
  - Update the external libraries. See r3822 to r3835.
    - Python 2.7.17
    - Boost 1.72.0
    - CMake 3.17.0
    - TBB renamed to oneTBB
    - ROOT 6.20.02
    - SNiPER v1.4

- event data model
  - RawEvent and RawDataIO is added. See r3751 and r3752.
  - TT related data model is update. See jpandre's commits r3764.

- geometry service
  - TT related update. See jpandre's commits.

- physics generator
  - SN: 
    - Neutrino truth energy is added. See r3780 and r3781.
    - pre-SN model. See r3783.
    - MSW oscillation. See r3784.

- detector simulation
  - Geometry update
    - The coordinate of detector is changed. All components including CD, WP, TT, Calibration units are updated.
      - PMT positions are updated. See r3804.
      - update the nodes in CD. See Miao Yu's commits r3786, r3787, r3791.
    - update SPMT QE. See Miao Yu's commits r3819, r3820.
  - Physics processes
    - Positronium is enabled by default. See r3788, r3789.
  - Sensitive detectors
    - rename the hit and SD name prefix with JUNO. See r3785.
    - A simplified PMT with photo-cathode only is used in veto Water Pool. See r3792.
  - deferred OP simulator. See JUNO-doc-5522.
- electronics simulation
  - Support scale of dark rate. See r3827.
  - SPMT update:
    - PMT ID and Elec channel mapping. See Cecile's commit r3845.

## Basic info

This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v1r1-Pre0
- offline data version: "trunk 50"
- external interface version: "trunk 27"
- sniper version: "v1.4" (GitHub)

The pre-compiled software could be found in CVMFS:
- SLC6+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r1-Pre0
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v1r1-Pre0

For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v1r1-Pre0/setup.sh
```

or
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc494/contrib/compat/bashrc
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r1-Pre0/setup.sh
```


Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.