#!/usr/bin/env python
#-*- coding: utf-8 -*-
# Author: Tao Lin <lintao AT ihep.ac.cn>
#
# ChangeLog:
#   - 2022-06-11
#     Support multiple tasks and the related configurations. 
#     A module container is a wrapper of sub-tasks:
#     {
#       "name": "SubTaskName"
#       "modules": [module1, module2]
#     }
#

from __future__ import print_function

import argparse
import sys
import os
import logging
log = logging.getLogger(__name__)

from .JUNOUtils import mh
from .JUNOModule import JUNOModuleContainer

##############################################################################
# JUNOApplication
##############################################################################

class JUNOApplication(object):

    DATA_LOG_MAP = {
        "Test":0, "Debug":2, "Info":3, "Warn":4, "Error":5, "Fatal":6
    }

    def __init__(self, description="JUNO Application"):
        self.module_container_default = JUNOModuleContainer("junotoptask")
        self.module_containers = {} # module containers
        self.description = description
        self.default_input = None
        self.default_input_action = None
        self.default_output = "output_edm.root"
        self.default_user_output = "output_user.root"

        self.offline_version = "dev"
        if os.getenv("JUNO_OFFLINE_VERSION"):
            self.offline_version = os.getenv("JUNO_OFFLINE_VERSION")

    def set_default_input(self, default_input):
        self.default_input = default_input
    def set_default_input_action(self, default_input_action):
        self.default_input_action = default_input_action

    def set_default_output(self, default_output):
        self.default_output = default_output
    def set_default_user_output(self, default_user_output):
        self.default_user_output = default_user_output

    def register(self, module, subtaskname=None):
        # if the subtask name is specified, register the module into the 
        # module container
        if subtaskname:
            if subtaskname not in self.module_containers:
                self.module_containers[subtaskname] = JUNOModuleContainer(subtaskname)
            self.module_containers[subtaskname].register(module)

        else:
            self.module_container_default.register(module)
        pass
        log.info("end of register")

    def get_parser(self):
        self.parser = argparse.ArgumentParser(description=self.description,
                                              fromfile_prefix_chars='@')
        self.parser.add_argument("--help-more", action='store_true', help="print more options")
        self.parser.add_argument("--version", action='version', version="JUNO_OFFLINE_VERSION=%s"%self.offline_version)
        self.parser.add_argument("--loglevel", default="Info", 
                                 choices=["Test", "Debug", "Info", "Warn", "Error", "Fatal"],
                                 help="Set the Log Level")
        self.parser.add_argument("--evtmax", type=int, default=10, help='events to be processed')
        self.parser.add_argument("--input", action=self.default_input_action, default=self.default_input, help="input file name")
        self.parser.add_argument("--input-list", default=None, help="input file name with file list")

        self.parser.add_argument("--output", default=self.default_output, help="output file name (Event Data Model)")
        self.parser.add_argument("--user-output", default=self.default_user_output, help="output user data file name")
        self.parser.add_argument("--output-stream", action="append", help="Control on or off of the streams in EDM output. Usage: --output-stream /Event/Sim:on --output-stream /Event/Gen:off")
        self.parser.add_argument("--output-reproducible", 
                                 dest="output_reproducible",
                                 action='store_true',
                                 help='enable output reproducible')
        self.parser.add_argument("--no-output-reproducible", 
                                 dest="output_reproducible",
                                 action='store_false',
                                 help='disable output reproducible (default)')
        self.parser.set_defaults(output_reproducible=False)
        self.parser.add_argument("--profiling", dest="profiling", 
                                 action='store_true', help='enable profiling')
        self.parser.add_argument("--no-profiling", dest="profiling",
                                 action='store_false', help='disable profiling')
        self.parser.set_defaults(profiling=True)
        self.parser.add_argument("--profiling-details", dest="profiling_with_details", 
                                 action='store_true', help='enable profiling with details saved')
        self.parser.add_argument("--no-profiling-details", dest="profiling_with_details", 
                                 action='store_false', help='disable profiling with details saved')
        self.parser.set_defaults(profiling_with_details=False)
       
        self.parser.add_argument("--no-toptask-show", dest="toptask_show", action="store_false", help="disable json dumping")
        self.parser.set_defaults(toptask_show=True)

        # Register other modules
        self.module_container_default.register_options(self.parser)

        # Register the modules in module container
        for module_container in self.module_containers.values():
            module_container.register_options(self.parser)

        return self.parser

    def setup_profiling(self, task):
        if not self.args.profiling:
            return

        import SniperProfiling
        profiling = task.createSvc("SniperProfiling")

        if self.args.profiling_with_details:
            profiling.property("SaveDetails").set(True)

    def run(self):
        log.info("[JUNOApplication.run")
        parser = self.get_parser()
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


        self.module_container_default.add_output_vec(self.args.output_vec, self.args)
        self.module_container_default.add_user_output_vec(self.args.user_output_vec, self.args)

        for module_container in self.module_containers.values():
            module_container.add_output_vec(self.args.output_vec, self.args)
            module_container.add_user_output_vec(self.args.user_output_vec,self.args)

        import Sniper

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

        if self.args.toptask_show:
            log.info("JUNOApplication.run toptask.show")
            self.toptask.show()
        else:
            log.info("JUNOApplication.run --no-toptask-show skipping toptask.show")
        pass
        log.info("JUNOApplication.run toptask.run")
        self.toptask.run()
        log.info("]JUNOApplication.run")

