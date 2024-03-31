#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo $DIR
for i in $( ls ./elecsim/user-elecsim-*.root); do
        VAR=$(echo $i | tr -d -c 0-9 )
        root -l -b -q "$DIR/validateCalib.C(\"./elecsim/user-elecsim-$VAR.root\",\"./calib/user-calib-$VAR.root\",\"$JUNOTEST_CALIB_ANA_OUTPUT/val-calib-$VAR.root\")" 
done

