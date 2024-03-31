# echo "cleanup G4Svc v0 in /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtG4Svctempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtG4Svctempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=G4Svc -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  $* >${cmtG4Svctempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=G4Svc -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  $* >${cmtG4Svctempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtG4Svctempfile}
  unset cmtG4Svctempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtG4Svctempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtG4Svctempfile}
unset cmtG4Svctempfile
return $cmtcleanupstatus

