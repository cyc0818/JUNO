#!/bin/bash

echo "parameters: " $1 $2

if [ ! -d "Elec/$1" ]; then
  mkdir Elec/$1
fi

cd ~
source  $2 ""
cd $WORKTOP/offline/Examples/TestSpmtElecAlg/share/tests/Elec/$1

ln -s ../../ROOT/$1.root sample_detsim.root
$WORKTOP/offline/Validation/JunoTest/production/libs/jobmom.sh $$ >& $1.log.mem.usage & python $WORKTOP/offline/Simulation/ElecSimV3/ElecSimAlg/share/run.py --evtmax -1 --input DEF:sample_detsim.root --loop DEF:0 --rate DEF:1.0  --startidx DEF:0
