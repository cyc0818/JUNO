#!/bin/bash

function create_list(){

GEN_NAME="sample-eplus.txt"
if [ -f $GEN_NAME ]; then
    rm $GEN_NAME
fi

for run in {10000..10049}
do
  USER_FILE_NAME="`pwd`/user-evt-$run-1000.root"
  echo $USER_FILE_NAME >> $GEN_NAME
done

}

function create_job(){
GEN_NAME="sub-$1-1000.condor"
if [ -f $GEN_NAME ]; then
    rm $GEN_NAME
fi

cat > $GEN_NAME <<EOF
Universe = vanilla
Executable = run-$1-1000.sh
Accounting_Group = juno
getenv = True
Queue
EOF
}

function create_gen(){
SH_NAME="run-$1-1000.sh"

if [ -f $SH_NAME ]; then
    rm $SH_NAME
fi

cat>$SH_NAME <<EOF
#!/bin/bash
cd $(pwd)
source $JUNOTOP/setup.sh
(time python $JUNOTOP/offline/Examples/Tutorial/share/tut_detsim.py --evtmax 1000  --no-gdml --seed $1 --output evt-$1-1000.root --user-output user-evt-$1-1000.root hepevt --exe IBD-eplus --volume pTarget) >& log-$1-1000.txt
EOF

chmod +x $SH_NAME
}

TYPE=${1:-condor}; shift

for run in {10000..10049}
do
    create_gen $run
    if [ "$TYPE" == "condor" ]; then
        create_job $run
    fi
done
create_list
