#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: huangxin

import sys
from Tutorial import JUNOApplication
from Tutorial import JUNODataBufferModule
from Tutorial import JUNORootOutputModule
from Tutorial import JUNORootInputModule
from Tutorial import JUNORootWriterModule

from Tutorial import JUNOGeometryModule
from Tutorial import JUNODBIModule
from Tutorial import JUNOParaDBModule
from Tutorial import JUNOMCParameterModule
from Tutorial import JUNOPMTParameterModule
from Tutorial import JUNOPMTCalibSvcModule
from Tutorial import JUNOCondDBModule

from Tutorial import JUNOElecSimModule

from OECTutorial import JUNOOECTagModule
from OECTutorial import JUNOOECInputModule
from OECTutorial import JUNOOECTopModule
from OECTutorial import JUNOLECModule
from OECTutorial import JUNOOECWaveRecModule
from OECTutorial import JUNOHECModule

juno_oecapp = JUNOApplication(description="JUNO Online event classification")
juno_oecapp.set_default_input_action("append")
juno_oecapp.set_default_output("sample_oec.root")
juno_oecapp.set_default_user_output("sample_oec_user.root")

#---------OEC mode----------
useElecSim = False
useWaveRec = True
useEventRec = True
if '--elecsim' in sys.argv:
    useElecSim = True
elif '--no-elecsim' in sys.argv:
    useElecSim = False
if '--waverec' in sys.argv:
    useWaveRec = True
elif '--no-waverec' in sys.argv:
    useWaveRec = False
if '--evtrec' in sys.argv:
    useEventRec = True
elif '--no-evtrec' in sys.argv:
    useEventRec = False
if useElecSim:
    useWaveRec, useEventRec = True, True
if useWaveRec:
    useEventRec = True

#---------Shared modules----------
rootwriter_module = JUNORootWriterModule()
dbisvc_module = JUNODBIModule()
paradb_module = JUNOParaDBModule()

mcparam_module = JUNOMCParameterModule()
pmtparam_module = JUNOPMTParameterModule()
oectag_module = JUNOOECTagModule()

#-----------For top task-----------
juno_oecapp.register(oectag_module)
juno_oecapp.register(rootwriter_module)
oectop_module = JUNOOECTopModule()
juno_oecapp.register(oectop_module)
#Electronic simulation is registered under top task
if useElecSim:
    juno_oecapp.register(dbisvc_module)
    juno_oecapp.register(paradb_module)
    juno_oecapp.register(mcparam_module)
    juno_oecapp.register(pmtparam_module)
    elecsim_module = JUNOElecSimModule()
    juno_oecapp.register(elecsim_module)

#-----------For input_Task-----------
juno_oecapp.register(oectag_module, 'input_Task')
if not useElecSim:
    rootinput_module = JUNORootInputModule()
    juno_oecapp.register(rootinput_module, 'input_Task')
oecinput_module = JUNOOECInputModule()
juno_oecapp.register(oecinput_module, 'input_Task')


#-----------For LEC_Task-----------
juno_oecapp.register(oectag_module, 'LEC_Task')
lecbuffer_module = JUNODataBufferModule()
juno_oecapp.register(lecbuffer_module, 'LEC_Task')
juno_oecapp.register(rootwriter_module, 'LEC_Task')
if useWaveRec or useEventRec:
    juno_oecapp.register(dbisvc_module, 'LEC_Task')
    juno_oecapp.register(paradb_module, 'LEC_Task')
    juno_oecapp.register(mcparam_module, 'LEC_Task')
    juno_oecapp.register(pmtparam_module, 'LEC_Task')
    geometry_module = JUNOGeometryModule()
    juno_oecapp.register(geometry_module, 'LEC_Task')
    pmtcalib_module = JUNOPMTCalibSvcModule()
    juno_oecapp.register(pmtcalib_module, 'LEC_Task')
if useWaveRec:
    conddb_module = JUNOCondDBModule()
    juno_oecapp.register(conddb_module, 'LEC_Task')
    waverec_module = JUNOOECWaveRecModule()
    juno_oecapp.register(waverec_module, 'LEC_Task')
    pass
    pass
lec_module = JUNOLECModule()
juno_oecapp.register(lec_module, 'LEC_Task')

#-----------For HEC_Task-----------
juno_oecapp.register(oectag_module, 'HEC_Task')
hec_module = JUNOHECModule()
juno_oecapp.register(hec_module, 'HEC_Task')

#-----------For output_Task-----------
outputbuffer_module = JUNODataBufferModule()
juno_oecapp.register(outputbuffer_module, 'output_Task')
rootoutput_module = JUNORootOutputModule()
juno_oecapp.register(rootoutput_module, 'output_Task')

juno_oecapp.run()
