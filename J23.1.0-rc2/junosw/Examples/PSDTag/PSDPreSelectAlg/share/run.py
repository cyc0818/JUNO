#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Yuxiang -*-

##############################################################################
# Import
##############################################################################
from Tutorial import JUNOApplication
from Tutorial import JUNOModule
from Tutorial import JUNODataBufferModule
from Tutorial import JUNORootInputModule
from Tutorial import JUNOPSDModule
from Tutorial import JUNORootWriterModule



from Tutorial import JUNOMCParameterModule
from Tutorial import JUNOPMTParameterModule
from Tutorial import JUNOPMTCalibSvcModule
from Tutorial import JUNOCondDBModule
from Tutorial import JUNOPSDModule
from Tutorial import JUNOPSDPreSelectModule
from Tutorial import JUNORootOutputModule
from Tutorial import JUNORootInputModule


# add below modules suggested by Cailian
from Tutorial import JUNODBIModule
from Tutorial import JUNOParaDBModule
from Tutorial import JUNOGeometryModule


from Tutorial.JUNOUtils import mh


##############################################################################
# Application
##############################################################################
juno_application = JUNOApplication(description="Tag PSD select trigger event")
juno_application.set_default_input("sample_detsim.root")
juno_application.set_default_user_output("sample_PSD_user.root")
rootinput_module = JUNORootInputModule()
psdpreselect_module = JUNOPSDPreSelectModule()

juno_application.register(rootinput_module)
juno_application.register(psdpreselect_module)



#import PSDTools

geom_module = JUNOGeometryModule()

rootwriter_module = JUNORootWriterModule()
databuffer_module = JUNODataBufferModule()
rootoutput_module = JUNORootOutputModule()
mcparam_module = JUNOMCParameterModule()
pmtparam_module = JUNOPMTParameterModule()
pmtcalib_module = JUNOPMTCalibSvcModule()
conddb_module = JUNOCondDBModule()

dbisvc_module = JUNODBIModule()
paradb_module = JUNOParaDBModule()

psd_module = JUNOPSDModule()
psd_module.set_shared_buffer_flag(flag = True)


juno_application.register(module = geom_module,       subtaskname="psdtask")
juno_application.register(module = rootwriter_module,subtaskname="psdtask")
juno_application.register(module = rootoutput_module ,subtaskname="psdtask")
juno_application.register(module = mcparam_module   ,subtaskname = "psdtask")
juno_application.register(module = pmtparam_module  ,subtaskname = "psdtask")
juno_application.register(module = pmtcalib_module  ,subtaskname = "psdtask")
juno_application.register(module = conddb_module    ,subtaskname = "psdtask")
juno_application.register(module = psd_module,       subtaskname="psdtask")
# ==================== #
juno_application.register(module = dbisvc_module,       subtaskname="psdtask")
juno_application.register(module = paradb_module,       subtaskname="psdtask")



juno_application.run()
