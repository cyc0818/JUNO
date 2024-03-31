#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper
from MemoryCheck.MemoryTask import MemoryTask

if __name__ == "__main__":
    Sniper.setLogLevel(0)
    mtask = MemoryTask("memorytask")

    mtask.asTop()
    mtask.setEvtMax(10)

    mtask.show()
    mtask.run()
    mtask.finalize()
