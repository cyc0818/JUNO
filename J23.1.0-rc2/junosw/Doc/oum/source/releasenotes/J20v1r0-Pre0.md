# J20v1r0-Pre0 (with Geant4 10.04.p02) is released
## What's New?

J20v1r0-Pre0 share the same offline code with J20v1r1-Pre0. The only difference is that the Geant4 version is 10.04.p02.

See J20v1r1-Pre1 release notes: https://juno.ihep.ac.cn/trac/ticket/94

## Basic info

This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v1r0-Pre0
- offline data version: "trunk 50"
- external interface version: "trunk 27"
- sniper version: "v1.4" (GitHub)

The pre-compiled software could be found in CVMFS:
- SLC6+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre0

For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:

```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc494/contrib/compat/bashrc
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre0/setup.sh
```


Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.