#!/bin/bash -

function job-top-dir() {
    echo ${TOPDIR:-/junofs/users/liuq/J16v1r2-validation/center}
}

function particle-type() {
    echo ${PARTICLE:-e+}
}

function particle-energy() {
    echo ${ENERGY:-0MeV}
}

function tag() {
      echo ${TAG:-test}
}

function run-script() {
      echo ${SCRIPTDIR:-/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Release/J16v1r2/offline/Examples/Tutorial/share}
}

function setup-script() {
    echo ${SETUPSCRIPT:-/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Release/J16v1r2/setup.sh}
}

function gen-name() {
    local prefix=$1; shift
    local seed=$1; shift
    local evtmax=$1; shift
    local suffix=$1; shift

#echo ${prefix}-${seed}-${evtmax}.${suffix}
    echo ${prefix}-${seed}.${suffix}
}

jobdir=$(job-top-dir)/$(particle-type)_$(particle-energy)
echo $jobdir
[ -d "$jobdir" ] || mkdir -p $jobdir

# parse arguments
# - seed
# - evtmax
seed=${1:-0}; shift
evtmax=${1:-10}; shift

echo \$SEED: $SEED
seedstart=${SEED:-200000}
seed=$(($seed+$seedstart))

input=$(gen-name sim $seed $evtmax root)
output=$(gen-name cal $seed $evtmax root)
log=$(gen-name callog $seed $evtmax txt)

cd $jobdir
source $(setup-script)
(time python $(run-script)/tut_det2calib.py --evtmax $evtmax \
                           --input $input \
                           --output $output \
                           --detoption Acrylic) >& $log
