# J21v1r0-Pre0 is released

## What's New?

Compared to the previous release J20v2r0-Pre2, there are following major changes:

- Installation and 3rd party libraries
  - JUNOENV
    - It is updated to support both Python2 and Python3 builds.
    - Opticks sub-command is added by Simon Blyth. 
  - Update the external libraries.
    - Python: from 2.7.17 to 3.8.8. Users still could install Python 2 via junoenv by setting envvar JUNO_USE_PYTHON2. 
    - Boost: from 1.72.0 to 1.75.0.
    - CMake: from 3.17.0 to 3.19.6.
    - sqlite: from 3.29.0 to 3.35.2.
    - xrootd: from 4.10.0 to 4.12.6.
    - ROOT: from 6.20.02 to 6.22.08.
    - Geant4: add the patches on Li9/He8 decays from Cecile Jollet.
    - frontier: from 2.8.20 to 2.9.1
  - SNiPER v1.7
    - The compilation optimization "-O2" is added.
    - Migration the scripts to Python 3.

- Tutorial package
  - Re-implement the existing scripts, including tut_detsim.py, tut_det2elec.py, tut_elec2calib.py and tut_calib2rec.py.
  - Add a new script tut_elec2rec.py, which will do the electronics simulation, waveform reconstruction and event reconstruction.
    The /Event/Elec is not saved into the data file. 
  - All these scripts could be run in both Python 2 and Python 3. 

- Event Data Model
  - The XOD tool is forced to use Python 2 explictly.
  - The GenHeader and GenEvent is stored into ROOT file. 
  - ElecTruth
    - Add track absolute timestamp into track level truth info

- ROOT IO
  - The ROOT output service supports to create new files.
  
- Physics Generator
  - The DSNB generator is renamed to AtmNC.
  - Proton Decay generator is added. 
  - Muon: adjust wavefront time of tracks in muon bundle consistently and add a option for the number of tracks in muon bundle.
    
- Detector Simulation
  - Geometry and parameters
    - Acrylic cover  Absorption length
    - Acrylic Absorption length
    - Refractive index of PMT glass 
  - PMT optical model
    - New PMT optical model implemented, can be enabled by adding the option "--enable-pmt-optical-model" before executing detector simulation.
    - Multilayer thin film simulation service added, help to handle the optical processes on PMT photocathode
  - Opticks
    - Better support of QE/CE.
    - More modes to control the simulation in GPU or CPU. 
  - OP simulator
    - Implement the Cerenkov generation in OP simulator.
  - Voxel method
    - The histograms are updated based on the previous version. 

- Electronics Simulation
  - PMT response simulation:
    - add option for charge sharing function
  - Trigger Simulation (a new tool developed named "TriggerVFLTooLpmt"):
    - updates trigger logic as the same with hardware design;
    - update default trigger parameters as suggested by hardware groups;
    - update trigger data structure for more information storage;
    - add time offset correction option before trigger search.
  - Add a force trigger simulation tool

- SN monitor simulation
  - New package to simulate supernova prompt monitor alert.

- Reconstruction
  - COTIWaveRec: fix bugs in COTIWaveRec package:
    - time offset of reconstructed time is corrected properly now
    - the result is stored in the current EvtNavigator now instead of creating a new EvtNavigator
  - OMILREC
    - Enable Ge68+Laser time pdfs, which can reduce the radial bias at the total reflection region.
    - Enable Q-independent time pdfs, as described in JUNO-doc-6191-v2 and reported in JUNO-doc-6577-v3.
    - Define m_NQE as total charge.
    - Save likelihood value.
  - RecCdMuonAlg
    - Bugfix: modified the judgement in RecCdMuonAlg::freshPmtData() because of different number of PMTs in the new version
  
- Event Display
  - The event display is updated to support DQM. 

- DQM 
  - New package. 

## Basic info


This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J21v1r0-Pre0
- offline data version: "trunk 86"
- external interface version: "trunk 33"
- sniper version: "v1.7" (GitHub)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.
