#!/usr/bin/python

import Sniper
task = Sniper.Task("task")
task.setLogLevel(3)

Sniper.loadDll("libTensorFlowInference.so")
alg = task.createAlg("TensorFlowInferenceAlg/alg_example")
alg.property("model").set("graph.pb")
task.setEvtMax(10)
task.show()
task.run()
