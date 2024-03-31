#!/bin/bash

#########################################
# This is a template for Chain:
# + mode 1:
#   - detsim
#   - elecsim
#   - calib
#   - rec
# + mode 2:
#   - detsim
#   - calib-woelec
#   - rec-woelec
#########################################

#########################################
# Prelude
#########################################
source $JUNOTESTROOT/production/libs/all.sh
# register the help function
export HELPS_LIST="$HELPS_LIST chain-template-help"
function chain-template-help() {
cat << EOF
# This is a template for Chain:
# + chain 1:
#   - detsim
#   - elecsim (allow event mixing)
#   - calib
#   - rec
# + chain 2:
#   - detsim
#   - calib-woelec
#   - rec-woelec
EOF
}
export HELPS_LIST="$HELPS_LIST chain-mixing-help"
function chain-mixing-help() {
cat << EOF
# For Event Mixing, we need to specify extra background data.
# Using following options:
#   --extra-mixing-tags "T1:F1:R1 T2:F2:R2 .."
# T1, T2.. : extra tag name for event mixing
# F1, F2.. : input files 
# R1, R2.. : rate 
EOF
}
#########################################
# parse the tag 
#   TAG="$NAME_$MOMENTUM"
#########################################
function l-parse-tag() {
    local tag=$1; shift;
    local name=$(echo $tag | cut -d '_' -f 1)
    local momentum=$(echo $tag | cut -d '_' -f 2)
    # particle name
    export L_PARTICLE_NAME=$name
    # momentum. need to parse MeV
    momentum=$(echo ${momentum/T/*1000G})
    momentum=$(echo ${momentum/G/*1000M})
    momentum=$(echo ${momentum/MeV/})
    momentum=$(echo $momentum | bc)
    export L_PARTICLE_MOMENTUM=$momentum
}

#########################################
# Local variables
#########################################
function l-with-gdml() {
    : with gdml enabled or not
    [ -n "$WITHOUT_GDML" ] && echo '--no-gdml'
}

function l-particle-name() {
    : the particle name
    echo ${L_PARTICLE_NAME:-e+}
}

function l-particle-momentum() {
    : the particle momentum
    echo ${L_PARTICLE_MOMENTUM:-0.0}
}

#########################################
# Detector Simulation
#########################################
function detsim-mode() {
    : TODO need to override by developers
cat << EOF
    gun --particles $(l-particle-name)
        --momentums $(l-particle-momentum)
EOF
}
function cmd-detsim() {
cat << EOF
python \$TUTORIALROOT/share/tut_detsim.py 
    --evtmax $(g-evtmax) 
    --seed $(g-seed)
    --output $(g-output)
    --user-output $(g-user-output)
    --anamgr-normal-hit
    $(l-with-gdml)
    $(g-detsim-mode)
    $(comment: 'begin the different modes')
    $(detsim-mode)
EOF
}
function detsim() {
    # mode should be detsim
    local mode=$FUNCNAME 
    local seed

    for seed in $(seqs)
    do
        export SEED=$seed
        export INPUT="" # input
        export OUTPUT="detsim/detsim-$seed.root" # output
        export USER_OUTPUT="detsim/user-detsim-$seed.root" # user output
        export WITHOUT_GDML=1
        genjob-condor-XXX "$mode"
        genjob-shell-XXX "$mode"
    done

    ## generate an extra event, ensure Rec could load geometry
    ## to avoid the environment pollution, run following commands
    ## in a sub-shell.

    ## TODO
    ## Sometimes user will run a high energy events, so it is not a good
    ## idea to generate the data again. Let the user to specify where the 
    ## GDML file is.
    (
    export SEED=0
    export EVTMAX=1
    export OUTPUT="sample_detsim.root"
    export USER_OUTPUT="sample_detsim_user.root"
    export WITHOUT_GDML=
    genjob-condor-XXX "$mode"
    genjob-shell-XXX "$mode"
    )
}

#########################################
# Electronics Simulation
#########################################
function elecsim-mode() {
    : Override by developers
}
function elecsim-mode-mixing() {
    : If user specify extra-mixing-tags, enable mixing mode
    local electag
    local t #
    local i # input
    local r # rate
    for electag in $(g-mixing-tags)
    do
        debug: $electag
        t=$(echo $electag | cut -d ':' -f 1)
        i=$(echo $electag | cut -d ':' -f 2)
        r=$(echo $electag | cut -d ':' -f 3)
cat << EOF
        --input $t:$i --rate $t:$r
EOF
    done
}
function elecsim-mode-default-rate() {
    : Override this function or define variable for it
    echo ${ELEC_DEFAULT_RATE:-1.0}
}
function cmd-elecsim() {
cat << EOF
python \$TUTORIALROOT/share/tut_det2elec.py 
    --evtmax $(g-evtmax)
    --seed $(g-seed)
    --input $(g-input)
    --output $(g-output)
    --user-output $(g-user-output)
    --rate $(elecsim-mode-default-rate)
    $(g-elecsim-mode)
    $(elecsim-mode)
    $(elecsim-mode-mixing)
EOF
}
function elecsim() {
    local mode=$FUNCNAME 
    local seed
    for seed in $(seqs)
    do
        export SEED=$seed
        export INPUT="detsim/detsim-$seed.root" # input
        export OUTPUT="elecsim/elecsim-$seed.root" # output
        export USER_OUTPUT="elecsim/user-elecsim-$seed.root" # output
        genjob-condor-XXX "$mode"
        genjob-shell-XXX "$mode"
    done
}

#########################################
# Waveform Reconstruction (Calib)
#########################################
function calib-mode() {
    : Override by developers
}
function cmd-calib() {
cat << EOF
python \$TUTORIALROOT/share/tut_elec2calib.py 
    --evtmax $(g-evtmax)
    --input $(g-input)
    --output $(g-output)
    --user-output $(g-user-output)
    $(g-calib-mode)
    $(calib-mode)
EOF
}
function calib() {
    local mode=$FUNCNAME 
    local seed
    for seed in $(seqs)
    do
        export SEED=$seed
        export INPUT="elecsim/elecsim-$seed.root" # input
        export OUTPUT="calib/calib-$seed.root" # output
        export USER_OUTPUT="calib/user-calib-$seed.root" # output
        genjob-condor-XXX "$mode"
        genjob-shell-XXX "$mode"
    done
}

#########################################
# Vertex/Energy Reconstruction
#########################################
function rec-mode() {
    : Override by developers
}
function cmd-rec() {
cat << EOF
python \$TUTORIALROOT/share/tut_calib2rec.py 
    --evtmax $(g-evtmax)
    --input $(g-input)
    --output $(g-output)
    $(g-rec-mode)
    $(rec-mode)
EOF
}
function rec() {
    local mode=$FUNCNAME 
    local seed
    for seed in $(seqs)
    do
        export SEED=$seed
        export INPUT="calib/calib-$seed.root" # input
        export OUTPUT="rec/rec-$seed.root" # output
        genjob-condor-XXX "$mode"
        genjob-shell-XXX "$mode"
    done
}

#########################################
# Following is Chain without ElecSim,
# with suffix -woelec
#########################################
#########################################
# Calib without ElecSim
#########################################
function cmd-calib-woelec() {
cat << EOF
python \$TUTORIALROOT/share/tut_det2calib.py 
    --evtmax $(g-evtmax)
    --input $(g-input)
    --output $(g-output)
EOF
}
function calib-woelec() {
    local mode=$FUNCNAME 
    local seed
    for seed in $(seqs)
    do
        export SEED=$seed
        export INPUT="detsim/detsim-$seed.root" # input
        export OUTPUT="calib-woelec/calib-woelec-$seed.root" # output
        genjob-condor-XXX "$mode"
        genjob-shell-XXX "$mode"
    done
}

#########################################
# Vertex/Energy Reconstruction w/o ElecS
#########################################
function cmd-rec-woelec() {
cat << EOF
python \$TUTORIALROOT/share/tut_calib2rec.py 
    --evtmax $(g-evtmax)
    --input $(g-input)
    --output $(g-output)
    $(g-rec-mode)
    $(rec-mode)
EOF
}
function rec-woelec() {
    local mode=$FUNCNAME 
    local seed
    for seed in $(seqs)
    do
        export SEED=$seed
        export INPUT="calib-woelec/calib-woelec-$seed.root" # input
        export OUTPUT="rec-woelec/rec-woelec-$seed.root" # output
        genjob-condor-XXX "$mode"
        genjob-shell-XXX "$mode"
    done
}

if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    parse_cmds "$@"
fi
