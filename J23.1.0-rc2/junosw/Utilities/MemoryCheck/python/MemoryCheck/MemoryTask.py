#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

from Sniper import Task
from Sniper import Incident
import os
import commands

import ROOT

class MemoryTask(Task):

    def __init__(self, name):
        Task.__init__(self, name)
        self.output = None
        self.interval = 1000

        self.regist("memory_record")

    def initialize(self):
        print "Initialize..."
        self.execNumber = 0
        self.virt = {}
        self.res = {}
        self.virtMax = 0
        self.virtMin = 999
        self.resMax = 0
        self.resMin = 999

        if not self.output:
            self.output = "memory.root"

        return Task.initialize(self)

    def execute(self):
        st = Task.execute(self)
        Incident("memory_record").fire()
        return st

    def finalize(self):

        self.hist = {}

        self.rootfile = ROOT.TFile(self.output, "recreate")
        self.rootfile.cd()

        # Define histogram for virtual and resident memory
        self.hist["virt"] = ROOT.TH2F("virt", "Virtual Memory",
            self.execNumber + 1, 0., self.execNumber, 
            100, self.virtMin*0.95, self.virtMax*1.05)

        self.hist["res"] = ROOT.TH2F("res", "Resident Memory",
            self.execNumber + 1, 0., self.execNumber, 
            100, self.resMin*0.95, self.resMax*1.05)

        # Fill histogram
        for i in self.virt:
            self.hist["virt"].Fill(float(i),self.virt[i])
        for i in self.res:
            self.hist["res"].Fill(float(i),self.res[i])

        self.rootfile.Write()
        self.rootfile.Close()

        print "Finalizing ", self.objName()
        print "Max resident memory is %s GByte" % self.resMax
        print "Max virtual memory is %s GByte" % self.virtMax

        return Task.finalize(self)

    def handle(self, incident):
        # save the current memory
        if incident.name() == "memory_record":
            self.record_memory()
        return Task.handle(self, incident)

    def record_memory(self):
        print "Record Memeory..."
        # Use system command to record memory use
        #  Have to deal with fact that in linux, this generates a single line,
        # while on mac, the header line is also generated.
        procid = str(os.getpid())
        ps = 'ps uh ' + procid
        blob = commands.getoutput(ps)
        lines = blob.split('\n')
        word = None
        for line in lines:
            if procid in line :
                word = line.split()
                break

        # check that line manipulation was successful
        if word is None:
            if self.execNumber == 0 :
                print "MemoryCheck: ERROR No memory checking applied"
                print "MemoryCheck: ERROR Could not find procid",procid,"in",lines
        else:
            self.virt[self.execNumber] = float(word[4])/(1024.*1024.)
            self.res[self.execNumber] = float(word[5])/(1024.*1024.)

            # Find maximum and minimum memory use
            if self.virtMax < self.virt[self.execNumber]:
                self.virtMax = self.virt[self.execNumber]
            if self.virtMin > self.virt[self.execNumber]:
                self.virtMin = self.virt[self.execNumber]

            if self.resMax < self.res[self.execNumber]:
                self.resMax = self.res[self.execNumber]
            if self.resMin > self.res[self.execNumber]:
                self.resMin = self.res[self.execNumber]

        self.execNumber = self.execNumber + 1

