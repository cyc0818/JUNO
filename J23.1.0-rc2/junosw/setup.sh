#!/bin/bash
# 
# Description:
#     This script will setup the JUNO software environment.
#     It consists following steps:
#         1. if user don't source $JUNOTOP/setup.sh, this script will stop
#         2. setup the $PATH, $LD_LIBRARY_PATH, $PYTHONPATH, $CMAKE_PREFIX_PATH
#
# Author: 
#     Tao Lin <lintao AT ihep.ac.cn>
#

export LANG=C
export LANGUAGE=C
export LC_ALL=C
export LC_CTYPE=C
# source utilites
(return 0 2>/dev/null) && sourced=1 || sourced=0
if [ "$sourced" = 0 ]; then
    export JUNO_OFFLINE_SOURCE_DIR=$(dirname $(readlink -e $0 2>/dev/null) 2>/dev/null) # Darwin readlink doesnt accept -e
else
    export JUNO_OFFLINE_SOURCE_DIR=$(dirname $(readlink -e ${BASH_SOURCE[0]} 2>/dev/null) 2>/dev/null) # Darwin readlink doesnt accept -e
fi

#echo "sourced: $sourced. JUNO_OFFLINE_SOURCE_DIR: $JUNO_OFFLINE_SOURCE_DIR. \$0: $0. BASH_SOURCE: ${BASH_SOURCE[0]}"

function junotop() {
    echo $JUNOTOP
}

function check-junotop() {
    if [ -z "$(junotop)" ]; then
        echo "JUNOTOP is not setup"
        return
    fi
}

function install-dir() {
    local installdir=${JUNO_OFFLINE_SOURCE_DIR}/InstallArea

    # allow users to override the directory name of blddir
    if [ -n "$JUNO_OFFLINE_INSTALLDIR" ]; then
        installdir=${JUNO_OFFLINE_INSTALLDIR}
    fi

    echo $installdir
}

function check-install-dir() {
    local installdir=$(install-dir)
    if [ ! -d "$installdir" ]; then
        echo "Failed to find install directory: '$installdir'. Maybe the offline is not built yet"
        return
    fi
}


function setup-offline() {
    local installdir=$(install-dir)
    
    if [ -f "$installdir/setup.sh" ]; then
        source $installdir/setup.sh
    fi

}


check-junotop
check-install-dir
setup-offline
