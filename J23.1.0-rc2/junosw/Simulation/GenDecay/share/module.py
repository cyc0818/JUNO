#!/usr/bin/env python
# -*- coding:utf-8 -*_
import sys
sys.setdlopenflags( 0x100 | 0x2)
import libSniperMgr
import libSniperPython as SP
#-----------------------------------------------------------------
# Global Setting
#-----------------------------------------------------------------
mgr = libSniperMgr.SniperMgr()
mgr.setProp("EvtMax", 5)
mgr.setProp("LogLevel", 2)
mgr.setProp("Dlls", ["DataBufTest"])
mgr.configure()

#-----------------------------------------------------------------
# Svc Related
#-----------------------------------------------------------------
svcMgr = SP.SvcMgr.instance()

# Configure for buffer service  
bufsvc = SP.SvcMgr.get("DataBufSvc", True)
svcMgr.add(bufsvc.name())				# Add the svc to svcMgr
vp=["/Event/Rec/RecHeader", "Event/Calib/CaliHeader"]	# valid path(contain all the paths)
ip=["/Event/Calib/CalibHeader"]				# input path
op=["/Event/Rec/RecHeader"]				# output path
bufsvc.setProp("ValidPaths", vp)
bufsvc.setProp("InputItems", ip)
bufsvc.setProp("OutputItems", op)

# Configure for IO service
iosvc = SP.SvcMgr.get("SniperIOSvc", True)
svcMgr.add(iosvc.name())

filelist = ["recEvtIn.root"]				# Input file list
iosvc.setProp["InputFile", filelist]			# input file
iosvc.setProp["OutputFile", "recEvtOut.root"]		# output file

#-----------------------------------------------------------------
# Alg Related
#-----------------------------------------------------------------
algMgr = SP.AlgMgr.instance()


x = SP.AlgMgr.get("DataBufTest/dtbftest",True)		# Alg should be declared first,hwalg is the name of AlgBase
algMgr.add(x.name()) 					# Add the alg to algMgr



#-----------------------------------------------------------------
# Run
#-----------------------------------------------------------------
if mgr.initialize():
    mgr.run()
mgr.finalize()
