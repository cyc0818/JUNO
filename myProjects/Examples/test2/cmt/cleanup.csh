# echo "cleanup cmt_standalone v0 in /afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples/test/cmt"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc447/Release/J17v1r1/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtcmt_standalonetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtcmt_standalonetempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=cmt_standalone -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples/test/cmt  $* >${cmtcmt_standalonetempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=cmt_standalone -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples/test/cmt  $* >${cmtcmt_standalonetempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtcmt_standalonetempfile}
  unset cmtcmt_standalonetempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtcmt_standalonetempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtcmt_standalonetempfile}
unset cmtcmt_standalonetempfile
exit $cmtcleanupstatus

