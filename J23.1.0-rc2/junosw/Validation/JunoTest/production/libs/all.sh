#!/bin/bash
# In this script, you can find several useful helpers to generate jobs.

# About the directory structures
# * current directory (pwd)
#  * TAG (optional)
#   + detsim
#     - sub_*.condor
#     - run_*.sh
#     - log_*.txt
#   + elecsim
#   + calib
#   + rec
#
#   + calib-woelec
#   + rec-woelec
#
# Note the TAG, it could be defined by 
# $TAGS list.
# The developers should parse them.

##########################################
# Global Variables
#   prefix with g-
# 
# The main script could override the 
# envvar using export cmd.
#
# when using these variables, it is better
# to use function call instead.
##########################################
function g-evtmax() {
    : event max in each job
    echo ${EVTMAX:-10}
}

function g-njobs() {
    : 'N jobs for each type.'
    : 'this is usually used by the script.'
    echo ${NJOBS:-1}
}

function g-seed() {
    : the initial seed number is specified in the cmd line.
    : then it will be updated by the script.
    echo ${SEED:-42}
}

function g-input() {
    local nogeom=$1; shift

    : input file with data model
    if [ -z "$(g-external-input-dir)" ]; then
        echo ${INPUT}
    else
        local prefix=
        # note the order:
        # guess input file in this order:
        #  * EID / workDir / workSubDir / tag / detsim / detsim-xxx.root
        #  * EID           / workSubDir / tag / detsim / detsim-xxx.root
        #  * EID                        / tag / detsim / detsim-xxx.root
        #  * EID                              / detsim / detsim-xxx.root
        #  *                                    detsim / detsim-xxx.root
        #                                      |  this part is default  |
        if   [ -d "$(g-external-input-dir)/$(g-workdir)/$(g-worksubdir)/$(g-current-tag)" ]; then
            prefix=$(g-external-input-dir)/$(g-workdir)/$(g-worksubdir)/$(g-current-tag)
        elif [ -d "$(g-external-input-dir)/$(g-worksubdir)/$(g-current-tag)" ]; then
            prefix=$(g-external-input-dir)/$(g-worksubdir)/$(g-current-tag)
        elif [ -d "$(g-external-input-dir)/$(g-current-tag)" ]; then
            prefix=$(g-external-input-dir)/$(g-current-tag)
        elif [ -d "$(g-external-input-dir)" ]; then
            prefix=$(g-external-input-dir)
        else
            error: cannot detect external input dir
        fi
        echo $prefix/${INPUT}

        # TODO
        # in detsim mode, we don't create geom file.
        if [ "$nogeom" = "nogeom" ]; then
            return
        fi
        # note: Rec need geometry from sample_detsim.root, so check this file!
        #    $prefix/sample_detsim.root
        local geom_file=sample_detsim.root
        if [ ! -f "$geom_file" ]; then
            # create a link
            if [ ! -f "$prefix/$geom_file" ]; then
                error: cannot find geometry ROOT file
            else
                ln -s $prefix/$geom_file $geom_file
            fi

        fi
    fi
}

function g-output() {
    : output file with data model
    echo ${OUTPUT}
}

function g-user-output() {
    : user output file 
    echo ${USER_OUTPUT}
}

function g-setup() {
    : Sepcify the setup script
    echo ${SETUP:-${JUNOTOP}/setup.sh}
}

# for condor/LSF/PBS
#    Y for Yes
#    undefined or empty for No
function g-batch-mode() {
    : batch mode
    echo ${BATCH_MODE:-condor}
}
function g-is-condor() {
    : using condor
    [ "$(g-batch-mode)" = "condor" ] && echo Y
}

function g-condor-request-memory() {
    : allow user apply more memory
    [ -n "$G_REQUEST_MEMORY" ] || return

    echo request_memory = $G_REQUEST_MEMORY
}

function g-condor-benchmark() {
    : start benchmark
    [ -n "$G_BENCHMARK" ] && echo 'regexp("jnws007", Name)'
}

function g-condor-target-os() {
    [ -n "$G_TARGET_OS" ] && echo 'Target.OpSysAndVer =?= "'${G_TARGET_OS}'"'
}

function g-condor-extra-template() {
    # Read the extra template from file, and write it to
    # the final job description file.

    if [ -n "$G_CONDOR_TEMPLATE" ]; then
        if [ -f "$G_CONDOR_TEMPLATE" ]; then
            cat $G_CONDOR_TEMPLATE
        else
            error: "Failed to open condor template '$G_CONDOR_TEMPLATE'"
        fi

    fi

}

function g-is-lsf() {
    : using LSF
    [ "$(g-batch-mode)" = "LSF" ] && echo Y
}

function g-is-pbs() {
    : using PBS
    [ "$(g-batch-mode)" = "PBS" ] && echo Y
}

function g-tags() {
    : TAG list
    eval echo ${TAGS}
}

function g-current-tag() {
    : current tag
    echo $G_CURRENT_TAG
}

function g-mixing-tags() {
    : Extra TAGs used in event mixing
    echo ${EXTRA_MIXING_TAGS}
}

function get-rate() {
    local tag=$1;
    if [ -z "$tag" ] ; then
	return
    fi
    if [ -z "$EXTRA_DEFAULT_RATES" ]; then
	return
    fi
    for t in $EXTRA_DEFAULT_RATES; do
        if [ "${t/$tag}" != "${t}" ]; then
            #echo Match $tag rate is: "${t/$tag:}"
	    echo "${t/$tag:}"
        fi
    done
}

# global options in different stages
# NOTE: 
#   * g-XXX-mode is set in .ini or command line
#   * XXX-mode is overridden by developers
function g-detsim-mode() {
    : DetSim global options
    echo ${DETSIM_MODE}
}
function g-detsim-submode() {
    : DetSim sub commands
    echo ${DETSIM_SUBMODE}
}
function g-elecsim-mode() {
    : ElecSim
    echo ${ELECSIM_MODE}
}
function g-calib-mode() {
    : Calib
    echo ${CALIB_MODE}
}
function g-rec-mode() {
    : Rec
    echo ${REC_MODE}
}

# external input dir allows user starts from other stages
# it will effect:
# * g-input
function g-external-input-dir() {
    echo ${G_EXTERNAL_INPUT_DIR}
}

function g-workdir() {
    echo $JOB_WORKDIR
}
function g-worksubdir() {
    echo $JOB_WORKSUBDIR
}

##########################################
# HELP Functions List, separate by space.
# The caller should register the function
# to this list.
##########################################
export HELPS_LIST=""

##########################################
# LOG LEVEL
##########################################
## debug
function debug:() {
    [ -n "$DEBUG" ] && 1>&2 echo $*
}
## info 
function info:() {
    echo $*
}

## error
function error:() {
    1>&2 echo ERROR: $*
    exit -1
}

## inline comment
##   use like this:
##     $(comment: this is a comment)
function comment:() {
    echo
}

##########################################
# parse the command
##########################################
function parse_cmds() {
    : Helper to handle common command lines
    debug: parse_cmds $#
    local sub_cmd=
    while [[ $# -gt 0 ]] 
    do
        key="$1"
        debug: $key

        case $key in
            -h|--help)
                show_helps
                return
            ;;
            --evtmax)
                shift
                export EVTMAX=$1
            ;;
            --njobs)
                shift
                export NJOBS=$1
            ;;
            --setup)
                shift
                export SETUP=$1
            ;;
            --seed)
                shift
                export SEED=$1
            ;;
            --condor)
                export BATCH_MODE=condor
            ;;
            --condor-template)
                shift
                export G_CONDOR_TEMPLATE=$1
            ;;
            --lsf)
                export BATCH_MODE=LSF
            ;;
	    --pbs)
                export BATCH_MODE=PBS
            ;;
            --target-os)
                shift
                export G_TARGET_OS="$1"
            ;;
            --benchmark)
                export G_BENCHMARK=1
            ;;
            --tag-list|--tags)
                # make sure the user include "".
                # so during developing, note the difference between $@ and $*
                shift
                export TAGS="$1"
            ;;
            --extra-mixing-tags)
                shift
                export EXTRA_MIXING_TAGS="$1"
            ;;
	    --rates)
		shift
		export EXTRA_DEFAULT_RATES="$1"
            ;;
            # allow tuning the command line options
            --detsim-mode)
                shift
                export DETSIM_MODE="$1"
            ;;
            --detsim-submode)
                shift
                export DETSIM_SUBMODE="$1"
                debug: detsim submode: "$DETSIM_SUBMODE"
            ;;
            --elecsim-mode)
                shift
                export ELECSIM_MODE="$1"
            ;;
            --calib-mode)
                shift
                export CALIB_MODE="$1"
            ;;
            --rec-mode)
                shift
                export REC_MODE="$1"
            ;;
            --external-input-dir)
                shift
                export G_EXTERNAL_INPUT_DIR="$1"
            ;;
            --workdir)
                shift
                export JOB_WORKDIR="$1"
            ;;
            --worksubdir)
                shift
                export JOB_WORKSUBDIR="$1"
            ;;
            -*)
            # Unknown options
                error: unknown option $key
            ;;
            *)
            # maybe the sub commands
                sub_cmd=$key
                shift
                break
            ;;
        esac

        shift;
    done

    if [ -n "$sub_cmd" ]; then
        info: calling: $sub_cmd $*
        # check the global options TAGS
        # if no TAGS defined, just call it directly,
        # otherwise, loop all the TAGS
        if [ -z "$TAGS" ]; then
            info: no tags defined
            $sub_cmd $*
        else
            local job_tag
            for job_tag in $(g-tags)
            do
                export G_CURRENT_TAG=$job_tag
		# get rate from $EXTRA_DEFAULT_RATES
		local rate=$(get-rate $G_CURRENT_TAG)
		if [ -n "$rate" ]; then
		    export ELEC_DEFAULT_RATE=$rate
		else
		    unset ELEC_DEFAULT_RATE
		fi
                # create the tag directory
                [ -d "$job_tag" ] || mkdir -p $job_tag
                pushd $job_tag
                # parse the job_tag by developers
                # Then several variables could be setup.
                type -t l-parse-tag>&/dev/null && l-parse-tag $job_tag
                # call the real command
                $sub_cmd $*
                # update the seed number. 
                # - avoid duplication in different tags.
                export SEED=$[$SEED+1]
                popd
            done
        fi
    fi
}

##########################################
# available cmds
##########################################
function available_cmds() {
    for fn in $(compgen -A function cmd-)
    do
        fn=${fn/cmd-/}
        echo $fn
    done
}

##########################################
# show helps
##########################################
function show_helps() {
    : Print the functions with cmd- prefix
    info: options:
    info: '[-h|--help] [--seed SEED] [--evtmax EVTMAX] [--njobs NJOBS] [--setup SETUP]'
    info: '[--target-os TARGET_OS]' '[--benchmark]'
    info: '[--condor|--lsf|--pbs]' '[--tags|--tag-list "TAG1 TAG2 .."]'
    info: '[--extra-mixing-tags "T1:F1:R1 T2:F2:R2 .."]'
    info: supporting modes:
    for fn in $(available_cmds)
    do
        info: $fn
    done
    : Print the extral helps
    info:
    info: Additional Helps:
    local hf # help_function
    for hf in $HELPS_LIST;
    do
        type -t $hf >&/dev/null && $hf
    done
}

##########################################
# generate jobs
# * condor/ LSF?
# * bash script
##########################################

function genjob-condor-XXX-requirements() {
    declare -a requirementlist
    if [ -n "$(g-condor-target-os)" ]; then
        requirementlist+=("$(g-condor-target-os)")
    fi
    if [ -n "$(g-condor-benchmark)" ]; then
        requirementlist+=("$(g-condor-benchmark)")
    fi
    if [ -n "${CONDOR_REQUIREMENTS}" ]; then
        requirementlist+=("${CONDOR_REQUIREMENTS}")
    fi
    python -c 'import sys; print("&&".join(sys.argv[1:]))' "${requirementlist[@]}"
}

function genjob-condor-XXX() {
    : only call this when batch mode is condor
    [ "$(g-is-condor)" = "Y" ] || return

    local TYPE=$1; shift
    local GEN_NAME=$1; shift
    local SH_NAME=$1; shift
    [ -z "$GEN_NAME" ] && GEN_NAME="sub-$TYPE-$(g-seed).condor"
    [ -z "$SH_NAME" ] && SH_NAME="run-$TYPE-$(g-seed).sh"
    # put the condor script in $TYPE directory
    [ -d "$TYPE" ] || mkdir $TYPE
cat > $TYPE/$GEN_NAME << EOF
Universe = vanilla
Executable = $SH_NAME
Accounting_Group = juno
getenv = True
$(g-condor-request-memory)
Requirements = $(genjob-condor-XXX-requirements)
$(g-condor-extra-template)
Queue
EOF
}

function genjob-shell-XXX() {
    : Here, we will call cmd-XXX to generate command line
    : NOTE: to join multi lines into one line, we use:
    :          'cmd-TYPE | xargs'
    local TYPE=$1; shift
    local SH_NAME=$1; shift
    local LOG_NAME=$1; shift

    [ -z "$SH_NAME" ] && SH_NAME="run-$TYPE-$(g-seed).sh"
    [ -z "$LOG_NAME" ] && LOG_NAME="log-$TYPE-$(g-seed).txt"

    # put the condor script in $TYPE directory
    [ -d "$TYPE" ] || mkdir $TYPE

cat > $TYPE/$SH_NAME <<EOF
#!/bin/bash
cd $(pwd)
source $(g-setup)
$JUNOTESTROOT/production/libs/jobmom.sh \$\$ >& $TYPE/${LOG_NAME}.mem.usage &
(time $(cmd-$TYPE|xargs) ) >& $TYPE/$LOG_NAME
EOF

    chmod +x $TYPE/$SH_NAME

}

##########################################
# helper: generate the seed sequence
# It is used to generate split jobs.
##########################################
function seqs() {
    local startseed=$(g-seed)
    local N=$(g-njobs)
    local seed
    for ((j=0; j<$N; ++j))
    do
        seed=$[$startseed+$j]
        echo $seed
    done
}
