# J21v1r0-Pre1 is released

## What's New?

Compared to the previous release J21v1r0-Pre0, there are following major changes:

* Installation and 3rd party libraries
  * New external libraries
    * log4cpp: 1.1.3
    * LHAPDF: 6.3.0
    * pythia6: 6.4.28
    * genie: 3.00.06
    * nuwro: 19.02.2
    * talys: 1.95

* Improve the CMake support (prototype)
  * The code generation of event data models are run at configuration stage.
  * Support both make and ninja

* Tutorial package
  * New packages in cmt/requirements
    * Raw Data related packages: RawEvent, RawDataIOSvc, RawFile, RawAnalysis
    * Supernova monitor related package: PromptMonitor
    * Charge center reconstruction package: QCtrRecAlg
  * in detsim script:
    * Fix the bug due to the same mask name and PMT name, which cause the mask is not enabled in the simulation.
    * New options to tuning the MC optical parameters 
    * New option to enable the muon related anamgr. (Note: the previous option of anamgr config file is removed)
    * New options to use Opticks debugging generator. 
  * in elecsim script:
    * New option to enable MM trigger
    * New option to enable LPMT/SPMT in the same event
  * in waverec script:
    * Update the default values for WidthThreshold2 and Window1
    * Add the running of SPMTcalib
  * in evtrec script:
    * New option to enable QCtrRecAlg

* Raw Data related
  * Update time format in the RawEvent
  * Update the RawAnalysis algorithms with new demo

* Event Data Model
  * Update SimEvent to include the total hits in different types of PMTs. 
  * New CDTriggerEvent in ElecEvent package

* ROOT I/O
  * Update the merge.py script to support Python 3.

* Geometry service
  * The latest ROOT switches the default units in TGeoManager to the G4 units (mm/ns/MeV). For code compatibility, set the default units in TGeoManager to ROOT (cm/s/GeV) by default. 

* Physics Generator
  * The AtmNC is renamed to AtmNuGen

* Detector Simulation
  * Geometry and optical parameters
    * Switch to the MCParamSvc to manage all the optical parameters
    * The PMT related geometry information is managed by PMTParamSvc, while the parameters information is managed by PMTSimParamSvc.
    * The positions of veto PMTs are updated and now read from a file. 
  * Physics processes
    * Use the G4IonPhysicsPHP instead of G4IonPhysics to simulate the alpha. This requires the optional Geant4 dataset.
    * Support GRDM (which is already included in the previous patch version)
    * Enable the multiple components of LS decay time.
    * Fix the Cerenkov problem (which is already in the previous patch version)
      * fix the conditional jump due to the continue statement in do-while loop, which is pointed by Simon Blyth.
    * Provide an option which can specify different physics lists  

* Electronics Simulation
  * New MM Trigger.
  * New Trigger algorithms.
  * Enable LPMT/SPMT in the same readout event.
  * Use PMTSimParamSvc to access the PMT parameters.
  * Move the Service to read SPMT data (SpmtElecConfigSvc) from ElecSimAlg to SimSvc
  * Update the trigger in WP
    * Change from the Nhit trigger to zone trigger
    * Consider the GCU group

* Calibration
  * PMTCalibSvc
    * data: Update PMT parameters and SPE charge spectra for Deconvolution and COTI
  * SPMTCalibAlg
    * New package to run calibration for SPMT (number of pe determination) and add them in PmtCalibChannel output.

* Reconstruction
  * Deconvolution
    * add a switch of debug mode for Deconvolution
    * Disable the debug mode by default to avoid the large user root file.
    * data: Update filter and SPE spectra for Deconvolution. 
  * QCtrRecAlg
    * Merge the previous Zhengyun's implementation and Xin's implementation. This will be used in OEC.
  * OMILREC
    * Update nPE maps, time pdfs and charge pdfs for J21v1r0-Pre1(Pre0) 
    * Enable using different types of PMT for reconstruction (add enableDynInfoOnly and enableMcpInfoOnly)
    * Merge Calculate_Vertex_Likelihood into Calculate_EVLikelihood; save QMLE/QTMLE energy in pe units(m_PESum); fix calculation of IDNProb
  * RecTimeLikeAlg
    * Add r-z-dist tres pdf, as shown in docdb-6563
    * Add charge map to estimate expected npe
    * Add function to use Dynode or MCP only for reconstruction (SwitchPMTType)

## Basic info


This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J21v1r0-Pre1
- offline data version: "trunk 104"
- external interface version: "trunk 33"
- sniper version: "v1.7" (GitHub)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre1


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre1/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.
