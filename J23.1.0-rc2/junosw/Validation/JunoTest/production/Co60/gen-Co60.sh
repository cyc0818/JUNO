#!/bin/bash

function detsim_script() {
    echo $JUNOTOP/offline/Examples/Tutorial/share/tut_detsim.py
}

function gen_name() {
    local prefix=$1; shift
    local z=$1; shift
    local seed=$1; shift
    local suffix=$1; shift
    echo ${prefix}-0mm-0mm-${z}mm-${seed}.${suffix}
}

function create_job(){
GEN_NAME=$(gen_name sub $1 $2 condor)
SH_NAME=$(gen_name run $1 $2 sh)
if [ -f $GEN_NAME ]; then
    rm $GEN_NAME
fi

cat > $GEN_NAME <<EOF
Universe = vanilla
Executable = $SH_NAME
Accounting_Group = juno
getenv = True
Queue
EOF
}

function create_gen(){
SH_NAME=$(gen_name run $1 $2 sh)
LOG_NAME=$(gen_name log $1 $2 txt)
OUTPUT=$(gen_name sim $1 $2 root)
USER_OUTPUT=$(gen_name user-sim $1 $2 root)

if [ -f $SH_NAME ]; then
    rm $SH_NAME
fi

cat>$SH_NAME <<EOF
#!/bin/bash
cd $(pwd)
source $JUNOTOP/setup.sh
(time python $(detsim_script) --evtmax 1000 --seed $2 --output $OUTPUT --user-output $USER_OUTPUT --pmtsd-v2 --ce-mode 20inchfunc --no-gdml hepevt --exe Co60 --global-position 0 0 $1) >& $LOG_NAME
EOF

chmod +x $SH_NAME
}

TYPE=${1:-condor}; shift
SEED_START=1000
N=50
declare -a Z_ARRAY=(0 {1..16}000 16{2..8..2}00 17{0..7}00)
seed=0

for z in "${Z_ARRAY[@]}"
do
    for ((i=0; i<$N; i++))
    do
        create_gen $z $[$SEED_START+$seed]
        if [ "$TYPE" == "condor" ]; then
            create_job $z $[$SEED_START+$seed]
        fi
        ((++seed))
    done
done
