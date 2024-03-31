#!/usr/bin/env python

import Sniper

task = Sniper.Task("task")
task.asTop()
task.setLogLevel(2)

import ConfigParser
# Put the ABSOLUTE path to my.conf here
import os
PathtoConf = "my.conf"
# dbtestroot = os.environ.get("DATABASETESTROOT")
dbtestroot = os.environ.get("DATABASESPMSALGROOT")
print dbtestroot
if dbtestroot:
    PathtoConf = os.path.join(dbtestroot, "share", "my.conf")
# The same name of section in my.conf
ConName = "mysql_one"
print PathtoConf
conf = ConfigParser.ConfigParser()
conf.read(PathtoConf)
url = conf.get(ConName, "Url")
user = conf.get(ConName, "User")
password = conf.get(ConName, "Password")

import DatabaseSvc
task.property("svcs").append("DatabaseSvc")
db = task.find("DatabaseSvc")
db.property("Url").set(url)
db.property("User").set(user)
db.property("Password").set(password)

import DatabaseSpmsAlg
alg = task.createAlg("DatabaseSpmsAlg/myalg")
alg.property("Mode").set("write")
alg.property("id").set("12")

#Note:if the request to database only need execute once, look out the EvtMax,
#execute a query too much times may cause greate burden of database
task.setEvtMax(1)
task.show()
task.run()
