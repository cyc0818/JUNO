#!/usr/bin/env python

import Sniper

def getParser():
  import argparse

  parser = argparse.ArgumentParser(description="Generate dummy event data")
  parser.add_argument("--loglevel", default="Test",
                       choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                       help="Set the Log Level")
  parser.add_argument("--evtmax", type=int, default=1000, help='events to be generated')
  parser.add_argument("--nhits", type=int, default=1000, help='dummy hits number in one event')
  parser.add_argument("--seed", type=int, default=42, help='random seed')
  parser.add_argument("--output", default="sample.root", help='output file name')
  parser.add_argument("--mode", default="Plain", choices=["Plain", "EDM"], help='run mode')

  return parser

if __name__ == "__main__":
  logLevel = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6}
  mode = {"Plain":0, "EDM":1}
  parser = getParser()
  args = parser.parse_args()

  task = Sniper.Task("task")

  task.setLogLevel(logLevel[args.loglevel])

  import RootIOTest
  alg = task.createAlg("MakeSample")
  alg.property("hitNum").set(args.nhits)
  alg.property("mode").set(mode[args.mode])
  alg.property("outputFile").set(args.output)
  alg.property("EventPerFile").set(1000)

  if args.mode == "EDM":
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    import RootIOSvc
    task.createSvc("RootOutputSvc/OutputSvc")
    ro = task.find("OutputSvc")
    ro.property("OutputStreams").set({"/Event/Sim": args.output})

  task.setEvtMax(args.evtmax)
  task.show()
  task.run()
