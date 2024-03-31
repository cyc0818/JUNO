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
function detsim-mode() {
cat << EOF
    --pmtsd-v2 
    --ce-mode 20inchfunc
    gun --particles $(l-particle-name)
        --momentums $(l-particle-momentum)
        --positions 0 0 0
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
