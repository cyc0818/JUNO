# echo "setup DummyAlg v0 in /cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/sniper/Examples"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtDummyAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtDummyAlgtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=DummyAlg -version=v0 -path=/cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/sniper/Examples  -no_cleanup $* >${cmtDummyAlgtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=DummyAlg -version=v0 -path=/cvmfs/juno.ihep.ac.cn/sl7_amd64_gcc48/Release/J17v1r1/sniper/Examples  -no_cleanup $* >${cmtDummyAlgtempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtDummyAlgtempfile}
  unset cmtDummyAlgtempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtDummyAlgtempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtDummyAlgtempfile}
unset cmtDummyAlgtempfile
exit $cmtsetupstatus

