import Sniper

## TopTask
task = Sniper.TopTask("TopTask")

import BufferMemMgr
bufMgr = task.createSvc("BufferMemMgr")
#bufMgr.property("TimeWindow").set([-0.01, 0.01]);

## I/O
import RootIOSvc
outputdata = {"/Event/Sim": "sample_classified.root"}
oSvc = task.createSvc("RootOutputSvc/OutputSvc")
oSvc.property("OutputStreams").set(outputdata)

## classification
Sniper.loadDll("libClassificationAlg.so")
dummyMCTruthCreatorAlg = task.createAlg("DummyMCTruthCreatorAlg")
classificationAlg = task.createAlg("ClassificationAlg")

Sniper.loadDll("libDetClassification.so")
Sniper.loadDll("libMCTruthClassification.so")
cd_classifier = classificationAlg.createTool("CentralDetectorClassifier")
tracklike_classifier = classificationAlg.createTool("MCTruthTrackLikeClassifier")
pointlike_classifier = classificationAlg.createTool("MCTruthPointLikeClassifier")
classificationAlg.property("ClassifierNames").set([
    cd_classifier.objName(),
    tracklike_classifier.objName(),
    pointlike_classifier.objName()])

## steering
Sniper.loadDll("libTopSteering.so")
task.createAlg("TopSteering")

## SubTask: RecPoint
sub = task.createTask("Task/RecPoint")
#this sub-task shares the same DataBuffer of the top-task:"/Event" via MappedDataSvc
md = sub.createSvc("MappedDataSvc")
md.regist(task, "/Event", "/Event")
sub.createAlg("SteeringTestAlg/RecPointAlg")

## SubTask: RecTrack
#it is an instance of the class TopTask, so that we can create sub-sub-tasks for it
sub = task.createTask("TopTask/RecTrack")
#this sub-task shares the same DataBuffer of the top-task:"/Event" via MappedDataSvc
md = sub.createSvc("MappedDataSvc")
md.regist(task, "/Event", "/Event")
sub.createAlg("SteeringTestAlg/RecTrackAlg")
sub.createTask("Task/RecSingle")
sub.createTask("Task/RecBundle")

## SubTask: RecWP
sub = task.createTask("Task/RecWP")
#this sub-task shares the same DataBuffer of the top-task:"/Event" via MappedDataSvc
md = sub.createSvc("MappedDataSvc")
md.regist(task, "/Event", "/Event")
sub.createAlg("SteeringTestAlg/RecWPAlg")

## SubTask: RecTT
sub = task.createTask("Task/RecTT")
#this sub-task shares the same DataBuffer of the top-task:"/Event" via MappedDataSvc
md = sub.createSvc("MappedDataSvc")
md.regist(task, "/Event", "/Event")
sub.createAlg("SteeringTestAlg/RecTTAlg")

task.setEvtMax(10)
task.show()
task.run()
