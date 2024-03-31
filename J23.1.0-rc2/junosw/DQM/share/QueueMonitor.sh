#!/bin/bash -
echo "wait 5s per checking"
file=/junofs/users/zhangshu/DQM/output/IsDone_cd_11111_300_600_log
for loog in {0..99999};do
    echo $(condor_q zhangshu -format "%s" Owner -format " %s " Args -format " ProcId = %d\n" ProcId | wc -l)
    sleep 5
    if [ -f "$file" ];then
        echo "job is done"
        exit 0
    fi
done
exit 0