#!/usr/bin/env python
#-*- coding: utf-8 -*-
# Author: Jie Cheng <chengjie AT ncepu.edu.cn>

import argparse
import Sniper
import os

def get_parsers():
    parser = argparse.ArgumentParser(description="GENIE gAtmoEvGen in SNiPER")
    # here, we keep the similar interface to GENIE
    # below is an example from Jie Cheng:
    # Settings: flux: 3D Honda flux; xsec: G18_10b_02_11b; target: JUNO-LS; neutrino initial energy range: 0.1-20 GeV
    #
    #       -n 120000
    #       --flux HAKKM:/cvmfs/juno.ihep.ac.cn/sw/GENIE-MC/input-data/groups/atmo-ihep/juno-ally-20-12-solmax.txt[12],/cvmfs/juno.ihep.ac.cn/sw/GENIE-MC/input-data/groups/atmo-ihep/juno-ally-20-12-solmax.txt[14],/cvmfs/juno.ihep.ac.cn/sw/GENIE-MC/input-data/groups/atmo-ihep/juno-ally-20-12-solmax.txt[-12],/cvmfs/juno.ihep.ac.cn/sw/GENIE-MC/input-data/groups/atmo-ihep/juno-ally-20-12-solmax.txt[-14]
    #       --tar-tag 0
    #       --ev-min 0.1
    #       --ev-max 20
    #       --run 100000101
    #       --seed 154748252
    #       --cross-sections /junofs/users/chengjie/workdir/J22.2.x/Generator/GenGenie/xsec/v3_02_00/G18_10b_02_11b.xml
    #       --tune G18_10b_02_11b

    parser.add_argument("-n", "--evtmax", type=int, default=10, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--run", type=int, default=42, help='run')
    parser.add_argument("--output", default="sample.root", help="output file name, including the EDM")
    parser.add_argument("--user-output", default="user_sample.root", help="output file name, including the user tree")

    parser.add_argument("--tar-tag", type=int, default=0, help="0:LS; 1:Water; 2:Rock")
    parser.add_argument("--ev-min", type=float, default=0.1, help="minimal initial neutrino energy, unit: GeV")
    parser.add_argument("--ev-max", type=float, default=20, help="maximal initial neutrino energy, unit: GeV")

    parser.add_argument("-t", "--targets", default="", help="pdg1[weight1],pdg2[weight2],...")
    ###flux input format###, 3D honda flux e.g.,
    # --flux HAKKM:.../juno-ally-20-12-solmax.d[12],.../juno-ally-20-12-solmax.d[14],.../juno-ally-20-12-solmax.d[-12],.../juno-ally-20-12-solmax.d[-14]
    ###flux input format###, 1D honda flux e.g.,
    # --flux /junofs/users/chengjie/workdir/atmgen-202203/data/flux/hondaflux/hist.root,14[fluxgnumumax],-14[fluxgnumubarmax],12[fluxgnuemax],-12[fluxgnuebarmax]
    ###If you wish to use you own flux, please prepare it in the same way as the samples above.

    parser.add_argument("-f", "--flux", default="",  help="flux")
    parser.add_argument("--cross-sections", default="", help="cross section")
    parser.add_argument("--tune", default="", help="genie tune.")
    
    parser.add_argument("--enable-isotropic", action="store_true", dest="isotropic", help="If 1D hist flux is used, neutrino direction is isotropic")
    parser.add_argument("--disable-isotropic", action="store_false", dest="isotropic", help="If 1D hist flux is used, neutrino direction is (0,0,z) (Default)")
    parser.set_defaults(isotropic=False)
    parser.add_argument("--user-flux-frac", default="", help="if --enable-user-flux is chosen, specify the fractions of input 1D hist neutrino fluxes (please follow the order of neutrino flavors in --flux ), e.g., --user-flux-frac weight1,weight2,...")

    parser.add_argument("--event-generator-list", default="Default", help="Specifies the list of event generators to use in the MC job. e.g. --event-generator-list CC(NC or other choices, see $GENIE/config/EventGeneratorListAssembler.xml for more details)")
    
    parser.add_argument("--enable-deex", action="store_true", dest="deex", help="Turn on the deexcitation process")
    parser.add_argument("--disable-deex", action="store_false", dest="deex", help="Turn off the deexcitation process (Default)")
    parser.set_defaults(deex=False)

    return parser

def help_deex(args,gt):
    import GenTools
    deex = gt.createTool("GtGenDeexTool")
    JUNOTOPPath = os.getenv('JUNOTOP')
    deex.property("TalysDir").set(JUNOTOPPath+"/data/Generator/AtmNC/data/talys_sample")
    gt.property("GenToolNames").append([deex.objName()])

def gevgen_atmo(args,task):
    # = gen tools =
    import GenTools
    gt = task.createAlg("GenTools")

    Sniper.loadDll("libGenGenie.so")

    genie_evgen = gt.createTool("GtGenieAtmoEvGenTool/gun")

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
        help_deex(args, gt)

if __name__ == "__main__":

    parser = get_parsers()
    args = parser.parse_args()

    task = Sniper.Task("task")
    task.setEvtMax(args.evtmax)
    task.setLogLevel(0)

    # = Data Buffer Mgr =
    import RootIOSvc
    import BufferMemMgr
    bufMgr = task.createSvc("BufferMemMgr")
    readout=task.createSvc("RootOutputSvc/OutputSvc")
    outputdata={"/Event/Gen": args.output}
    readout.property("OutputStreams").set(outputdata)

    # = random svc =
    import RandomSvc
    task.property("svcs").append("RandomSvc")
    rndm = task.find("RandomSvc")
    rndm.property("Seed").set(args.seed)

    # = root writer =
    import RootWriter
    rootwriter = task.createSvc("RootWriter")

    # rootwriter.property("Output").set({"SIMEVT":"genie_user.root"})
    rootwriter.property("Output").set({"SIMEVT": args.user_output})
    
    gevgen_atmo(args,task)

    task.run()
