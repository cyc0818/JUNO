#!/usr/bin/env python
#-*- coding: utf-8 -*-

# Description:
#   This script is used to test the new schema (2022.07)
#
# Author:
#   Tao Lin <lintao AT ihep.ac.cn>


import os.path
import CondDB

url = "http://junodb1.ihep.ac.cn:8080/Frontier"

# db =  CondDB.DBRepo(url)

# or 
import Sniper
Sniper.setLogLevel(0)
task = Sniper.Task("testtask")
conddbsvc = task.createSvc("CondDBSvc")
conddbsvc.property("RepoTypes").set({
    "frontier": "Frontier",
})

conddbsvc.property("RepoURIs").set({
    "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier",
})
# conddbsvc.property("RepoProxies").set(
#     ["http://squid-01.ihep.ac.cn:3128"]
# )
conddbsvc.property("GlobalTag").set(
    "J22.1.0"
)
conddbsvc.initialize()

# choose one of them
reponame = "frontier"

db = conddbsvc.getRepo(reponame)
print(db)
for gt in db.findGlobalTags("J22.1.0"):
    for tag in db.findTags(gt):
        print("Find tag %s"% tag.name())
        for iov in db.findIOVs(tag):
            print("since: %d" %iov.since())
            for payload in db.findPayloads(iov):
                print("Find payload %s "% payload.hash())
                path = payload.path()
                print(path)
                data = payload.data()
                print(data)

