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

test.addCase("OutputPlain_heavy", "python %s --mode Plain --evtmax 300000 --nhits 1000 --output heavy_plain.root" % oscript)
test.addCase("OutputEDM_heavy", "python %s --mode EDM --evtmax 300000 --nhits 1000 --output heavy_edm.root" % oscript)
test.addCase("InputPlain_heavy", "python %s --mode Plain --evtmax 300000 --input heavy_plain.root" % iscript)
test.addCase("InputEDM_heavy", "python %s --mode EDM --evtmax 300000 --input heavy_edm.root" % iscript)
test.addCase("InputEDM_heavy_half", "python %s --mode EDM --evtmax 300000 --ratio 0.5 --input heavy_edm.root" % iscript)

test.run()
