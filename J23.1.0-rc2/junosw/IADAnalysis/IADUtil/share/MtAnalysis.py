def GlobalInput():
    task = Sniper.Task("GlobalInput")
    ialg = task.createAlg("InputThreadAlg")
    #ialg.property("IADFile").set("/home/yixiang/data/IADAnalysis/analyTest/WithDic.IAD")
    ialg.property("IADFile").set(options.input)
    return task

def GlobalOutput():
    task = Sniper.Task("GlobalOutput")
    oalg = task.createAlg("ClearBufferAlg")
    return task

def Worker():
    analysisTask = Sniper.TopTask("AnalysisTask")
    analysisTask.createAlg("AnalyTopAlg")
    memMgr = analysisTask.createSvc("FilterMemMgr")
    memMgr.property("TimeWindow").set([-0.1,0.1])
    import SniperProfiling
    profiling = analysisTask.createSvc("SniperProfiling")

    sub = analysisTask.createTask("Task/FilterTask")
    sub.createAlg("WorkerFilterAlg")
    sub.createSvc("DataExchangeSvc")
    sub.createSvc("SniperProfiling")

    return analysisTask

if __name__ == "__main__":
    ## load libraries
    import Sniper, SniperMuster
    Sniper.loadDll("libIADUtility.so")

    import sys
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", help = "input IAD file")
    options = parser.parse_args(sys.argv[1:])
    
    #import ROOT
    #ROOT.EnableThreadSafety()
    ## I/O and global buffer
    gs = SniperMuster.createGlobalStream("IADGlobalStream")
    gs.configBuffer(100, 60)  #the buffer must be created before the I/O tasks
    gs.configInput(GlobalInput)
    gs.configOutput(GlobalOutput)

    ## config the muster
    muster = SniperMuster.Muster()
    muster.config(Worker, 4)
    muster.setIO(gs)
    muster.setEvtMax(-1)

    ## show the configuration and run
    SniperMuster.show()
    muster.run()