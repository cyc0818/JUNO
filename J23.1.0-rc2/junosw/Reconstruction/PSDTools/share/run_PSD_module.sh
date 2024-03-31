#!/bin/bash
source /afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/J21v2r0-branch/bashrc

option=_atm
INPUTFILELIST=fileList$option.txt
USEROUTPUT=user${option}.root
LOGFILE=log${option}.txt

METHOD=PSD_TMVA
#path_atm=root://junoeos01.ihep.ac.cn//eos/juno/valprod/valprod0/chengjie_J21/AtmNC/Model-G
id=$(printf "%06d\n" $1)
echo $id

(time python $PSDTOOLSROOT/share/tut_calib_rec2PSD.py --evtmax 20 --input-list  $INPUTFILELIST  --user-output ${USEROUTPUT} --method ${METHOD} --PrepareForTraining  )
