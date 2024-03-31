# echo "setup test v0 in /afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J20v2r0-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmttesttempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmttesttempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=test -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  -no_cleanup $* >${cmttesttempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=test -version=v0 -path=/afs/ihep.ac.cn/users/y/yuchincheng/myProjects/Examples  -no_cleanup $* >${cmttesttempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmttesttempfile}
  unset cmttesttempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmttesttempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmttesttempfile}
unset cmttesttempfile
return $cmtsetupstatus

