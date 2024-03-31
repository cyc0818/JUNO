#!/bin/bash

source $JUNOTESTROOT/production/libs/chain-template.sh

#########################################
# override the detsim command
#########################################
function l-parse-tag() {
    local tag=$1; shift;
    local volume=$(echo $tag | cut -d '_' -f 1)
    local mult_name=$(echo $tag | cut -d '_' -f 2 -s)

    local mult=1

    if [[ "x${mult_name}" == "xsingle" ]]; then
        mult=0
    elif [[ "x${mult_name}" == "x" ]]; then
        mult=0
    fi

    export L_VOLUME=${volume}
    export L_MULT=${mult}

}

function detsim-mode() {
if [[ ${WITHOUT_GDML} -eq 1 ]]; then
cat << EOF
    --no-optical hepevt --exe Muon --file $(g-output).txt
EOF
else
cat << EOF
    gun
EOF
fi
}

function cmd-detsim() {
if [[ ${WITHOUT_GDML} -eq 1 ]]; then
cat << EOF
Muon.exe -seed $(g-seed) -o $(g-output).txt -n $(g-evtmax) -s juno -v ${L_VOLUME:-TT} -r Yes -mult ${L_MULT:-0} -music_dir \${JUNOTOP}/data/Generator/Muon/data &&
EOF
fi
cat << EOF
python \$TUTORIALROOT/share/tut_detsim.py
    --evtmax $(g-evtmax)
    --seed $(g-seed)
    --output $(g-output)
    --user-output $(g-user-output)
    $(l-with-gdml)
    $(g-detsim-mode)
    $(comment: 'begin the different modes')
    $(detsim-mode)
EOF
}


function rec-mode() {
    export USER_OUTPUT="rec-woelec/user-rec-woelec-$seed.root" # user output
cat << EOF
    --user-output $(g-user-output)
    --detoption TT --method tt-track
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
