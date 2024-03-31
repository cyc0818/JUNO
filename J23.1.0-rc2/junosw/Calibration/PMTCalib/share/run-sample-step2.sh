#!/bin/bash
source /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/contrib/compat/bashrc
source /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Pre-Release/J19v1r0-Pre2/setup.sh
pushd /junofs/users/zhangxt/github/calibRec_juno/PMTCalib/share
#source /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Pre-Release/J19v1r0-Pre2/offline/Calibration/PMTCalib/cmt/cleanup.sh
source /junofs/users/zhangxt/github/calibRec_juno/PMTCalib/cmt/setup.sh
#(time python run.py --evtmax -1 --input /junofs/users/zhangxt/20inch/rec/deconvolution/runs/elec.list --output SPE_v19.root) >& SPE-log-v19.txt
#(time python /junofs/users/zhangxt/github/calibRec_juno/PMTCalib/share/run.py --evtmax -1 --input /junofs/users/zhangxt/20inch/rec/deconvolution/runs/elec.list --step 1 --output SPE_v19_step1.root) >& SPE-log-v19-step1.txt
(time python /junofs/users/zhangxt/github/calibRec_juno/PMTCalib/share/run.py --evtmax -1 --input /junofs/users/zhangxt/20inch/rec/deconvolution/runs/elec.list --step 2 --output SPE_v19.root) >& SPE-log-v19-step2.txt
popd
