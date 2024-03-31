# echo "cleanup Chimney v0 in /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtChimneytempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtChimneytempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=Chimney -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  $* >${cmtChimneytempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=Chimney -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  $* >${cmtChimneytempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtChimneytempfile}
  unset cmtChimneytempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtChimneytempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtChimneytempfile}
unset cmtChimneytempfile
return $cmtcleanupstatus
