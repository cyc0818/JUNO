#!/bin/bash -l

name=PMTParamData_test 

defarg=build_run_ana
arg=${1:-$defarg}

echo $BASH_SOURCE arg $arg 

export FOLD=/tmp/$name
mkdir -p $FOLD

if [ "${arg/build}" != "$arg" ]; then
    gcc $name.cc \
         -g -std=c++11 -lstdc++ \
         -I.. \
         -I$HOME/np \
         -DWITH_NP \
         -o $FOLD/$name

    [ $? -ne 0 ] && echo $BASH_SOURCE build error && exit 1
fi

if [ "${arg/run}" != "$arg" ]; then
    $FOLD/$name
    [ $? -ne 0 ] && echo $BASH_SOURCE run error && exit 2
fi

if [ "${arg/ana}" != "$arg" ]; then
    ${IPYTHON:-ipython} --pdb -i $name.py
    [ $? -ne 0 ] && echo $BASH_SOURCE ana error && exit 3
fi

if [ "${arg/load}" != "$arg" ]; then
    base=$HOME/.opticks/GEOM/J005/CSGFoundry/SSim/juno/PMTParamData
    export LOAD=$base

    $FOLD/$name
    [ $? -ne 0 ] && echo $BASH_SOURCE run error && exit 2

    FOLD=$base ${IPYTHON:-ipython} --pdb -i $name.py
fi

exit 0





