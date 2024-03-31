#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao


import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.setEvtMax(1)
    task.setLogLevel(2)

    import RandomSvc
    task.property("svcs").append("RandomSvc")

    rndm = task.find("RandomSvc")
    print rndm
    rndm.property("Seed").set(42)


    print "Using Original"
    for i in range(100):
        print rndm.random()

