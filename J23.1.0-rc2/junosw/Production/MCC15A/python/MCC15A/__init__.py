#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao
import Sniper

CALIB_GENERATOR_EXEC = {#"IBD": "IBD.exe -n {EVENT} -seed {SEED}|",
                  #"IBD-eplus": "IBD.exe -n {EVENT} -seed {SEED} -eplus_only|",
                  "IBD-neutron": "IBD.exe -n {EVENT} -seed {SEED} -neutron_only|",
                  "AmC": "AmCNeutron.exe -n {EVENT} -seed {SEED}|",
                  "Co60": "Co60.exe -n {EVENT} -seed {SEED}|",
                  "Ge68": "Ge68.exe -n {EVENT} -seed {SEED}|",
                  "Ge68-geom": "Ge68.exe -n {EVENT} -seed {SEED} -geom 1|",
                  }
IBD_GENERATOR_EXEC = {"IBD": "IBD.exe -n {EVENT} -seed {SEED}|",
                  "IBD-eplus": "IBD.exe -n {EVENT} -seed {SEED} -eplus_only|",
                  "IBD-neutron": "IBD.exe -n {EVENT} -seed {SEED} -neutron_only|",
                  }
DECAY_DATA = {"U-238": 1.5e5, 
             "Th-232": 280, 
               "K-40": 1e9, 
              "Co-60": 1e9,
               "B-12": 1e9} # unit: ns
# = PARSER =
import argparse

class MakeTVAction(argparse.Action):
    def __init__(self, option_strings, dest, nargs=None, **kwargs):
        #print "__init__ begin"
        #print option_strings 
        #print dest 
        #print nargs
        #print kwargs
        #print "__init__ end"
        super(MakeTVAction, self).__init__(option_strings, dest, nargs, **kwargs)
    def __call__(self, parser, namespace, values, option_string=None):
        #print "__call__ begin"
        #print parser
        #print namespace
        #print values
        # == convert a list into 3-tuple ==
        if len(values) % 3:
            print("please set %s like x1 y1 z1 x2 y2 z2 ..." %option_string)
            sys.exit(-1)
        it = iter(values)
        values = zip(*([it]*3))
        setattr(namespace, self.dest, values)

def get_parser():


    parser = argparse.ArgumentParser(description='MCC15A Detector Simulation.',
                        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--evtmax", type=int, default=10, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--restore-seed-status", default=None, 
                                         help='restore the random engine, both '
                                         'a list of integer or a file contains '
                                         'the list of integer is supported. '
                                         'such as:'
                                         '   --restore-seed-status "1,2,3..."')
    parser.add_argument("--output", default="sample_detsim.root", help="output file name")
    parser.add_argument("--user-output", default="sample_detsim_user.root", help="output file name")
    parser.add_argument("--mac", default="run.mac", help="mac file")

    parser.add_argument("--detoption", default="Acrylic", 
                                       choices=["Acrylic", "Balloon"],
                                       help="Det Option")
    parser.add_argument("--qescale", default=0.9, type=float, 
                                     help="QE scale for ElecSim.")

    parser.add_argument("--gdml", dest="gdml", action="store_true", help="Save GDML.")
    parser.add_argument("--no-gdml", dest="gdml", action="store_false",
                                                  help="Don't Save GDML. ")
    parser.set_defaults(gdml=False)

    parser.add_argument("--pmt3inch", dest="pmt3inch", action="store_true",
                                      help="Enable 3inch PMTs. ")
    parser.add_argument("--no-pmt3inch", dest="pmt3inch", action="store_false",
                                      help="Disable 3inch PMTs.")
    parser.set_defaults(pmt3inch=True)

    parser.add_argument("--secret-file", default=None, 
                             help="Secret gdml file with parameters")
    parser.add_argument("--gamma-slower-time", default=190., type=float,
                             help="Gamma Slower time constant (ns)")
    parser.add_argument("--gamma-slower-ratio", default=0.15, type=float,
                             help="Gamma Slower time ratio")

    # = global position =
    # The global position is optional, it will override the (r,theta,phi)
    parser.add_argument("--global-position", default=None,
                                     nargs=3, type=float, action=MakeTVAction,
                                     help="Global Postion. ")
    # = gentool =
    subparsers = parser.add_subparsers(help='Please select the generator mode', 
                                       dest='gentool_mode')
    # == calib mode ==
    add_subparser_calib(subparsers)
    # == ibd mode ==
    add_subparser_ibd(subparsers)
    # == B12 mode ==
    add_subparser_b12(subparsers)
    # == low energy neutron mode ==
    add_subparser_neutron(subparsers)

    return parser
#############################################################################
def add_subparser_calib(parser):
    parser_calib = parser.add_parser("calib", 
            formatter_class=argparse.ArgumentDefaultsHelpFormatter,
            help="Calibration mode")
    parser_calib.add_argument("--exe", default="Ge68", 
                                         choices=CALIB_GENERATOR_EXEC.keys(),
                                         help="select the Generator to run")
    parser_calib.add_argument("--radius", default=0, type=float,
                                     help="Radius (mm)")
    parser_calib.add_argument("--theta", default=0.0, type=float,
                                     help="Theta (deg)")
    parser_calib.add_argument("--phi", default=0.0, type=float,
                                     help="Phi (deg)")

    pass
def setup_generator_calib(gt, args):
    from GenTools import makeTV
    gun = gt.createTool("GtHepEvtGenTool")
    gun.property("Source").set(
            CALIB_GENERATOR_EXEC[args.exe].format(SEED=args.seed,
                                            EVENT=args.evtmax)
            )
    gt.property("GenToolNames").append([gun.objName()])
    import math
    theta = math.radians(args.theta)
    phi   = math.radians(args.phi)
    x = args.radius * math.sin(theta) * math.cos(phi)
    y = args.radius * math.sin(theta) * math.sin(phi)
    z = args.radius * math.cos(theta)
    gun_pos = gt.createTool("GtPositionerTool")
    gun_pos.property("PositionMode").set("GenInGlobal")
    gun_pos.property("Positions").set(makeTV(x,y,z))
    gt.property("GenToolNames").append([gun_pos.objName()])

#############################################################################
def add_subparser_ibd(parser):
    parser_ibd = parser.add_parser("ibd", 
            formatter_class=argparse.ArgumentDefaultsHelpFormatter,
            help="IBD mode")
    parser_ibd.add_argument("--exe", default="IBD", 
                                         choices=IBD_GENERATOR_EXEC.keys(),
                                         help="select the Generator to run")
    parser_ibd.add_argument("--material", default="LS", help="material")
    parser_ibd.add_argument("--volume", default="pTarget", 
                                        choices=["pTarget"],
                                        help="Volume name")
    pass
def setup_generator_ibd(gt, args):
    from GenTools import makeTV
    gun = gt.createTool("GtHepEvtGenTool")
    gun.property("Source").set(
            IBD_GENERATOR_EXEC[args.exe].format(SEED=args.seed,
                                            EVENT=args.evtmax)
            )
    gt.property("GenToolNames").append([gun.objName()])
    # == position related ==
    gun_pos = gt.createTool("GtPositionerTool")
    gun_pos.property("PositionMode").set("Random")
    gun_pos.property("GenInVolume").set(args.volume)
    gun_pos.property("Material").set(args.material)
    gt.property("GenToolNames").append([gun_pos.objName()])

#############################################################################
def add_subparser_b12(parser):
    parser_b12 = parser.add_parser("b12", 
            formatter_class=argparse.ArgumentDefaultsHelpFormatter,
            help="B12 mode (or GenDecay Mode)")
    parser_b12.add_argument("--exe", default="B-12", 
                                         help="select the Generator to run")
    parser_b12.add_argument("--material", default="LS", help="material")
    parser_b12.add_argument("--volume", default="pTarget", 
                                        choices=["pTarget"],
                                        help="Volume name")
    pass
def setup_generator_b12(gt, args):
    # == gendecay related ==
    Sniper.loadDll("libGenDecay.so")
    era = gt.createTool("GtDecayerator")
    era.property("ParentNuclide").set(args.exe)
    era.property("CorrelationTime").set(DECAY_DATA[args.exe])
    era.property("ParentAbundance").set(5e16)
    gt.property("GenToolNames").append([era.objName()])
    # == position related ==
    gun_pos = gt.createTool("GtPositionerTool")
    gun_pos.property("PositionMode").set("Random")
    gun_pos.property("GenInVolume").set(args.volume)
    gun_pos.property("Material").set(args.material)
    gt.property("GenToolNames").append([gun_pos.objName()])

#############################################################################
def add_subparser_neutron(parser):
    parser_neutron = parser.add_parser("neutron", 
            formatter_class=argparse.ArgumentDefaultsHelpFormatter,
            help="Neutron mode")
    parser_neutron.add_argument("--momentum", default=0.05, type=float,
                                        help="momentum (MeV)")
    parser_neutron.add_argument("--material", default="LS", help="material")
    parser_neutron.add_argument("--volume", default="pTarget", 
                                        choices=["pTarget"],
                                        help="Volume name")
def setup_generator_neutron(gt, args):
    gun = gt.createTool("GtGunGenTool/gun")
    gun.property("particleNames").set("neutron")
    gun.property("particleMomentums").set(args.momentum)
    gt.property("GenToolNames").append([gun.objName()])
    # == position related ==
    gun_pos = gt.createTool("GtPositionerTool")
    gun_pos.property("PositionMode").set("Random")
    gun_pos.property("GenInVolume").set(args.volume)
    gun_pos.property("Material").set(args.material)
    gt.property("GenToolNames").append([gun_pos.objName()])

