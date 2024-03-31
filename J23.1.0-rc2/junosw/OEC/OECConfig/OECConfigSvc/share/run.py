#!/usr/bin/env python
#-*- coding: utf-8 -*-

import Sniper

def get_parser():
    import argparse

    parser=argparse.ArgumentParser(description='run RecQT')
    parser.add_argument("--loglevel",default="Test",choices=["Test","Debug","Info","Warn","Error","Fatal"],help="set the log level")
    parser.add_argument("--evtmax",type=int,default=1,help="events to be processed")
    return parser

DATA_LOG_MAP = {"Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6}

if __name__ == "__main__":
    parser=get_parser()
    args = parser.parse_args()

    task=Sniper.Task("OECtask")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(DATA_LOG_MAP[args.loglevel])
    
    import OECConfigSvc
    #from EvtConfig import EvtConfigSvc
    OECConf = task.createSvc("OECConfigSvc")
    OECConf.property("OECListFile").set("/junofs/users/liuyan/OEC/oec-prototype/OECConfig/OECConfigSvc/sig.xml")

    import TestForConfig
    oecalg=task.createAlg("TestForConfig")
    
    
    task.run()
    #task.show()



