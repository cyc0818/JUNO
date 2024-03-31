#!/usr/bin/python

from JunoTest import *
import os,sys

top = os.getenv('TUTORIALROOT')
if not top:
  sys.exit(1)
top += '/share'

test = UnitTest()

wf = Workflow()
wf.addStep('detsim', 'python %s/tut_detsim.py gun' % top, RESMonitor = True)
wf.addStep('calib', 'python %s/tut_det2calib.py' % top, genLog = True)
wf.addStep('rec', 'python %s/tut_calib2rec.py' % top)
test.addWorkflow(wf)

test.run()
