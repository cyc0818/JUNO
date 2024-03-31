#!/usr/bin/env python
#-*- coding: utf-8 -*-
#-*- author: Tao Lin -*-

from Tutorial import JUNOApplication
from Tutorial import JUNOModule

from Tutorial import JUNODataBufferModule
from Tutorial import JUNORootOutputModule
from Tutorial import JUNORandomSvcModule

##############################################################################
# Event Mixing Module
##############################################################################
class JUNOEventMixingModule(JUNOModule):

    def __init__(self):
        pass

    def register_options(self, parser):
        parser.add_argument("--time", default=90, type=float, help="data taking time (s)")
        parser.add_argument("--filter", default=None, action="append", 
                                        choices=["PreMixingFilterNPE"])
        parser.add_argument("--filter-nhits-cdlpmt", default=0, type=int,
                            help="Threshold nhits cdlpmt")
        parser.add_argument("--filter-nhits-cdspmt", default=0, type=int,
                            help="Threshold nhits cdspmt")
        parser.add_argument("--filter-nhits-wp", default=0, type=int,
                            help="Threshold nhits wp")
        parser.add_argument("--filter-nhits-tt", default=0, type=int,
                            help="Threshold nhits tt")

    def add_output_vec(self, output_vec, args):
        # generator and detector simulation
        output_vec.append("/Event/Gen")
        output_vec.append("/Event/Sim")

    def init(self, task, args):

        import Sniper
        Sniper.loadDll("libEvtMixing.so")

        mixalg = task.createAlg("EvtMixingAlg")

        # parse the input
        print(args.input)

        self.rates = {}
        self.inputs = {}
        self.startidxs = {}

        ##############
        # YAML format
        ##############
        if args.input.endswith(".yaml"):
            self.parse_yaml(args.input)

        mixalg.property("Rates").set(self.rates)
        mixalg.property("Inputs").set(self.inputs)
        mixalg.property("StartIdxs").set(self.startidxs)
        mixalg.property("Time").set(args.time)

        if args.filter:
            mixalg.property("Filters").set(args.filter)

            for filtername in args.filter:
                ft = mixalg.createTool(filtername)

                if filtername == "PreMixingFilterNPE":
                    ft.property("ThresholdNhitsCdLpmt").set(args.filter_nhits_cdlpmt)
                    ft.property("ThresholdNhitsCdSpmt").set(args.filter_nhits_cdspmt)
                    ft.property("ThresholdNhitsWp").set(args.filter_nhits_wp)
                    ft.property("ThresholdNhitsTt").set(args.filter_nhits_tt)

    def parse_yaml(self, fn):
        # Format:
        # - label: U238
        #   rate: 3.234
        #   input: @U238-LS.txt
        import yaml
        with open(fn) as f:
            infos = yaml.safe_load(f)
            print(infos)

            for info in infos:
                inputlist = []
                inputitem = info["input"]

                inputlist = self.parse_input_list(inputitem)


                self.rates[info["label"]] = float(info["rate"])
                self.inputs[info["label"]] = inputlist

                if "startidx" in info:
                    self.startidxs[info["label"]] = int(info["startidx"])

    def parse_input_list(self, inputitem):
        inputlist = []

        # the input could be a string or a list
        items = []
        if type(inputitem) is str:
            items.append(inputitem)
        elif type(inputitem) is list:
            items.extend(inputitem)

        for item in items:
            if item.startswith("@"):
                item = inputitem[1:]
                with open(item) as flist:
                    for i in flist:
                        i = i.strip()
                        if len(i):
                            inputlist.append(i)
            else:
                inputlist.append(item)

        return inputlist

##############################################################################
# Application
##############################################################################

juno_application = JUNOApplication(description="JUNO Event Mixing")

juno_application.set_default_output("sample_mixing.root")

databuffer_module = JUNODataBufferModule()
rootoutput_module = JUNORootOutputModule()
randomsvc_module = JUNORandomSvcModule()
eventmixing_module = JUNOEventMixingModule()

juno_application.register(databuffer_module)
juno_application.register(rootoutput_module)
juno_application.register(randomsvc_module)
juno_application.register(eventmixing_module)

juno_application.run()
