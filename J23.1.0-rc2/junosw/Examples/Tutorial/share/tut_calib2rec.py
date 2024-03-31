#!/usr/bin/env python
#-*- coding: utf-8 -*-

from Tutorial import JUNOApplication
from Tutorial import JUNODataBufferModule
from Tutorial import JUNORootWriterModule
from Tutorial import JUNORootInputModule
from Tutorial import JUNORootOutputModule
from Tutorial import JUNOGeometryModule
from Tutorial import JUNODBIModule
from Tutorial import JUNOParaDBModule
from Tutorial import JUNOMCParameterModule
from Tutorial import JUNOPMTParameterModule
from Tutorial import JUNOPMTCalibSvcModule
from Tutorial import JUNOCondDBModule
from Tutorial import JUNOJobInfoSvcModule

from Tutorial import JUNOEventRecModule

juno_application = JUNOApplication(description="JUNO Event Reconstruction")
juno_application.set_default_input("sample_calib.root")
juno_application.set_default_output("sample_rec.root")
juno_application.set_default_user_output("sample_rec_user.root")

databuffer_module = JUNODataBufferModule()
rootinput_module = JUNORootInputModule()
rootoutput_module = JUNORootOutputModule()
rootwriter_module = JUNORootWriterModule()

dbisvc_module = JUNODBIModule()
paradb_module = JUNOParaDBModule()
conddb_module = JUNOCondDBModule()
geometry_module = JUNOGeometryModule()
mcparam_module = JUNOMCParameterModule()
pmtparam_module = JUNOPMTParameterModule()
pmtcalib_module = JUNOPMTCalibSvcModule()
jobinfosvc_module = JUNOJobInfoSvcModule()

eventrec_module = JUNOEventRecModule()

juno_application.register(databuffer_module)
juno_application.register(rootinput_module)
juno_application.register(rootoutput_module)
juno_application.register(rootwriter_module)

juno_application.register(dbisvc_module)
juno_application.register(paradb_module)
juno_application.register(conddb_module)
juno_application.register(geometry_module)
juno_application.register(mcparam_module)
juno_application.register(pmtparam_module)
juno_application.register(pmtcalib_module)
juno_application.register(jobinfosvc_module)

juno_application.register(eventrec_module)

juno_application.run()
