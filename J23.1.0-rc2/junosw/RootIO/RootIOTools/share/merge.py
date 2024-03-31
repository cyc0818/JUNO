#!/usr/bin/env python
# -*- coding:utf-8 -*-

import sys
import argparse
import Sniper
import RootIOTools
import glob
import BufferMemMgr
import RootIOSvc

if __name__ == '__main__':
  parser = argparse.ArgumentParser(description='merge data model files.')
  parser.add_argument("output", help="output filename")
  parser.add_argument("input", nargs='+', help="input filenames")
  args = parser.parse_args()

  # check the existance
  import os.path
  if os.path.exists(args.output):
    print("%s already exists."%args.output)
    print("Please remove it before merge")
    sys.exit(-1)

  task = Sniper.TopTask("TopTask")
  task.setLogLevel(0)

  sub_task_name = "input_task"
  mfalg = task.createAlg("HaddAlg/mfalg")
  mfalg.property("OutputFile").set(args.output)
  mfalg.property("SubTaskName").set(sub_task_name)
  bufMgr_o = task.createSvc("BufferMemMgr")
  roSvc = task.createSvc("RootOutputSvc/OutputSvc")
  # Output streams will be set automatically based on the input data.
  # Settings here will be overwritten 
  # roSvc.property("OutputStreams").set({})

  sub_task = task.createTask("Task/"+sub_task_name)
  bufMgr_i = sub_task.createSvc("BufferMemMgr")
  riSvc = sub_task.createSvc("RootInputSvc/InputSvc")
  riSvc.property("InputFile").set(args.input)

  task.setEvtMax(-1)
  task.show()
  task.run()
