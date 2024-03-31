#!/bin/bash
source /cvmfs/juno.ihep.ac.cn/sw/anaconda/Anaconda3-2020.11-Linux-x86_64/bin/activate root624 && python TrainSklearn_PSDTools.py --name-sig e+ --name-bkg n --Range-fileNo-bkg 0 10 --Range-fileNo-sig 0 10 --input-path "root://junoeos01.ihep.ac.cn//eos/juno/user/huyuxiang/NucleonDecay/PSD_Train/{particle}/PsdSklearn/root/{particle}_{{fileNo}}.root" --Range-E 0 40 --Range-R3 0 4096
