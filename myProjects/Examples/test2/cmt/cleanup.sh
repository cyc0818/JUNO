# echo "cleanup cmt_standalone v0 in /afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples/test/cmt"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc447/Release/J17v1r1/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtcmt_standalonetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtcmt_standalonetempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=cmt_standalone -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples/test/cmt  $* >${cmtcmt_standalonetempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=cmt_standalone -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples/test/cmt  $* >${cmtcmt_standalonetempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtcmt_standalonetempfile}
  unset cmtcmt_standalonetempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtcmt_standalonetempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtcmt_standalonetempfile}
unset cmtcmt_standalonetempfile
return $cmtcleanupstatus

