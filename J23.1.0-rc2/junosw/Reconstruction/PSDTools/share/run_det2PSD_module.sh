#!/bin/bash
source /afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/J21v2r0-trunk2/bashrc

Opt_ERec="--method energy-point --enableLTSPEs --enableTimeInfo --enableUseEkMap --enableQTimePdf"
option=_atm
INPUTFILELIST=fileList$option.txt
USEROUTPUT=user${option}.root
LOGFILE=log${option}.txt

METHOD=PSD_TMVA
path_atm=""
id=$1


(time python $PSDTOOLSROOT/share/tut_det2PSD.py --evtmax 1  --seed $1 \
--disableAfterPulse  --input "eventinput:${path_atm}det_sample_$id.root" --rate eventinput:1 \
--gdml $Opt_ERec  --SaveMC 0 --VtxCorr 0 --elec yes \
 --user-output ${USEROUTPUT} --method-PSD ${METHOD} --PrepareForTraining  )

