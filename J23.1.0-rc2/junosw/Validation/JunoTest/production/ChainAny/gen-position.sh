#!/bin/bash

#########################################
# Description
#   ChainAny allows user to configure 
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
cat << EOF
    $(g-detsim-submode)
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
