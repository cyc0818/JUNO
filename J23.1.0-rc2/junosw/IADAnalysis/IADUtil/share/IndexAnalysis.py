import Sniper, SniperMuster

Sniper.loadDll("libIADUtility.so")

import sys
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input", help = "input IAD file")
options = parser.parse_args(sys.argv[1:])

#Create your data analysis task
analysisTask = Sniper.TopTask("AnalysisTask")
#Your data analysis algorithm, you can config several algoithms as your will
AnalyAlg = analysisTask.createAlg("AnalyTopAlg")

#Important: Please must config this FilterMemMgr,  
#it can help to load the whole window of the target event
memMgr = analysisTask.createSvc("FilterMemMgr")
#here you can set your window size as your will
memMgr.property("TimeWindow").set([-0.1,0.1])

#Create the filter task
sub = analysisTask.createTask("Task/FilterTask")
#config the filter
alg = sub.createAlg("FilterAlg")#Config your IAD selection algorithm
sub.createSvc("DataExchangeSvc")#Used to forward the event to the analysis task
#alg.property("Input").set("/home/yixiang/data/IADAnalysis/analyTest/WithDic.IAD")
alg.property("Input").set(options.input)

import SniperProfiling
profiling = analysisTask.createSvc("SniperProfiling")

analysisTask.setEvtMax(-1)
analysisTask.run()