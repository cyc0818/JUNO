#!/usr/bin/env python
#-*- coding: utf-8 -*-

from Tutorial import JUNOApplication
from Tutorial import JUNODataBufferModule
from Tutorial import JUNORootWriterModule
# from Tutorial import JUNORootInputModule # Note: ElecSim implements its own input
from Tutorial import JUNORootOutputModule

from Tutorial import JUNOGeometryModule
from Tutorial import JUNODBIModule
from Tutorial import JUNOParaDBModule
from Tutorial import JUNOMCParameterModule
from Tutorial import JUNOPMTParameterModule
from Tutorial import JUNOPMTCalibSvcModule
from Tutorial import JUNOCondDBModule
from Tutorial import JUNOJobInfoSvcModule

from Tutorial import JUNOElecSimModule
from Tutorial import JUNOWaveRecModule
from Tutorial import JUNOEventRecModule

##############################################################################
# flag to control:
# * waveform reconstruction (--waverec or --no-waverec)
# * event reconstruction    (--evtrec  or --no-evtrec)
##############################################################################

FLAG_WAVEREC = True
FLAG_EVTREC = True
FLAG_GENEDM = True
FLAG_WAVESAVE = False

import sys
if '--no-waverec' in sys.argv:
    FLAG_WAVEREC = False
    FLAG_EVTREC = False
    sys.argv.remove('--no-waverec') # disable waveform rec.
if '--no-evtrec' in sys.argv:
    FLAG_EVTREC = False
    sys.argv.remove('--no-evtrec')  # disable event rec.
if '--disableGen' in sys.argv:
    FLAG_GENEDM = False
if "--WF_nPEcut" in sys.argv:
    FLAG_WAVESAVE = True

juno_application = JUNOApplication(description="JUNO Electronics Simulation && Waveform Reconstruction && Event Reconstruction")
juno_application.set_default_input_action("append")
juno_application.set_default_output("sample_rec.root")
juno_application.set_default_user_output("sample_rec_user.root")

databuffer_module = JUNODataBufferModule()
# rootinput_module = JUNORootInputModule()
rootoutput_module = JUNORootOutputModule()
rootoutput_module.unregister_output_key("/Event/Elec") # disable the waveform
if not FLAG_WAVESAVE:
    rootoutput_module.unregister_output_key("/Event/CdWaveform") 
    # else controled by CDWaveFormSaving algorithm in JUNOWaveRecModule

rootoutput_module.unregister_output_key("/Event/WpWaveform") # disable the waveform
if not FLAG_GENEDM:
    rootoutput_module.unregister_output_key("/Event/Gen")

rootwriter_module = JUNORootWriterModule()

dbisvc_module = JUNODBIModule()
paradb_module = JUNOParaDBModule()
geometry_module = JUNOGeometryModule()
mcparam_module = JUNOMCParameterModule()
pmtparam_module = JUNOPMTParameterModule()
pmtcalib_module = JUNOPMTCalibSvcModule()
conddb_module = JUNOCondDBModule()
jobinfosvc_module = JUNOJobInfoSvcModule()

elecsim_module = JUNOElecSimModule()
waverec_module = JUNOWaveRecModule()
eventrec_module = JUNOEventRecModule()

juno_application.register(databuffer_module)
# juno_application.register(rootinput_module)
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

juno_application.register(elecsim_module)

if FLAG_WAVEREC:
    juno_application.register(waverec_module)
if FLAG_WAVEREC and FLAG_EVTREC:
    juno_application.register(eventrec_module)

juno_application.run()
