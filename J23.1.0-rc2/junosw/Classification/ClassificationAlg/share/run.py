#!/usr/bin/env python
#-*- coding: utf-8 -*-
#
# Description:
#   An example to show how ClassificationAlg and ClassificationTools work together
#
# Author: Tao Lin <lintao AT ihep.ac.cn>
#

import Sniper

if __name__ == "__main__":
    
    task = Sniper.Task("task")
    task.setEvtMax(10)
    task.setLogLevel(3)

    # = Data Buffer Mgr =
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")

    # = I/O =
    outputdata = {"/Event/Sim": "sample_classified.root"}
    roSvc = task.createSvc("RootOutputSvc/OutputSvc")
    roSvc.property("OutputStreams").set(outputdata)

    # = Classification =
    import ClassificationAlg
    # = create dummy simevent =
    dummyMCTruthCreatorAlg = task.createAlg("DummyMCTruthCreatorAlg")

    # = classify =
    classificationAlg = task.createAlg("ClassificationAlg")

    import DetClassification
    import MCTruthClassification

    cd_classifier = classificationAlg.createTool("CentralDetectorClassifier")

    tracklike_classifier = classificationAlg.createTool("MCTruthTrackLikeClassifier")
    pointlike_classifier = classificationAlg.createTool("MCTruthPointLikeClassifier")

    muonbundle_classifier = classificationAlg.createTool("MCTruthMuonBundleClassifier")

    classificationAlg.property("ClassifierNames").set([cd_classifier.objName(),
                                                       tracklike_classifier.objName(),
                                                       pointlike_classifier.objName(),
                                                       muonbundle_classifier.objName()])

    # run
    task.show()
    task.run()
