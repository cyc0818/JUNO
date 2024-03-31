#!/usr/bin/env python
#-*- coding: utf-8 -*-
# Description:
#   This script will setup a multi-threadinged OEC
#
#   - Input Task: input / electronics simulation
#   - Work Tasks: LEC
#   - Output Task: HEC / output
#
#   Reference:
#   - OEC: OEC/OECTutorial/share/tut_oec.py
#   - MT REC: Utilities/MtUtilities/share/MtRecTimeLikeAlg.py
#
# Author: 
#   Tao Lin <lintao AT ihep.ac.cn>
#

##############################################################################
# Import existing high level modules
##############################################################################

import ROOT
ROOT.EnableThreadSafety()

import Sniper
import SniperMuster
import BufferMemMgr
import RootIOSvc
import RootWriter
import Geometry

Sniper.loadDll("libMtUtilities.so")

import QCtrRecAlg
import OMILREC
import RecTimeLikeAlg
import RecCdMuonAlg
import SpmtMuonRecTool

import OECProcessor

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

##############################################################################
# Implement JUNOMtApplication
##############################################################################

class JUNOMtApplication(JUNOApplication):

    def __init__(self, *args, **kargs):
        super().__init__(*args, **kargs)

    # override the run
    def run(self):
        parser = self.get_parser()
        parser.add_argument("--nthreads", default=1, type=int, help="Number of threads for LEC")

        if '--help-more' in sys.argv:
            parser.print_help()
            sys.exit()
        if '--version' in sys.argv:
            parser.parse_args(['--version'])
            sys.exit()

        self.args = parser.parse_args()
        print(self.args)

        # collect output information
        self.args.output_vec = []
        self.args.user_output_vec = []

        # Note: the output is not in the default module. 
        self.module_containers['GlobalOutput'].add_output_vec(self.args.output_vec, self.args)
        self.module_containers['GlobalOutput'].add_user_output_vec(self.args.user_output_vec, self.args)


        ######################################################################
        # Prepare the multi-threading tasks
        ######################################################################

        def initInput():
            task = Sniper.TopTask("GlobalInput")

            module_container = self.module_containers['input_Task']
            module_container.init(task, self.args)

            # testing only
            # task.createSvc("BufferMemMgr")
            # task.createSvc("RootInputSvc/InputSvc").property("InputFile").set("sample_elecsim.root")

            falg = task.createAlg("FillGlobalBuffer")
            falg.property("ReadAhead").set(["/Event/CdWaveform"])
            return task

        def initOutput():
            task = Sniper.TopTask("GlobalOutput")
            # task.createSvc("BufferMemMgr") # .property("TimeWindow").set([0, 0])
            # task.createSvc("RootOutputSvc/OutputSvc").property("OutputStreams").set({
            #     "/Event/CdWaveform": self.args.output,
            #     "/Event/Oec": self.args.output
            # })

            module_container = self.module_containers['GlobalOutput']
            module_container.init(task, self.args)

            # oecpgba = task.createAlg("OECPruneGBAlg")
            # oecpgba.property("TimeWindow").set([0, 0.01])
            
            ###################################################################
            # Input Task
            ###################################################################
            task_input = task.createTask("Task/HECInputTask")

            bmm = task_input.createSvc("BufferMemMgr")

            task_input.createAlg("PruneGlobalBuffer")

            ###################################################################
            # HEC Task
            ###################################################################
            task_hec = task.createTask("Task/HECTask")
            module_container = self.module_containers['HEC_Task']
            module_container.init(task_hec, self.args)

            ###################################################################
            # Output Task
            ###################################################################
            task_output = task.createTask("Task/HECOutputTask")
            module_container = self.module_containers['output_Task']
            module_container.init(task_output, self.args)


            return task

        def initWorker():
            task = SniperMuster.createWorker("TopTask/junotoptask")
            
            module_container = self.module_containers['LEC_Task']
            module_container.init(task, self.args)

            # testing only
            task.createSvc("BufferMemMgr")

            task.createSvc("MtNavBufferMgr")

            return task

        gs = SniperMuster.createGlobalStream("JunoGlobalStream")
        gs.configBuffer(20, 8)  #the buffer must be created before the I/O tasks
        gs.configInput(initInput)
        gs.configOutput(initOutput)

        muster = SniperMuster.Muster()
        muster.config(initWorker, self.args.nthreads)
        muster.setIO(gs)
        muster.setEvtMax(self.args.evtmax)

        ## show the configuration and run
        SniperMuster.show()
        muster.run()


        return


        # Create all tasks: top task + sub tasks
        self.toptask = Sniper.TopTask("junotoptask")
        self.toptask.setEvtMax(self.args.evtmax)
        self.toptask.setLogLevel(self.DATA_LOG_MAP[self.args.loglevel])
        Sniper.setLogLevel(self.DATA_LOG_MAP[self.args.loglevel])

        for module_container in self.module_containers.values():
            # create task in toptask
            module_container.create_task(self.toptask)

        # Initialize all the modules: default + others
        self.module_container_default.init(self.toptask, self.args)

        for module_container in self.module_containers.values():
            # init all the modules 
            for module in module_container.modules:
                module.init(module_container.task, self.args)

            self.setup_profiling(module_container.task)

        self.setup_profiling(self.toptask)

        self.toptask.show()
        self.toptask.run()

##############################################################################
# Put all together (copied from tut_oec.py)
##############################################################################


juno_oecapp = JUNOMtApplication(description="JUNO Online event classification")
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

# if the input is the result of elecsim, the input action is changed 
# from 'append' to None. 
if not useElecSim:
    juno_oecapp.set_default_input_action(None)

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
juno_oecapp.register(oectag_module, 'GlobalOutput')
juno_oecapp.register(rootwriter_module)
oectop_module = JUNOOECTopModule()
juno_oecapp.register(oectop_module, 'GlobalOutput')

#-----------For input_Task-----------
# juno_oecapp.register(oectag_module, 'input_Task')
oecinput_module = JUNOOECInputModule()
juno_oecapp.register(oecinput_module, 'input_Task')
#Electronic simulation is registered under top task
if useElecSim:
    juno_oecapp.register(rootwriter_module, 'input_Task')
    juno_oecapp.register(dbisvc_module, 'input_Task')
    juno_oecapp.register(paradb_module, 'input_Task')
    juno_oecapp.register(mcparam_module, 'input_Task')
    juno_oecapp.register(pmtparam_module, 'input_Task')
    elecsim_module = JUNOElecSimModule()
    juno_oecapp.register(elecsim_module, 'input_Task')
else:
    rootinput_module = JUNORootInputModule()
    juno_oecapp.register(rootinput_module, 'input_Task')



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


