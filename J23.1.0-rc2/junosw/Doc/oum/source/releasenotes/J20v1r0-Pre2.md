# J20v1r0-Pre2 (with Geant4 10.04.p02) is released

## What's New?


In J20v1r0-Pre2, the default Geant4 version is 10.04.p02, since only the Geant4 10.4 is validated by simulation group.

Compared to Pre1, there are following changes:

* Common service and Utilities
  * BufferMemMgr: add an option to stop whether root task or current task.
* Detector simulation
  * Support simulate the sub-chain of the Geant4 radioactivity decay module. 
  * Allow simulate Pa234m (pdg: 1000912341, E = 73.92 keV)
* Electronics simulation
  * Fixed the event index bug
  * Fixed the event missing bug
  * Enable the Supernova mode
  * Gain marker: Two electronics measuring ranges have been implemented,  use gain_marker to record electronics range used in current time window.
* Deconvolution: 
  1. Reconstruct overthreshold algorithm; 
  2. Add peaks seeking and hits width results in user_output.
* COTIwaverec: 
  1. A new algorithm for waveform reconstruction using continuous overthreshold integral method
* PMTCalibAlg: 
  1. Add calibration sources events selection algorithm
* PMTCalibSvc: 
  1. Update new calibration parameters.
* TimeCorrAlg: 
  1. Simplify codes; 
  2. Add a new parameter "forceTriggerSigma" to control the precision of force trigger
* RecTimeLikelihood : 
  1. Add TMinuit to replace GridSearch; 
  2. Solve DarkNoise issue near detector border; 
  3.  Add fos correction and bias correction; 
  4. Provide full chain of ToyElec simulation function
* OMILREC
  1. Implement the charge and time combining maximum likelihood  estimation method (QTMLE)
  2. Pre-calculate the 3-D nPE map for the QMLE/QTMLE algorithm
  3. Add reconstruction of the ideal case (use truth)
  4. Add reconstruction of the toy case (mimic the electronic  effect)
  5. Save more information about the minimization process  (CovMatrixStatus, fMemResident, corrEvtT, ...)
  6. Polish the code

## Basic info

This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J20v1r0-Pre2
- offline data version: "trunk 56" (Patch on Pre2: Updated in 2020.06.05)
- external interface version: "trunk 27"
- sniper version: "v1.4" (GitHub)


The pre-compiled software could be found in CVMFS:
- SLC6+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre2

For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:

```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc494/contrib/compat/bashrc
source /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre2/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.