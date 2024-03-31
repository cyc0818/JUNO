#!/usr/bin/python

from JunoTest import *

test = UnitTest()

for i in range(6):
  test.addCase("fatal%d" %i, "python run_fatal.py %d" %i)

test.addCase("monitoredFatal", "python run_fatal.py 3", genLog = True, CPUMonitor = True)

test.run()
