#!/bin/bash

function cmp-doc () {
cat << EOF

* same event and same seed status
    Compare following situation:

    1. only set seed number (s0)
        -> we can get the seed status vector (ssv0)
    2. set same seed number (s0) and the seed status vector (ssv0)
    3. set an different seed number (s1) and the previous (ssv0)

    The detector simulation will run here.

    $ # source this script
    $ cmp-stage-1
    $ cmp-stage-2
    $ cmp-stage-3

    Then compare the file stage_1.seed.log, stage_2.seed.log and
    stage_3.seed.log

    You will find that all the file are the same.
    So restore seed status will override the seed.

    WARNNING: if the generator simulation and detector simulation are running
    separately, it's hard for us to repeat the result. The only solution is
    that we also save the seed status of the generator.

* run a partial of the job
    4. start from the original event 3 ( event id is from 0. ) 
EOF
}

function cmp-data-original-seed {
    echo 19900418
}

function cmp-data-new-seed {
    echo 42
}

function cmp-stage-1() {
    local seed=$(cmp-data-original-seed)
    local log_origin=stage_1.orig.log
    local log_only_seed=stage_1.seed.log
    # run the simulation
    python $TUTORIALROOT/share/tut_detsim.py --seed $seed gun >& $log_origin
    # get the seed status
    grep 'Random Engine Seed Status:' $log_origin >& $log_only_seed
}

function cmp-stage-2() {
    local seed=$(cmp-data-original-seed)
    local log_origin=stage_2.orig.log
    local log_only_seed=stage_2.seed.log
    local output=sample_detsim_2.root
    local user_output=sample_detsim_user_2.root

    local stage_1_seed_status=stage_1.seed.log
    local seed_status_file=stage_2_orig_status.txt
    # get the status file
    cat $stage_1_seed_status | cut -d ':' -f 2 >& $seed_status_file
    # run the simulation
    python $TUTORIALROOT/share/tut_detsim.py \
        --seed $seed \
        --restore-seed-status $seed_status_file \
        --output $output --user-output $user_output \
        gun >& $log_origin
    # get the seed status
    grep 'Random Engine Seed Status:' $log_origin >& $log_only_seed
}

function cmp-stage-3() {
    local seed=$(cmp-data-new-seed)
    local log_origin=stage_3.orig.log
    local log_only_seed=stage_3.seed.log
    local output=sample_detsim_3.root
    local user_output=sample_detsim_user_3.root

    local stage_1_seed_status=stage_1.seed.log
    local seed_status_file=stage_3_orig_status.txt
    # get the status file
    cat $stage_1_seed_status | cut -d ':' -f 2 >& $seed_status_file
    # run the simulation
    python $TUTORIALROOT/share/tut_detsim.py \
        --seed $seed \
        --restore-seed-status $seed_status_file \
        --output $output --user-output $user_output \
        gun >& $log_origin
    # get the seed status
    grep 'Random Engine Seed Status:' $log_origin >& $log_only_seed
}

function cmp-stage-4() {
    local seed=$(cmp-data-new-seed)
    local log_origin=stage_4.orig.log
    local log_only_seed=stage_4.seed.log
    local output=sample_detsim_4.root
    local user_output=sample_detsim_user_4.root

    local stage_1_seed_status=stage_1.seed.log
    local seed_status_file=stage_4_orig_status.txt
    # get the status file
    cat $stage_1_seed_status | cut -d ':' -f 2 | sed -n '4,$p'>& $seed_status_file
    # run the simulation
    python $TUTORIALROOT/share/tut_detsim.py \
        --seed $seed \
        --restore-seed-status $seed_status_file \
        --output $output --user-output $user_output \
        gun >& $log_origin
    # get the seed status
    grep 'Random Engine Seed Status:' $log_origin >& $log_only_seed
}
