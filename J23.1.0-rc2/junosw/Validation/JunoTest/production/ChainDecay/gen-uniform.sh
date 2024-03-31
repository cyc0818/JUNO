#!/bin/bash

#########################################
# Description
#   * load gen-positron-center.sh, 
#   * override the 'detsim-mode' command
#########################################

source $JUNOTESTROOT/production/libs/chain-template.sh

#########################################
# parse the tag 
#   TAG="$NAME"
#########################################
function l-parse-tag() {
    local tag=$1; shift;
    export L_PARTICLE_NAME=$(echo $tag | cut -d '_' -f 1)

    local depth=$(echo $tag | cut -d '_' -f 2 -s)

    if [[ "x${depth}" == "x" ]]; then
        export has_depth=0
        return
    else
        export has_depth=1
    fi
    depth=$(echo ${depth/mm/})
    depth=$(echo ${depth/cm/*10})
    depth=$(echo ${depth/m/*1000})
    depth=$(echo ${depth} | bc)

    export L_ZMIN=20000
    export L_ZMAX=$(( 40350 + depth ))

    export L_XMAX=$(( 24000 + depth ))

    export L_YMAX=$(( 24000 + depth ))
}
#########################################
# override the detsim command
#########################################
# allow U-238, Th-232, K-40, Co-60
function l-particle-name() {
    echo ${L_PARTICLE_NAME:-U-238}
}
function l-rock-depth() {
    if [[ ${has_depth:-0} -eq 0 ]]; then
        echo ""
    else
        cat <<EOF
             --volume-z-min  ${L_ZMIN} --volume-z-max ${L_ZMAX}
             --volume-x-min -${L_XMAX} --volume-x-max ${L_XMAX}
             --volume-y-min -${L_YMAX} --volume-y-max ${L_YMAX}
EOF
    fi
}
function detsim-mode() {
    local submode="$(g-detsim-submode)"
    if [ -z "$submode" ]; then
        submode="--volume pTarget --material LS"
    fi
    if [[ ${has_depth:-0} -eq 1 && !( ${submode} =~ "Rock" ) ]]; then
        # As the ERROR below notes, setting optinos are not compatible.
        # Unfortunately raising an exception is not particularly feasible from bash to python
        # and error codes are currently ignored, so just unset has_depth for now.
        echo "ERROR: Rock depth set, but Rock option not used!" 1>&2
        echo "ERROR: Ignoring Rock depth setting." 1>&2
        #exit 1
        export has_depth=0
    fi

cat << EOF
    gendecay --nuclear $(l-particle-name)
             ${submode}
             $(l-rock-depth)
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
