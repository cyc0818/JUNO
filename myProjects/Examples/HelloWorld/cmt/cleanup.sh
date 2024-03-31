# echo "cleanup HelloWorld v0 in /afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtHelloWorldtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtHelloWorldtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=HelloWorld -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  $* >${cmtHelloWorldtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=HelloWorld -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  $* >${cmtHelloWorldtempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtHelloWorldtempfile}
  unset cmtHelloWorldtempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtHelloWorldtempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtHelloWorldtempfile}
unset cmtHelloWorldtempfile
return $cmtcleanupstatus

