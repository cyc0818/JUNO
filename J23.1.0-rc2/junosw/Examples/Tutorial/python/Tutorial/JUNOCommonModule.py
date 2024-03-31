#!/usr/bin/env python
#-*- coding: utf-8 -*-
from __future__ import print_function

import sys
import os
import logging
from .JUNOModule import JUNOModule
from .JUNOUtils import mh

##############################################################################
# JUNODataBufferModule
##############################################################################
class JUNODataBufferModule(JUNOModule):

    def __init__(self):
        pass

    def register_options(self, parser):
        pass

    def init(self, toptask, args):
        import BufferMemMgr
        buffermgr = toptask.createSvc("BufferMemMgr")


##############################################################################
# JUNORootWriterModule
##############################################################################
class JUNORootWriterModule(JUNOModule):

    def __init__(self):
        self.output_keys = []
        self.output_keys_disabled = []

        self.rootwriter = None # cache the global shared element
        pass

    # register the keys to be output
    def register_output_key(self, name):
        self.output_keys.append(name)

    def unregister_output_key(self, name):
        self.output_keys_disabled.append(name)

    def register_options(self, parser):
        pass

    def init(self, toptask, args):

        output_keys = []

        for k in args.user_output_vec:
            if k in self.output_keys_disabled:
                continue
            output_keys.append(k)

        for k in self.output_keys:
            if k in self.output_keys_disabled:
                continue
            output_keys.append(k)

        # for reproducible user root file
        user_output = args.user_output
        if args.output_reproducible and '?reproducible' not in user_output:
            user_output += '?reproducible'

        output_map = {k: user_output for k in output_keys}

        import RootWriter
        import Sniper
        if self.rootwriter is None:
            self.rootwriter = Sniper.create("SharedElem<RootWriter>/RootWriter")
            self.rootwriter.property("Output").set(output_map)
        toptask.addSvc(self.rootwriter)

##############################################################################
# JUNORootInputModule
##############################################################################
class JUNORootInputModule(JUNOModule):

    def __init__(self):
        pass

    def register_options(self, parser):
        pass

    def init(self, toptask, args):
        import RootIOSvc
        inputsvc = toptask.createSvc("RootInputSvc/InputSvc")

        # input files could be one or a list of files
        inputs = []
        if args.input_list:
            import sys
            import os.path
            if not os.path.exists(args.input_list):
                sys.exit(-1)
            with open(args.input_list) as f:
                for line in f:
                    line = line.strip()
                    inputs.append(line)
        else:
            inputs.append(args.input)
        inputsvc.property("InputFile").set(inputs)


##############################################################################
# JUNORootOutputModule
##############################################################################
class JUNORootOutputModule(JUNOModule):

    def __init__(self):
        self.output_keys = []
        self.output_keys_disabled = []
        pass

    # register the keys to be output
    def register_output_key(self, name):
        self.output_keys.append(name)

    def unregister_output_key(self, name):
        self.output_keys_disabled.append(name)

    def register_options(self, parser):
        pass

    def init(self, toptask, args):
        import RootIOSvc

        output_keys = []

        for k in args.output_vec:
            if k in self.output_keys_disabled:
                continue
            output_keys.append(k)

        for k in self.output_keys:
            if k in self.output_keys_disabled:
                continue
            output_keys.append(k)

        # Now, update the output_streams according to args.output_stream.
        # if it is on, add it. If it is off, remove it from it.
        if args.output_stream is None:
            args.output_stream = []

        for kv in args.output_stream:
            k, v = kv.split(":")
            if v.lower() in ('on', 'yes', 'true', 't', 'y', '1'):
                output_keys.append(k)
            elif v.lower() in ('off', 'no', 'false', 'f', 'n', '0'):
                if k in output_keys:
                    output_keys.remove(k)
            else:
                print("ERROR: Unknown flag %s for EDM path %s. "%(v, k))
                sys.exit(-1)

        outputdata = {k: args.output for k in output_keys}

        roSvc = toptask.createSvc("RootOutputSvc/OutputSvc")
        roSvc.property("OutputStreams").set(outputdata)
        roSvc.property("IsReproducible").set(args.output_reproducible)

##############################################################################
# JUNOGeometryModule
##############################################################################
class JUNOGeometryModule(JUNOModule):
    DEFAULT_GDML_OUTPUT = {"Acrylic": "default", #"sample_detsim.root",  #"geometry_acrylic.gdml"
                           "Balloon": "default",
                           "TT": "default",
                           }  #"geometry_balloon.gdml"

    def __init__(self):

        self.geom = None # cache the global shared element

        pass

    def register_options(self, parser):
        parser.add_argument("--detoption", default="Acrylic", 
                                           choices=["Acrylic", "Balloon", "TT"],
                                           help="Det Option")
        parser.add_argument("--gdml", default="True", action="store_true", help="Use GDML.")
        parser.add_argument("--gdml-file", help="the file contains the geometry info.")
        pass

    def init(self, toptask, args):
        # load the geom info from root file by default.
        self.gdml_filename = self.DEFAULT_GDML_OUTPUT[args.detoption]
        self.geom_path_inroot = "JunoGeom"
        if args.gdml:
            if args.gdml_file:
                self.gdml_filename = args.gdml_file
        if self.gdml_filename.endswith("gdml"):
            self.geom_path_inroot = ""
        # === check the existance of the geometry file ===
        import os.path
        if self.gdml_filename != "default" and not self.gdml_filename.startswith("root://") and not os.path.exists(self.gdml_filename):
            import sys
            print("can't find the geometry file %s"%self.gdml_filename)
            sys.exit(-1)

        import Geometry
        import Sniper
        if self.geom is None:
            self.geom = Sniper.create("SharedElem<RecGeomSvc>/RecGeomSvc")
            self.geom.property("GeomFile").set(self.gdml_filename)
            self.geom.property("GeomPathInRoot").set(self.geom_path_inroot)
            self.geom.property("FastInit").set(True)
        toptask.addSvc(self.geom)
    
        pass


##############################################################################
# JUNODBIModule
##############################################################################
class JUNODBIModule(JUNOModule):
    def __init__(self):
        self.dbisvc = None

    def register_options(self, parser):

        parser.add_argument("--dbconf", default=None,
                            help="Database Configuration (YAML format)")

    def init(self, toptask, args):
        if not self.dbisvc:
            self.init_dbisvc(toptask, args)

        toptask.addSvc(self.dbisvc)

    def init_dbisvc(self, toptask, args):

        import DBISvc
        import Sniper

        # Handle the dbconf path
        dbconf = args.dbconf
        if dbconf is None:
            # set to DBISVCROOT/share/dbi.yaml
            dbconf = os.environ.get("DBISVCROOT")
            if dbconf is None:
                print("ERROR: dbconf is not set. ")
                sys.exit(-1)
            dbconf = os.path.join(dbconf, "share", "dbi.yaml")

        if not os.path.exists(dbconf):
            print("ERROR: dbconf '%s' does not exist. "%dbconf)
            sys.exit(-1)

        # Load the conf
        def load_config(db_config_file):
            import yaml
            with open(db_config_file) as f:
                return yaml.safe_load(f)

        db_config = load_config(dbconf)

        connections = db_config.get('connections')
        clients = db_config.get('clients')
        schemas = db_config.get('schemas')

        self.dbisvc = Sniper.create("SharedElem<DBISvc>/DBISvc")
        self.dbisvc.property("Connections").set(connections)
        self.dbisvc.property("Clients").set(clients)
        self.dbisvc.property("Schemas").set(schemas)

##############################################################################
# JUNOParaDBModule
##############################################################################
class JUNOParaDBModule(JUNOModule):
    def __init__(self):
        self.paradbsvc = None
        pass

    def register_options(self, parser):
        parser.add_argument("--paradb", dest="paradb", action="store_true", help="Enable ParaDB")
        parser.add_argument("--no-paradb", dest="paradb", action="store_false", help="Disable ParaDB")
        parser.set_defaults(paradb=True) # by default, paradb is enabled

        parser.add_argument("--para-gtag", default="J22.1.0", help="Global Tag for ParaDB")
        pass

    def init(self, toptask, args):
        if not args.paradb:
            print("ParaDB is not enabled.")
            return

        import ParaDB
        import Sniper

        if self.paradbsvc is None:
            self.paradbsvc = Sniper.create("SharedElem<ParaDBSvc>/ParaDBSvc")
            self.paradbsvc.property("RepoNames").set(["DBParaRepo"])
            self.paradbsvc.property("RepoTypes").set(["DBI"])
            self.paradbsvc.property("ParaGTag").set(args.para_gtag)

        toptask.addSvc(self.paradbsvc)



##############################################################################
# JUNOMCParameterModule
##############################################################################
class JUNOMCParameterModule(JUNOModule):
    def __init__(self):

        self.mcparamssvc = None

        pass

    def init(self, toptask, args):
        import Sniper
        Sniper.loadDll("libMCParamsSvc.so")
        if self.mcparamssvc is None:
            self.mcparamssvc = Sniper.create("SharedElem<MCParamsFileSvc>/MCParamsSvc")
        toptask.addSvc(self.mcparamssvc)


##############################################################################
# JUNOPMTParameterModule
##############################################################################
class JUNOPMTParameterModule(JUNOModule):
    def __init__(self):

        self.pmtparamsvc = None
        self.spmtelecconfigsvc = None
        self.pmtsimparamsvc = None
        pass

    def register_options(self, parser):
        parser.add_argument("--ranges-pmt-enabled", default=None, 
                            help="ranges of PMTs whether PMT is enabled or not. For example, 10,50,70,80 will enable [10,50), [70,80)")
        parser.add_argument("--ranges-pmt-constructed", default=None, help="ranges of PMTs whether PMT is constructed or not (control the detsim)")
        parser.add_argument("--pmtsimparam-dbtype", default="File", help="PMTSimParamSvc db type", choices=["File", "MySQL", "Frontier"])
        pass

    def init(self, toptask, args):

        import Geometry
        import Sniper
        if self.pmtparamsvc is None:
            self.pmtparamsvc = Sniper.create("SharedElem<PMTParamSvc>/PMTParamSvc")
        toptask.addSvc(self.pmtparamsvc)

        if args.ranges_pmt_enabled:
            l = args.ranges_pmt_enabled.split(",")
            l = [int(i) for i in l if i.isdigit()]
            self.pmtparamsvc.property("Ranges_PMT_Enabled").set(l)

        if args.ranges_pmt_constructed:
            l = args.ranges_pmt_constructed.split(",")
            l = [int(i) for i in l if i.isdigit()]
            self.pmtparamsvc.property("Ranges_PMT_Constructed").set(l)


        import SpmtElecConfigSvc
        if self.spmtelecconfigsvc is None:
            self.spmtelecconfigsvc = Sniper.create("SharedElem<SpmtElecConfigSvc>/SpmtElecConfigSvc")
        toptask.addSvc(self.spmtelecconfigsvc)

        project_name = os.environ.get("JUNOSW_NAME", "junosw")

        SpmtDataFile = os.path.join(os.environ["JUNOTOP"], project_name, "Examples/TestSpmtElecAlg/share/", "SpmtElecConfig.txt")
        
        if os.environ.get("WORKTOP"):
            SpmtDataFile = os.path.join(os.environ["WORKTOP"], project_name, "Examples/TestSpmtElecAlg/share/", "SpmtElecConfig.txt")
        
        if not os.path.exists(SpmtDataFile):
            print("ERROR: Can't find SpmtElecConfig under %s/Examples/TestSpmtElecAlg/share"%project_name)
            import sys
            sys.exit(-1)
        
    
        self.spmtelecconfigsvc.property("ConfigFile").set(SpmtDataFile)
        self.spmtelecconfigsvc.setLogLevel(4)

        import PMTSimParamSvc
        if self.pmtsimparamsvc is None:
            self.pmtsimparamsvc = Sniper.create("SharedElem<PMTSimParamSvc>/PMTSimParamSvc")
            self.pmtsimparamsvc.property("DBType").set(args.pmtsimparam_dbtype)
        toptask.addSvc(self.pmtsimparamsvc)

##############################################################################
# JUNOPMTCalibSvcModule
##############################################################################
class JUNOPMTCalibSvcModule(JUNOModule):
    def __init__(self):
        
        self.calibsvc = None
        pass

    def register_options(self, parser):
        parser.add_argument("--pmtcalibsvc-ReadDB", default="0", type = int,  help="0:Not read DataBase data, 1:Read")
        parser.add_argument("--pmtcalibsvc-DBcur", default="20220704", type = int,  help="timestamp of DB data")
        parser.add_argument("--CalibFile",  help="Deconv SPE spectra")
        parser.add_argument("--Filter", help="Deconv User's Filters")
        pass

    def init(self, toptask, args):
        import PMTCalibSvc
        import Sniper

        if self.calibsvc is None:
            self.calibsvc = Sniper.create("SharedElem<PMTCalibSvc>/PMTCalibSvc")
            self.calibsvc.property("ReadDB").set(args.pmtcalibsvc_ReadDB)
            self.calibsvc.property("DBcur").set(args.pmtcalibsvc_DBcur)
            if args.CalibFile:
                self.calibsvc.property("CalibFile").set(args.CalibFile)
            if args.Filter:
                self.calibsvc.property("Filter").set(args.Filter)
        toptask.addSvc(self.calibsvc)

##############################################################################
# JUNOCondDBModule
##############################################################################
class JUNOCondDBModule(JUNOModule):
    def __init__(self):
        self.conddbsvc = None
        pass

    def register_options(self, parser):
        parser.add_argument("--conddb", dest="conddb", action="store_true", help="Enable CondDB")
        parser.add_argument("--no-conddb", dest="conddb", action="store_false", help="Disable CondDB")
        parser.set_defaults(conddb=True) # by default, conddb is enabled

        parser.add_argument("--global-tag", default="J22.1.0", help="Global Tag for CondDB")
        pass

    def init(self, toptask, args):
        if not args.conddb:
            print("CondDB is not enabled.")
            return

        import CondDB
        import Sniper

        if self.conddbsvc is None:
            self.conddbsvc = Sniper.create("SharedElem<CondDBSvc>/CondDBSvc")
            self.conddbsvc.property("RepoTypes").set({
                #    "local": "LocalFSRepo",
                # "frontier": "Frontier"
                "dbi": "DBI"
            })
            self.conddbsvc.property("RepoURIs").set({
                #    "local": os.path.join("dummy-repo"),
                # "frontier": "http://junodb1.ihep.ac.cn:8080/Frontier",
                "dbi": "dbi://conddb" # configured by DBISvc
            })
            self.conddbsvc.property("GlobalTag").set(
                args.global_tag
            )

        toptask.addSvc(self.conddbsvc)

##############################################################################
# JUNORandomSvcModule
##############################################################################

class JUNORandomSvcModule(JUNOModule):
    def __init__(self):
        pass

    def register_options(self, parser):
        parser.add_argument("--seed", type=int, default=42, 
                            help="Random Seed (for both CLHEP and ROOT)")

    def init(self, task, args):
        import RandomSvc
        import Sniper
        rndmsvc = Sniper.create("RandomSvc")
        task.addSvc(rndmsvc)
        rndmsvc.property("Seed").set(args.seed)

##############################################################################
# JUNOJobInfoSvcModule
##############################################################################

class JUNOJobInfoSvcModule(JUNOModule):

    def __init__(self):
        pass

    def register_options(self, parser):
        pass

    def init(self, task, args):
        import JobInfoSvc

        jobinfosvc = task.createSvc("JobInfoSvc")

        offline_version = "dev"
        if os.getenv("JUNO_OFFLINE_VERSION"):
            offline_version = os.getenv("JUNO_OFFLINE_VERSION")
        jobinfosvc.property("OfflineVersion").set(offline_version)

        cmdhist = " ".join("'%s'"%a for a in sys.argv)
        jobinfosvc.property("CommandLine").set(cmdhist)

