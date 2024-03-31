# J20v2r0-Pre1 is released
## What's New?

Compared to the previous release J20v2r0-Pre0, there are following major changes:

- installation and 3rd party libraries
  - Add mt.sniper v0.1.

- common services
  - ROOT I/O is updated to support the large files (maximum: 1TB).
  - Add a new Frontier service to connect the database via Frontier.
  - CondDB:
    - Fix the SQL queries according to new databases,
    - Add a user interface to get the payload string

- generator
  - Supernova generator: add two options on mass hierarchy and flavour. for details see #103.
  
- detector simulation
  - Major changes (PLEASE BE CAREFUL):
    - To reduce the data volume of detsim user data, now all the hit/photon level truth are disabled.
    - Please use option "--anamgr-normal-hit" to enable them. See r4129.
  - Several new features, which are not enabled by default:
    - the new LS optical model
    - the new PMT optical model
    - the multi-threading detector simulation

- electronics simulation
  - SPMT update, see r4138 and r4146.

- calibration
  - PMTCalibSvc: update the calibration files.
  
- reconstruction
  - Deconvolution:
    - Update the SPE spectrum
    - set default value of "NTimesRMS" from 1.5 to 2 in tut_elec2calib.py.
  - RecTimeLikeAlg:
    - Polish code;
    - Consider DN in expected npe estimation, improve energy reconstruction performance;
    - Update PDF to match J20v2r0 electronic simulation.
  - OMILREC:
    -  Enable Ge68+Laser and phi-correction nPE map, which can improve the energy uniformity performance and solve the radial bias problem of QMLE. The Ge68+Laser nPE map has been uploaded to the offline-data folder.
    -  Fixed a small bug in RemoveDN function.
    -  Polish the code for concision and readability.
    
- validation
  - The plot scripts are updated according to the latest version.
    
## Basic info


This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v2r0-Pre1
- offline data version: "trunk 69"
- external interface version: "trunk 30"
- sniper version: "v1.5" (GitHub: https://github.com/SNiPER-Framework/sniper/releases/tag/v1.5) 
- mt.sniper version: "v0.1" (GitHub: https://github.com/SNiPER-Framework/mt.sniper/releases/tag/v0.1)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.