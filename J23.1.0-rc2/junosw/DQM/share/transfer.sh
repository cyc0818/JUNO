#!/bin/bash -
file=cd_11111_300_600_
log=/junofs/users/zhangshu/DQM/output/queue_${file}.log

(/junofs/users/zhangshu/DQM/script/queuemonitor.sh ) >& $log &
sleep 1
(/junofs/users/zhangshu/DQM/script/transfer_tt.sh ) >& /junofs/users/zhangshu/DQM/output/tt.log &
sleep 1
for loop in {0..299};do
    filename=${file}${loop}
    cp ../offline/sl6_CD/${filename} ../test/
    sleep 2
    echo "transfer ${loop} "
    sleep 4
done

echo "transfer finished"

for loop in {0..9999999};do
	jobs=$(tail -n 1 /junofs/users/zhangshu/DQM/output/${log}.log)
	if [ $jobs=0 ]; then
		echo "rec is done , please kill queuemonitor subshell!!!!!"
        sleep 10
        touch /junofs/users/zhangshu/DQM/output/IsDone_${file}log
        exit 0
	fi
    sleep 30
    
done
exit 0
