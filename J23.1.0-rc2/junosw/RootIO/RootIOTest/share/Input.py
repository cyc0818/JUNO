#!/usr/bin/env python

import Sniper

def getParser():
  import argparse

  parser = argparse.ArgumentParser(description="Input and select dummy event data")
  parser.add_argument("--loglevel", default="Test",
                       choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                       help="Set the Log Level")
  parser.add_argument("--evtmax", type=int, default=1000, help='events to be processed')
  parser.add_argument("--seed", type=int, default=42, help='random seed')
  parser.add_argument("--ratio", type=float, default=1.0, help='selection ratio')
  parser.add_argument("--mode", default="Plain", choices=["Plain", "EDM"], help='run mode')
  parser.add_argument("--input", default="sample.root", help='input file name')

  return parser

if __name__ == "__main__":
  logLevel = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6}
  mode = {"Plain":0, "EDM":1}
  parser = getParser()
  args = parser.parse_args()

  task = Sniper.Task("task")

  task.setLogLevel(logLevel[args.loglevel])

  import RootIOTest
  alg = task.createAlg("SelectEventData")
  alg.property("Mode").set(mode[args.mode])
  alg.property("InputFile").set(args.input)
  alg.property("Ratio").set(args.ratio)

  if args.mode == "EDM":
    import RootIOSvc
    task.createSvc("RootInputSvc/InputSvc")
    ri = task.find("InputSvc")
    ri.property("InputFile").set([args.input])

    #create Data Buffer Manager
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

  task.setEvtMax(args.evtmax)
  task.show()
  task.run()
