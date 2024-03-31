#!/usr/bin/env python

import argparse
parser = argparse.ArgumentParser(description="Run ONNX Runtime.")
parser.add_argument("--evtmax", type=int, default=10, help="number of events")
parser.add_argument("--nintra", type=int, default=1, help="intra num threads")
parser.add_argument("--ninter", type=int, default=1, help="inter num threads")
parser.add_argument("model", help='Model path.')
args = parser.parse_args()

import Sniper
task = Sniper.Task("task")
task.setLogLevel(3)

Sniper.loadDll("libONNXRuntimeInference.so")
alg = task.createAlg("OrtInferenceAlg")
alg.property("Model").set(args.model)
alg.property("IntraOpNumThreads").set(args.nintra)
alg.property("InterOpNumThreads").set(args.ninter)
task.setEvtMax(args.evtmax)
task.show()
task.run()
