# J21v1r0-Pre2 is released

## What's New?

Compared to the previous release J21v1r0-Pre1, there are following major changes:

* Event Data Model
  * New TTElecEvent. 

* Detector Simulation
  * Geometry and optical parameters
    * Add following options to enable the real surfaces (at 19.629 m) behind the PMTs.
      * `--real-surface-in-cd`: only enable the geometry of the surface. It will shrink the dimensions of all the PMTs and struts. 
      * `--enable-optical-surface-in-cd`: enable the optical property for that surface.
    * Add following options to enable the real masks behind the PMTs.
      * `--real-mask-tail`: only enable the geometry of the tail masks. 
      * `--enable-mask-tail-surface`: enable the optical property of the mask surface.
    * Add following option to enable the optical properties of the struts:
      * `--enable-strut-optical-surface`

* Electronics Simulation
  * New TT trigger simulation. 

## Basic info


This is a pre-release version for validation.
- svn url: http://juno.ihep.ac.cn/svn/offline/tags/J21v1r0-Pre2
- offline data version: "trunk 107"
- external interface version: "trunk 33"
- sniper version: "v1.7" (GitHub)


The pre-compiled software could be found in CVMFS:
- CentOS7+gcc 8.3.0: /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2


For users in IN2P3 and INFN-CNAF, you can also use CVMFS as it is mounted already:
```bash
export CMTCONFIG=amd64_linux26
source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
```

Please contact with Tao Lin <lintao@ihep.ac.cn> if there are any problems.


