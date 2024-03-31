#!/usr/bin/python

from JunoTest import *

test = UnitTest()

# Overall options
test.enableCPUMonitor()
test.enableVIRMonitor()
test.enableRESMonitor()

import os,sys
top = os.getenv('ROOTIOTESTROOT')
if not top:
  sys.exit(1)
oscript = os.path.join(top, 'share/Output.py')
iscript = os.path.join(top, 'share/Input.py')

test.addCase("OutputPlain", "python %s --mode Plain --evtmax 10000 --nhits 30000 --output simple_plain.root" % oscript)
test.addCase("OutputEDM", "python %s --mode EDM --evtmax 10000 --nhits 30000 --output simple_edm.root" % oscript)
test.addCase("InputPlain", "python %s --mode Plain --evtmax 10000 --input simple_plain.root" % iscript)
test.addCase("InputEDM", "python %s --mode EDM --evtmax 10000 --input simple_edm.root" % iscript)
test.addCase("InputEDM_half", "python %s --mode EDM --evtmax 10000 --ratio 0.5 --input simple_edm.root" % iscript)

test.run()
