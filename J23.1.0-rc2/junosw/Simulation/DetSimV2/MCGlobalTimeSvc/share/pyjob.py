#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao


import Sniper

if __name__ == "__main__":

    task = Sniper.Task("task")
    task.setEvtMax(1)
    task.setLogLevel(2)

    Sniper.loadDll("libMCGlobalTimeSvc.so")
    timesvc = task.createSvc("MCGlobalTimeSvc")
    # we need to handle the error cases
    begintime = [
                    "",                    # empty string -> fail
                    "1970",                # year only    -> fail
                    "1970-01",             # with month   -> fail
                    "1970-01-01",          # with day     -> fail
                    "1970-01-01 00:00",    # with HH:MM   -> fail
                    "1970-01-01 00:00:01", # works well   -> OK
                    "1970-01-01-00:00:01", # extra -      -> fail
                    "1970-01-01 00:00:01-",# extra -      -> OK
                                           #   it's success.
                                           #   actually, we should report it.
                    "1970-01-01 00:00:01--",# extra --    -> OK
                ]

    endtime = [
                    "",
                    "0000-00-00 00:00:00", # month/day wrong -> fail
                    "0000-01-01 00:00:00", #                 -> OK, 
                                           #                    wrong result
                    "9999-01-01 00:00:00", #                 -> OK, 
                                           #                    wrong result
                    "2039-01-01 00:00:00", #                 -> OK, 
                                           #                    wrong result
                    "2038-01-19 03:14:07",
              ]
    timesvc.property("BeginTime").set(begintime[5])
    #timesvc.property("BeginTime").set(begintime[8])
    timesvc.property("EndTime").set(endtime[5])
    #timesvc.property("EndTime").set(endtime[4])

    task.show()
    task.run()
