# echo "cleanup HelloWorld v0 in /afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtHelloWorldtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtHelloWorldtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=HelloWorld -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  $* >${cmtHelloWorldtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=HelloWorld -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  $* >${cmtHelloWorldtempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtHelloWorldtempfile}
  unset cmtHelloWorldtempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtHelloWorldtempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtHelloWorldtempfile}
unset cmtHelloWorldtempfile
exit $cmtcleanupstatus

