#!/bin/bash

#########################################
# Description
#   * load gen-positron-center.sh, 
#   * override the 'detsim-mode' command
#########################################

source $JUNOTESTROOT/production/libs/chain-template.sh

#########################################
# override the detsim command
#########################################
# allow IBD, IBD-eplus, IBD-neutron
function l-particle-name() {
    echo ${L_PARTICLE_NAME:-IBD}
}
function detsim-mode() {
cat << EOF
    hepevt --exe $(l-particle-name)
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
