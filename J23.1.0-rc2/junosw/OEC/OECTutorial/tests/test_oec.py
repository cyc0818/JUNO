#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import logging

logging.basicConfig(level=logging.INFO)

from JunoTest import *

sim2rec_test = UnitTest()
sim2rec_test.enableGenLog()
sim2rec_test.runner.verbosity = 2

# our detsim script
import os
default_path = os.path.dirname(os.path.realpath(__file__)) # test
default_path = os.path.dirname(default_path) # Tutorial
tutorial_dir = os.getenv("TUTORIALROOT", default_path)
oectutorial_dir = os.getenv("OECTUTORIALROOT", default_path)

logging.info("TUTORIAL DIR (default): %s"% default_path)
logging.info("TUTORIAL DIR: %s"% tutorial_dir)

script_detsim = os.path.join(tutorial_dir, "share", "tut_detsim.py")
script_det2elec = os.path.join(tutorial_dir, "share", "tut_det2elec.py")
script_elec2oec = os.path.join(oectutorial_dir, "share", "tut_oec.py")

for script in [script_detsim, script_det2elec, script_elec2oec]:
    logging.info(script)
    
    if not os.path.exists(script):
        logging.error("%s does not exist"%script)
        import sys
        sys.exit(-1)

# = add different test cases =

# == detector simulation ==
cmd_name = "detsim/default"
cmd = "python %s --evtmax 10 --no-gdml gun"%script_detsim
sim2rec_test.addCase(cmd_name, cmd)

# === detsim -> elec ===

cmd_name = "elecsim/default"
cmd = "python %s --evtmax -1"%script_det2elec
sim2rec_test.addCase(cmd_name, cmd)

# === elec -> oec ===
cmd_name = "elec2oec/default"
cmd = "python %s --evtmax -1 --input sample_elecsim.root"%script_elec2oec
sim2rec_test.addCase(cmd_name, cmd)

# run all the tests
sim2rec_test.run()
