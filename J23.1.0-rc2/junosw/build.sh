#!/bin/bash
#
# Description:
#     This script is used to build the offline software using cmake.
#     It consists following steps:
#         1. configuration
#         2. make
#         3. make install
#
#     Usage:
#         cd offline
#         bash build.sh
#
#     Envvars:
#         - $JUNOTOP: the JUNO release directory
#         - $JUNO_OFFLINE_BLDDIR: the build directory name, default: "build"
#         - $JUNO_OFFLINE_INSTALLDIR: the install directory name, default: "/path/to/offline/InstallArea"
#
# Author: 
#     Tao Lin <lintao AT ihep.ac.cn>
#

export LANG=C
export LANGUAGE=C
export LC_ALL=C
export LC_CTYPE=C
# source utilites
export JUNO_OFFLINE_SOURCE_DIR=$(dirname $(readlink -e $0 2>/dev/null) 2>/dev/null) # Darwin readlink doesnt accept -e


##############################################################################
# Helpers
##############################################################################
function info:() {
    echo "INFO: $*" 1>&2
}

function error:() {
    echo "ERROR: $*" 1>&2
    exit -1
}

function check-juno-envvar() {
    if [ -z "$JUNOTOP" ]; then
        error: "The JUNOTOP is not set. "
    fi
    info: "JUNOTOP: ${JUNOTOP}"
    info: "Build dir: $(build-dir)"
    info: "Install dir: $(install-dir)"
}

#############################################################################
# Check if a variable is defined in environment variables (on)
# if enabled, then print the value
#
# The rule here is:
# - function var-xyz-envvar()   -> return the environment variable name; 
# - function var-xyz-value()   -> return the value; 
#
#############################################################################
function check-var-enabled() {
    local varlabel=$1; shift
    if [ -z "$varlabel" ]; then
        error: "Please specify the varname"
    fi
    # the first argument is the label
    type -t var-$varlabel-envvar >& /dev/null || error: "The function var-$varlabel-envvar is not defined"
    type -t var-$varlabel-value  >& /dev/null || error: "The function var-$varlabel-value is not defined"

    local is_enabled=false
    local envvarname=$(var-$varlabel-envvar)
    # https://tldp.org/LDP/abs/html/ivr.html
    local envvarvalue=${!envvarname} ### WARNNING: dereference ###
    info: "The envvar: ${envvarname}, value: ${envvarvalue}"

    # if the value is not set, skip this option
    if [ -z "$envvarvalue" ]; then
        return
    fi

    if [ -n "$envvarvalue" ]; then
        case $envvarvalue in
            1)
                is_enabled=true;
                ;;
            0)
                is_enabled=false;
                ;;
            on)
                is_enabled=true;
                ;;
            off)
                is_enabled=false;
                ;;
            *) # if other, also assume enabled.
                is_enabled=true;
                ;;
        esac
    fi

    if $is_enabled; then
        var-$varlabel-value
    else
        # if there is a function to handle disabled, invoke it
        type -t var-$varlabel-value-if-disabled >& /dev/null && var-$varlabel-value-if-disabled
    fi

}

##############################################################################
# List of vars will be used 
##############################################################################

## Generate the graphviz dot from the targets:
##   - The cmake/CMakeGraphVizOptions.cmake is used to tuning
##   - run below with build.sh:
##       JUNO_GEN_GRAPH=1 ./build.sh
##   - run below command under build:
##       dot -Tpdf -o junooffline.pdf junooffline.dot
function var-graphviz-envvar() { echo JUNO_GEN_GRAPH; }
function var-graphviz-value()  { echo --graphviz=junooffline.dot; }

## Build offline with OEC
##   - There are macros WITH_OEC in the source code
##   - An option WITH_OEC is defined in cmake
##   - run below command:
##       JUNO_WITH_OEC=1 ./build.sh
function var-withoec-envvar() { echo JUNO_WITH_OEC; }
function var-withoec-value() { echo -DWITH_OEC=ON; }
function var-withoec-value-if-disabled() { echo -DWITH_OEC=OFF; }

## Build online version with OEC
##  -Some packages is not needed when runing online
##  -An option BUILD_ONLINE is defined in cmake
##  -run below command:
##      JUNO_BUILD_ONLINE=1 JUNO_WITH_OEC=1 ./build.sh
function var-online-envvar() { echo JUNO_BUILD_ONLINE; }
function var-online-value() { echo -DBUILD_ONLINE=ON; }
function var-online-value-if-disabled() { echo -DBUILD_ONLINE=OFF; }

## Conditional compilation with the preprocessors for PERFORMANCE_CHECK
##  - to enable it, run below command:
##  - PERFORMANCE_CHECK=1 ./build.sh
function var-PerformanceCheck-envvar() { echo PERFORMANCE_CHECK; }
function var-PerformanceCheck-value() { echo -DPERFORMANCE_CHECK=ON; }
function var-PerformanceCheck-value-if-disabled() { echo -DPERFORMANCE_CHECK=OFF; }

##############################################################################
# END: List of vars will be used 
##############################################################################


function build-dir() {
    local blddir=$JUNO_OFFLINE_SOURCE_DIR/build

    # allow users to override the directory name of blddir
    if [ -n "$JUNO_OFFLINE_BLDDIR" ]; then
        blddir=${JUNO_OFFLINE_BLDDIR}
    fi

    echo $blddir
}

function install-dir() {
    local installdir=${JUNO_OFFLINE_SOURCE_DIR}/InstallArea

    # allow users to override the directory name of blddir
    if [ -n "$JUNO_OFFLINE_INSTALLDIR" ]; then
        installdir=${JUNO_OFFLINE_INSTALLDIR}
    fi

    echo $installdir
}

function check-build-dir() {
    local blddir=$(build-dir)
    if [ ! -d "$blddir" ]; then
        mkdir $blddir || error: "Failed to create build directory: '$blddir'"
    fi
}

function check-install-dir() {
    local installdir=$(install-dir)
    if [ ! -d "$installdir" ]; then
        mkdir $installdir || error: "Failed to create install directory: '$installdir'"
    fi
}

function cmake-build-type() {
   echo ${JUNO_CMAKE_BUILD_TYPE:-Release}
}

function run-build() {
    local installdir=$(install-dir)
    local blddir=$(build-dir)
    check-build-dir
    check-install-dir

    pushd $blddir

    cmake .. $(check-var-enabled graphviz) \
             $(check-var-enabled withoec) \
             $(check-var-enabled online) \
             $(check-var-enabled PerformanceCheck) \
             -DCMAKE_CXX_STANDARD=17 \
             -DCMAKE_BUILD_TYPE=$(cmake-build-type) \
             -DCMAKE_INSTALL_PREFIX=$installdir \
                     || error: "ERROR Found during cmake stage. "

    local njobs=-j$(nproc)
    cmake --build . $njobs || error: "ERROR Found during make stage. "
    cmake --install . || error: "ERROR Found during make install stage. "

    popd
}

##############################################################################
# Parse the command line options
##############################################################################

check-juno-envvar
date
run-build
date


