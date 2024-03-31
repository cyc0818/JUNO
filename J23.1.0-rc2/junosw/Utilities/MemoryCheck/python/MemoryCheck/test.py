#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

from MemoryCheck import checkMemory

if __name__ == "__main__":

    import Sniper
    task = Sniper.Task("task")
    task.asTop()
    task.setEvtMax(10)

    cm = checkMemory("CheckMemory")
    cm.initialize()
    for i in xrange(1000):
        cm.execute()
    cm.finalize()
