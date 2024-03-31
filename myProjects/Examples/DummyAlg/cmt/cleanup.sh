# echo "cleanup DummyAlg v0 in /cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/sniper/Examples"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDummyAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDummyAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=DummyAlg -version=v0 -path=/cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/sniper/Examples  $* >${cmtDummyAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=DummyAlg -version=v0 -path=/cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/sniper/Examples  $* >${cmtDummyAlgtempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtDummyAlgtempfile}
  unset cmtDummyAlgtempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtDummyAlgtempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtDummyAlgtempfile}
unset cmtDummyAlgtempfile
return $cmtcleanupstatus

