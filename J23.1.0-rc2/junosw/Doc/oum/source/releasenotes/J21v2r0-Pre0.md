# J21v2r0-Pre0 is released

## What's New? 

Compared to the previous release J21v1r0-Pre2, there are following major changes:

* IMPORTANT: The CMake is the default build tool for the offline. 
  * CMT related directories and files are not removed yet. 
  * All the CMT related part will remove in the next year release. 
* IMPORTANT: The SNiPER is upgraded to SNiPER v2
  * Due to several interfaces are changed in SNiPER, the offline is also updated.
  * As SNiPER uses JSON, the boolean value needs to be set explictly.
* Installation and 3rd party libraries
  * JUNOENV
    * Enable the cmake support of sniper, offline. 
  * Update the external libraries
    * Python: from 3.8.8 to 3.8.12.
    * Python-setuptools: from 54.2.0 to 58.0.4.
    * Python-pip: from 20.3.4 to 21.2.4
    * Python-cython: from 0.29.22 to 0.29.24
    * Python-numpy: from 1.20.1 to 1.21.2
    * Boost: from 1.75.0 to 1.77.0
    * CMake: from 3.19.6 to 3.21.2
    * Git: from 1.8.4.3 to 2.33.0
    * Sqlite3: from 3.35.2 to 3.35.5
    * xrootd: from 4.12.6 to 5.3.1
    * ROOT: from 6.22.08 to 6.24.06 (c++17)
    * libyaml: from 0.2.2 to 0.2.4
    * mysql-connector-cpp: from 1.1.8 to 1.1.12 (c++17)
  * SNiPER v2.0.3

* Tutorial package
  * module JUNOCommonModule.py
    * Add a MCParamsSvc into the common module.
  * module JUNODetSimModule.py 
    * Add a new generator option ReactorNuES in hepevt command.
    * Add an option --use-pmtsimsvc to enable/disable the new QE/CE interface.
    * Add an anamgr --anamgr-atmo to save the MCtruth for atmospheric neutrino.
    * Add an option --enableIonPHP to enable/disable the PHP model. Because the memory usage will be increased when PHP model is used. 
  * module JUNOElecSimModule.py
    * Add options to enable/disable the pulse level truths for the different inputs.
    * Add options to change the time windows. 
  * module JUNOEventRecModule.py 
    * Fix the boolean property.

* Event Data Model
  * EDMUtil
    * remove the dependecies on SNiPER, so that the EDM libraries only depends on ROOT.
    * WppmtElecTruthEvent is registered into JunoEDMDefinitions.
  * ElecEvent
    * Add TTElecEvent and TTElecChannel.
    * Change the CoarseTime from UInt_t to Double_t in SpmtElecAbcBlock.
    * Add a method AddAbcBlock in SpmtElecEvent.
  * ElecTruth
    * Add WppmtElecTruthEvent and WppmtElecTruth. 
    * Add methods to insert truth directly without copying the vector of truth. 
  * EvtNavigator
    * Use a macro SNIPER_VERSION_2 to use the new header "SniperKernel/DataBuffer.h" in SNiPER. 

* RootIO
  * RootIOTools
    * Fix the dependencies problem: need SimEventV2 for the merger.

* Detector
  * Geometry
    * Add the lightweight interfaces to access the PMT positions in IPMTParamSvc. 
    * Support the ideal positions and the real positions at the same time. 

* Physics Generator 
  * Add a new generate 'ReactorNuES' by Jinhao Huang.

* Detector Simulation
  * Geometry
    * The mask at 19.926m behind the PMTs is enabled by default. 
    * The PMT protector (tail) is enabled by default. 
    * The reflection on optical mask, PMT protector and strut bar are enabled by default, with a constant reflectivity of 53.5%.  
  * PMT optical model
    * Update the PMT optical parameters used in the new PMT optical model
    * Update light yield to 9507/MeV for the new PMT optical model
    * The new PMT optical model is enabled by default. Use --disable-pmt-optical-model option to switch to the old model.
  * PMTSimParamSvc
    * Provide API used to obtain QE and CE
    * Use linear interpolation to get CE
    * Add two new interfaces in PMTSimParamSvc which will be used by the new PMT optical model. 
  * Optical parameters
    * update absorption length of pyrex, QE of Hamamatsu PMT and NNVTMCP PMT
  * Sensitive detector
    * Use API of PMTSimParamSvc to get CE and QE
  * Physics processes
    * Add an option for users to modify the Cerenkov yield factor

* Electronics Simulation
  * Reduce the copy of pulse objects to optimize memory usage.
  * The MC truth could be turned off by users. 

* Reconstruction
  * RecTools
    * Add package RecTools. 
    * Use PMTTable as input and RecInfo as output.
  * QCtrRecAlg
    * Add another reconstruction option which utilizes "QcRecTool" and "PtfRecTool" to give vertex 
    * and utilizes "QSumTool" and "NPEUnifCTool" to give energy.
  * RecTimeLikeAlg
    * Isolate QcRecTool and PtfRecTool
    * Update the QcScale factor to solve low energy rec issue, which rec to local minimum (below 1 MeV)
  * OMILREC
    * Rewrite NPEUnifCTool based on IRecTool

* Validation
  * JunoTest
    * Update the log parser in drawmem.C due to the log is changed in SNiPER v2. 
    * Update the condor servers in IHEP. 

* OEC
  * The OEC package is now also integrated. Use the cmake option WITH_OEC to enable it.




## Basic info


This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J21v2r0-Pre0
- offline data version: "trunk 115"
- external interface version: "trunk 33"
- sniper version: "v2.0.3" (GitHub)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.


