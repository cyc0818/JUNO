CMTPATH=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
CMT_tag=$(tag)
CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
CMT_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
CMTVERSION=v1r26
CMT_offset=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs
cmt_hardware_query_command=uname -m
cmt_hardware=`$(cmt_hardware_query_command)`
cmt_system_version_query_command=${CMTROOT}/mgr/cmt_linux_version.sh | ${CMTROOT}/mgr/cmt_filter_version.sh
cmt_system_version=`$(cmt_system_version_query_command)`
cmt_compiler_version_query_command=${CMTROOT}/mgr/cmt_gcc_version.sh | ${CMTROOT}/mgr/cmt_filter3_version.sh
cmt_compiler_version=`$(cmt_compiler_version_query_command)`
PATH=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/InstallArea/${CMTCONFIG}/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/InstallArea/${CMTCONFIG}/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/InstallArea/${CMTCONFIG}/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26/${CMTBIN}:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/frontier/2.8.20/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/pacparser/1.3.7/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/podio/00-10/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-yaml/5.1.2/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/libyaml/0.2.2/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-cython/0.29.16/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/mysql-connector-cpp/1.1.8/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/mysql-connector-c/6.1.9/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/libmore/0.8.3/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Geant4/10.04.p02/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/HepMC/2.06.09/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/ROOT/6.20.02/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/xrootd/4.10.0/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/CLHEP/2.4.1.0/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/tbb/2019_U8/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/sqlite3/3.29.0/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/fftw3/3.3.8/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/gsl/2.5/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Xercesc/3.2.2/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Cmake/3.17.0/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Boost/1.72.0/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Python/2.7.17/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/contrib/gcc/8.3.0/bin:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/contrib/binutils/2.28/bin:/usr/lib64/qt-3.3/bin:/usr/local/bin:/usr/bin:/usr/local/sbin:/usr/sbin:/opt/puppetlabs/bin
CLASSPATH=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26/java
debug_option=-g
cc=gcc
ccomp=$(cc) -c $(includes) $(cdebugflags) $(cflags) $(pp_cflags)
clink=$(cc) $(clinkflags) $(cdebugflags)
ppcmd=-I
preproc=c++ -MD -c 
cpp=g++
cppflags=-std=c++11 -fPIC -pipe -W -Wall -Wwrite-strings -Wpointer-arith -Woverloaded-virtual 
pp_cppflags=-D_GNU_SOURCE
cppcomp=$(cpp) -c $(includes) $(cppdebugflags) $(cppflags) $(pp_cppflags)
cpplink=$(cpp) $(cpplinkflags) $(cppdebugflags)
for=g77
fflags=$(debug_option)
fcomp=$(for) -c $(fincludes) $(fflags) $(pp_fflags)
flink=$(for) $(flinkflags)
javacomp=javac -classpath $(src):$(CLASSPATH) 
javacopy=cp
jar=jar
X11_cflags=-I/usr/include
Xm_cflags=-I/usr/include
X_linkopts=-L/usr/X11R6/lib -lXm -lXt -lXext -lX11 -lm
lex=lex $(lexflags)
yaccflags= -l -d 
yacc=yacc $(yaccflags)
ar=ar cr
ranlib=ranlib
make_shlib=${CMTROOT}/mgr/cmt_make_shlib_common.sh extract
shlibsuffix=so
shlibbuilder=g++ $(cmt_installarea_linkopts) 
shlibflags=-shared
symlink=/bin/ln -fs 
symunlink=/bin/rm -f 
library_install_command=python $(SniperPolicy_root)/cmt/fragments/install.py -xCVS -x.svn -x*~ -x*.stamp -s --log=./install.history 
build_library_links=$(cmtexe) build library_links -tag=$(tags)
remove_library_links=$(cmtexe) remove library_links -tag=$(tags)
cmtexe=${CMTROOT}/${CMTBIN}/cmt.exe
build_prototype=$(cmtexe) build prototype
build_dependencies=$(cmtexe) -tag=$(tags) build dependencies
build_triggers=$(cmtexe) build triggers
format_dependencies=${CMTROOT}/mgr/cmt_format_deps.sh
implied_library_prefix=-l
SHELL=/bin/sh
q="
src=../src/
doc=../doc/
inc=../src/
mgr=../cmt/
application_suffix=.exe
library_prefix=lib
unlock_command=rm -rf 
lock_name=cmt
lock_suffix=.lock
lock_file=${lock_name}${lock_suffix}
svn_checkout_command=python ${CMTROOT}/mgr/cmt_svn_checkout.py 
gmake_hosts=lx1 rsplus lxtest as7 dxplus ax7 hp2 aleph hp1 hpplus papou1-fe atlas
make_hosts=virgo-control1 rio0a vmpc38a
everywhere=hosts
install_command=python $(SniperPolicy_root)/cmt/fragments/install.py -xCVS -x.svn -x*~ -x*.stamp --log=./install.history 
uninstall_command=python $(SniperPolicy_root)/cmt/fragments/install.py -u --log=./install.history 
cmt_installarea_command=python $(SniperPolicy_root)/cmt/fragments/install.py -xCVS -x.svn -x*~ -x*.stamp -s --log=./install.history 
cmt_uninstallarea_command=/bin/rm -f 
cmt_install_area_command=$(cmt_installarea_command)
cmt_uninstall_area_command=$(cmt_uninstallarea_command)
cmt_install_action=$(CMTROOT)/mgr/cmt_install_action.sh
cmt_installdir_action=$(CMTROOT)/mgr/cmt_installdir_action.sh
cmt_uninstall_action=$(CMTROOT)/mgr/cmt_uninstall_action.sh
cmt_uninstalldir_action=$(CMTROOT)/mgr/cmt_uninstalldir_action.sh
mkdir=mkdir
cmt_cvs_protocol_level=v1r1
cmt_installarea_prefix=InstallArea
CMT_PATH_remove_regexp=/[^/]*/
CMT_PATH_remove_share_regexp=/share/
NEWCMTCONFIG=x86_64-slc78-gcc830
SimUtil_tag=$(tag)
SIMUTILROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/SimUtil
SimUtil_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/SimUtil
SIMUTILVERSION=v0
SimUtil_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
SimUtil_offset=Simulation/DetSimV2
SimUtil_project=offline
SniperRelease_tag=$(tag)
SNIPERRELEASEROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperRelease
SniperRelease_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperRelease
SNIPERRELEASEVERSION=v2
SniperRelease_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
SniperRelease_project=sniper
SniperPolicy_tag=$(tag)
SNIPERPOLICYROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperPolicy
SniperPolicy_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperPolicy
SNIPERPOLICYVERSION=v0
SniperPolicy_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
SniperPolicy_project=sniper
libraryshr_linkopts=-fPIC -ldl -Wl,--as-needed -Wl,--no-undefined 
application_linkopts=-Wl,--export-dynamic 
BINDIR=$(tag)
remove_command=$(cmt_uninstallarea_command)
SniperKernel_tag=$(tag)
SNIPERKERNELROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperKernel
SniperKernel_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperKernel
SNIPERKERNELVERSION=v2
SniperKernel_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
SniperKernel_project=sniper
Boost_tag=$(tag)
BOOSTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Boost
Boost_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Boost
BOOSTVERSION=v0
Boost_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
Boost_offset=Externals
Boost_project=ExternalInterface
Python_tag=$(tag)
PYTHONROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Python
Python_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Python
PYTHONVERSION=v0
Python_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
Python_offset=Externals
Python_project=ExternalInterface
Python_linkopts= `pkg-config --libs python` 
Python_cppflags= `pkg-config --cflags python` 
Boost_home=${JUNO_EXTLIB_Boost_HOME}
Boost_pysuffix=${BOOST_PYTHON_SUFFIX}
Boost_linkopts= -L$(Boost_home)/lib  -lboost_python$(Boost_pysuffix) 
includes= $(ppcmd)"$(srcdir)" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/SimUtil/include" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/InstallArea/include" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/InstallArea/include" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/InstallArea/include" $(use_includes)
PYTHONPATH=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/InstallArea/python:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/InstallArea/python:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/InstallArea/python:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/InstallArea/amd64_linux26/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/InstallArea/amd64_linux26/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/InstallArea/amd64_linux26/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-yaml/5.1.2/lib/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-cython/0.29.16/lib/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/ROOT/6.20.02/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/xrootd/4.10.0/lib64/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Python/2.7.17/lib/./python2.7/lib-dynload:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Python/2.7.17/lib/python2.7/site-packages
SniperKernel_linkopts= -lSniperKernel  -lSniperPython 
SniperKernel_stamps=${SNIPERKERNELROOT}/${BINDIR}/SniperPython.stamp 
SniperKernel_linker_library=SniperPython
SniperPython_dependencies=SniperKernel
SniperPython_shlibflags= -lSniperKernel 
DataBuffer_tag=$(tag)
DATABUFFERROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperUtil/DataBuffer
DataBuffer_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperUtil/DataBuffer
DATABUFFERVERSION=v0
DataBuffer_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
DataBuffer_offset=SniperUtil
DataBuffer_project=sniper
HelloWorld_tag=$(tag)
HELLOWORLDROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/Examples/HelloWorld
HelloWorld_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/Examples/HelloWorld
HELLOWORLDVERSION=v1
HelloWorld_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
HelloWorld_offset=Examples
HelloWorld_project=sniper
PyDataStore_tag=$(tag)
PYDATASTOREROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperUtil/PyDataStore
PyDataStore_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperUtil/PyDataStore
PYDATASTOREVERSION=v0
PyDataStore_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
PyDataStore_offset=SniperUtil
PyDataStore_project=sniper
PyDataStore_linkopts= -lRootWriter 
PyDataStore_stamps=${PYDATASTOREROOT}/${BINDIR}/RootWriter.stamp 
PyDataStore_linker_library=RootWriter
RootWriter_tag=$(tag)
ROOTWRITERROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperSvc/RootWriter
RootWriter_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/SniperSvc/RootWriter
ROOTWRITERVERSION=v0
RootWriter_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
RootWriter_offset=SniperSvc
RootWriter_project=sniper
ROOT_tag=$(tag)
ROOTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/ROOT
ROOT_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/ROOT
ROOTVERSION=v0
ROOT_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
ROOT_offset=Externals
ROOT_project=ExternalInterface
ROOT_home=${JUNO_EXTLIB_ROOT_HOME}
ROOT_cppflags=`root-config --cflags` -Wno-long-long 
ROOT_linkopts=`root-config --evelibs` -lPyROOT 
rootcint="$(ROOT_home)/bin/rootcint"
RootWriter_linkopts= -lRootWriter 
RootWriter_stamps=${ROOTWRITERROOT}/${BINDIR}/RootWriter.stamp 
RootWriter_linker_library=RootWriter
DummyAlg_tag=$(tag)
DUMMYALGROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/Examples/DummyAlg
DummyAlg_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/Examples/DummyAlg
DUMMYALGVERSION=v0
DummyAlg_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
DummyAlg_offset=Examples
DummyAlg_project=sniper
DetSimPolicy_tag=$(tag)
DETSIMPOLICYROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/DetSimPolicy
DetSimPolicy_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/DetSimPolicy
DETSIMPOLICYVERSION=v0
DetSimPolicy_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
DetSimPolicy_offset=Simulation/DetSimV2
DetSimPolicy_project=offline
Geant4_tag=$(tag)
GEANT4ROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Geant4
Geant4_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Geant4
GEANT4VERSION=v0
Geant4_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
Geant4_offset=Externals
Geant4_project=ExternalInterface
Geant4_home=${JUNO_EXTLIB_Geant4_HOME}
Geant4_cppflags= `geant4-config --cflags` 
Geant4_linkopts= `geant4-config --libs` 
CLHEP_tag=$(tag)
CLHEPROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/CLHEP
CLHEP_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/CLHEP
CLHEPVERSION=v0
CLHEP_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
CLHEP_offset=Externals
CLHEP_project=ExternalInterface
CLHEP_home=${JUNO_EXTLIB_CLHEP_HOME}
CLHEP_linkopts= -L${JUNO_EXTLIB_CLHEP_HOME}/lib -lCLHEP 
CLHEP_cppflags= `clhep-config --include` 
Xercesc_tag=$(tag)
XERCESCROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Xercesc
Xercesc_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/Xercesc
XERCESCVERSION=v0
Xercesc_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
Xercesc_offset=Externals
Xercesc_project=ExternalInterface
Xercesc_linkopts= `pkg-config --libs xerces-c` 
Xercesc_cppflags= `pkg-config --cflags xerces-c` 
OpticksG4OK_tag=$(tag)
OPTICKSG4OKROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/OpticksG4OK
OpticksG4OK_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface/Externals/OpticksG4OK
OPTICKSG4OKVERSION=v0
OpticksG4OK_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
OpticksG4OK_offset=Externals
OpticksG4OK_project=ExternalInterface
DetSimAlg_tag=$(tag)
DETSIMALGROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/DetSimAlg
DetSimAlg_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/DetSimAlg
DETSIMALGVERSION=v0
DetSimAlg_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
DetSimAlg_offset=Simulation/DetSimV2
DetSimAlg_project=offline
G4Svc_tag=$(tag)
G4SVCROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/G4Svc
G4Svc_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/G4Svc
G4SVCVERSION=v0
G4Svc_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
G4Svc_offset=Simulation/DetSimV2
G4Svc_project=offline
G4Svc_linkopts= -lG4Svc 
G4Svc_stamps=${G4SVCROOT}/${BINDIR}/G4Svc.stamp 
G4Svc_linker_library=G4Svc
DetSimAlg_linkopts= -lDetSimAlg 
DetSimAlg_stamps=${DETSIMALGROOT}/${BINDIR}/DetSimAlg.stamp 
DetSimAlg_linker_library=DetSimAlg
SimEventV2_tag=$(tag)
SIMEVENTV2ROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/SimEventV2
SimEventV2_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/SimEventV2
SIMEVENTV2VERSION=v0
SimEventV2_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
SimEventV2_offset=DataModel
SimEventV2_project=offline
BaseEvent_tag=$(tag)
BASEEVENTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/BaseEvent
BaseEvent_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/BaseEvent
BASEEVENTVERSION=v0
BaseEvent_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
BaseEvent_offset=DataModel
BaseEvent_project=offline
XmlObjDesc_tag=$(tag)
XMLOBJDESCROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/XmlObjDesc
XmlObjDesc_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/XmlObjDesc
XMLOBJDESCVERSION=v0
XmlObjDesc_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
XmlObjDesc_project=offline
xmlsrc=xml
XODflags= -n JM 
BaseEvent_dependencies= BaseEventObj2Doth  BaseEventDict  BaseEventxodsrc 
install_more_includes_dependencies= BaseEventObj2Doth 
BaseEventDict_dependencies= BaseEventObj2Doth  install_more_includes 
BaseEventxodsrc_dependencies= BaseEventObj2Doth 
BaseEvent_linkopts= -lBaseEvent 
BaseEvent_stamps=${BASEEVENTROOT}/${BINDIR}/BaseEvent.stamp 
BaseEvent_linker_library=BaseEvent
EDMUtil_tag=$(tag)
EDMUTILROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/EDMUtil
EDMUtil_root=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/EDMUtil
EDMUTILVERSION=v0
EDMUtil_cmtpath=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
EDMUtil_offset=DataModel
EDMUtil_project=offline
EDMUtil_dependencies= EDMUtilDict  EDMUtilDict  EDMUtilDict  EDMUtilDict 
EDMUtilDict_dependencies= EDMUtilObj2Doth  install_more_includes  EDMUtilObj2Doth  install_more_includes  EDMUtilObj2Doth  install_more_includes  EDMUtilObj2Doth  install_more_includes 
EDMUtil_linkopts= -lEDMUtil 
EDMUtil_stamps=${EDMUTILROOT}/${BINDIR}/EDMUtil.stamp 
EDMUtil_linker_library=EDMUtil
SimEventV2_cintflags= -I$(SIMEVENTV2ROOT)  -I$(BASEEVENTROOT)  -I$(EDMUTILROOT) 
SimHeader_cintflags= $(SimEventV2_cintflags) 
SimPMTHit_cintflags= $(SimEventV2_cintflags) 
SimTrack_cintflags= $(SimEventV2_cintflags) 
SimEvent_cintflags= $(SimEventV2_cintflags) 
SimTTHit_cintflags= $(SimEventV2_cintflags) 
SimEventV2_dependencies= SimEventV2Dict  SimEventV2Dict  SimEventV2Dict  SimEventV2Dict  SimEventV2Dict 
SimEventV2Dict_dependencies= SimEventV2Obj2Doth  install_more_includes  SimEventV2Obj2Doth  install_more_includes  SimEventV2Obj2Doth  install_more_includes  SimEventV2Obj2Doth  install_more_includes  SimEventV2Obj2Doth  install_more_includes 
SimEventV2_linkopts= -lSimEventV2 
SimEventV2_stamps=${SIMEVENTV2ROOT}/${BINDIR}/SimEventV2.stamp 
SimEventV2_linker_library=SimEventV2
test_SimEvent_write_dependencies=SimEventV2
test_SimEvent_read_dependencies=SimEventV2
SimUtil_linkopts= -lSimUtil 
SimUtil_shlibflags=$(libraryshr_linkopts) $(cmt_installarea_linkopts) $(SimUtil_use_linkopts)
SimUtil_use_linkopts=    $(DetSimAlg_linkopts)  $(SniperRelease_linkopts)  $(DataBuffer_linkopts)  $(HelloWorld_linkopts)  $(PyDataStore_linkopts)  $(DummyAlg_linkopts)  $(RootWriter_linkopts)  $(G4Svc_linkopts)  $(SimEventV2_linkopts)  $(EDMUtil_linkopts)  $(SniperKernel_linkopts)  $(BaseEvent_linkopts)  $(SniperPolicy_linkopts)  $(Boost_linkopts)  $(Python_linkopts)  $(ROOT_linkopts)  $(DetSimPolicy_linkopts)  $(Geant4_linkopts)  $(CLHEP_linkopts)  $(Xercesc_linkopts)  $(OpticksG4OK_linkopts) 
SimUtil_stamps=${SIMUTILROOT}/${BINDIR}/SimUtil.stamp 
SimUtil_linker_library=SimUtil
tag=amd64_linux26
package=SimUtil
version=v0
PACKAGE_ROOT=$(SIMUTILROOT)
srcdir=../src
bin=../$(SimUtil_tag)/
javabin=../classes/
mgrdir=cmt
BIN=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2/SimUtil/amd64_linux26/
project=offline
cmt_installarea_paths= $(cmt_installarea_prefix)/$(CMTCONFIG)/bin $(sniper_installarea_prefix)/$(CMTCONFIG)/lib $(sniper_installarea_prefix)/share/lib $(sniper_installarea_prefix)/share/bin $(mt.sniper_installarea_prefix)/$(CMTCONFIG)/lib $(mt.sniper_installarea_prefix)/share/lib $(mt.sniper_installarea_prefix)/share/bin $(offline_installarea_prefix)/$(CMTCONFIG)/lib $(offline_installarea_prefix)/share/lib $(offline_installarea_prefix)/share/bin
use_linkopts= $(cmt_installarea_linkopts)   $(SimUtil_linkopts)  $(DetSimAlg_linkopts)  $(SniperRelease_linkopts)  $(DataBuffer_linkopts)  $(HelloWorld_linkopts)  $(PyDataStore_linkopts)  $(DummyAlg_linkopts)  $(RootWriter_linkopts)  $(G4Svc_linkopts)  $(SimEventV2_linkopts)  $(EDMUtil_linkopts)  $(SniperKernel_linkopts)  $(BaseEvent_linkopts)  $(SniperPolicy_linkopts)  $(Boost_linkopts)  $(Python_linkopts)  $(ROOT_linkopts)  $(DetSimPolicy_linkopts)  $(Geant4_linkopts)  $(CLHEP_linkopts)  $(Xercesc_linkopts)  $(OpticksG4OK_linkopts) 
ExternalInterface_installarea_prefix=$(cmt_installarea_prefix)
ExternalInterface_installarea_prefix_remove=$(ExternalInterface_installarea_prefix)
LD_LIBRARY_PATH=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/InstallArea/${CMTCONFIG}/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/InstallArea/${CMTCONFIG}/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/InstallArea/${CMTCONFIG}/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/frontier/2.8.20/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/pacparser/1.3.7/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/podio/00-10/lib64:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-yaml/5.1.2/lib/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-yaml/5.1.2/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/libyaml/0.2.2/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-cython/0.29.16/lib/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/python-cython/0.29.16/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/mysql-connector-cpp/1.1.8/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/mysql-connector-c/6.1.9/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/libmore/0.8.3/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Geant4/10.04.p02/lib64:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/HepMC/2.06.09/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/ROOT/6.20.02/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/xrootd/4.10.0/lib64/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/xrootd/4.10.0/lib64:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/CLHEP/2.4.1.0/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/tbb/2019_U8/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/sqlite3/3.29.0/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/fftw3/3.3.8/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/gsl/2.5/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Xercesc/3.2.2/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Boost/1.72.0/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Python/2.7.17/lib/python2.7/site-packages:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Python/2.7.17/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/contrib/gcc/8.3.0/lib64:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/contrib/gcc/8.3.0/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/contrib/binutils/2.28/lib64:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/contrib/binutils/2.28/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CLHEP/2.4.1.0/lib:/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/Xercesc/3.2.2/lib
sniper_installarea_prefix=$(cmt_installarea_prefix)
sniper_installarea_prefix_remove=$(sniper_installarea_prefix)
mt.sniper_installarea_prefix=$(cmt_installarea_prefix)
mt.sniper_installarea_prefix_remove=$(mt.sniper_installarea_prefix)
offline_installarea_prefix=$(cmt_installarea_prefix)
offline_installarea_prefix_remove=$(offline_installarea_prefix)
cmt_installarea_linkopts= -L/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/$(offline_installarea_prefix)/$(CMTCONFIG)/lib  -L/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/$(mt.sniper_installarea_prefix)/$(CMTCONFIG)/lib  -L/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/$(sniper_installarea_prefix)/$(CMTCONFIG)/lib 
ExternalInterface_home=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalInterface
sniper_home=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper
mt.sniper_home=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper
offline_home=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline
offline_install_include= /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/$(offline_installarea_prefix)/include 
mt.sniper_install_include= /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/$(mt.sniper_installarea_prefix)/include 
sniper_install_include= /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/$(sniper_installarea_prefix)/include 
sniper_python_path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/$(sniper_installarea_prefix)/$(tag)/lib
mt.sniper_python_path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/$(mt.sniper_installarea_prefix)/$(tag)/lib
offline_python_path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/$(offline_installarea_prefix)/$(tag)/lib
sniper_install_python=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/sniper/$(sniper_installarea_prefix)/python
mt.sniper_install_python=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/mt.sniper/$(mt.sniper_installarea_prefix)/python
offline_install_python=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/$(offline_installarea_prefix)/python
CMTINSTALLAREA=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/$(cmt_installarea_prefix)
use_requirements=requirements $(CMT_root)/mgr/requirements $(DetSimAlg_root)/cmt/requirements $(SniperRelease_root)/cmt/requirements $(DataBuffer_root)/cmt/requirements $(HelloWorld_root)/cmt/requirements $(PyDataStore_root)/cmt/requirements $(DummyAlg_root)/cmt/requirements $(RootWriter_root)/cmt/requirements $(G4Svc_root)/cmt/requirements $(SimEventV2_root)/cmt/requirements $(EDMUtil_root)/cmt/requirements $(SniperKernel_root)/cmt/requirements $(BaseEvent_root)/cmt/requirements $(XmlObjDesc_root)/cmt/requirements $(SniperPolicy_root)/cmt/requirements $(Boost_root)/cmt/requirements $(Python_root)/cmt/requirements $(ROOT_root)/cmt/requirements $(DetSimPolicy_root)/cmt/requirements $(Geant4_root)/cmt/requirements $(CLHEP_root)/cmt/requirements $(Xercesc_root)/cmt/requirements $(OpticksG4OK_root)/cmt/requirements 
use_includes= $(ppcmd)"$(DetSimAlg_root)/src" $(ppcmd)"$(SniperRelease_root)/src" $(ppcmd)"$(DataBuffer_root)/src" $(ppcmd)"$(HelloWorld_root)/src" $(ppcmd)"$(PyDataStore_root)/src" $(ppcmd)"$(DummyAlg_root)/src" $(ppcmd)"$(RootWriter_root)/src" $(ppcmd)"$(G4Svc_root)/src" $(ppcmd)"$(SimEventV2_root)/src" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/SimEventV2/" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/SimEventV2/Event" $(ppcmd)"$(EDMUtil_root)/src" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/EDMUtil/include" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/EDMUtil/EDMUtil" $(ppcmd)"$(SniperKernel_root)/src" $(ppcmd)"$(BaseEvent_root)/src" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/DataModel/BaseEvent/Event" $(ppcmd)"$(SniperPolicy_root)/src" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/Boost/1.72.0/include" $(ppcmd)"$(Python_root)/src" $(ppcmd)"/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/ExternalLibs/ROOT/6.20.02/include" $(ppcmd)"$(DetSimPolicy_root)/src" $(ppcmd)"$(Geant4_root)/src" $(ppcmd)"$(CLHEP_root)/src" $(ppcmd)"$(Xercesc_root)/src" $(ppcmd)"$(OpticksG4OK_root)/src" 
use_fincludes= $(use_includes)
use_stamps=  $(SimUtil_stamps)  $(DetSimAlg_stamps)  $(SniperRelease_stamps)  $(DataBuffer_stamps)  $(HelloWorld_stamps)  $(PyDataStore_stamps)  $(DummyAlg_stamps)  $(RootWriter_stamps)  $(G4Svc_stamps)  $(SimEventV2_stamps)  $(EDMUtil_stamps)  $(SniperKernel_stamps)  $(BaseEvent_stamps)  $(SniperPolicy_stamps)  $(Boost_stamps)  $(Python_stamps)  $(ROOT_stamps)  $(DetSimPolicy_stamps)  $(Geant4_stamps)  $(CLHEP_stamps)  $(Xercesc_stamps)  $(OpticksG4OK_stamps) 
use_cflags=  $(SimUtil_cflags)  $(DetSimAlg_cflags)  $(SniperRelease_cflags)  $(DataBuffer_cflags)  $(HelloWorld_cflags)  $(PyDataStore_cflags)  $(DummyAlg_cflags)  $(RootWriter_cflags)  $(G4Svc_cflags)  $(SimEventV2_cflags)  $(EDMUtil_cflags)  $(SniperKernel_cflags)  $(BaseEvent_cflags)  $(SniperPolicy_cflags)  $(Boost_cflags)  $(Python_cflags)  $(ROOT_cflags)  $(DetSimPolicy_cflags)  $(Geant4_cflags)  $(CLHEP_cflags)  $(Xercesc_cflags)  $(OpticksG4OK_cflags) 
use_pp_cflags=  $(SimUtil_pp_cflags)  $(DetSimAlg_pp_cflags)  $(SniperRelease_pp_cflags)  $(DataBuffer_pp_cflags)  $(HelloWorld_pp_cflags)  $(PyDataStore_pp_cflags)  $(DummyAlg_pp_cflags)  $(RootWriter_pp_cflags)  $(G4Svc_pp_cflags)  $(SimEventV2_pp_cflags)  $(EDMUtil_pp_cflags)  $(SniperKernel_pp_cflags)  $(BaseEvent_pp_cflags)  $(SniperPolicy_pp_cflags)  $(Boost_pp_cflags)  $(Python_pp_cflags)  $(ROOT_pp_cflags)  $(DetSimPolicy_pp_cflags)  $(Geant4_pp_cflags)  $(CLHEP_pp_cflags)  $(Xercesc_pp_cflags)  $(OpticksG4OK_pp_cflags) 
use_cppflags=  $(SimUtil_cppflags)  $(DetSimAlg_cppflags)  $(SniperRelease_cppflags)  $(DataBuffer_cppflags)  $(HelloWorld_cppflags)  $(PyDataStore_cppflags)  $(DummyAlg_cppflags)  $(RootWriter_cppflags)  $(G4Svc_cppflags)  $(SimEventV2_cppflags)  $(EDMUtil_cppflags)  $(SniperKernel_cppflags)  $(BaseEvent_cppflags)  $(SniperPolicy_cppflags)  $(Boost_cppflags)  $(Python_cppflags)  $(ROOT_cppflags)  $(DetSimPolicy_cppflags)  $(Geant4_cppflags)  $(CLHEP_cppflags)  $(Xercesc_cppflags)  $(OpticksG4OK_cppflags) 
use_pp_cppflags=  $(SimUtil_pp_cppflags)  $(DetSimAlg_pp_cppflags)  $(SniperRelease_pp_cppflags)  $(DataBuffer_pp_cppflags)  $(HelloWorld_pp_cppflags)  $(PyDataStore_pp_cppflags)  $(DummyAlg_pp_cppflags)  $(RootWriter_pp_cppflags)  $(G4Svc_pp_cppflags)  $(SimEventV2_pp_cppflags)  $(EDMUtil_pp_cppflags)  $(SniperKernel_pp_cppflags)  $(BaseEvent_pp_cppflags)  $(SniperPolicy_pp_cppflags)  $(Boost_pp_cppflags)  $(Python_pp_cppflags)  $(ROOT_pp_cppflags)  $(DetSimPolicy_pp_cppflags)  $(Geant4_pp_cppflags)  $(CLHEP_pp_cppflags)  $(Xercesc_pp_cppflags)  $(OpticksG4OK_pp_cppflags) 
use_fflags=  $(SimUtil_fflags)  $(DetSimAlg_fflags)  $(SniperRelease_fflags)  $(DataBuffer_fflags)  $(HelloWorld_fflags)  $(PyDataStore_fflags)  $(DummyAlg_fflags)  $(RootWriter_fflags)  $(G4Svc_fflags)  $(SimEventV2_fflags)  $(EDMUtil_fflags)  $(SniperKernel_fflags)  $(BaseEvent_fflags)  $(SniperPolicy_fflags)  $(Boost_fflags)  $(Python_fflags)  $(ROOT_fflags)  $(DetSimPolicy_fflags)  $(Geant4_fflags)  $(CLHEP_fflags)  $(Xercesc_fflags)  $(OpticksG4OK_fflags) 
use_pp_fflags=  $(SimUtil_pp_fflags)  $(DetSimAlg_pp_fflags)  $(SniperRelease_pp_fflags)  $(DataBuffer_pp_fflags)  $(HelloWorld_pp_fflags)  $(PyDataStore_pp_fflags)  $(DummyAlg_pp_fflags)  $(RootWriter_pp_fflags)  $(G4Svc_pp_fflags)  $(SimEventV2_pp_fflags)  $(EDMUtil_pp_fflags)  $(SniperKernel_pp_fflags)  $(BaseEvent_pp_fflags)  $(SniperPolicy_pp_fflags)  $(Boost_pp_fflags)  $(Python_pp_fflags)  $(ROOT_pp_fflags)  $(DetSimPolicy_pp_fflags)  $(Geant4_pp_fflags)  $(CLHEP_pp_fflags)  $(Xercesc_pp_fflags)  $(OpticksG4OK_pp_fflags) 
use_libraries= $(DetSimAlg_libraries)  $(SniperRelease_libraries)  $(DataBuffer_libraries)  $(HelloWorld_libraries)  $(PyDataStore_libraries)  $(DummyAlg_libraries)  $(RootWriter_libraries)  $(G4Svc_libraries)  $(SimEventV2_libraries)  $(EDMUtil_libraries)  $(SniperKernel_libraries)  $(BaseEvent_libraries)  $(XmlObjDesc_libraries)  $(SniperPolicy_libraries)  $(Boost_libraries)  $(Python_libraries)  $(ROOT_libraries)  $(DetSimPolicy_libraries)  $(Geant4_libraries)  $(CLHEP_libraries)  $(Xercesc_libraries)  $(OpticksG4OK_libraries) 
fincludes= $(includes)
SimUtil_GUID={88BF15AB-5A2D-4bea-B64F-02752C2A1F4F}
make_GUID={88BF15AB-5A2D-4bea-B64F-02752C2A1F4F}
constituents= SimUtil 
all_constituents= $(constituents)
constituentsclean= SimUtilclean 
all_constituentsclean= $(constituentsclean)
cmt_actions_constituents= make 
cmt_actions_constituentsclean= makeclean 
SimUtilprototype_dependencies= $(SimUtilcompile_dependencies)
