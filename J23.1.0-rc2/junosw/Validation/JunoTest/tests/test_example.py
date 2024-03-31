#!/usr/bin/python

from JunoTest import *

test = UnitTest()

test.setTimeLimit(3000)
#test.enableCPUMonitor()
#test.enableResMonitor()

test.addCase("sleep1", "sleep 1", genLog = True, CPUMonitor = True)
test.addCase("python", "python run_sleep.py", genLog = True, CPUMonitor = True, RESMonitor = True, VIRMonitor = True)
test.addCase("list", "ls -la")
test.addCase("list2", "ls -la")

test.run()


