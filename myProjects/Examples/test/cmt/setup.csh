# echo "setup test v0 in /afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmttesttempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmttesttempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=test -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  -no_cleanup $* >${cmttesttempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=test -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  -no_cleanup $* >${cmttesttempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmttesttempfile}
  unset cmttesttempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmttesttempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmttesttempfile}
unset cmttesttempfile
exit $cmtsetupstatus

