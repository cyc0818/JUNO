#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: liuyan

# using the new Mgr.

import Sniper
import os

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6}
if __name__ == "__main__":
    topTask=Sniper.TopTask("TopTask")
    topTask.setLogLevel(DATA_LOG_MAP["Debug"])
    topTask.setEvtMax(5)

    import OECTagSvc
    tagsvc=topTask.createSvc("OECTagSvc")
    tagsvc.property("OECTagFile").set(os.getenv('OECTAGSVCROOT')+"/share/tag.xml")

    import EvtSteering
    Sniper.loadDll("libSniperCoreUsages.so")
    stephandler = topTask.createAlg("StepHandler")
    stepsequencer = stephandler.createTool("StepSequencer")
    stepsequencer.property("ConfigFile").set("seq.json")
    stepdecision= stephandler.createTool("StepDecision")
    stepdecision.property("ConfigFile").set("sig.json")

    topTask.show()
    topTask.run()

