# echo "setup OPSimulator v0 in /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtOPSimulatortempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtOPSimulatortempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=OPSimulator -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  -no_cleanup $* >${cmtOPSimulatortempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=OPSimulator -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  -no_cleanup $* >${cmtOPSimulatortempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtOPSimulatortempfile}
  unset cmtOPSimulatortempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtOPSimulatortempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtOPSimulatortempfile}
unset cmtOPSimulatortempfile
exit $cmtsetupstatus
