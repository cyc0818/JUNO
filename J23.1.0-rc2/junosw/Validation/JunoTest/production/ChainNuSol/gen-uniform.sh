#!/bin/bash

#########################################
# Description
#   * load gen-positron-center.sh, 
#   * override the 'detsim-mode' command
#########################################

source $JUNOTESTROOT/production/libs/chain-template.sh

#########################################
# override parse the tag 
#   TAG="$CHANNEL"
#########################################
function l-parse-tag() {
    local tag=$1; shift;
    export L_PARTICLE_NAME=$tag
}

#########################################
# override the detsim command
#########################################
# allow pp,Be7,B8,N13,O15,F17,pep,hep
function l-particle-name() {
    echo ${L_PARTICLE_NAME:-Be7}
}
function detsim-mode() {
cat << EOF
    nusol --type $(l-particle-name)
        --volume pTarget
        --material LS
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
