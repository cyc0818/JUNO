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
script = os.path.join(top, 'share/tut_detsim.py')
nevt = 1000
momentum = 8.0

command = "python %s --no-gdml --evtmax %s --no-anamgr-edm gun --particle e+ --momentums %s" % (script, nevt, momentum)

test.addCase("positron_8mev_1000evt_no_edm", "python %s --no-gdml --evtmax %s --no-anamgr-edm gun --particle e+ --momentums %s" % (script, nevt, momentum), genLog=True)
test.addCase("positron_8mev_1000evt_edm", "python %s --no-gdml --evtmax %s gun --particle e+ --momentums %s" % (script, nevt, momentum), genLog=True)

test.run()
