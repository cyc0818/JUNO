#!/usr/bin/python

from JunoTest import *

test = UnitTest()

# Overall options
test.enableCPUMonitor()
test.enableVIRMonitor()
test.enableRESMonitor()

import os,sys
top = os.getenv('TUTORIALROOT')
if not top:
  sys.exit(1)
dscript = top + '/share/tut_detsim.py'

test.addCase("detsim1", "python %s --evtmax 10 gun" % dscript, genLog = True, CPUMonitor = False)
test.addCase("detsim2", "python %s --evtmax 200 gun" % dscript)
test.addCase("detsim3", "python %s --evtmax 200 gun" % dscript, genLog = True, maxTime = 100)
test.addCase("detsim4", "python %s --evtmax 200 gun" % dscript, genLog = True, maxVIR = 200)

test.run()
