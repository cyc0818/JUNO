#!/bin/bash -l

usage(){ cat << EOU
PMTSimParamData_test.sh
=========================

junosw/Simulation/SimSvc/PMTSimParamSvc/PMTSimParamSvc/PmtSimData_LPMT.h


EOU
}

name=PMTSimParamData_test 

defarg=build_run_ana
arg=${1:-$defarg}

clhep-
g4-


export FOLD=/tmp/$name
mkdir -p $FOLD

if [ "${arg/build}" != "$arg" ]; then
    gcc $name.cc \
         -g -std=c++11 -lstdc++ \
         -I.. \
         -DWITH_NP \
         -I$HOME/np \
         -I$HOME/opticks/u4 \
         -I$JUNOTOP/junosw/Detector/Geometry \
         -I$JUNOTOP/junosw/Simulation/SimSvc/PMTSimParamSvc \
         -I$(clhep-prefix)/include \
         -I$(g4-prefix)/include/Geant4 \
         -L$(clhep-prefix)/lib \
         -L$(g4-prefix)/lib \
         -lG4global \
         -lG4materials \
         -lCLHEP \
         -o $FOLD/$name 

    [ $? -ne 0 ] && echo $BASH_SOURCE build error && exit 1 
fi

if [ "${arg/run}" != "$arg" ]; then
    $FOLD/$name
    [ $? -ne 0 ] && echo $BASH_SOURCE run error && exit 2 
fi

if [ "${arg/dbg}" != "$arg" ]; then
    lldb__ $FOLD/$name
    [ $? -ne 0 ] && echo $BASH_SOURCE dbg error && exit 20 
fi

if [ "${arg/ana}" != "$arg" ]; then
    ${IPYTHON:-ipython} --pdb -i $name.py
    [ $? -ne 0 ] && echo $BASH_SOURCE ana error && exit 3 
fi

if [ "${arg/load}" != "$arg" ]; then
    base=$HOME/.opticks/GEOM/${GEOM:-J006}/CSGFoundry/SSim/juno/PMTSimParamData
    export LOAD=$base

    lldb__ $FOLD/$name
    [ $? -ne 0 ] && echo $BASH_SOURCE run error && exit 2

    FOLD=$base ${IPYTHON:-ipython} --pdb -i $name.py
fi


exit 0 

