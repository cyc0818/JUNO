import Sniper
import BufferMemMgr
import RootIOSvc
import SniperMuster
#Generate an IAD file from an EDM file

Sniper.loadDll("libIADUtility.so")

import sys
import argparse
parser = argparse.ArgumentParser()
parser.add_argument("-i", "--input", help = "input Oec file")
parser.aad_argument("-o" "--output", help = "output IAD file")
options = parser.parse_args(sys.argv[1:])

task = Sniper.Task("GenIADWithDic")
bufMgr = task.createSvc("BufferMemMgr")
riSvc = task.createSvc("RootInputSvc/InputSvc")
#riSvc.property("InputFile").set(["/home/yixiang/data/IADAnalysis/analyTest/analyOec.root"])
riSvc.property("InputFile").set([options.input])

x = task.createAlg("GenIADWithDic")
x.property("inputFile").set(options.input)
#x.property("outputFile").set("/home/yixiang/data/IADAnalysis/analyTest/WithDic.IAD")
x.property("outputFile").set(options.output)
x.setLogLevel(3)

task.setEvtMax(-1)
task.run()

