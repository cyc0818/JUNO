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

function l-parse-tag() {
    local tag=$1; shift;

    local _r=$(echo $tag | cut -d 'm' -f 1) # meter
    local _tp=$(echo $tag | cut -d 'm' -f 2)

    local _theta=$(echo $_tp | cut -d 'd' -f 1) # degree
    local _phi=$(echo $_tp | cut -d 'd' -f 2) # degree

    # calculate x,y,z
    export L_X=$(python -c "import math; print ${_r}*1e3*math.sin(math.radians(${_theta}))*math.cos(math.radians(${_phi}))")
    export L_Y=$(python -c "import math; print ${_r}*1e3*math.sin(math.radians(${_theta}))*math.sin(math.radians(${_phi}))")
    export L_Z=$(python -c "import math; print ${_r}*1e3*math.cos(math.radians(${_theta}))")

}
function detsim-mode() {
cat << EOF
    $(g-detsim-submode)
    --global-position ${L_X} ${L_Y} ${L_Z}
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
