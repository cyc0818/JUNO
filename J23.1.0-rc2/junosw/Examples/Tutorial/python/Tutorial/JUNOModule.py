#!/usr/bin/env python
#-*- coding: utf-8 -*-

from __future__ import print_function
import argparse
import sys

##############################################################################
# JUNOModule
##############################################################################

class JUNOModule(object):

    def __init__(self):
        pass

    def register_options(self, parser):
        pass

    def add_output_vec(self, output_vec, args):
        pass

    def add_user_output_vec(self, user_output_vec, args):
        pass

    def init(self, toptask, args):
        pass

##############################################################################
# helper class: a module container to manage a list of modules in a sub task.
##############################################################################

class JUNOModuleContainer(object):

    # keep the modules which already invoke register_options
    _list_of_modules_registered_options = []

    def __init__(self, subtaskname):
        self.name = subtaskname
        self.task = None
        self.modules = []

    def register(self, module):
        self.modules.append(module)

    def create_task(self, toptask):
        self.task = toptask.createTask("Task/%s"%self.name)

    def register_options(self, parser):
        for module in self.modules:
            if module.__class__.__name__ in self._list_of_modules_registered_options:
                print("skip register_options as %s already registered"%module.__class__.__name__)
                continue
            module.register_options(parser)
            self._list_of_modules_registered_options.append(module.__class__.__name__)
            # print(self._list_of_modules_registered_options)

    def add_output_vec(self, output_vec, args):
        for module in self.modules:
            module.add_output_vec(output_vec, args)

    def add_user_output_vec(self, user_output_vec, args):
        for module in self.modules:
            module.add_user_output_vec(user_output_vec, args)

    def init(self, task, args):
        for module in self.modules:
            module.init(task, args)
            

