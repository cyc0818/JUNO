#!/bin/bash
mkdir Elec/$1

(
cat << zzzEndOfFilezzz
 Universe = vanilla
 Executable = run_tests_elec.sh
 Arguments = $1 $2
 Output = Elec/$1/conf_$1.out
 Error = Elec/$1/conf_$1.err
 Log = Elec/$1/conf_$1.log
 Accounting_Group = juno
 Requirements = Target.OpSysAndVer =?= "SL6"
 Queue
zzzEndOfFilezzz
) > Confs/conf_elec_$1.descr

condor_submit Confs/conf_elec_$1.descr -name job@schedd04.ihep.ac.cn

