#!/bin/bash
source /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Pre-Release/J19v1r0-Pre2/setup.sh
pushd /junofs/users/zhangxt/github/calibRec_juno/PMTCalibAlg/share
source /junofs/users/zhangxt/github/calibRec_juno/PMTCalibAlg/cmt/setup.sh
#(time python run.py --evtmax -1 --input /junofs/users/zhangxt/20inch/rec/deconvolution/runs/calib.list --calibmode calib --output sample_calibCorr_v19.root --user-output sample_calibCorr_user_v19.root) >& caliblog-test-v19.txt
(time python run.py --evtmax 1 --input /junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009sample/Laser/center/photon_110000/user-calib-1385.root --calibmode /junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009samplephy_m/CalibPars_m.txt --calibfile /junofs/users/zhangxt/20inch/rec/deconvolution/runs/20191009samplephy_m/C14/C14/user_calibCorr.root) >& caliblog-savepar.txt
popd
pushd /junofs/users/huangx/HXProject/MyProject/PMTCalibSvc/share
chmod 777 *
popd
