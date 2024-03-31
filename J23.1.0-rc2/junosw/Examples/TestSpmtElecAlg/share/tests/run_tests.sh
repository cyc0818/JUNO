#!/bin/bash

echo "parameters: " $1 $2 $3 $4 $5

cd ~
source  $5 ""
cd $WORKTOP/offline/Examples/TestSpmtElecAlg/share/tests

generator=$1
num=$2
seed=$3
tag=$4

echo `pwd`

if [ ${generator} == "1" ]; then
  of_name=electron_${tag}_${seed}_${num}_1MeV_Center_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml gun  --particles e-  --momentums 1.0
fi

if [ ${generator} == "2" ]; then
  of_name=electron_${tag}_${seed}_${num}_1MeV_Top_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml gun  --particles e-  --momentums 1.0 --positions 0 0 17500
fi

if [ ${generator} == "3" ]; then
  of_name=positrons_${tag}_${seed}_${num}_1MeV_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml gun  --particles e+  --momentums 1.0
fi

if [ ${generator} == "4" ]; then
  of_name=positrons_${tag}_${seed}_${num}_To20MeV_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml gun  --particles e+  --momentums 10.0 --momentums-mode Uniform --momentums-extra-params 10.0
fi

if [ ${generator} == "5" ]; then
  of_name=positrons_${tag}_${seed}_${num}_CDTo20MeV_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml gun  --particles e+  --momentums 10.0 --momentums-mode Uniform --momentums-extra-params 10.0 --volume pTarget
fi


if [ ${generator} == "6" ]; then
  of_name=IBD-eplus_${tag}_${seed}_${num}_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml hepevt --exe IBD-eplus --volume pTarget
fi

if [ ${generator} == "7" ]; then
  of_name=IBD-neutron_${tag}_${seed}_${num}_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml hepevt --exe IBD-neutron --volume pTarget
fi

if [ ${generator} == "8" ]; then
  of_name=IBD_${tag}_${seed}_${num}_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml hepevt --exe IBD --volume pTarget
fi

if [ ${generator} == "9" ]; then
  of_name=Mu_${tag}_${seed}_${num}_output
  $WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& Log/$1_$2_$3_$4.log.mem.usage &  python $TUTORIALROOT/share/tut_detsim.py  --evtmax ${num} --seed ${seed} --output ROOT/${of_name}.root --user-output ROOT_user/${of_name}_user.root --no-gdml --no-anamgr-normal --no-anamgr-deposit --no-anamgr-interesting-process --anamgr-list MuProcessAnaMgr --pmtsd-v2 --pmtsd-merge-twindow 1.0 --pmt-hit-type 2 hepevt --exe Muon
fi

