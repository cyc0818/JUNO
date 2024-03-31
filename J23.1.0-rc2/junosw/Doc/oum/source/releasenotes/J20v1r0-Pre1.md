# J20v1r0-Pre1 (with Geant4 10.04.p02) is released

## What's New?

In J20v1r0-Pre1, the default Geant4 version is 10.04.p02, since only the Geant4 10.4 is validated by simulation group.

Compared to Pre0, there are following changes:

* Detector simulation
  * Switch the default option from "--gdml" to "--no-gdml". Since this version, the geometry information will not be saved into event data file by default. If you need the geometry information, run the simulation with "--gdml" option.
* Electronics simulation
  * A memory leakage problem is fixed.

## Basic info

This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v1r0-Pre1
- offline data version: "trunk 50"
- external interface version: "trunk 27"
- sniper version: "v1.4" (GitHub)

The pre-compiled software could be found in CVMFS:
- SLC6+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1

For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:

```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc494/contrib/compat/bashrc
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1/setup.sh
```


Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.
