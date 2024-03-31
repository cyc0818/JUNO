# echo "setup DetSimAlg v0 in /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDetSimAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDetSimAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=DetSimAlg -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  -no_cleanup $* >${cmtDetSimAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=DetSimAlg -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  -no_cleanup $* >${cmtDetSimAlgtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtDetSimAlgtempfile}
  unset cmtDetSimAlgtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtDetSimAlgtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtDetSimAlgtempfile}
unset cmtDetSimAlgtempfile
return $cmtsetupstatus

