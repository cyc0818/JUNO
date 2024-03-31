#!/bin/bash

#########################################
# Description
#   * load gen-alpha-center.sh, 
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
    gun --particles "alpha"
        --momentums 4.0 --momentums-extra-params 10.0
	--momentums-mode "Range" --momentums-interp "KineticEnergy"
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
