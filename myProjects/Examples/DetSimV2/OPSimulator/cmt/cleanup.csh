# echo "cleanup OPSimulator v0 in /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtOPSimulatortempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtOPSimulatortempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=OPSimulator -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  $* >${cmtOPSimulatortempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=OPSimulator -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  $* >${cmtOPSimulatortempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtOPSimulatortempfile}
  unset cmtOPSimulatortempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtOPSimulatortempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtOPSimulatortempfile}
unset cmtOPSimulatortempfile
exit $cmtcleanupstatus

