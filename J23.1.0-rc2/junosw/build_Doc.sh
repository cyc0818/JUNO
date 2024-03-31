#!/bin/bash
# Description: build the Offline User Manual
# Author: Tao Lin <lintao AT ihep.ac.cn>

export LANG=C
export LANGUAGE=C
export LC_ALL=C
export LC_CTYPE=C
# source utilites
export JUNO_OFFLINE_SOURCE_DIR=$(dirname $(readlink -e $0 2>/dev/null) 2>/dev/null) # Darwin readlink doesnt accept -e

export JUNO_OUM_SOURCE_DIR=${JUNO_OFFLINE_SOURCE_DIR}/Doc/oum

export JUNO_OUM_HOST=0.0.0.0
export JUNO_OUM_PORT=8888 # default port

function build() {
    pushd ${JUNO_OUM_SOURCE_DIR}
    make html
    popd
}

function serve() {
    pushd ${JUNO_OUM_SOURCE_DIR}
    python -m http.server ${JUNO_OUM_PORT} --bind ${JUNO_OUM_HOST} --directory build/html
    popd
}

build
serve
