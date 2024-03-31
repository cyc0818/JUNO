#!/usr/bin/env python
#-*- coding: utf-8 -*-

from Tutorial import JUNOApplication
from Tutorial import JUNODataBufferModule
from Tutorial import JUNORootWriterModule
# from Tutorial import JUNORootInputModule # Note: ElecSim implements its own input
from Tutorial import JUNORootOutputModule

from Tutorial import JUNODBIModule
from Tutorial import JUNOParaDBModule
from Tutorial import JUNOMCParameterModule
from Tutorial import JUNOPMTParameterModule
from Tutorial import JUNOJobInfoSvcModule

from Tutorial import JUNOElecSimModule

FLAG_GENEDM = True
import sys 
if '--disableGen' in sys.argv:
    FLAG_GENEDM = False

juno_application = JUNOApplication(description="JUNO Electronics Simulation")
juno_application.set_default_input_action("append")
juno_application.set_default_output("sample_elecsim.root")
juno_application.set_default_user_output("sample_elecsim_user.root")

databuffer_module = JUNODataBufferModule()
# rootinput_module = JUNORootInputModule()
rootoutput_module = JUNORootOutputModule()
if not FLAG_GENEDM:
    rootoutput_module.unregister_output_key("/Event/Gen")
rootwriter_module = JUNORootWriterModule()

dbisvc_module = JUNODBIModule()
paradb_module = JUNOParaDBModule()
mcparam_module = JUNOMCParameterModule()
pmtparam_module = JUNOPMTParameterModule()
jobinfosvc_module = JUNOJobInfoSvcModule()

elecsim_module = JUNOElecSimModule()

juno_application.register(databuffer_module)
# juno_application.register(rootinput_module)
juno_application.register(rootoutput_module)
juno_application.register(rootwriter_module)
juno_application.register(dbisvc_module)
juno_application.register(paradb_module)
juno_application.register(mcparam_module)
juno_application.register(pmtparam_module)
juno_application.register(jobinfosvc_module)

juno_application.register(elecsim_module)

juno_application.run()
