#!/bin/bash

#########################################
# Description
#   * load gen-muon.sh, 
#   * override the 'detsim-mode' command
#########################################

# Force Memory >= 4000
export G_REQUEST_MEMORY=4000

source $JUNOTESTROOT/production/libs/chain-template.sh

#########################################
# some more local variables
#########################################
function l-pos-x() {
    : the muons position x
    echo ${L_POS_X:-0.0}
}

function l-pos-y() {
    : the muons position y
    echo ${L_POS_Y:-0.0}
}

function l-pos-z() {
    : the muons position z
    echo ${L_POS_Z:-21000}
}

function l-direc-x() {
    : the muons direction x
    echo ${L_DIR_X:-0.0}
}

function l-direc-y() {
    : the muons direction y
    echo ${L_DIR_Y:-0.0}
}

function l-direc-z() {
    : the muons direction z
    echo ${L_DIR_Z:--1.0}
}

#########################################
# Override the parser 
#   TAG="$NAME/$MOMENTUM/$DISTANCE/$INCLINATION"
#########################################
function l-parse-tag() {
    local tag=$1; shift;
    local name=$(echo $tag | cut -d '/' -f 1)
    local momentum=$(echo $tag | cut -d '/' -f 2)
    local distance=$(echo $tag | cut -d '/' -f 3)
    local inclination=$(echo $tag | cut -d '/' -f 4)
    # particle name
    export L_PARTICLE_NAME=$name
    # momentum. need to parse MeV
    momentum=$(echo ${momentum/T/*1000G})
    momentum=$(echo ${momentum/G/*1000M})
    momentum=$(echo ${momentum/MeV/})
    momentum=$(echo $momentum | bc)
    export L_PARTICLE_MOMENTUM=$momentum
    # position need to be converted to mm and cut
    local _d=$(echo $distance | cut -d 'm' -f 1) # meter
    local _inc=$(echo $inclination | cut -d 'd' -f 1) # degree

    # calculate x,y,z of gun
    local _theta=$(python -c "import math; print math.asin(${_d}*1000./21000.)-math.radians(180-${_inc})")
    export L_POS_X=$(python -c "import math; print 21e3*math.sin(${_theta})*math.cos(0)")   # for now all tracks in phi=0 plane
    export L_POS_Y=$(python -c "import math; print 21e3*math.sin(${_theta})*math.sin(0)")   # for now all tracks in phi=0 plane
    export L_POS_Z=$(python -c "import math; print 21e3*math.cos(${_theta})")   # for now all tracks in phi=0 plane

    # direction is now in XYZ
    export L_DIR_X=$(python -c "import math; print 1*math.sin(math.radians(${_inc}))*math.cos(0)")
    export L_DIR_Y=$(python -c "import math; print 1*math.sin(math.radians(${_inc}))*math.sin(0)")
    export L_DIR_Z=$(python -c "import math; print 1*math.cos(math.radians(${_inc}))")

}


#########################################
# override the detsim command
#########################################
function detsim-mode() {
cat << EOF
    --no-anamgr-normal
	--no-anamgr-deposit
	--no-anamgr-interesting-process
    --anamgr-list MuProcessAnaMgr
	--pmtsd-v2
	--pmtsd-merge-twindow 1.0
	--pmt-hit-type 2
    gun --particles $(l-particle-name)
        --momentums $(l-particle-momentum)
        --positions $(l-pos-x) $(l-pos-y) $(l-pos-z)
        --directions $(l-direc-x) $(l-direc-y) $(l-direc-z)
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
    (
    export SEED=0
    export EVTMAX=1
    export OUTPUT="sample_detsim.root"
    export USER_OUTPUT="sample_detsim_user.root"
    export WITHOUT_GDML=
    export L_PARTICLE_MOMENTUM=1
    genjob-condor-XXX "$mode"
    genjob-shell-XXX "$mode"
    )
}


#########################################
# override the rec command
#########################################

function cmd-rec() {
cat << EOF
python \$TUTORIALROOT/share/tut_calib2rec.py 
    --evtmax $(g-evtmax)
    --input $(g-input)
    --output $(g-output)
    --method track
EOF
}

#########################################
# override the rec command without elec
#########################################
function cmd-rec-woelec() {
cat << EOF
python \$TUTORIALROOT/share/tut_calib2rec.py 
    --evtmax $(g-evtmax)
    --input $(g-input)
    --output $(g-output)
    --method track
EOF
}


#########################################
# main function
# this technique allows developers 
# reuse it.
#########################################
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    parse_cmds "$@"
fi
