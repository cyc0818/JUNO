# echo "setup CalibUnit v0 in /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtCalibUnittempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtCalibUnittempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=CalibUnit -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  -no_cleanup $* >${cmtCalibUnittempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=CalibUnit -version=v0 -path=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-Pre1/offline/Simulation/DetSimV2  -no_cleanup $* >${cmtCalibUnittempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtCalibUnittempfile}
  unset cmtCalibUnittempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtCalibUnittempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtCalibUnittempfile}
unset cmtCalibUnittempfile
return $cmtsetupstatus

