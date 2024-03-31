# J20v2r0-Pre2 is released
## What's New?

Compared to the previous release J20v2r0-Pre1, there are following major changes:

- installation and 3rd party libraries
  - add Docker and Singularity support.
  - xrootd: update from 4.10.0 to 4.12.5
    - If the files in EOS can't close sucessfully,
      set envvar `export XRD_STREAMTIMEOUT=600` common service
  - BufferMemMgr: fix a bug if there is no event in the input stream.
  - RootIO merger: fixed a bug that dropped the first sub-SimEvent.
- detector simulation
  - PrintG4TrackAnaMgr: enable the Geant4 tracking version for selected events.
  - allow split the hits in the edm output.
- electronics simulation
  - SPMT: save SPMT tree into user output and link the electronics channel and PMT.
- calibration
  - new package CondObj: the common conditions objects which provides the path().
- reconstruction
  - Deconvolution:
    - update the calibration constants in offline-data.
    - Optimize FFT's speed by Luoxj and associate variables:
      (widthThreshold,integralWidth) with nnvt and hmmt
  - RecTimeLikeAlg: update the parameters and support accessing the parameter paths from CondObj.
  - QTMLE: Enable vertex dependent time pdfs P(t|k, r, d).  

## Basic info

This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v2r0-Pre2
- offline data version: "trunk 73"
- external interface version: "trunk 30"
- sniper version: "v1.5" (GitHub: https://github.com/SNiPER-Framework/sniper/releases/tag/v1.5) 
- mt.sniper version: "v0.1.1" (GitHub: https://github.com/SNiPER-Framework/mt.sniper/releases/tag/v0.1.1)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre2


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre2/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.