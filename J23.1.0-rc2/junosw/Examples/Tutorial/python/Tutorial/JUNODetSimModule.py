#!/usr/bin/env python
#-*- coding: utf-8 -*-
from __future__ import print_function

import sys
import os
import logging
import textwrap
from .JUNOModule import JUNOModule
from .JUNOUtils import mh
from .JUNOUtils import MakeTVAction

log = logging.getLogger(__name__)


import argparse


def convert_arg_line_to_args(self, arg_line):
    return arg_line.split()

argparse.ArgumentParser.convert_arg_line_to_args = convert_arg_line_to_args

class JUNODetSimModule(JUNOModule):

    DEFAULT_GDML_OUTPUT = {"Acrylic": "geometry_acrylic.gdml"}
    DEFAULT_DAE_OUTPUT = {"Acrylic": "geometry_acrylic.dae"}

    # Note: please register the Physical Volume name. 
    #       You could get the list of PV names when running simulation with volumes mode.
    # Update:
    #       - add the alias mode:
    #           aliasName: (realPhyName, realMaterial)
    #       - the data is maintained under 
    #           Examples/Tutorial/share/mac/GtPosGeoMapping.yaml
    #       - it could be overriden by --config-gen-materials
    DATA_MATERIALS = {}

    DECAY_DATA = {"U-238": 1.5e5, "U238": 1.5e5, # alias
                  "Th-232": 280, "Th232": 280,
                  "K-40": 1e9, "K40": 1e9,
                  "Co-60": 1e9, "Co60": 1e9,
                  } # unit: ns
    
    # Note: 
    #   - the template is moved to
    #     Examples/Tutorial/share/mac/GtGenExecMapping.yaml
    #   - it could be overriden by --config-gen-exec
    GENERATOR_EXEC = {}
    
    def __init__(self):
        self.global_time_enabled = False
        self.gdml_filename = None
        self.dae_filename = None

        self.rootio_taskname = "detsimiotask"

        self.init_yaml_conf()
        log.info("end of __init__")
        pass

    def init_yaml_conf(self):
        import yaml

        # helpers
        def load_config(config_file):
            with open(config_file) as f:
                return yaml.safe_load(f)

        def load_config_from_cmd(option, default_filename):
            data = None

            path_data = os.path.join(
                os.environ.get("TUTORIALROOT"),
                "share/mac",
                default_filename
            )
    
            # override if user specify a new config
            if option in sys.argv:
                i = sys.argv.index(option)
                path_data = sys.argv[i+1]
    
            if os.path.exists(path_data):
                log.debug("Loading data from %s"%path_data)
                data = load_config(path_data)
            else:
                log.fatal("Can't find %s"%path_data)
                sys.exit(-1)

            if data is None:
                log.fatal("Can't load any data. Please check the file %s. "%path_data)
                sys.exit(-1)

            return data


        ######################################################################
        # = GenTool =
        # == materials ==
        self.DATA_MATERIALS = load_config_from_cmd("--config-gen-materials", "GtPosGeoMapping.yaml")
        log.debug("DATA_MATERIALS: %s"%self.DATA_MATERIALS)
        # == generator execs ==
        self.GENERATOR_EXEC = load_config_from_cmd("--config-gen-exec", "GtGenExecMapping.yaml")
        log.debug("GENERATOR_EXEC: %s"%self.GENERATOR_EXEC)
        log.info("end of init_yaml_conf") 

    def register_options(self, parser):

        self.register_options_common(parser)
        self.register_options_splitmode(parser)
        self.register_options_calibunits(parser)
        self.register_options_detector_components(parser)
        self.register_options_material_properties(parser)
        self.register_options_pmt_and_optical(parser)
        self.register_options_physics_list(parser)
        self.register_options_structs_fasteners(parser)
        self.register_options_analysis_manager(parser)
        self.register_options_voxel_method(parser)
        self.register_options_global_time(parser)

        ####################################################
        # sub command
        ####################################################
        base_parser_positioner = self.register_options_positioner()

        subparsers = parser.add_subparsers(help='Please select the generator mode', 
                                           dest='gentool_mode')
        subparsers.required = '--vis' not in sys.argv # if vis is enabled, then don't need run. 
        self.register_subparser_gun(subparsers, base_parser_positioner)
        self.register_subparser_photon(subparsers, base_parser_positioner)
        self.register_subparser_gendecay(subparsers, base_parser_positioner)
        self.register_subparser_hepevt(subparsers, base_parser_positioner)
        self.register_subparser_beam(subparsers, base_parser_positioner)
        self.register_subparser_supernova(subparsers, base_parser_positioner)
        self.register_subparser_solar(subparsers, base_parser_positioner)
        self.register_subparser_atmospheric(subparsers, base_parser_positioner)
        self.register_subparser_gevgen_atmo(subparsers, base_parser_positioner)
        self.register_subparser_spallation_neutron(subparsers, base_parser_positioner)
        self.register_subparser_opticks(subparsers, base_parser_positioner)
        self.register_subparser_nuwro(subparsers, base_parser_positioner)

        pass

    def init(self, toptask, args):
        self.init_common(toptask, args)
        self.init_data_registrition(toptask, args)
        self.init_rootio_buffer(toptask, args)
        self.init_dbsvc(toptask, args)
        self.init_random_service(toptask, args)
        self.init_rootwriter(toptask, args)
        self.init_global_time(toptask, args)
        self.init_juno_timer(toptask, args)
        self.init_mc_parameters(toptask, args)
        self.init_geometry_and_parameters(toptask, args)

        self.init_gentools(toptask, args)

        self.init_op_simulator(toptask, args)

        self.init_detector_simulation(toptask, args)
        log.info("end of init")
        pass

    ##########################################################################
    # Split the arguments into different categories
    ##########################################################################
    def register_options_common(self, parser):
        parser.add_argument("--seed", type=int, default=42, help='seed')
        parser.add_argument("--start-evtid", type=int, default=0, help='start event number.')
        parser.add_argument("--restore-seed-status", default=None, 
                            help=mh('restore the random engine, both '
                                    'a list of integer or a file contains '
                                    'the list of integer is supported. '
                                    'such as:'
                                    '   --restore-seed-status "1,2,3..."'))

        parser.add_argument("--dbtype", default="File", help="PMTSimParamSvc db type", choices=["File", "MySQL","Frontier"])
        parser.add_argument("--paradb", dest="paradb", action="store_true", help="Enable ParaDB")
        parser.add_argument("--no-paradb", dest="paradb", action="store_false", help="Disable ParaDB")
        parser.set_defaults(paradb=False) # by default, paradb is disabled during dev

        parser.add_argument("--para-gtag", default="J22.1.0", help="Global Tag for ParaDB")
        parser.add_argument("--dbconf", default=None,
                            help="Database Configuration (YAML format)")


        parser.add_argument("--config-gen-materials", help="YAML config file of DATA_MATERIALS for GtPositioner")
        parser.add_argument("--config-gen-exec", help="YAML config file of GENERATOR_EXEC for GtHepEvtGenTool")

        parser.add_argument("--mac", default="run.mac", help="mac file")
        parser.add_argument("--vis", default=False, action="store_true", help="start vis.")
        parser.add_argument("--vis-mac", default="vis.mac", help="visualization macro file.")

        parser.add_argument("--detsim", action="store_true", dest="detsim", help="execute detsim algorithm")
        parser.add_argument("--no-detsim", action="store_false", dest="detsim", help="don't execute detsim algorithm. Run gen only.")
        parser.set_defaults(detsim=True)
        
        parser.add_argument("--detoption", default="Acrylic", 
                            choices=["Acrylic"],
                            help=mh("Det Option"))
        parser.add_argument("--qescale", default=1.0, type=float, 
                            help=mh("QE scale for ElecSim."))
        parser.add_argument("--light-yield", default=None,
	                            help=mh("Light Yield, the default value 11522/MeV will get from MCParamSvc."))
        parser.add_argument("--gdml", dest="gdml", action="store_true", help="Save GDML.")
        parser.add_argument("--no-gdml", dest="gdml", action="store_false",
                            help="Don't Save GDML. (Default option since version J20)")
        parser.set_defaults(gdml=False)
        parser.add_argument("--dae", dest="dae", action="store_true", help=mh("Save DAE."))
        parser.add_argument("--no-dae", dest="dae", action="store_false",
                            help=mh("Don't Save DAE."))
        parser.set_defaults(dae=False)


    def register_options_splitmode(self, parser):
        grp_split = parser.add_argument_group(mh("splitmode"), mh("Split mode related"))

        # split event based on max hits(default is 10000)
        grp_split.add_argument("--event-split", dest="splitevent", action="store_true", help=mh("enable split output"))
        grp_split.add_argument("--no-event-split", dest="splitevent", action="store_false", help=mh("disable split output"))
        grp_split.set_defaults(splitevent=False)

        grp_split.add_argument("--event-split-maxhits", type=int, default=None, help=mh("Max hits in sub event."))
        
        # split primary track step by step. For muon simulation
        grp_split.add_argument("--track-split", dest="splittrack", action="store_true", help=mh("enable split track"))
        grp_split.add_argument("--no-track-split", dest="splittrack", action="store_false", help=mh("disable split track"))
        grp_split.set_defaults(splittrack=False)
        grp_split.add_argument("--track-split-mode", default="PrimaryTrack", 
                               choices=["PrimaryTrack",
                                        "EveryTrack",
                                        "Time"
                                    ],
                               help=mh("Choose differet mode for track split."))
        grp_split.add_argument("--track-split-time", default=3000., type=float,
                               help=mh("Time cut for track split mode."))


    def register_options_calibunits(self, parser):
        grp_calib_unit = parser.add_argument_group(mh("calibunits"), mh("Calibration Units."))
        grp_calib_unit.add_argument("--pelletron", action="store_true",
                                    help=mh("enable pelletron in Central Detector."))

        grp_calib_unit.add_argument("--source", action="store_true",
                                    help=mh("enable source enclosure Central Detector."))
        grp_calib_unit.add_argument("--source_weights", action="store_true",
                                    help=mh("enable source enclosure and two weights in Central Detector."))
        grp_calib_unit.add_argument("--source_weight_QC", action="store_true",
                                    help=mh("enable source enclosure, bottom weight, and quick connector in Central Detector."))
        grp_calib_unit.add_argument("--ACU_source_weight_QC", action="store_true",
                                    help=mh("enable ACU source enclosure, bottom weight, and quick connector in Central Detector."))
        grp_calib_unit.add_argument("--CLS_source_weight_QC", action="store_true",
                                    help=mh("enable CLS source enclosure, bottom weight, and quick connector in Central Detector."))
        grp_calib_unit.add_argument("--K40_ACU_source_weight_QC", action="store_true",
                                    help=mh("enable K40 ACU source enclosure, bottom weight, and quick connector in Central Detector."))
        grp_calib_unit.add_argument("--submarine", action="store_true",
                                    help=mh("enable ROV source in Central Detector."))
        grp_calib_unit.add_argument("--OffsetInZ",type=float, default=0,
                                    help=mh("source assembly position in Z direction."))
        grp_calib_unit.add_argument("--OffsetInX",type=float, default=0,
                                    help=mh("source assembly position in X direction."))
        grp_calib_unit.add_argument("--OffsetInY",type=float, default=0,
                                    help=mh("source assembly position in Y direction."))
        grp_calib_unit.add_argument("--GT_source_theta",type=float, default=0,
                                    help=mh("GT source assembly position in theta direction."))
        grp_calib_unit.add_argument("--guide_tube", dest="guide_tube", action="store_true", help=mh("Add Guide Tube Into Detector"))
        grp_calib_unit.add_argument("--no-guide_tube", dest="guide_tube", action="store_false",
                                    help=mh("Don't Add Guide Tube"))
        grp_calib_unit.set_defaults(guide_tube=True)
        grp_calib_unit.add_argument("--dfball_tube", action="store_true", 
                                    help=mh("enable diffuser ball and tube in Central Detector."))
        grp_calib_unit.set_defaults(dfball_tube=False)
	
	
        grp_calib_unit.add_argument("--simplify-calib-anchor", dest="simplify_calib_anchor", action="store_true", help=mh(""))
        grp_calib_unit.add_argument("--no-simplify-calib-anchor", dest="simplify_calib_anchor", action="store_false", help=mh(""))
        grp_calib_unit.set_defaults( simplify_calib_anchor = True )
	
        


    def register_options_detector_components(self, parser):
        grp_det_comp = parser.add_argument_group(mh("detcomp"), mh("Detector Components."))
        # == CD ==
        grp_det_comp.add_argument("--cd", dest="cd_enabled", action="store_true",
                                          help=mh("Enable CD."))
        grp_det_comp.add_argument("--no-cd", dest="cd_enabled", action="store_false",
                                          help=mh("Disable CD."))
        grp_det_comp.set_defaults(cd_enabled=True)

        grp_det_comp.add_argument("--real-surface-in-cd", dest="real_surface_in_cd_enabled", action="store_true",
                                  help=mh("Reduce the tyvek from 20.05 m to 19.629 m"))
        grp_det_comp.add_argument("--ideal-surface-in-cd", dest="real_surface_in_cd_enabled", action="store_false",
                                  help=mh("Keep the tyvek at 20.05 m"))
        grp_det_comp.set_defaults(real_surface_in_cd_enabled=True)
        grp_det_comp.add_argument("--strut-optical-surface", dest="strut_surface_enabled", action="store_true", help=mh("Enable the optical surface for strut"))
        grp_det_comp.add_argument("--no-strut-optical-surface", dest="strut_surface_enabled", action="store_false", help=mh("Disable the optical surface for strut"))
        grp_det_comp.set_defaults(strut_surface_enabled=True)

        # === Mask Tail ===
        parser.add_argument("--real-mask-tail", dest="real_mask_tail", action="store_true", help="Use the real mask tail")
        parser.add_argument("--ideal-mask-tail", dest="real_mask_tail", action="store_false", help="Use the ideal mask tail")
        parser.set_defaults(real_mask_tail=True)
        parser.add_argument("--mask-tail-surface", dest="mask_tail_surface_enabled", action="store_true", help=mh("Enable mask tail optical surface"))
        parser.add_argument("--no-mask-tail-surface", dest="mask_tail_surface_enabled", action="store_false", help=mh("Disable mask tail optical surface"))
        parser.set_defaults(mask_tail_surface_enabled=True)

        # === enable the optical surface ===
        grp_det_comp.add_argument("--optical-surface-in-cd", dest="optical_surface_in_cd_enabled", action="store_true",
                                  help=mh("Enable the optical surface from inner water to tyvek"))
        grp_det_comp.add_argument("--no-optical-surface-in-cd", dest="optical_surface_in_cd_enabled", action="store_false",
                                  help=mh("Disable the optical surface from inner water to tyvek"))
        grp_det_comp.set_defaults(optical_surface_in_cd_enabled=True)

        grp_det_comp.add_argument("--check-overlap-in-cd", dest="check_overlap_in_cd_enabled", action="store_true",
                                   help=mh("Enable overlap checking in CD"))
        grp_det_comp.add_argument("--no-check-overlap-in-cd", dest="check_overlap_in_cd_enabled", action="store_false",
                                   help=mh("Disable overlap checking in CD"))
        grp_det_comp.set_defaults(check_overlap_in_cd_enabled=False)

        # == WP ==
        grp_det_comp.add_argument("--wp", dest="wp_enabled", action="store_true",
                                          help=mh("Enable WP."))
        grp_det_comp.add_argument("--no-wp", dest="wp_enabled", action="store_false",
                                          help=mh("Disable WP."))
        grp_det_comp.set_defaults(wp_enabled=True)
        grp_det_comp.add_argument("--wp-pmt", dest="wp_pmt_enabled", action="store_true",
                                          help=mh("Enable PMTs in WP."))
        grp_det_comp.add_argument("--no-wp-pmt", dest="wp_pmt_enabled", action="store_false",
                                          help=mh("Disable PMTs in WP."))
        grp_det_comp.set_defaults(wp_pmt_enabled=True)
    
        grp_det_comp.add_argument("--wp-latticed", dest="wp_latticed_enabled", action="store_true",
                                          help=mh("Enable Latticed SS shell in WP."))
        grp_det_comp.add_argument("--no-wp-latticed", dest="wp_latticed_enabled", action="store_false",
                                          help=mh("Disable Latticed SS shell in WP."))
        grp_det_comp.set_defaults(wp_latticed_enabled=True)
    
    
        # == TT ==
        grp_det_comp.add_argument("--tt", dest="tt_enabled", action="store_true",
                                          help=mh("Enable TT."))
        grp_det_comp.add_argument("--no-tt", dest="tt_enabled", action="store_false",
                                          help=mh("Disable TT."))
        grp_det_comp.set_defaults(tt_enabled=True)
        # == Chimney ==
        grp_det_comp.add_argument("--shutter", dest="shutter", action="store_true",
                                  help=mh("Enable Shutter"))
        grp_det_comp.add_argument("--no-shutter", dest="shutter", action="store_false",
                                  help=mh("Disable Shutter"))
        grp_det_comp.set_defaults(shutter=False) # disable shutter by default
    
    
        # == Commissioning ==
        grp_det_comp.add_argument("--commissioning", dest="commissioning_enabled", action="store_true",
                                  help=mh("Enable commissioning"))
        grp_det_comp.add_argument("--no-commissioning", dest="commissioning_enabled", action="store_false",
                                  help=mh("Disable commissioning"))
        grp_det_comp.set_defaults(commissioning_enabled=False) # disable commissioning by default
        grp_det_comp.add_argument("--below-z-is-water", type=float, default=0.0,
                                  help=mh("During commissioning, below z is water. If z=0, the surface between LS and water is equator. (unit is mm)"))

    def register_options_material_properties(self, parser):
        grp_mat_prop = parser.add_argument_group(mh("matprop"), mh("Material Properties."))

    def register_options_pmt_and_optical(self, parser):
        grp_pmt_op = parser.add_argument_group(mh("pmtop"), mh("PMT and Optical Progress"))

        grp_pmt_op.add_argument("--pmt20inch", dest="pmt20inch", action="store_true",
                                          help=mh("Enable 20inch PMTs."))
        grp_pmt_op.add_argument("--no-pmt20inch", dest="pmt20inch", action="store_false",
                                          help=mh("Disable 20inch PMTs."))
        grp_pmt_op.set_defaults(pmt20inch=True)
        grp_pmt_op.add_argument("--pmt20inch-name", default="PMTMask",
                                          choices = ["R12860", "OnlyPMT", "20inchPMT",
                                                   "R3600", "PMTMask",
                                                   "HamamatsuMask", "NNVTMask"
                                                   ],
                                          help=mh("20inch PMT name."))

        self.register_SWITCH_options(grp_pmt_op)

        grp_pmt_op.add_argument("--pmt20inch-extra", default="TWO-mask",
                                          choices = ["ONE", "TWO", "TWO-mask"],
                                          help=mh("ONE category or TWO categories of LPMT. TWO: pmts without mask. TWO-mask: pmts with mask"))
        grp_pmt_op.add_argument("--pmtmask-top-thick", default=10., type=float,
                                          help=mh("PMT Mask's thickness at top"))
        grp_pmt_op.add_argument("--pmt3inch", dest="pmt3inch", action="store_true",
                                          help=mh("Enable 3inch PMTs."))
        grp_pmt_op.add_argument("--no-pmt3inch", dest="pmt3inch", action="store_false",
                                          help=mh("Disable 3inch PMTs."))
        grp_pmt_op.set_defaults(pmt3inch=True)
        grp_pmt_op.add_argument("--pmt3inch-name", default="Tub3inchV3",
                                          choices = ["Tub3inch", 
                                                     "Tub3inchV2",
                                                     "Tub3inchV3",
                                                     "Hello3inch"],
                                          help=mh("Enable 3inch PMTs."))
        grp_pmt_op.add_argument("--pmt3inch-offset", type=float, default=-50.0,
                            help=mh("The offset of the 3inch PMT (mm)."))

        # copied from JUNOCommonModule.py
        parser.add_argument("--ranges-pmt-enabled", default=None, 
                            help="ranges of PMTs whether PMT is enabled or not. For example, 10,50,70,80 will enable [10,50), [70,80)")
        parser.add_argument("--ranges-pmt-constructed", default=None, help="ranges of PMTs whether PMT is constructed or not (control the detsim)")

    
        # add new optical model
        
        grp_pmt_op.add_argument("--new-optical-model", dest="new_optical_model", action="store_true",
                      help=mh("Use the new optical model."))
        grp_pmt_op.add_argument("--old-optical-model", dest="new_optical_model", action="store_false",
                      help=mh("Use the old optical model"))
        grp_pmt_op.set_defaults(new_optical_model=False)
       
    
        grp_pmt_op.add_argument("--MaterialDataDir" , default=None, help=mh("directory of Material optical params"))
        
    
    
        grp_pmt_op.add_argument("--pmtsd-v2", dest="pmtsd_v2", action="store_true",
                      help=mh("Use the new PMT SD v2. (without old PMT Optical Model)"))
        grp_pmt_op.add_argument("--no-pmtsd-v2", dest="pmtsd_v2", action="store_false",
                      help=mh("Don't use the new PMT SD v2."))
        grp_pmt_op.set_defaults(pmtsd_v2=True)
        grp_pmt_op.add_argument("--ce-mode", default="20inch",
                                         choices=["None",
                                                  "20inch",
                                                  "20inchflat",
                                                  "20inchfunc"],
                     help=mh("Different CE mode for PMTs. Only available in PMTSD-v2"))
        grp_pmt_op.add_argument("--ce-flat-value", default=0.9, type=float,
                            help=mh("Set the CE using a fixed number when 20inchflat is enabled."))
        grp_pmt_op.add_argument("--ce-func", default=None,
                            help=mh("a TF1 style string to specify CE function"))
        grp_pmt_op.add_argument("--ce-func-par", default=None, 
                            action="append", metavar="par", type=float,
                            help=mh("parameters for CE function. The first one is [0], second is [1]..."))
        grp_pmt_op.add_argument("--pmtsd-merge-twindow", type=float, default=0.0,
                            help=mh("Merge the hits in a PMT within the time window (ns)"))
        grp_pmt_op.add_argument("--optical", dest="useoptical", action="store_true",
                            help=mh("Enable Optical Progress"))
        grp_pmt_op.add_argument("--no-optical", dest="useoptical", action="store_false",
                            help=mh("Disable Optical Progress"))
        grp_pmt_op.set_defaults(useoptical=True)
        grp_pmt_op.add_argument("--cerenkov-only", dest="cerenkov_only", action="store_true",
                            help=mh("Only enable Cerenkov generation. Note: Reemission is also enabled."))
        grp_pmt_op.set_defaults(cerenkov_only=False)
        # == enable/disable cerenkov ==
        # note: cerenkov_only means disable the scintillation.
        grp_pmt_op.add_argument("--cerenkov", dest="cerenkov", action="store_true",
                            help=mh("Enable Cerenkov (default is enable)"))
        grp_pmt_op.add_argument("--no-cerenkov", dest="cerenkov", action="store_false",
                            help=mh("Disable Cerenkov (default is enable)"))
        grp_pmt_op.set_defaults(cerenkov=True)
        grp_pmt_op.add_argument("--cerenkov-type", default="modified", choices=["original", "modified"],
                                help=mh("Select the Cerenkov implementation"))
        # == enable/disable new optical model, absreemit ==
        # note: cerenkov_only means disable the scintillation.
        '''
        grp_pmt_op.add_argument("--absreemit", dest="absreemit", action="store_true",
                            help=mh("Enable Absorption and Reemission by PPO and bis_MSB (default is disabled)"))
        grp_pmt_op.add_argument("--no-absreemit", dest="absreemit", action="store_false",
                            help=mh("Disable Absorption and Reemission by PPO and bis_MSB (default is disabled)"))
        grp_pmt_op.set_defaults(absreemit=False)
        # == use simple scint or not. Default scint also impl re-emission. Simple scint only do scint. ==
        grp_pmt_op.add_argument("--scintsimple", dest="scintsimple", action="store_true",
                            help=mh("Scint process without re-emission in it."))
        grp_pmt_op.add_argument("--no-scintsimple", dest="scintsimple", action="store_false",
                            help=mh("Disable scint process without re-emission in it. Use default one."))
        grp_pmt_op.set_defaults(scintsimple=False)
        '''
    
        # == use new pmt optical model or not ==
        grp_pmt_op.add_argument("--pmt-optical-model", dest="pmt_optical_model", action="store_true", help=mh("Enable New PMT optical model (default is enabled)"))
        grp_pmt_op.add_argument("--no-pmt-optical-model", dest="pmt_optical_model", action="store_false", help=mh("Disable New PMT optical model"))
        grp_pmt_op.set_defaults(pmt_optical_model=True)

        grp_pmt_op.add_argument("--pmt-natural-geometry", dest="pmt_natural_geometry", action="store_true", help=mh("Natural 2-volume PMT geometry (default)"))
        grp_pmt_op.add_argument("--pmt-unnatural-geometry", dest="pmt_natural_geometry", action="store_false", help=mh("Unnatural 4-volume FastSim kludged PMT geometry"))
        grp_pmt_op.set_defaults(pmt_natural_geometry=True)
 
 
        # == track optical photons first or not ==
        grp_pmt_op.add_argument("--track-op-first", dest="track_op_first", action="store_true",
                            help=mh("Track optical photon first."))
        grp_pmt_op.add_argument("--no-track-op-first", dest="track_op_first", action="store_false",
                            help=mh("Do not track optical photon first."))
        grp_pmt_op.set_defaults(track_op_first=True)
    
        # == OP simulator: deferred OP simulation ==
        grp_pmt_op.add_argument("--deferred-op", dest="deferred_op", action="store_true",
                                help=mh("Enable deferred Optical simulation"))
        grp_pmt_op.add_argument("--no-deferred-op", dest="deferred_op", action="store_false",
                                help=mh("Disable deferred Optical simulation"))
        grp_pmt_op.set_defaults(deferred_op=False)
        grp_pmt_op.add_argument("--deferred-op-testing-ratio", type=float, default=1.0,
                                help=mh("The ratio will be used to invoke the OP simulator randomly"))
    
        # == opticks ==
        grp_pmt_op.add_argument("--opticks-mode", type=int, dest="opticks_mode", default=0,
                                help=mh("Control Opticks GPU Optical simulation"))
        grp_pmt_op.add_argument("--opticks-anamgr", action="store_true", dest="opticks_anamgr", default=False,
                                help=mh("Enable G4OpticksAnaMgr for optional use of G4OpticksRecorder saving Geant4 optical propagations into OpticksEvent NumPy arrays."))
    
    
        # == Load optical parameters from service. ==
        grp_pmt_op.add_argument("--replace-param",default=None,help=mh('usage:--replace-param "Material.LS.RAYLEIGH :RAYLEIGH , Material.LS.ConstantProperty.ScintillationYield:1/keV "  ' ))
        # == enable new qe and ce implementation
        grp_pmt_op.add_argument("--pmtsimsvc", dest="usepmtsimsvc", action="store_true",
                             help=mh("Enable PMTSimSvc  (default is enable)"))
        grp_pmt_op.add_argument("--no-pmtsimsvc", dest="usepmtsimsvc", action="store_false",
                             help=mh("Disable PMTSimSvc (default is enable)"))
        grp_pmt_op.set_defaults(usepmtsimsvc=True)

        # == enable/disable quenching ==
        grp_pmt_op.add_argument("--quenching", dest="quenching", action="store_true",
                            help=mh("Enable Quenching (default is enable)"))
        grp_pmt_op.add_argument("--no-quenching", dest="quenching", action="store_false",
                            help=mh("Disable Quenching (default is enable)"))
        grp_pmt_op.set_defaults(quenching=True)
    
        grp_pmt_op.add_argument("--pmt-hit-type", type=int, default=1, choices=[1,2],
                            help=mh("1 for normal hit, 2 for muon"))
        grp_pmt_op.add_argument("--pmt-disable-process", action="store_true",
                                help=mh("disable SD::ProcessHits"))
        grp_pmt_op.set_defaults(pmt_disable_process=False)

        # == Photon Conversion Particle ==
        grp_pmt_op.add_argument("--photon-conversion", dest="photon_conversion", action="store_true",
                               help=mh("Enable photon-conversion"))
        grp_pmt_op.add_argument("--no-photon-conversion", dest="photon_conversion", action="store_false",
                               help=mh("Disable photon-conversion"))
        grp_pmt_op.set_defaults(photon_conversion=False)
        grp_pmt_op.add_argument("--nphotonsperparticle", type=int, default=10, help="total generated photons per one SourcePhoton")
        grp_pmt_op.add_argument("--fixed-energy", type=float, help="If specified, the energy will be fixed.")


    def register_options_physics_list(self, parser):
        grp_physics_list = parser.add_argument_group(mh("physicslist"), mh("physics list"))
        grp_physics_list.add_argument("--physics-list", default="JUNO", help=mh("physics list selector"))
        grp_physics_list.add_argument("--positronium", dest="positronium", action="store_true",
                                      help=mh("Enable positronium (default is enabled.)"))
        grp_physics_list.add_argument("--no-positronium", dest="positronium", action="store_false",
                                      help=mh("Disable positronium"))
        grp_physics_list.set_defaults(positronium=True)
     
        grp_physics_list.add_argument("--ion-php", dest="enableIonPHP", action="store_true",
                                      help=mh("Enable IonPhysicPHP (default is enabled.)"))
        grp_physics_list.add_argument("--no-ion-php", dest="enableIonPHP", action="store_false",
                                      help=mh("Disable IonPhysicPHP"))
        grp_physics_list.set_defaults(enableIonPHP=True)
    
    def register_options_structs_fasteners(self, parser):
        grp_struts_fastener = parser.add_argument_group(mh("strutsfastener"), mh("struts and fastener"))
        grp_struts_fastener.add_argument("--disable-struts-fastens", 
                                default="none",
                                dest="flag_struts_fasteners",
                                choices=["all", "strut", "fastener", "none"],
                                help=mh("disable struts and fasteners"))

    def register_options_analysis_manager(self, parser):
        grp_anamgr = parser.add_argument_group(mh("anamgr"), mh("analysis manager"))
    
        # == radioactivity related ==
        grp_anamgr.add_argument("--anamgr-grdm", action="store_true", dest="anamgr_grdm", help=mh("enable Geant4 Radioactivity Decay Module related anamgr (Default is enabled)"))
        grp_anamgr.add_argument("--no-anamgr-grdm", action="store_false", dest="anamgr_grdm", help=mh("disable Geant4 Radioactivity Decay Module related anamgr (Default is enabled)"))
        grp_anamgr.set_defaults(anamgr_grdm=True)
        grp_anamgr.add_argument("--stop-at-Pa234m", action="store_true", dest="stopAtPa234m", help=mh('During Geant4 Radioactivity Decay simulation, force stop at Pa234m and kill all secondaries.'))
        grp_anamgr.add_argument("--no-stop-at-Pa234m", action="store_false", dest="stopAtPa234m", help=mh('During Geant4 Radioactivity Decay simulation, force stop at Pa234m and kill all secondaries.'))
        grp_anamgr.set_defaults(stopAtPa234m=True)
    
        # == event data model ==
        grp_anamgr.add_argument("--anamgr-edm", action="store_true", dest="anamgr_edm", help=mh("enable event data model writer, including the writer with split"))
        grp_anamgr.add_argument("--no-anamgr-edm", action="store_false", dest="anamgr_edm", help=mh("disable event data model writer, including the writer with split"))
        grp_anamgr.set_defaults(anamgr_edm=False)
        #== split event data model ==#
        
        grp_anamgr.add_argument("--anamgr-edm-v2", action="store_true", dest="anamgr_edm_v2", help=mh("enable event data model writer with split"))
        grp_anamgr.add_argument("--no-anamgr-edm-v2", action="store_false", dest="anamgr_edm_v2", help=mh("disable event data model writer with split"))
        grp_anamgr.set_defaults(anamgr_edm_v2=True)

        # == SimTrack ==
        grp_anamgr.add_argument("--anamgr-simtrack", action="store_true", dest="anamgr_simtrack", help=mh("/Event/Sim/Track is filled"))
        grp_anamgr.add_argument("--no-anamgr-simtrack", action="store_false", dest="anamgr_simtrack", help=mh("/Event/Sim/Track is not filled"))
        grp_anamgr.set_defaults(anamgr_simtrack=True)
        
        # == EDM Gen ==
        grp_anamgr.add_argument("--anamgr-edm-gen", action="store_true", dest="anamgr_edm_gen", help=mh("/Event/Gen is saved"))
        grp_anamgr.add_argument("--no-anamgr-edm-gen", action="store_false", dest="anamgr_edm_gen", help=mh("/Event/Gen is not saved"))
        grp_anamgr.set_defaults(anamgr_edm_gen=True)
       
        # == tt ==
        grp_anamgr.add_argument("--anamgr-tt", action="store_true", dest="anamgr_tt", help=mh("enable TT output"))
        grp_anamgr.add_argument("--no-anamgr-tt", action="store_false", dest="anamgr_tt", help=mh("disable TT output"))
        grp_anamgr.set_defaults(anamgr_tt=False)
    
        # == normal ==
        grp_anamgr.add_argument("--anamgr-normal", action="store_true", dest="anamgr_normal", help=mh("Enable the normal anamgr"))
        grp_anamgr.add_argument("--no-anamgr-normal", action="store_false", dest="anamgr_normal", help=mh("Disable the normal anamgr"))
        grp_anamgr.set_defaults(anamgr_normal=True)
       
        grp_anamgr.add_argument("--anamgr-normal-hit",action="store_true",dest="anamgr_normal_hit",help=mh("Enable the hit level information in normal anamgr"))
        grp_anamgr.add_argument("--no-anamgr-normal-hit",action="store_false",dest="anamgr_normal_hit",help=mh("Disable the hit level information in normal anamgr"))
        grp_anamgr.set_defaults(anamgr_normal_hit=False) 

        # == muons ==
        grp_anamgr.add_argument("--anamgr-muon", action="store_true", dest="anamgr_muon", help=mh("enable Muon output (MuProcess, MuIsoProcess, MuFastnProcess, MuTracking)"))
        grp_anamgr.add_argument("--no-anamgr-muon", action="store_false", dest="anamgr_muon", help=mh("disable Muon output"))
        grp_anamgr.set_defaults(anamgr_muon=False)
      
        # == atmospheric neutrino==
        grp_anamgr.add_argument("--anamgr-atmo", action="store_true", dest="anamgr_atmo", help=mh("enable atmospheric neutrino output"))
        grp_anamgr.add_argument("--no-anamgr-atmo", action="store_false", dest="anamgr_atmo", help=mh("disable atmospheric neutrino output"))
        grp_anamgr.set_defaults(anamgr_atmo=False)
 
    
        # == genevt ==
        grp_anamgr.add_argument("--anamgr-genevt", action="store_true", dest="anamgr_genevt", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-genevt", action="store_false", dest="anamgr_genevt", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_genevt=True)
        # == deposit ==
        grp_anamgr.add_argument("--anamgr-deposit", action="store_true", dest="anamgr_deposit", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-deposit", action="store_false", dest="anamgr_deposit", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_deposit=True)
        grp_anamgr.add_argument("--anamgr-deposit-tt", action="store_true", dest="anamgr_deposit_tt", help=mh("Enable TT output"))
        grp_anamgr.add_argument("--no-anamgr-deposit-tt", action="store_false", dest="anamgr_deposit_tt", help=mh("Disable TT output"))
        grp_anamgr.set_defaults(anamgr_deposit_tt=True)
        # == interesting process ==
        grp_anamgr.add_argument("--anamgr-interesting-process", action="store_true", dest="anamgr_interesting_process", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-interesting-process", action="store_false", dest="anamgr_interesting_process", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_interesting_process=True)

        grp_anamgr.add_argument("--anamgr-extra-procnames", help=mh("Interested Process Names. Separate by ':'."))
        grp_anamgr.add_argument("--anamgr-legacy-michel-electron", action="store_true", help=mh("Select Michel Electron (legacy)"))
        grp_anamgr.add_argument("--anamgr-legacy-ncapture", action="store_true", help=mh("Select neutron capture (legacy)"))
        grp_anamgr.add_argument("--anamgr-legacy-decay", action="store_true", help=mh("Select decay and radioactivity decay (legacy)"))


        # == neutron inelastic process ==
        grp_anamgr.add_argument("--anamgr-neutron-inelastic", action="store_true", dest="anamgr_neutron_inelastic", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-neutron-inelastic", action="store_false", dest="anamgr_neutron_inelastic", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_neutron_inelastic=False)
        # == optical parameter ==
        grp_anamgr.add_argument("--anamgr-optical-parameter", action="store_true", dest="anamgr_optical_parameter", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-optical-parameter", action="store_false", dest="anamgr_optical_parameter", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_optical_parameter=True)
        # == timer ==
        grp_anamgr.add_argument("--anamgr-timer", action="store_true", dest="anamgr_timer", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-timer", action="store_false", dest="anamgr_timer", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_timer=True)
        # == photon tracking ==
        grp_anamgr.add_argument("--anamgr-photon-tracking", action="store_true", dest="anamgr_photon_tracking", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-photon-tracking", action="store_false", dest="anamgr_photon_tracking", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_photon_tracking=False)
        #== print G4 track information of specified event ==#
        grp_anamgr.add_argument("--anamgr-g4tracking-verbose",type=int,help=mh("print G4 tracking information verbose!"))
        grp_anamgr.add_argument("--anamgr-g4tracking-evtlist",default=None,
                                             help=mh('store the event ID list , both '
                                             'a list of integer or a file contains '
                                             'the list of integer is supported. '
                                             'such as:'
                                             '   --evtlist "1,2,3..."'))
        # == extend the anamgr ==
        grp_anamgr.add_argument("--anamgr-list", action="append", metavar="anamgr", default=[],
                help=mh("append anamgr to the anamgr list. You can specify anamgrs multiple times. "
                    "such as: "
                    " \"--anamgr-list anamgr1 --anamgr-list anamgr2\", so that "
                    "both anamgr1 and anamgr2 are added to the list. "
                    ))
        grp_anamgr.add_argument("--anamgr-config-file", 
                help=mh("configure the anamgr from file."))
    

    def register_options_voxel_method(self, parser):
        grp_voxel = parser.add_argument_group(mh("voxel"), mh("Voxel Method Related"))
        grp_voxel.add_argument("--voxel-fast-sim", dest="voxel_fast_sim", action="store_true", help=mh("TBD"))
        grp_voxel.add_argument("--no-voxel-fast-sim", dest="voxel_fast_sim", action="store_false", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_fast_sim=False)
        # == enable/disable merge mode and time window ==
        grp_voxel.add_argument("--voxel-merge-flag", action="store_true", dest="voxel_merge_flag", help=mh("TBD"))
        grp_voxel.add_argument("--voxel-no-merge-flag", action="store_false", dest="voxel_merge_flag", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_merge_flag=False)
        grp_voxel.add_argument("--voxel-merge-twin", default=1, type=float, help=mh("TBD"))
        # == debug mode: fill ntuple ==
        grp_voxel.add_argument("--voxel-fill-ntuple", action="store_true", dest="voxel_fill_ntuple", help=mh("TBD"))
        grp_voxel.add_argument("--voxel-no-fill-ntuple", action="store_false", dest="voxel_fill_ntuple", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_fill_ntuple=False)
        grp_voxel.add_argument("--voxel-fast-dir", help=mh("Stored data for fast simulation."))
        grp_voxel.add_argument("--voxel-geom-file", 
                               default="geom-geom-20pmt.root",
                               help=mh("File name for geom (don't include the directory)."))
        grp_voxel.add_argument("--voxel-npe-file", 
                               default="npehist3d_single.root",
                               help=mh("File name for nPE hist (don't include the directory)."))
        grp_voxel.add_argument("--voxel-time-file", 
                               default="dist_tres_single.root",
                               help=mh("File name for hit time hist (don't include the directory)."))
        # === gen npe ===
        grp_voxel.add_argument("--voxel-gen-npe-on", action="store_true", dest="voxel_gen_npe", help=mh("TBD"))
        grp_voxel.add_argument("--voxel-gen-npe-off", action="store_false", dest="voxel_gen_npe", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_gen_npe=True)
        # === gen time ===
        grp_voxel.add_argument("--voxel-gen-time-on", action="store_true", dest="voxel_gen_time", help=mh("TBD"))
        grp_voxel.add_argument("--voxel-gen-time-off", action="store_false", dest="voxel_gen_time", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_gen_time=True)
        # === save hits ===
        grp_voxel.add_argument("--voxel-save-hits-on", action="store_true", dest="voxel_save_hits", help=mh("TBD"))
        grp_voxel.add_argument("--voxel-save-hits-off", action="store_false", dest="voxel_save_hits", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_save_hits=True)
        # == no PMTs and Structs ==
        grp_voxel.add_argument("--voxel-pmts-structs", action="store_true", dest="voxel_pmts_structs", help=mh("TBD"))
        grp_voxel.add_argument("--voxel-no-pmts-structs", action="store_false", dest="voxel_pmts_structs", help=mh("TBD"))
        grp_voxel.set_defaults(voxel_pmts_structs=True)
        grp_voxel.add_argument("--voxel-quenching-scale", type=float, default=0.93,
                               help=mh("Quenching factor, Qedep->edep. gamma 0.93, e- 0.98"))
    

    def register_options_global_time(self, parser):
        grp_globaltime = parser.add_argument_group(mh("globaltime"), mh("Global time related"))
        grp_globaltime.add_argument("--global-time-begin", 
                                    default="1970-01-01 00:00:01",
                                    help=mh("Global time begin"))
        grp_globaltime.add_argument("--global-time-end",
                                    default="2038-01-19 03:14:07",
                                    help=mh("Global time end"))
        grp_globaltime.add_argument("--global-event-rate",
                                    default=0.0, type=float,
                                    help=mh("Event rate. if greater than 0, global time mode is enabled."))

    def register_options_positioner(self):
        base_parser_positioner = argparse.ArgumentParser('positioner', add_help=False)
        base_parser_positioner.add_argument("--material", default="None", help="material")
        base_parser_positioner.add_argument("--volume", default="None", 
                                         choices=list(self.DATA_MATERIALS.keys()),
                                         help="Volume name")
        base_parser_positioner.add_argument("--volume-radius-min", default=0.0, type=float,
                                         help="min of the radius")
        base_parser_positioner.add_argument("--volume-radius-max", default=0.0, type=float,
                                         help="max of the radius")
        # z cut
        base_parser_positioner.add_argument("--volume-z-min", default=None, type=float,
                                         required='--volume-z-max' in sys.argv,
                                         help="min Z")
        base_parser_positioner.add_argument("--volume-z-max", default=None, type=float,
                                         required='--volume-z-min' in sys.argv,
                                         help="max Z")
        # x cut
        base_parser_positioner.add_argument("--volume-x-min", default=None, type=float,
                                         required='--volume-x-max' in sys.argv,
                                         help="min X")
        base_parser_positioner.add_argument("--volume-x-max", default=None, type=float,
                                         required='--volume-x-min' in sys.argv,
                                         help="max X")
        # y cut
        base_parser_positioner.add_argument("--volume-y-min", default=None, type=float,
                                         required='--volume-y-max' in sys.argv,
                                         help="min Y")
        base_parser_positioner.add_argument("--volume-y-max", default=None, type=float,
                                         required='--volume-y-min' in sys.argv,
                                         help="max Y")
        # rock depth cut (only works with pTopRock)
        base_parser_positioner.add_argument("--volume-rock-depth", default=None, type=float,
                                         help="rock depth")

        base_parser_positioner.add_argument("--global-position", default=None,
                                         nargs='+', type=float, action=MakeTVAction,
                                         help="Global Postion. It will omit the volume and material")
        return base_parser_positioner

    def register_subparser_gun(self, subparsers, base_parser_positioner):
        parser_gun = subparsers.add_parser("gun", help="gun mode", parents=[base_parser_positioner])
        parser_gun.add_argument("--particles",default="gamma", nargs='+',
                                help="Particles to do the simulation.")
        parser_gun.add_argument("--momentums",default=1.0, nargs='+',
                                type=float, 
                                help="Momentums(MeV) p1 p2 ....")
        parser_gun.add_argument("--momentums-mode", default="Fix",
                                    choices=["Fix", "Uniform", "Range", "Gaus"],
                                    help="different momentum modes")
        parser_gun.add_argument("--momentums-extra-params", nargs='+',
                                type=float, 
                                help="Extra Momentums Parameters(MeV) p1 p2 .... when mode is different, meaning is different."
                                     " Uniform: [mom-param, mom+param];"
                                     " Range: [mom, param];"
                                     " Gaus: Gaus(mom, param);"
                                )
        parser_gun.add_argument("--momentums-interp", default="Momentum",
                                    choices=["Momentum", "KineticEnergy", "TotalEnergy"],
                                    help="Interpret momentum.")
        parser_gun.add_argument("--positions",default=[(0,0,0)], nargs='+',
                                type=float, action=MakeTVAction,
                                help="Positions (mm) x1 y1 z1 x2 y2 z2 ....")
        parser_gun.add_argument("--times",default=None, nargs='+',
                                type=float, 
                                help="Time (mm) t1 t2 ....")
        parser_gun.add_argument("--directions",default=None, nargs='+',
                                type=float, action=MakeTVAction,
                                help="If you don't set, the directions are randoms. "
                                     "Directions dx1 dy1 dz1 dx2 dy2 dz2 ....")
    
    def register_subparser_nuwro(self, subparsers, base_parser_positioner):
        parser_nuwro = subparsers.add_parser("nuwro", help="nuwro generator", parents=[base_parser_positioner])
        parser_nuwro.add_argument("--input", help="nuwro input file")


    def register_subparser_opticks(self, subparsers, base_parser_positioner):
        parser_opticks = subparsers.add_parser("opticks", help="opticks debugging input photon  generator", parents=[base_parser_positioner])

    
    def register_subparser_photon(self, subparsers, base_parser_positioner):
        parser_photon = subparsers.add_parser("photon", help="optical photon mode", parents=[base_parser_positioner])
        parser_photon.add_argument("--totalphotons", type=int, default=11522, help="total generated photons")
        parser_photon.add_argument("--cos-theta-lower", type=float, default=-1.)
        parser_photon.add_argument("--cos-theta-upper", type=float, default=+1.)
        parser_photon.add_argument("--fixed-energy", type=float, help="If specified, the energy will be fixed.")

    def register_subparser_gendecay(self, subparsers, base_parser_positioner):
        parser_gendecay = subparsers.add_parser("gendecay", help="GenDecay mode", parents=[base_parser_positioner])
        parser_gendecay.add_argument("--nuclear", default="U-238", help="mother nuclide name")
        parser_gendecay.add_argument("--stop-nuclear", default="", help="Stop decay when reach the nuclide.")
        parser_gendecay.add_argument("-t", "--correlation-time", default=None, type=float, help="correlation time (ns).")
        parser_gendecay.add_argument("-d", "--decay-depth", default=-1, type=int, help="decay depth")

    def register_subparser_hepevt(self, subparsers, base_parser_positioner):
        parser_hepevt = subparsers.add_parser("hepevt", help="HepEvt mode", parents=[base_parser_positioner])
        parser_hepevt.add_argument("--exe", default="IBD", 
                                   choices=list(self.GENERATOR_EXEC.keys()),
                                   help="select the Generator to run")
        parser_hepevt.add_argument("--file", default=None,
                                   help="specify the HepEvt filename.")

    def register_subparser_beam(self, subparsers, base_parser_positioner):
        parser_beam = subparsers.add_parser("beam", help="Pelletron Beam mode")
        parser_beam.add_argument("--particle", default="e+", help="Particle Name")
        # === position of plane===
        parser_beam.add_argument("--plane-r", default=10., type=float,
                                              help="Plane Radius (mm)")
        parser_beam.add_argument("--plane-x", default=0, type=float,
                                              help="Plane position X (mm)")
        parser_beam.add_argument("--plane-y", default=0, type=float,
                                              help="Plane position Y (mm)")
        parser_beam.add_argument("--plane-z", default=1e3, type=float,
                                              help="Plane position Z (mm)")
        # === direction of plane ===
        parser_beam.add_argument("--plane-dirx", default=0, type=float,
                                              help="Plane direction X (global coord)")
        parser_beam.add_argument("--plane-diry", default=0, type=float,
                                              help="Plane direction Y (global coord)")
        parser_beam.add_argument("--plane-dirz", default=-1, type=float,
                                              help="Plane direction Z (global coord)")
        # === beam momentum ===
        parser_beam.add_argument("--momentum", default=1., type=float,
                                              help="Momentum (MeV)")
        parser_beam.add_argument("--momentum-spread", default=1.e-2, type=float,
                                              help="Momentum Spread (MeV)")
        parser_beam.add_argument("--divergence", default=0.10, type=float,
                                              help="Beam divergence (deg)")
    
    def register_subparser_supernova(self, subparsers, base_parser_positioner):
        parser_sn = subparsers.add_parser("sn", help="supernova mode", parents=[base_parser_positioner])
        parser_sn.add_argument("--input", help="supernova input file")
        parser_sn.add_argument("--index", type=int, default=0, help='supernova start index')
        parser_sn.add_argument("--relative-hittime", help="Use relative hit time in each event, while save the event time in event navigator. (default)",
                               dest="relative_hittime", action="store_true")
        parser_sn.add_argument("--absolute-hittime", help="Use absolute hit time in each event",
                               dest="relative_hittime", action="store_false")
        parser_sn.set_defaults(relative_hittime=True)
    
    def register_subparser_solar(self, subparsers, base_parser_positioner):
        parser_nusol = subparsers.add_parser("nusol", help="solar neutrino mode", parents=[base_parser_positioner])
        parser_nusol.add_argument("--type", default="B8",help="neutrino type", 
                                  choices = ["pp", "Be7", "B8", "N13", "O15", "F17", "pep", "hep"]) 


    def register_subparser_atmospheric(self, subparsers, base_parser_positioner):
        parser_atmo = subparsers.add_parser("atmo", help="atmospheric mode", parents=[base_parser_positioner])
        parser_atmo.add_argument("--input", help="atmospheric input file, you can find the default data: $JUNOTOP/data/Generator/NuAtm/data/tree_100000100.root")
        parser_atmo.add_argument("--index", type=int, default=0, help='atmospheric start index')
        parser_atmo.add_argument("--gst-xyz", action="store_true", dest="gst_xyz", help="Use the vertex x/y/z from gst file")
        parser_atmo.add_argument("--no-gst-xyz", action="store_false", dest="gst_xyz", help="Don't use the vertex x/y/z from gst file (Default)")
        parser_atmo.set_defaults(gst_xyz=False)
        parser_atmo.add_argument("--gst-t", action="store_true", dest="gst_t", help="Use the vertex t from gst file")
        parser_atmo.add_argument("--no-gst-t", action="store_false", dest="gst_t", help="Don't use the vertex t from gst file (Default)")
        parser_atmo.set_defaults(gst_t=False)
        parser_atmo.add_argument("--scale-gst-xyz", type=float, default=10., help="Scale the XYZ in gst")
        parser_atmo.add_argument("--scale-gst-t", type=float, default=1., help="Scale the T in gst")

    def register_subparser_gevgen_atmo(self, subparsers, base_parser_positioner):
        # Note: this sub command will invoke the GENIE, which is similar to gevgen_atmo
        parser_gevgen_atmo = subparsers.add_parser("gevgen_atmo", help="GENIE atmospheric mode", parents=[base_parser_positioner])
        parser_gevgen_atmo.add_argument("--tar-tag", type=int, default=0, choices=[0,1,2], help="0:LS; 1:Water; 2:Rock")
        parser_gevgen_atmo.add_argument("--ev-min", type=float, default=0.1, help="minimal initial neutrino energy, unit: GeV")
        parser_gevgen_atmo.add_argument("--ev-max", type=float, default=20, help="maximal initial neutrino energy, unit: GeV")
    
        parser_gevgen_atmo.add_argument("-t", "--targets", default="", help="pdg1[weight1],pdg2[weight2],...")
        ###flux input format###, 3D honda flux e.g.,
        # --flux HAKKM:.../juno-ally-20-12-solmax.d[12],.../juno-ally-20-12-solmax.d[14],.../juno-ally-20-12-solmax.d[-12],.../juno-ally-20-12-solmax.d[-14]
        ###flux input format###, 1D honda flux e.g.,
        # --flux /junofs/users/chengjie/workdir/atmgen-202203/data/flux/hondaflux/hist.root,14[fluxgnumumax],-14[fluxgnumubarmax],12[fluxgnuemax],-12[fluxgnuebarmax]
        ###If you wish to use you own flux, please prepare it in the same way as the samples above.
    
        parser_gevgen_atmo.add_argument("-f", "--flux", default="",  help="flux")
        parser_gevgen_atmo.add_argument("--cross-sections", default="", help="cross section")
        parser_gevgen_atmo.add_argument("--tune", default="", help="genie tune.")
        parser_gevgen_atmo.add_argument("--deex", action="store_true", dest="deex", help="Turn on the deexcitation process")
        parser_gevgen_atmo.add_argument("--no-deex", action="store_false", dest="deex", help="Turn off the deexcitation process (Default)")
        parser_gevgen_atmo.set_defaults(deex=False)
        parser_gevgen_atmo.add_argument("--isotropic", action="store_true", dest="isotropic", help="If 1D hist flux is used, neutrino direction is isotropic")
        parser_gevgen_atmo.add_argument("--no-isotropic", action="store_false", dest="isotropic", help="If 1D hist flux is used, neutrino direction is (0,0,z) (Default)")
        parser_gevgen_atmo.set_defaults(isotropic=False)
        parser_gevgen_atmo.add_argument("--user-flux-frac", default="", help="if --enable-user-flux is chosen, specify the fractions of input 1D hist neutrino fluxes (please follow the order of neutrino flavors in --flux ), e.g., --user-flux-frac weight1,weight2,...")
        parser_gevgen_atmo.add_argument("--event-generator-list", default="Default", help="Specifies the list of event generators to use in the MC job. e.g. --event-generator-list CC(NC or other choices, see $GENIE/config/EventGeneratorListAssembler.xml for more details)")
    
        # pre-select FC events
        parser_gevgen_atmo.add_argument("--preselect", action="store_true", dest="preselect", help="Pre-select the FC events")
        parser_gevgen_atmo.add_argument("--no-preselect", action="store_false", dest="preselect", help="Not pre-select the FC events (Default)")
        parser_gevgen_atmo.set_defaults(preselect=False)
        
        return parser_gevgen_atmo

    def register_subparser_spallation_neutron(self, subparsers, base_parser_positioner):
        parser_neutron = subparsers.add_parser("neutron", help="spallation neutron mode")
        parser_neutron.add_argument("--input", help="spallation neutron input file")
        parser_neutron.add_argument("--index", type=int, default=0, help='start index')
        parser_neutron.add_argument("--energy", type=float, default=None, help='Ek (MeV)')


    ##########################################################################
    # init
    ##########################################################################
    def init_common(self, toptask, args):
        if args.gdml:
            self.gdml_filename = self.DEFAULT_GDML_OUTPUT[args.detoption]
        if args.dae:
            self.dae_filename = self.DEFAULT_DAE_OUTPUT[args.detoption]
    
        # initial seed check
        if args.seed == 0:
            args.seed = 0x19900418 # MAGIC NUMBER
        elif args.seed < 0:
            args.seed = -args.seed
    
    def init_data_registrition(self, task, args):
        pass

    def init_rootio_buffer(self, task, args):
        # if split output, we need to create another iotask for output
        iotask = task
        if args.anamgr_edm_v2 and not args.anamgr_edm:
            iotask = task.createTask("Task/%s"%self.rootio_taskname)

            #dr.property("EventToPath").set({"JM::SimEvent": "/Event/Sim"})
            import BufferMemMgr
            bufMgr = iotask.createSvc("BufferMemMgr")

        ################################################
        # TODO: will move to the common part
        ################################################
        print("== ROOT IO Svc ==")
        import RootIOSvc
        ro = iotask.createSvc("RootOutputSvc/OutputSvc")
        output_streams = {}
        if args.anamgr_edm or args.anamgr_edm_v2:
            output_streams["/Event/Sim"] = args.output
            if args.anamgr_edm_gen:
                output_streams["/Event/Gen"] = args.output

        # Now, update the output_streams according to args.output_stream.
        # if it is on, add it. If it is off, remove it from it.
        if args.output_stream is None:
            args.output_stream = []

        for kv in args.output_stream:
            k, v = kv.split(":")
            if v.lower() in ('on', 'yes', 'true', 't', 'y', '1'):
                output_streams[k] = args.output
            elif v.lower() in ('off', 'no', 'false', 'f', 'n', '0'):
                if k in output_streams:
                    output_streams.pop(k)
            else:
                print("ERROR: Unknown flag %s for EDM path %s. "%(v, k))
                sys.exit(-1)

        ro.property("OutputStreams").set(output_streams)
        ro.property("IsReproducible").set(args.output_reproducible)

        # = Job Info Svc (copying from JUNOCommonModule) =
        import JobInfoSvc

        jobinfosvc = iotask.createSvc("JobInfoSvc")

        offline_version = "dev"
        if os.getenv("JUNO_OFFLINE_VERSION"):
            offline_version = os.getenv("JUNO_OFFLINE_VERSION")
        jobinfosvc.property("OfflineVersion").set(offline_version)

        cmdhist = " ".join("'%s'"%a for a in sys.argv)
        jobinfosvc.property("CommandLine").set(cmdhist)
    
        # = Data Buffer =
        print("== Buffer Memory Management ==")
        import BufferMemMgr
        bufMgr = task.createSvc("BufferMemMgr")


    def init_dbsvc(self, task, args):
        if (args.dbtype == "Frontier"):
            import FrontierSvc
            frontiersvc = task.createSvc("FrontierSvc")
            frontiersvc.property("ServerUrls").set(["http://junodb1.ihep.ac.cn:8080/Frontier"])
            frontiersvc.property("ProxyUrls").set(["http://squid-01.ihep.ac.cn:3128"])
    
        if args.paradb:
            import DBISvc
            import ParaDB
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
            
            dbisvc = task.createSvc("DBISvc")
            dbisvc.property("Connections").set(connections)
            dbisvc.property("Clients").set(clients)
            dbisvc.property("Schemas").set(schemas)

            paradbsvc = task.createSvc("ParaDBSvc")
            paradbsvc.property("RepoNames").set(["DBParaRepo"])
            paradbsvc.property("RepoTypes").set(["DBI"])
            paradbsvc.property("ParaGTag").set(args.para_gtag)

    def init_random_service(self, task, args):
        print("== Random Svc ==")
        import RandomSvc
        task.property("svcs").append("RandomSvc")
        rndm = task.find("RandomSvc")
        rndm.property("Seed").set(args.seed)
        if args.restore_seed_status:
            # == maybe this is a file? ==
            import os.path
            if os.path.exists(args.restore_seed_status):
                filename = args.restore_seed_status
                with open(filename) as f:
                    for line in f:
                        print(line)
                        l = line.strip()
                        break
            else:
                l = args.restore_seed_status
            import re
            l = re.split(',\s*|\s+', l)
            seedstatus = [int(i) for i in l if i.isdigit()]
            print("loaded seed status: ", seedstatus)
            rndm.property("SeedStatusInputVector").set(seedstatus)

    def init_rootwriter(self, task, args):
        print("== Root Writer ==")
        import RootWriter
        rootwriter = task.createSvc("RootWriter")
    
        user_output = args.user_output
        if args.output_reproducible and '?reproducible' not in user_output:
            user_output += '?reproducible'
        rootwriter.property("Output").set({"SIMEVT":user_output})

    def init_global_time(self, task, args):
        self.global_time_enabled = args.global_event_rate > 0.
        if self.global_time_enabled:
            import MCGlobalTimeSvc
            globaltime = task.createSvc("MCGlobalTimeSvc")
            globaltime.property("BeginTime").set(args.global_time_begin)
            globaltime.property("EndTime").set(args.global_time_end)
            globaltime.property("EventRate").set(args.global_event_rate) # Hz

    def init_juno_timer(self, task, args):
        try:
            import JunoTimer
            task.createSvc("JunoTimerSvc")
        except:
            pass

    def init_geometry_and_parameters(self, task, args):
        import Geometry
        pmt_param_svc = task.createSvc("PMTParamSvc")

        if args.ranges_pmt_enabled:
            l = args.ranges_pmt_enabled.split(",")
            l = [int(i) for i in l if i.isdigit()]
            pmt_param_svc.property("Ranges_PMT_Enabled").set(l)

        if args.ranges_pmt_constructed:
            l = args.ranges_pmt_constructed.split(",")
            l = [int(i) for i in l if i.isdigit()]
            pmt_param_svc.property("Ranges_PMT_Constructed").set(l)


        tt_geom_svc = task.createSvc("TTGeomSvc")

        import PMTSimParamSvc
        print(" == PMTSimParamSvc == ")
        pmt_sim_param_svc = task.createSvc("PMTSimParamSvc")
        pmt_sim_param_svc.property("DBType").set(args.dbtype)

    def init_gentools(self, task, args):
        # = generator related =
        print("GENTOOL MODE: ", args.gentool_mode)
        if args.gentool_mode == "gun":
            self.setup_generator(task, args)
        elif args.gentool_mode == "photon":
            # using optical photon
            self.setup_generator_photon(task, args)
            # disable several anamgrs
            args.anamgr_genevt = False
            args.anamgr_deposit = False
        elif args.gentool_mode == "gendecay":
            self.setup_generator_gendecay(task, args)
        elif args.gentool_mode == "hepevt":
            self.setup_generator_hepevt(task, args)
        elif args.gentool_mode == "beam":
            self.setup_generator_beam(task, args)
        elif args.gentool_mode == "sn":
            self.setup_generator_sn(task, args)
        elif args.gentool_mode == "nusol":
            self.setup_generator_nusol(task, args)
        elif args.gentool_mode == "atmo":
            self.setup_generator_atmo(task, args)
        elif args.gentool_mode == "gevgen_atmo":
            self.setup_generator_gevgen_atmo(task, args)
        elif args.gentool_mode == "neutron":
            self.setup_generator_neutron(task, args)
        elif args.gentool_mode == "opticks":
            self.setup_generator_opticks(task, args)
        elif args.gentool_mode == "nuwro":
            self.setup_generator_nuwro(task, args)
        pass

        gt = task.find("GenTools")
        if gt:
            gt.property("EnableGlobalTime").set(self.global_time_enabled)
            #gt.setLogLevel(2)
            # = setup start index =
            #   Note: even though we could set event index in DetSimAlg, it would be
            #         more consistent when EvtNavigator is created with event id.
            gt.property("EvtID").set(args.start_evtid)
    
        # If detsim is disabled, print the hepmc info
        if gt and not args.detsim:
            # Dump the GenTool
            dumper = gt.createTool("GtHepMCDumper")
            gt.property("GenToolNames").append(dumper.objName())

    ##########################################################################
    # Handle the gentools
    ##########################################################################
    def helper_deex(self,args,gt):
        import GenTools
        deex = gt.createTool("GtGenDeexTool")
        JUNOTOPPath = os.getenv('JUNOTOP')
        deex.property("TalysDir").set(JUNOTOPPath+"/data/Generator/AtmNC/data/talys_sample")
        gt.property("GenToolNames").append([deex.objName()])

    def helper_positioner(self, args, gt, default="pTarget"):
        # if detsim is disabled, the geomemtry will be missing. 
        # so we need to disable positioner tool.
        if not args.detsim:
            log.info("As detsim is disabled, the positioner is also disabled.")
            return

        if args.volume == "None" and default is None:
            return
        elif args.volume == "None":
            args.volume = default
    
        print("SETUP POSITIONER")
    
        # = enable the gen in volume mode =
        # == positioner related ==
        gun_pos = gt.createTool("GtPositionerTool")
    
        # check if it is an alias
        volume = args.volume
        volume_mappings = None
        default_material = args.material
    
        if isinstance(self.DATA_MATERIALS[args.volume], tuple):
            volume = self.DATA_MATERIALS[args.volume][0] # get the real phyName
            default_material = self.DATA_MATERIALS[args.volume][1]
        elif isinstance(self.DATA_MATERIALS[args.volume], str):
            default_material = self.DATA_MATERIALS[args.volume]
        elif isinstance(self.DATA_MATERIALS[args.volume], dict):
            materials = set(v for v in self.DATA_MATERIALS[args.volume].values())
            if len(materials) != 1:
                log.fatal("Mismatched default materials for volume %s in DATA_MATERIALS "%args.volume)
                log.fatal(self.DATA_MATERIALS[args.volume])
                sys.exit(-1)

            default_material = materials.pop()

            # This is used to remap the user friendly volume names
            volume_mappings = [k for k in self.DATA_MATERIALS[args.volume]]

            # keep the user-friendly name if there are more than 1 volumes
            # otherwise, replace the volume name
            if len(volume_mappings) == 1:
                volume = volume_mappings[0]
                volume_mappings = None            

        else:
            print("Wrong DATA_MATERIALS[%s]"%args.volume)
            sys.exit(-1)
    
        gun_pos.property("GenInVolume").set(volume)
        if volume_mappings:
            gun_pos.property("RealVolumes").set(volume_mappings)
        if args.material == "None":
            gun_pos.property("Material").set(default_material)
        else:
            gun_pos.property("Material").set(args.material)
        # === volume cut ===
        radius_vec = []
        if args.volume_radius_min != 0.0:
            radius_vec.append(args.volume_radius_min)
        if args.volume_radius_max != 0.0:
            radius_vec.append(args.volume_radius_max)
        gun_pos.property("RadiusCut").set(radius_vec)
        # === Z cut ===
        z_vec = []
        if args.volume_z_min is not None:
            z_vec.append(args.volume_z_min)
        if args.volume_z_max is not None:
            z_vec.append(args.volume_z_max)
        gun_pos.property("ZCut").set(z_vec)
        # === X cut ===
        x_vec = []
        if args.volume_x_min is not None:
            x_vec.append(args.volume_x_min)
        if args.volume_x_max is not None:
            x_vec.append(args.volume_x_max)
        gun_pos.property("XCut").set(x_vec)
        # === Y cut ===
        y_vec = []
        if args.volume_y_min is not None:
            y_vec.append(args.volume_y_min)
        if args.volume_y_max is not None:
            y_vec.append(args.volume_y_max)
        gun_pos.property("YCut").set(y_vec)
    
        rock_depth_cut = []
        if args.volume_rock_depth is not None and args.volume_rock_depth > 0:
            rock_depth_cut.append(args.volume_rock_depth)
        gun_pos.property("RockDepthCut").set(rock_depth_cut)
    
        # == global positions ==
        if args.global_position:
            if len(args.global_position) != 1:
                assert(len(args.global_position) != 1)
            gun_pos.property("PositionMode").set("GenInGlobal")
            gun_pos.property("Positions").set(args.global_position[0])
        # = append it =
        gt.property("GenToolNames").append(gun_pos.objName())
    
    # FC events pre selection
    def pre_selecter(self, args, gt):
        import GenTools
        ps = gt.createTool("GtFCSelTool")
        ps.property("dedx").set(0.2082)
        ps.property("rcut").set(2e4)
        
        gt.property("GenToolNames").append([ps.objName()])

    def setup_generator(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
    
        gun = gt.createTool("GtGunGenTool/gun")
        gun.property("particleNames").set(args.particles)
        gun.property("particleMomentums").set(args.momentums)
        gun.property("particleMomentumMode").set(args.momentums_mode)
        if args.momentums_extra_params:
            gun.property("particleMomentumParams").set(args.momentums_extra_params)
        gun.property("particleMomentumInterp").set(args.momentums_interp)
        if args.directions:
            gun.property("DirectionMode").set("Fix")
            gun.property("Directions").set([makeTV(px,py,pz) for px,py,pz in args.directions])
        print(args.positions, args.times)
        if len(args.positions) == 1:
            gun.property("PositionMode").set("FixOne")
        else:
            gun.property("PositionMode").set("FixMany")

        gun.property("Positions").set([makeTV(x,y,z) for x,y,z in args.positions])

    
        if args.times is None:
            gun.property("TimeMode").set("Omit")
        else:
            if len(args.times) == 1:
                gun.property("TimeMode").set("FixOne")
            else:
                gun.property("TimeMode").set("FixMany")
            gun.property("Times").set(args.times)
    
    
        gt.property("GenToolNames").set([gun.objName()])
        
        # positioner
        # Note: by default, we choose fixed position instead of using positioner
        self.helper_positioner(args, gt, None)
    

    def WHEEL_IN_OLD_METHOD_TO_AVOID_MERGE_CONFLICT_setup_generator_opticks(self, task, args):
        import GenTools
        gt = task.createAlg("GenTools")
        ok = gt.createTool("GtOpticksTool/ok")
        import os.path 
        path = os.path.expandvars(os.path.expanduser(args.input_photon_path))
        repeat = args.input_photon_repeat
        wavelength = args.input_photon_wavelength
        objName = ok.objName()
        log.info("setting inputPhotonPath:%s objName:%s repeat:%d wavelength:%s " % (path, objName, repeat, wavelength) )
        if os.path.exists(path):
            ok.property("inputPhotonPath").set(path)
            ok.property("inputPhotonRepeat").set(repeat)
            ok.property("inputPhotonWavelength").set(wavelength)
        else:
            log.fatal("input_photon_path:%s DOES NOT EXIST " % path ) 
        pass
        gt.property("GenToolNames").set([objName])

    def setup_generator_nuwro(self, task, args):
        import GenTools
        gt = task.createAlg("GenTools")
        import Sniper
        Sniper.loadDll("libGenNuWro.so")
        nuwro_gen = gt.createTool("NuWroEvGen/nuwro")
        path = args.input
        nuwro_gen.property("NuWroConfigFile").set(path)
        gt.property("GenToolNames").set([nuwro_gen.objName()])


    def setup_generator_photon(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        # optical photon gun (using LS emission spectrum)
        gun = gt.createTool("GtOpScintTool/gun")
        gun.property("PhotonsPerEvent").set(args.totalphotons)
        gun.property("cosThetaLower").set(args.cos_theta_lower)
        gun.property("cosThetaUpper").set(args.cos_theta_upper)
    
        # Some user need to fix the wavelength
        if args.fixed_energy:
            gun.property("EnergyMode").set("Fixed")
            gun.property("FixedEnergy").set(args.fixed_energy)
            gun.property("TimeMode").set("Fixed")
            gun.property("EnableDfball").set(args.dfball_tube)
    
        gt.property("GenToolNames").set([gun.objName()])
        # positioner
        if args.dfball_tube:
            gun.property("OffsetInX").set(args.OffsetInX)
            gun.property("OffsetInY").set(args.OffsetInY)
            gun.property("OffsetInZ").set(args.OffsetInZ)
        else:
            self.helper_positioner(args, gt)
    
    def setup_generator_gendecay(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        # == gendecay related ==
        import Sniper
        Sniper.loadDll("libGenDecay.so")
        era = gt.createTool("GtDecayerator")
        era.property("ParentNuclide").set(args.nuclear)
        era.property("StopNuclide").set(args.stop_nuclear)
        correlation_time = args.correlation_time
        if correlation_time is None and args.nuclear in self.DECAY_DATA:
            correlation_time = self.DECAY_DATA[args.nuclear]
        # if correlation_time is still None, assign default value 1s
        if correlation_time is None:
            correlation_time = 1e9
        era.property("CorrelationTime").set(correlation_time)
        era.property("ParentAbundance").set(5e16)
        era.property("DecayDepth").set(args.decay_depth)
        gt.property("GenToolNames").set([era.objName()])
        # == positioner related ==
        self.helper_positioner(args, gt)
        # == GtTimeOffsetTool ==
        toffset = gt.createTool("GtTimeOffsetTool")
        gt.property("GenToolNames").append(toffset.objName())
    
    def setup_generator_hepevt(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        # == HepEvt to HepMC ==
        gun = gt.createTool("GtHepEvtGenTool/gun")
        #gun.property("Source").set("K40.exe -seed 42 -n 100|")
        source = self.GENERATOR_EXEC[args.exe].format(SEED=args.seed,
                                                      EVENT=args.evtmax)
        if args.file:
            source = args.file
        gun.property("Source").set(source)
        gt.property("GenToolNames").set([gun.objName()])
        # == positioner related ==
        # === if muon event, use the hepevt file's position ===
        if args.exe == "Muon":
            pass
        else:
            self.helper_positioner(args, gt)
        # == GtTimeOffsetTool ==
        toffset = gt.createTool("GtTimeOffsetTool")
        gt.property("GenToolNames").append([toffset.objName()])
    
    def setup_generator_beam(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
    
        from GenTools import makeTV
        gun = gt.createTool("GtPelletronBeamerTool/gun")
        gun.property("particleName").set(args.particle)
        gun.property("planeCentrePos").set(makeTV(args.plane_x,
                                                  args.plane_y,
                                                  args.plane_z)) # (0,0,1m)
        gun.property("planeDirection").set(makeTV(args.plane_dirx,
                                                  args.plane_diry,
                                                  args.plane_dirz)) # down
        gun.property("planeRadius").set(args.plane_r) # 20mm
        import math
        gun.property("beamThetaMax").set(math.radians(args.divergence)) # 10deg -> rad
        gun.property("beamMomentum").set(args.momentum) # 1MeV
        gun.property("beamMomentumSpread").set(args.momentum_spread) # 0.1MeV
    
        gt.property("GenToolNames").set([gun.objName()])


    # = setup supernova =
    def setup_generator_sn(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        # supernova
        gun = gt.createTool("GtSNTool/gun")
        # check the input file
        import os.path
        if not args.input or not os.path.exists(args.input):
            print("can't find the supernova input file '%s'"%args.input)
            sys.exit(-1)
        gun.property("inputSNFile").set(args.input)
        gun.property("StartIndex").set(args.index)
    
        gt.property("GenToolNames").set([gun.objName()])
        # positioner
        self.helper_positioner(args, gt)
        # time
        gt.property("EnableSNTime").set(args.relative_hittime)
    
    # = setup solar neutrinos=
    def setup_generator_nusol(self, task, args):
        import GenTools
        import NuSolGen
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        # nusol
        gun = gt.createTool("GtNuSolTool/gun")
        gun.property("neutrinoType").set(args.type);
    
        gt.property("GenToolNames").set([gun.objName()])
        # positioner
        self.helper_positioner(args, gt)
    
    # = setup atmospheric neutrinos=
    def setup_generator_atmo(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        #gst is genie format of atmospheric neutrino generator
        gun_gst = gt.createTool("GtGstTool")
        # check the input file
        import os.path
        if not args.input or not os.path.exists(args.input):
            print("can't find atmospheric input file '%s'"%args.input)
            sys.exit(-1)
    
        gun_gst.property("inputGstFile").set(args.input)
        gun_gst.property("GstStartIndex").set(args.index)
        gun_gst.property("GstVertexXYZEnabled").set(args.gst_xyz)
        gun_gst.property("GstVertexTEnabled").set(args.gst_t)
        gun_gst.property("ScaleXYZ").set(args.scale_gst_xyz)
        gun_gst.property("ScaleT").set(args.scale_gst_t)
        gt.property("GenToolNames").set([gun_gst.objName()])
        # If don't use the vertex info in gst, then we need to setup the positioner
        # positioner
        if not args.gst_xyz:
            self.helper_positioner(args, gt)

    # = setup GENIE integration =
    def setup_generator_gevgen_atmo(self, task, args):
        import GenTools
        gt = task.createAlg("GenTools")
    
        import GenGenie
        genie_evgen = gt.createTool("GtGenieAtmoEvGenTool/gevgen_atmo")
    
        # Energy
        genie_evgen.property('EvMax').set(args.ev_max)
        genie_evgen.property('EvMin').set(args.ev_min)
    
        # Target
        # target_mix = {}
        # for pw in args.targets.split(","):
            # i1 = pw.find("[")
            # i2 = pw.find("]")
            # p = int(pw[:i1])
            # w = float(pw[i1+1:i2])
            # target_mix[p] = w
        genie_evgen.property("TgtMix").set(args.targets)
        genie_evgen.property("JUNOTarget").set(args.tar_tag)
    
        genie_evgen.property("Flux").set(args.flux)
        genie_evgen.property("RanSeed").set(args.seed)
        genie_evgen.property("InpXSecFile").set(args.cross_sections)
    
        genie_evgen.property("Tune").set(args.tune)
    
        genie_evgen.property('IsotroEnable').set(args.isotropic)
        if '.root' in args.flux and len(args.user_flux_frac) == 0:
            assert 0, "Error! --user-flux-frac should be set when using 1d hist flux."
        genie_evgen.property('FluxFrac').set(args.user_flux_frac)
        genie_evgen.property('EventGeneratorList').set(args.event_generator_list)
    
        # register genie evgen
        gt.property("GenToolNames").set([genie_evgen.objName()])

        if args.deex:
            self.helper_deex(args,gt)

        # positioner
        self.helper_positioner(args, gt)
        
        # pre-select
        if args.preselect:
            self.pre_selecter(args, gt)

        pass
    
    def setup_generator_neutron(self, task, args):
        import GenTools
        from GenTools import makeTV
        gt = task.createAlg("GenTools")
        gun = gt.createTool("GtNeutronTool")
        # check the input file
        import os.path
        if not args.input or not os.path.exists(args.input):
            print("can't find neutron input file '%s'"%args.input)
            sys.exit(-1)
    
        gun.property("inputFile").set(args.input)
        gun.property("startIndex").set(args.index)
        if args.energy:
            gun.property("neutronEnergy").set(args.energy)
        gt.property("GenToolNames").set([gun.objName()])


    def setup_generator_opticks(self, task, args):
        """
        Former declProp controls have been removed. 
        Are now using Opticks input photon machinery in order to 
        profit from its automated frame transformation of input photons.

        OPTICKS_INPUT_PHOTON
           selects path to .npy array of shape (n,4,4) 
           with the input photons, see ana/input_photons.sh 

        MOI/INST/OPTICKS_INPUT_PHOTON_FRAME
           controls target frame, see CSGFoundry::getFrameE 

        To follow how the hookup and transformation works see::

            SEvt::GetInputPhoton 
            G4CXOpticks::setupFrame
            CSGFoundry::getFrameE

        """
        noop = os.environ.get("OPTICKS_PREFIX", None) is None
        if noop:
            log.fatal("setup_generator_opticks : Opticks Input Photons cannot be used when not compiled WITH_G4CXOPTICKS")
            log.fatal("lack of WITH_G4CXOPTICKS is inferred by lack of OPTICKS_PREFIX envvar") 
            sys.exit(-1)
        pass
        import GenTools
        gt = task.createAlg("GenTools")
        ok = gt.createTool("GtOpticksTool/ok")
        objName = ok.objName()
        gt.property("GenToolNames").set([objName])
        log.info("setup_generator_opticks : objName:[%s]" % (objName) )



    ##########################################################################
    # END: Handle the gentools
    ##########################################################################

    def init_mc_parameters(self, task, args):
        import Sniper
        Sniper.loadDll("libMCParamsSvc.so")
        mcsvc = task.createSvc("MCParamsFileSvc/MCParamsSvc")
        paramMap={}
        if args.light_yield:
            paramMap["Material.LS.ConstantProperty.ScintillationYield"] = args.light_yield
        if args.replace_param:
            import re
            param = args.replace_param.replace(" ","")
            param = re.split(',\s*|\s+',param)      
            for r in param:
                kv = r.split(":")
                if len(kv) == 1: 
                    print ("Error! dismatch of parameters" )
                    sys.exit(-1)
                k, v = kv
                paramMap[k]=v
        mcsvc.property("ReplaceParam").set(paramMap)
        mcsvc.property("UseParaDB").set(args.paradb)

    def init_op_simulator(self, task, args):
        # if the deferred op simulator is not enabled, don't need to setup OPSimSvc
        if not args.deferred_op:
            return

        import Sniper
        Sniper.loadDll("libOPSimulator.so")
        opsim = task.createSvc("OPSimSvc")

    ##########################################################################
    # = DETECTOR SIMULATION =
    ##########################################################################
    def init_detector_simulation(self, task, args):
        ###################################################################################
        # If user only want to run generation, then don't need to load detector simulation.
        ###################################################################################
        if not args.detsim:
            return

        import DetSimOptions
        sim_conf = None
        if args.detoption == "Acrylic":
            from DetSimOptions.ConfAcrylic import ConfAcrylic
            acrylic_conf = ConfAcrylic(task)
            acrylic_conf.configure()
            sim_conf = acrylic_conf
    
        if sim_conf is None:
            log.fatal("The sim_conf is None.")
            log.fatal("Maybe the detector option %s is not supported."%args.detoption)
            sys.exit(-1)

        # = detector components =
        self.init_detsim_detcomp(task, args, sim_conf)
        # = analysis manager control =
        self.init_detsim_anamgr(task, args, sim_conf)
        self.init_detsim_anamgr_fastsim(task, args, sim_conf)

        # physics list
        self.init_detsim_phys(task, args, sim_conf)
        # optical related
        self.init_detsim_optical(task, args, sim_conf)

        # simulation framework
        self.init_detsim_sim_framework(task, args, sim_conf)
        log.info("end of init_detector_simulation")


    ##########################################################################
    # == SIM FRAMEWORK IN DETECTOR SIMULATION ==
    ##########################################################################
    def init_detsim_sim_framework(self, task, args, sim_conf):

        # == geant4 run mac ==
        detsimalg = sim_conf.detsimalg()
        detsimalg.property("RunCmds").set([
                     #"/run/initialize",
                     #"/tracking/verbose 2",
                     #"/process/inactivate Scintillation",
                     #"/process/inactivate Cerenkov",
                 ])
        if args.mac:
            if os.path.exists(args.mac):
                detsimalg.property("RunMac").set(args.mac)
            else:
                print("WARNING: mac file %s does not exist."%args.mac)
                detsimalg.property("RunMac").set("")

        if args.vis:
            default_vis_mac_path = None
            if os.getenv("TUTORIALROOT"):
                default_vis_mac_path = os.path.join(os.getenv("TUTORIALROOT"), "share", "vis.mac")
            
            if os.path.exists(args.vis_mac):
                detsimalg.property("VisMac").set(args.vis_mac)
            elif default_vis_mac_path and os.path.exists(default_vis_mac_path):
                detsimalg.property("VisMac").set(default_vis_mac_path)
            else:
                log.fatal("WARNING: vis mac file %s does not exist."%args.vis_mac)
                sys.exit(-1)
            

    ##########################################################################
    # == OPTICAL IN DETECTOR SIMULATION ==
    ##########################################################################
    def init_detsim_optical(self, task, args, sim_conf):
        cd = sim_conf.cd()
        detsimfactory = sim_conf.detsimfactory()
    
        # == QE scale ==
        sim_conf.set_qe_scale(args.qescale)
        # == 20inch PMT ==
        log.info("PMTName %s --pmt20inch-name " % args.pmt20inch_name)
        log.info("LPMTExtra %s --pmt20inch-extra " % args.pmt20inch_extra)

        detsimfactory.property("OpticksMode").set(args.opticks_mode)

        #--------------------------------------------------------------------#    
        if args.new_optical_model:
            detsimfactory.property("UseLSOpticalModel").set("new")
        else:
            detsimfactory.property("UseLSOpticalModel").set("old")

        if args.pmt_optical_model:
            detsimfactory.property("UsePMTOpticalModel").set("new")
        else:
            detsimfactory.property("UsePMTOpticalModel").set("old")
        #--------------------------------------------------------------------#
        detsimfactory.property("UsePmtSimSvc").set(args.usepmtsimsvc)
        #MaterialDataDir-----------------------------------------# 
        if args.MaterialDataDir:
            if os.path.exists(args.MaterialDataDir+"/Material"):
                os.environ['MCPARAMSROOT'] = args.MaterialDataDir
                print(args.MaterialDataDir) 
            else:
                print("Error:" + args.MaterialDataDir+"/Material"+" doesn't exists!")
                sys.exit(-1)        
               
        #-----------------------------------------#
        # Handle Ideal or Real Surface in the CD  #
        #-----------------------------------------#
        cd.property("UseRealSurface").set(args.real_surface_in_cd_enabled)
        strutbar = sim_conf.tool("StrutAcrylicConstruction")
        strutbar.property("UseRealSurface").set(args.real_surface_in_cd_enabled)
        strutbar.property("UseStrutOpSurface").set(args.strut_surface_enabled)
        # strutbar.property
        strutbar2 = sim_conf.tool("StrutBar2AcrylicConstruction")
        strutbar2.property("UseRealSurface").set(args.real_surface_in_cd_enabled)
        strutbar2.property("UseStrutOpSurface").set(args.strut_surface_enabled)

        detsimfactory.property("CDInnerReflectorEnabled").set(args.optical_surface_in_cd_enabled)

        #-----------------------------------------#
        # PMT                                     #
        #-----------------------------------------#

        detsimfactory.property("PMTName").set(args.pmt20inch_name)
        detsimfactory.property("LPMTExtra").set(args.pmt20inch_extra)
        if args.pmt20inch_name == "R12860":
            r12860 = sim_conf.tool("R12860PMTManager/PMT_20inch")
            r12860.property("FastCover").set(True)
            r12860.property("FastCoverMaterial").set("Water")
        elif args.pmt20inch_name == "PMTMask":
            mask = sim_conf.tool("R12860MaskManager")
            mask.property("TopThickness").set(args.pmtmask_top_thick)
        pass

        #----------------------------------------------------------------#
        # SET INTERNAL ENVVARS CONFIGURING MAINLY PMT CSG MODELLING      #
        #----------------------------------------------------------------#
        self.init_SWITCH_envvars(args)

        ## debug: add cover for NNVT MCP-PMT

        if args.pmt20inch_extra == "TWO":
            log.info("TWO . args.pmt20inch_extra %s " % args.pmt20inch_extra)  
            nnvt_mcp_pmt = sim_conf.tool("NNVTMCPPMTManager/NNVTMCPPMT")
            nnvt_mcp_pmt.property("FastCover").set(True)
            nnvt_mcp_pmt.property("FastCoverMaterial").set("Water")
            nnvt_mcp_pmt.property("UsePMTOpticalModel").set(args.pmt_optical_model)
            nnvt_mcp_pmt.property("UsePMTNaturalGeometry").set(args.pmt_natural_geometry)

            hamamatsu_pmt = sim_conf.tool("HamamatsuR12860PMTManager/HamamatsuR12860")
            hamamatsu_pmt.property("FastCover").set(True)
            hamamatsu_pmt.property("FastCoverMaterial").set("Water")
            hamamatsu_pmt.property("UsePMTOpticalModel").set(args.pmt_optical_model)
            hamamatsu_pmt.property("UsePMTNaturalGeometry").set(args.pmt_natural_geometry)

        elif args.pmt20inch_extra == "TWO-mask":
            log.info("TWO-mask . args.pmt20inch_extra %s " % args.pmt20inch_extra)  
            nnvt_mcp_pmt = sim_conf.tool("NNVTMCPPMTManager/NNVTMCPPMT_PMT_20inch")
            nnvt_mcp_pmt.property("UsePMTOpticalModel").set(args.pmt_optical_model)
            nnvt_mcp_pmt.property("UsePMTNaturalGeometry").set(args.pmt_natural_geometry)
            nnvt_mcp_pmt.property("UseRealSurface").set(args.real_surface_in_cd_enabled)

            nnvt_mcp_mask = sim_conf.tool("NNVTMaskManager/NNVTMCPPMT")
            nnvt_mcp_mask.property("UseRealSurface").set(args.real_surface_in_cd_enabled)
            nnvt_mcp_mask.property("UseRealMaskTail").set(args.real_mask_tail)
            nnvt_mcp_mask.property("UseMaskTailOpSurface").set(args.mask_tail_surface_enabled)

            hamamatsu_pmt = sim_conf.tool("HamamatsuR12860PMTManager/HamamatsuR12860_PMT_20inch")
            hamamatsu_pmt.property("UsePMTOpticalModel").set(args.pmt_optical_model)
            hamamatsu_pmt.property("UsePMTNaturalGeometry").set(args.pmt_natural_geometry)
            hamamatsu_pmt.property("UseRealSurface").set(args.real_surface_in_cd_enabled)

            hamamatsu_mask = sim_conf.tool("HamamatsuMaskManager/HamamatsuR12860")
            hamamatsu_mask.property("UseRealSurface").set(args.real_surface_in_cd_enabled)
            hamamatsu_mask.property("UseRealMaskTail").set(args.real_mask_tail)
            hamamatsu_mask.property("UseMaskTailOpSurface").set(args.mask_tail_surface_enabled)

        else:
            log.info("args.pmt20inch_extra %s " % args.pmt20inch_extra)  
        pass

        # == enable or disable 20inch PMTs ==
        if not args.pmt20inch:
            # set the range 0:0, so no pmt will be constructed
            cd.property("IsPMT20inchEnabled").set(args.pmt20inch)

        # == enable or disable 3inch PMTs ==
        if not args.pmt3inch:
            cd.property("IsPMT3inchEnabled").set(args.pmt3inch)
        else:
            print("3inch PMT type: ", args.pmt3inch_name)
            sim_conf.set_3inch_pmt_name(args.pmt3inch_name)
            sim_conf.set_3inch_pmt_offset(args.pmt3inch_offset)

        # == enable or disable PMTs in WP ==
        if not args.wp_pmt_enabled:
            wp = sim_conf.wp()
            wp.property("IsVetoPMT20inchEnabled").set(args.wp_pmt_enabled)
        if args.pmtsd_v2:
            sim_conf.enable_PMTSD_v2()
            pmtsdmgr = sim_conf.pmtsd_mgr()
            pmtsdmgr.property("CollEffiMode").set(args.ce_mode)
            pmtsdmgr.property("CEFlatValue").set(args.ce_flat_value)
            pmtsdmgr.property("OpticksMode").set(args.opticks_mode)
            pmtsdmgr.property("UsePMTOpticalModel").set(args.pmt_optical_model)
            pmtsdmgr.property("UsePmtSimSvc").set(args.usepmtsimsvc)
            if not args.usepmtsimsvc:
                assert(not args.pmt_optical_model)
        if args.pmtsd_merge_twindow>0:
            pmtsdmgr = sim_conf.pmtsd_mgr()
            pmtsdmgr.property("EnableMergeHit").set(True)
            pmtsdmgr.property("MergeTimeWindow").set(args.pmtsd_merge_twindow)
        # pmt hit type
        pmtsdmgr = sim_conf.pmtsd_mgr()
        pmtsdmgr.property("HitType").set(args.pmt_hit_type)
        pmtsdmgr.property("DisableSD").set(args.pmt_disable_process)
        if args.ce_func:
            pmtsdmgr.property("CEFunction").set(args.ce_func)
        if args.ce_func_par:
            pmtsdmgr.property("CEFuncParams").set(args.ce_func_par)
        # disable struts and fasteners
        if args.flag_struts_fasteners != "none":
            sim_conf.disable_struts_in_cd(args.flag_struts_fasteners)
        # gdml output
        if self.gdml_filename:
            sim_conf.set_gdml_output(self.gdml_filename)
            # if split mode enabled, then need let geosvc know the iotask name
            if args.anamgr_edm_v2 and self.rootio_taskname:
                sim_conf.geom().property("RootIOTask").set(self.rootio_taskname)
        if self.dae_filename:
            sim_conf.set_dae_output(self.dae_filename)
        pass
        log.info("end of init_detsim_optical")


    ##########################################################################
    # == HELPER METHOD for opticks ==
    ##########################################################################

    SWITCH = textwrap.dedent("""

    FORMER OPTION --pmt20inch-polycone-neck IS NOW THE DEFAULT 
        Use economical polycone 20inch PMT neck shape replacing cylinder-torus.

    FORMER OPTION --pmt20inch-simplify-csg IS NOW THE DEFAULT
        Simplify CSG modelling of 20inch PMTs, avoiding Inner_Separator anti-pattern, see HamamatsuR12860PMTManager + NNVTMCPPMTManager 


    --additionacrylic-simplify-csg 
        Simplify CSG modelling avoiding CSG subtraction for daughters, see AdditionAcrylicConstruction



    --pmt20inch-obsolete-torus-neck
        USE ONLY FOR DEBUGGING : Revert to the obsolete torus neck which has spurious intersects, bad performance and no Opticks support

    --pmt20inch-profligate-tail-cut
        USE ONLY FOR DEBUGGING : Revert to profligate G4IntersectionSolid "cutting" that substantially adds complexity and breaks Opticks 

    --debug-disable-sticks
        USE ONLY FOR DEBUGGING : see LSExpDetectorConstruction::setupCentralDetector

    --debug-disable-xj
        USE ONLY FOR DEBUGGING : see LSExpDetectorConstruction::setupCD_Sticks

    --debug-disable-sj
        USE ONLY FOR DEBUGGING : see LSExpDetectorConstruction::setupCD_Sticks

    --pmt20inch-plus-dynode
        USE ONLY FOR DEBUGGING : Adds PMT dynode geometry even when not using new PMT optical model, eg to check dynode does not impinge

    """)

    def register_SWITCH_options(self, group):
        """
        Called from *register_options_pmt_and_optical* : parses self.SWITCH adding the listed options and help text to the argparse group 
       
        NB use envvar LOGLEVEL=debug to see python logging debug output
        NB the installed version of this module is used by tut_detsim.py so make sure to build+install after making changes 
        """
        name = "register_SWITCH_options"
        lines = self.SWITCH.split("\n")
        log.debug("%s : parsed from %d SWITCH lines" % (name, len(lines)))
        log.debug("%s : SWITCH : \n%s\n" % (name, self.SWITCH))

        for i in range(1,len(lines)):
            if lines[i-1][:2] == "--":
                argument = lines[i-1].rstrip()
                argdesc = lines[i].lstrip().rstrip()
                assert not "_" in argument, argument
                key = "JUNO_%s" % argument[2:].upper().replace("-","_")
                argdesc += " (sets internal envvar %s )" % key
                log.debug("argument[%s] argdesc[%s] key[%s] " % (argument, argdesc, key))
                pass
                group.add_argument(argument, default=False, action="store_true", help=mh(argdesc))
            pass
        pass
    pass

    def init_SWITCH_envvars(self, args):
        """  
        Called after argument parsing from *init_detector_simulation* 

        Sets internal envvar when some commandline switches are used.
        Note that externally set envvars with these keys **ARE IGNORED**.
        Messaging via internal envvars has the advantages:

        1. instanciation order is unchanged 
        2. requires a minimum of code
        3. works in standalone manner for convenient testing

        Constraints followed/imposed:

        * command line arguments use "-" not "_", hence "_" are disallowed in self.SWITCH
        * argparser args uses "_" not "-" for attributes
        * envvar keys start with "JUNO_" and use "_" not "-"

        """
        name = "init_SWITCH_envvars"
        lines = self.SWITCH.split("\n")
        log.info("%s : parsing %d SWITCH lines" % (name, len(lines)))

        for line in lines:
            if not line[:2] == "--": continue
            attn = line[2:].rstrip().replace("-","_")
            att = getattr( args, attn, None )

            log.debug("attn:[%s] att:[%s]" % (attn, att))

            key = "JUNO_%s" % attn.upper().replace("-","_") 
            # assert att, key 
            if att == True:
                log.info("%s : setting key %s from args.%s  " % (name, key, attn))
                os.environ[key] = "ENABLED"
            else:
                ## prevent sensitivity to external envvars with these keys 
                if key in os.environ:
                    log.info("%s : un-setting key %s from args.%s  " % (name, key, attn))
                    os.environ.pop(key)
                pass
            pass
        pass


    ##########################################################################
    # == DETECTOR COMPONENTS IN DETECTOR SIMULATION ==
    ##########################################################################
    def init_detsim_detcomp(self, task, args, sim_conf):
        cd = sim_conf.cd()
        cd.property("CheckOverlap").set(args.check_overlap_in_cd_enabled)
        # = commissioning =
        if args.commissioning_enabled:
            cd.property("IsCommissioning").set(True)
            cd.property("ZbelowWater").set(args.below_z_is_water)

        # cd.setLogLevel(0) # enable when debug contruction of cd.
        detsimfactory = sim_conf.detsimfactory()

        ## Chimney is enabled by default
        #enable the blow 2 lines only when you want to define new geomery parameters.
        #acrylic_conf.set_top_chimney(3.5, 0.1) #(upper_chimney_height,inner_reflectivity) 
        #acrylic_conf.set_lower_chimney(0.3, 0.1) # (blocker_Z_position, inner_reflectivity)
    
        #acrylic_conf.disable_chimney() # enable this line to simulate without chimney


        detsimfactory.property("PhysicsList").set(args.physics_list)
        # = detector components =
        detsimfactory.property("CDEnabled").set(args.cd_enabled)
        detsimfactory.property("WPEnabled").set(args.wp_enabled)
        detsimfactory.property("TTEnabled").set(args.tt_enabled)
    
        if args.shutter:
            sim_conf.enable_shutter()

        # = WP =
        # == Latticed Shell ==
        if args.wp_enabled and args.wp_latticed_enabled:
            sim_conf.enable_wp_latticed()
        else:
            sim_conf.disable_wp_latticed()

        # global geom info
        geom_info = sim_conf.tool("GlobalGeomInfo")

        #==========  simplify calib anchor ============#
        if args.simplify_calib_anchor:
            os.environ["JUNO_SIMPLIFY_CALIB_ANCHOR"]="Enable"
        elif "JUNO_SIMPLIFY_CALIB_ANCHOR" in os.environ:
            os.environ.pop("JUNO_SIMPLIFY_CALIB_ANCHOR")
            print("unset key : JUNO_SIMPLIFY_CALIB_ANCHOR ")       

        # == beam mode ==
        self.setup_calib_pelletron(args, sim_conf, args.pelletron)
        # == calibration units ==
        self.setup_calib_unit(args, sim_conf, args.guide_tube)


    ##########################################################################
    # == ANALYSIS MANAGER IN DETECTOR SIMULATION ==
    ##########################################################################
    def init_detsim_anamgr(self, task, args, sim_conf):
        detsimfactory = sim_conf.detsimfactory()

        # == reset the anamgr list to data model writer ==
        detsimfactory.property("AnaMgrList").set([])
        # == op simulator interface ==
        if args.deferred_op:
            detsimfactory.property("AnaMgrList").append("OPSimAnaMgr")
            deferred_opsim_anamgr = sim_conf.tool("OPSimAnaMgr")
            deferred_opsim_anamgr.property("TestingRatio").set(args.deferred_op_testing_ratio)
        # == opticks
        if args.opticks_mode > 0 and args.opticks_anamgr:
            print("appending U4RecorderAnaMgr to AnaMgrList")    
            detsimfactory.property("AnaMgrList").append("U4RecorderAnaMgr")
            g4ok_anamgr = sim_conf.tool("U4RecorderAnaMgr")
            g4ok_anamgr.setLogLevel(4)
        pass
        # == grdm (Geant4 Radioactivity Decay Module) ==
        if args.anamgr_grdm:
            detsimfactory.property("AnaMgrList").append("RadioAnaMgr")
            ram = sim_conf.tool("RadioAnaMgr")
            ram.property("StopAtPa234m").set(args.stopAtPa234m)

        # == edm (event data model) ==
        # == if split mode enable, disable others ==   
        if args.anamgr_edm:
            detsimfactory.property("AnaMgrList").append("DataModelWriter")
        elif args.anamgr_edm_v2:
            detsimfactory.property("AnaMgrList").append(
                                        "DataModelWriterWithSplit",
                                        )
            dmwws = sim_conf.tool("DataModelWriterWithSplit")
            dmwws.property("SaveSimTrack").set(args.anamgr_simtrack)
            if args.splitevent:
                dmwws.property("disable_split").set(False)
                if args.event_split_maxhits: 
                    dmwws.property("HitsMax").set(args.event_split_maxhits)
                else:
                    dmwws.property("HitsMax").set(10000)
            else:
                if args.event_split_maxhits != None: #Force to add option --output-split when using --split-maxhits
                    assert 0, "Error ! : --event-split should be set."
                dmwws.property("disable_split").set(True)   
        # == TT ==
        if args.anamgr_tt:
            sim_conf.set_tt_edep_output()
        # == normal anamgr ==
        if args.anamgr_normal:
            detsimfactory.property("AnaMgrList").append("NormalAnaMgr")
            if args.anamgr_normal_hit:
               normal_anamgr=sim_conf.tool("NormalAnaMgr")
               normal_anamgr.property("EnableHitInfo").set(args.anamgr_normal_hit)

        # == muon anamgrs ==
        if args.anamgr_muon:
            detsimfactory.property("AnaMgrList").append("MuProcessAnaMgr")
            detsimfactory.property("AnaMgrList").append("MuIsoProcessAnaMgr")
            detsimfactory.property("AnaMgrList").append("MuFastnProcessAnaMgr")
            detsimfactory.property("AnaMgrList").append("MuTrackingAnaMgr")

        # == atmo anamgr ==
        if args.anamgr_atmo:
            detsimfactory.property("AnaMgrList").append("DepEnAnaMgr")

        # == genevt anamgr ==
        if args.anamgr_genevt:
            detsimfactory.property("AnaMgrList").append("GenEvtInfoAnaMgr")
        # == deposit anamgr ==
        if args.anamgr_deposit:
            detsimfactory.property("AnaMgrList").append("DepositEnergyAnaMgr")
            cd_anamgr = sim_conf.tool("DepositEnergyAnaMgr")
            cd_anamgr.property("UseQuenching").set(args.quenching)
        if args.anamgr_deposit_tt:
            detsimfactory.property("AnaMgrList").append("DepositEnergyTTAnaMgr")
            tt_anamgr = sim_conf.tool("DepositEnergyTTAnaMgr")
            tt_anamgr.property("EnableNtuple").set(True)
        # == interesting process ==
        if args.anamgr_interesting_process:
            detsimfactory.property("AnaMgrList").append("InteresingProcessAnaMgr")
            ipam_anamgr = sim_conf.tool("InteresingProcessAnaMgr")
            if args.anamgr_extra_procnames:
                ipam_anamgr.property("ExtraProcNames").set(args.anamgr_extra_procnames.split(":"))
            if args.anamgr_legacy_michel_electron:
                ipam_anamgr.property("legacyMichelElectron").set(args.anamgr_legacy_michel_electron)
            if args.anamgr_legacy_ncapture:
                ipam_anamgr.property("legacyNeutronCapture").set(args.anamgr_legacy_ncapture)
            if args.anamgr_legacy_decay:
                ipam_anamgr.property("legacyDecay").set(args.anamgr_legacy_decay)
        # == neutron_inelastic process ==
        if args.anamgr_neutron_inelastic:
            detsimfactory.property("AnaMgrList").append("SelectNeutronProcessAnaMgr")
        # == optical parameter ==
        if args.anamgr_optical_parameter:
            detsimfactory.property("AnaMgrList").append("OpticalParameterAnaMgr")
        # == timer anamgr ==
        if args.anamgr_timer:
            detsimfactory.property("AnaMgrList").append("TimerAnaMgr")
            timer = sim_conf.tool("TimerAnaMgr")
            timer.setLogLevel(3)
        # == photon tracking ==
        if args.anamgr_photon_tracking:
            detsimfactory.property("AnaMgrList").append("PhotonTrackingAnaMgr")
        # == print G4 track information of specified event ==#
        if args.anamgr_g4tracking_verbose and args.anamgr_g4tracking_evtlist:
            detsimfactory.property("AnaMgrList").append("PrintG4TrackAnaMgr")
            print_anamgr = sim_conf.tool("PrintG4TrackAnaMgr")
            import os.path
            if os.path.exists(args.anamgr_g4tracking_evtlist):
                filename = args.anamgr_g4tracking_evtlist
                with open(filename) as f:
                    for line in f:
                        print(line)
                        l = line.strip()
                        break
            else:
                l = args.anamgr_g4tracking_evtlist
            import re
            l = re.split(',\s*|\s+', l)
            evtid = [int(i) for i in l if i.isdigit()]
            print("evtid list: ", evtid)
            print_anamgr.property("VerBose").set(args.anamgr_g4tracking_verbose)
            print_anamgr.property("EventID").set(evtid)
        if args.anamgr_g4tracking_verbose and not args.anamgr_g4tracking_evtlist:
            detsimfactory.property("AnaMgrList").append("PrintG4TrackAnaMgr")
            print_anamgr = sim_conf.tool("PrintG4TrackAnaMgr")
            print_anamgr.property("VerBose").set(args.anamgr_g4tracking_verbose)
            li=list(range(args.evtmax))
            print_anamgr.property("EventID").set(li) 
        #================================#     
        # == append other anamgr into the list ==
        for tmp_anamgr in args.anamgr_list:
            detsimfactory.property("AnaMgrList").append(tmp_anamgr)
        # == for extension: load config file =
        import os.path
        if args.anamgr_config_file and os.path.exists(args.anamgr_config_file):
            # 
            print("The option --anamgr-config-file is not supported in the current version. ")
            sys.exit(-1)
            # print("Loading config file: '%s'"%args.anamgr_config_file)

            # if sys.version_info[0] == 3:
            #     exec(compile(open(args.anamgr_config_file, "rb").read(), args.anamgr_config_file, 'exec'))
            # else:
            #     execfile(args.anamgr_config_file)

    ##########################################################################
    # == FAST SIMULATION related ANALYSIS MANAGER IN DETECTOR SIMULATION ==
    ##########################################################################
    def init_detsim_anamgr_fastsim(self, task, args, sim_conf):
        # voxel fast simulation. need to disable the optical progress
        if args.voxel_fast_sim:
            print("voxel method enabled")
            # disable pmts and struts
            if not args.voxel_pmts_structs:
                print("disable pmts and structs")
                sim_conf.disable_pmts_and_struts_in_cd()
            import os
            if os.environ.get("DETSIMOPTIONSROOT", None) is None:
                print("Missing DETSIMOPTIONSROOT")
                sys.exit(-1)
            dp = lambda f: os.path.join(os.environ.get("DETSIMOPTIONSROOT"),
                                        "share", "examples", "voxelmuon", f)
            if (os.environ.get("VOXELFASTDIR")):
                dp = lambda f: os.path.join(os.environ.get("VOXELFASTDIR"), f)
            if args.voxel_fast_dir:
                dp = lambda f: os.path.join(args.voxel_fast_dir, f)
            sim_conf.add_anamgr("MuonFastSimVoxel")
            mfsv = sim_conf.tool("MuonFastSimVoxel")
            mfsv.property("GeomFile").set(dp(args.voxel_geom_file))
            mfsv.property("NPEFile").set(dp(args.voxel_npe_file))
            # mfsv.property("HitTimeMean").set(dp("hist3d.root")) # Since r4309, mean is not used.
            mfsv.property("HitTimeRes").set(dp(args.voxel_time_file))
            mfsv.property("MergeFlag").set(args.voxel_merge_flag)
            mfsv.property("MergeTimeWindow").set(args.voxel_merge_twin)
            mfsv.property("EnableNtuple").set(args.voxel_fill_ntuple)
            mfsv.property("QuenchingFactor").set(args.voxel_quenching_scale)
            mfsv.property("SampleNPE").set(args.voxel_gen_npe)
            mfsv.property("SampleTime").set(args.voxel_gen_time)
            mfsv.property("SaveHits").set(args.voxel_save_hits)

        # split the primary track. step -> sub event
        if args.splittrack:
            sim_conf.add_anamgr("PostponeTrackAnaMgr")
            pta = sim_conf.tool("PostponeTrackAnaMgr")
            pta.property("SplitMode").set(args.track_split_mode)
            pta.property("TimeCut").set(args.track_split_time)
            #pta.setLogLevel(2)


    # == additional Calib Unit ==
    def setup_calib_unit(self, args, acrylic_conf, enable):
    
        # old one: Calib_GuideTube
        guide_tube_option = "Calib_GuideTube_V1"
    
        detsim0 = acrylic_conf.detsimfactory()
        detsimalg = acrylic_conf.detsimalg()
        import Sniper
        Sniper.loadDll("libCalibUnit.so")
        detsim0.property("CalibUnitEnable").set(enable)
        if not enable:
            print("setup_calib_unit exit as not enabled") 
            return
        pass 
    
        detsim0.property("CalibUnitName").set(guide_tube_option) # this is default one
        detsim0.property("CalibUnitExtras").set([]) # Enable more calibration units here
    
        if guide_tube_option == "Calib_GuideTube_V1":
            guide_tube_v1_0 = detsimalg.createTool("Calib_GuideTube_Construction/Calib_GuideTube_Construction_V1_0")
            guide_tube_v1_0.property("Option").set("V1_0")
    
            if args.GT_source_theta > 0:
                guide_tube_v1_0.property("Theta").set(args.GT_source_theta)
                guide_tube_v1_0.property("UseSource").set(True)
    
            guide_tube_v1_1 = detsimalg.createTool("Calib_GuideTube_Construction/Calib_GuideTube_Construction_V1_1")
            guide_tube_v1_1.property("Option").set("V1_1")
    
            if args.GT_source_theta < 0:
                guide_tube_v1_1.property("Theta").set(args.GT_source_theta)
                guide_tube_v1_1.property("UseSource").set(True)
    
            import math
    
            # the default guide tube is constructed at -x-z plane. 
            # V1_0: 123.40 deg. Rotate from 90 to 123.40, hence 123.40-90
            # V1_1: 267.40 deg. Rotate from 90 to 267.40, hence 267.40-90
    
            guide_tube_place_v1_0 = detsimalg.createTool("Calib_GuideTube_Placement/Calib_GuideTube_Placement_V1_0")
            # guide_tube_place_v1_0.property("Phi").set(-(90.+2.6)*math.pi/180.)
            guide_tube_place_v1_0.property("Phi").set((123.40-90.0)*math.pi/180.)
    
            guide_tube_place_v1_1 = detsimalg.createTool("Calib_GuideTube_Placement/Calib_GuideTube_Placement_V1_1")
            # guide_tube_place_v1_1.property("Phi").set((303.4-180.)*math.pi/180.)
            guide_tube_place_v1_1.property("Phi").set((267.40-90.0)*math.pi/180.)
    
        elif guide_tube_option == "Calib_GuideTube" and args.GT_source_theta:
            guide_tube_source_place = detsimalg.createTool("Calib_GuideTube_Construction")
            guide_tube_source_place.property("Theta").set(args.GT_source_theta);
            guide_tube_source_place.property("UseSource").set(True);
    
        # == Add calib source geometry into detector if necessary
        if args.source or args.source_weight_QC or args.source_weights or args.submarine or args.dfball_tube or args.ACU_source_weight_QC or args.CLS_source_weight_QC or args.K40_ACU_source_weight_QC: 
            detsim0.property("CalibUnitExtras").set(["lSourceWorld"]) #
        
            Calib_Source = detsimalg.createTool("GDMLDetElemConstruction/lSourceWorld")
        
            path = os.environ.get("CALIBUNITROOT")
            if args.source: 
                Calib_Source.property("GdmlFilename").set(path+"/share/source.gdml") # for only source enclosure
            if args.source_weight_QC: 
                Calib_Source.property("GdmlFilename").set(path+"/share/source_weight_QuickConnector.gdml")#include source enclosure, bottom weight and top quick connector
            if args.ACU_source_weight_QC: 
                Calib_Source.property("GdmlFilename").set(path+"/share/ACU_source_weight_QuickConnector.gdml")#include ACU source enclosure, bottom weight and top quick connector
            if args.CLS_source_weight_QC: 
                Calib_Source.property("GdmlFilename").set(path+"/share/CLS_source_weight_QuickConnector.gdml")#include CLS source enclosure, bottom weight and top quick connector
            if args.K40_ACU_source_weight_QC: 
                Calib_Source.property("GdmlFilename").set(path+"/share/K40_ACU_source_weight_QuickConnector.gdml")#include ACU source enclosure, bottom weight and top quick connector for K40
            if args.source_weights: 
                Calib_Source.property("GdmlFilename").set(path+"/share/source_weights.gdml") # source enclosure and two weights
            if args.submarine: 
                Calib_Source.property("GdmlFilename").set(path+"/share/submarine.gdml") # this is for ROV calibration system
            if args.dfball_tube:
                Calib_Source.property("GdmlFilename").set(path+"/share/laserdiffuser.gdml")#include LASER diffuser ball and Ni Rod attachment
           
            calibsourceplace = detsimalg.createTool("CalibSourcePlacement")
            calibsourceplace.property("OffsetInZ").set(args.OffsetInZ)
            calibsourceplace.property("OffsetInY").set(args.OffsetInY)
            calibsourceplace.property("OffsetInX").set(args.OffsetInX)

    
    # == additional Calib Unit ==
    def setup_calib_pelletron(self, args, acrylic_conf, enable):
        detsim0 = acrylic_conf.detsimfactory()
        detsimalg = acrylic_conf.detsimalg()
        import Sniper
        Sniper.loadDll("libCalibUnit.so")
        detsim0.property("CalibUnitEnable").set(enable)
        if not enable:
            return
        pass
    
        detsim0.property("CalibUnitName").set("CalibTube")
        # Calib Unit Related
        calibtube = detsimalg.createTool("CalibTubeConstruction")
        print(calibtube)
        calibtubeplace = detsimalg.createTool("CalibTubePlacement")
        # FIXME a more general geometry service is needed.
        calibTubeLength1 = 17.3e3; # 17.3m
        calibTubeLength2 = 0.3e3   #  0.3m
        offset_z_in_cd = (calibTubeLength1+calibTubeLength2)/2.
        calibtubeplace.property("OffsetInZ").set(offset_z_in_cd)
    
        acrylic_conf.add_anamgr("DepositEnergyCalibAnaMgr")
        calib_anamgr = detsimalg.createTool("DepositEnergyCalibAnaMgr")
        calib_anamgr.property("EnableNtuple").set(True)
    

    ##########################################################################
    # == PHYSICS PROCESSES IN DETECTOR SIMULATION ==
    ##########################################################################
    def init_detsim_phys(self, task, args, sim_conf):
        # = em =
        em_process = sim_conf.em_process()
        em_process.property("UsePositronium").set(args.positronium)
        # = ion =
        ion_process = sim_conf.ion_process()
        ion_process.property("EnableIonPHP").set(args.enableIonPHP)
        # disable the optical progress
        op_process = sim_conf.optical_process()
        op_process.property("OpticksMode").set(args.opticks_mode)  # see DsPhysConsOptical
        op_process.property("UseCerenkov").set(args.cerenkov)
        op_process.property("UseCerenkovType").set(args.cerenkov_type)
        if not args.useoptical or args.voxel_fast_sim:
            print("Disable Optical Process")
            op_process.property("UseScintillation").set(False)
            op_process.property("UseCerenkov").set(False)
        if args.cerenkov_only:
            print("Enable Cerenkov. (note: Scintillation is used to do reemission only)")
            op_process.property("UseScintillation").set(True)
            op_process.property("UseCerenkov").set(True)
            op_process.property("ScintDoReemissionOnly").set(True)

        # For testing of OPSimulator only
        if args.deferred_op:
            op_process.property("UseScintillation").set(True)
            # op_process.property("UseCerenkov").set(False)
            op_process.property("UseCerenkov").set(True)
            op_process.property("CerenPhotonStack").set(False)
            op_process.property("ScintDoReemissionOnly").set(True)

        op_process.property("UseQuenching").set(args.quenching)
        # new optical model
        if args.new_optical_model:
            op_process.property("UseAbsReemit").set(True)
            op_process.property("UseScintSimple").set(True)
        else:
            op_process.property("UseAbsReemit").set(False)
            op_process.property("UseScintSimple").set(False)
        # pmt optical model
        op_process.property("UsePMTOpticalModel").set(args.pmt_optical_model)
        op_process.property("UsePMTNaturalGeometry").set(args.pmt_natural_geometry)
       # op_process.property("UseAbsReemit").set(args.absreemit)
       # op_process.property("UseScintSimple").set(args.scintsimple)
        # other flags:
        op_process.property("doTrackSecondariesFirst").set(args.track_op_first)

        # Photon Conversion
        if args.photon_conversion:
            op_process.property("UsePhotonConversion").set(True)
            op_process.property("EnableDfball").set(args.dfball_tube)
            op_process.property("nProducedPhotons").set(args.nphotonsperparticle)
            if args.fixed_energy:
                op_process.property("FixedEnergy").set(args.fixed_energy)
            if args.dfball_tube:
                op_process.property("OffsetInX").set(args.OffsetInX)
                op_process.property("OffsetInY").set(args.OffsetInY)
                op_process.property("OffsetInZ").set(args.OffsetInZ)
             


