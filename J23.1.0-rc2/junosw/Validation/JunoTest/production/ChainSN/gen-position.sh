#!/bin/bash

#########################################
# Description
#   ChainSN allows user to configure 
#   detsim sub command in .ini directly.
#
#   So user needs to make sure the sub
#   command is right.
#########################################

source $JUNOTESTROOT/production/libs/chain-template.sh

#########################################
# override the detsim command
#########################################
function detsim-mode() {
    export INPUT=evt-${SEED}.root # output file name from SN.exe

    if [[ $SEED = 0 ]]; then
cat <<EOF
    gun
EOF
    else
cat << EOF
    sn --input $(g-input nogeom)
    $(g-detsim-submode)
EOF

    fi
}

#########################################
# main function
# this technique allows developers 
# reuse it.
#########################################
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    parse_cmds "$@"
fi
