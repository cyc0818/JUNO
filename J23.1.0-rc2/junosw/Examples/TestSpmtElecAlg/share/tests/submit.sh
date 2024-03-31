#!/bin/bash

# $1 : generator
# $2 : events number 
# $3 : tag
# $4 : jobs number
# $5 : setup script

if [ $1 == "1" ] || [ $1 == "2" ] || [ $1 == "3" ] || \
   [ $1 == "4" ] || [ $1 == "5" ] || [ $1 == "6" ] || \
   [ $1 == "7" ] || [ $1 == "8" ] || [ $1 == "9" ]; then
(
cat << zzzEndOfFilezzz
 Universe = vanilla
 Executable = run_tests.sh
 Arguments = $1 $2 \$(Process) $3 $5
 Output = Out/conf_$1_$2_$3_\$(Process).out
 Error = Err/conf_$1_$2_$3_\$(Process).err
 Log = Log/conf_$1_$2_$3_\$(Process).log
 Accounting_Group = juno
 Requirements = Target.OpSysAndVer =?= "SL6"
 Queue $4
zzzEndOfFilezzz
) > Confs/conf_$1_$2_$3_$4.descr

condor_submit Confs/conf_$1_$2_$3_$4.descr -name job@schedd04.ihep.ac.cn
fi

