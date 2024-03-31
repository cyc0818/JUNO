#!/bin/bash
# Description:
#     Run doxygen and Create a tarball
#
# Author: Tao Lin <lintao AT ihep.ac.cn>

if [ ! -f Doxyfile ]; then
    echo "Can't find Doxyfile" 1>&2
    exit -1
fi

doxygen

if [ ! -d html ]; then
    echo "No html output" 1>&2
    exit -1
fi

fn=doxygen-edm-$(date +%Y%m%d-%H%M).tar.gz
tar czvf "$fn" html && echo "$fn is created."
