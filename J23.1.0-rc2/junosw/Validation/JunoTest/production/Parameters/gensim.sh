#!/bin/bash

function create_job(){
GEN_NAME="sub-detsim.condor"
if [ -f $GEN_NAME ]; then
    rm $GEN_NAME
fi

cat > $GEN_NAME <<EOF
Universe = vanilla
Executable = run-detsim.sh
Accounting_Group = juno
getenv = True
Queue
EOF
}

function create_gen(){
SH_NAME="run-detsim.sh"

if [ -f $SH_NAME ]; then
    rm $SH_NAME
fi

cat>$SH_NAME <<EOF
#!/bin/bash
cd $(pwd)
source $JUNOTOP/setup.sh
(time python $JUNOTOP/offline/Examples/Tutorial/share/tut_detsim.py --no-gdml --evtmax --anamgr-normal-hit 1 gun) >& log-detsim.txt
EOF

chmod +x $SH_NAME
}

TYPE=${1:-condor}; shift

create_gen $run
if [ "$TYPE" == "condor" ]; then
    create_job $run
fi
