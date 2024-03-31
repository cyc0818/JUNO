#!/usr/bin/env python

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
    "local": "LocalFSRepo"
})

conddbsvc.property("RepoURIs").set({
    "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier",
    "local": os.path.join("dummy-repo")
})
conddbsvc.property("RepoProxies").set(
    ["http://squid-01.ihep.ac.cn:3128"]
)
conddbsvc.property("GlobalTag").set(
    "GT_J18_v1"
)
conddbsvc.initialize()

# choose one of them
# reponame = "local"
reponame = "frontier"

db = conddbsvc.getRepo(reponame)
print(db)
import base64
for gt in db.findGlobalTags("GT_J18_v1"):
    for tag in db.findTags(gt):
        print "Find tag %s"% tag.name()
        for iov in db.findIOVs(tag):
            print "since: %d" %iov.since()
            for payload in db.findPayloads(iov):
                print "Find payload %s "% payload.hash()
                data = payload.data()
                print data
                ddata = base64.b64decode(data)
                print ddata

# create a testing object
obj_CD_LS_matprop = CondDB.CD_LS_MatProp()
conddbsvc.declCondObj("CD.LS.MatProp", obj_CD_LS_matprop)
for cur in range(0,10000,1000):
    conddbsvc.setCurrent(cur)
    print "Cur %d\n -> light yield: %d"%(cur, obj_CD_LS_matprop.light_yield)
    print " -> %s" % ",".join("%f:%f"%(k,v) for k,v in zip(obj_CD_LS_matprop.refractive_energy,
                                                           obj_CD_LS_matprop.refractive_index))
