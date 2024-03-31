#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper


if __name__ == "__main__":

    task = Sniper.Task("task")
    task.asTop()

    import JunoTimer
    junoTimerSvc = task.createSvc("JunoTimerSvc")
    print junoTimerSvc

    t = junoTimerSvc.get("t")
    t.start()
    import time
    time.sleep(10)
    t.stop()
    print t.elapsed()

