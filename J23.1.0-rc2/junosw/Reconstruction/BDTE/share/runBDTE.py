from BDTE import Reco
from BDTE import SELECTED_FEATURES, Mode, Config
from SniperHelper import SNiPERLogLevel

import os

from argparse import ArgumentParser

INFERENCE="inference"
TRAINING="training"
CONTROL="control"

def string_to_mode(string: str) -> int:
    if(string == INFERENCE): return Mode.INFERENCE
    elif(string == TRAINING): return Mode.TRAINING
    elif(string == CONTROL): return Mode.CONTROL
    else: return -1

if __name__ == "__main__":

    def get_parser() -> ArgumentParser:
        parser = ArgumentParser(description="Implementation of the BDTE algorithm")

        parser.add_argument("--input", default="sample_calib.root", nargs='+', help="input file name")
        parser.add_argument("--output", default="sample_rec.root", help="output file name")
        parser.add_argument("--verbose", default=SNiPERLogLevel.INFO, help="verbosity", type=int, choices=[
                SNiPERLogLevel.TEST,
                SNiPERLogLevel.DEBUG,
                SNiPERLogLevel.INFO,
                SNiPERLogLevel.EROOR,
                SNiPERLogLevel.FATAL,
            ])
        parser.add_argument("--mode", default=INFERENCE, help=f"Algorithm mode", choices=[INFERENCE, TRAINING, CONTROL])
        parser.add_argument("--model", default="test_model.json", help="Model path")


        return parser

    parser = get_parser()
    args = parser.parse_args()

    if(args.mode != INFERENCE and args.mode != TRAINING and args.mode != CONTROL):
        raise ValueError(f"Invalid value for `--mode` option. Should be `{INFERENCE}` or `{TRAINING}` or `{CONTROL}` but got {args.mode}")

    import Sniper

    task = Sniper.Task("BDTE")
    task.setLogLevel(args.verbose)

    # Step 2 : Create the memory manager service (SNIPER too to manage memory)
    import BufferMemMgr
    buffer_manager_svc = task.createSvc("BufferMemMgr")
    buffer_manager_svc.property("TimeWindow").set([0, 0])

    # Step 3 : Create the Input svc
    import RootIOSvc
    input_svc = task.createSvc("RootInputSvc/InputSvc")
    input_svc.property("InputFile").set(args.input)

    # Step 4 : Create the output svc
    import RootWriter
    Sniper.loadDll("libCalibEvent.so")
    Sniper.loadDll("libRecEvent.so")
    output_svc = task.createSvc("RootOutputSvc/OutputSvc")

    # Those output streams are standardised name, corresponding to the classes
    # {OS_NAME}Header and {OS_NAME}Evt
    if string_to_mode(args.mode) != Mode.CONTROL:
        output_svc.property("OutputStreams").set({
            "/Event/Gen": args.output,
            "/Event/Sim": args.output,
            "/Event/CdSpmtCalib": args.output,
            "/Event/CdLpmtCalib": args.output,
            "/Event/CdVertexRec": args.output,
        })

    # Step 5 : Import Geometry

    gdml_filename = "default" # Trust default on this one
    geom_path_inroot = "JunoGeom"

    import Geometry
    geom_svc = task.createSvc("RecGeomSvc")
    geom_svc.property("GeomFile").set(gdml_filename)
    geom_svc.property("GeomPathInRoot").set(geom_path_inroot)
    geom_svc.property("FastInit").set(True)

    # Create a python store
    import SniperPython
    task.createSvc("PyDataStoreSvc/DataStore")

    # Create EdmReader
    import BDTE
    task.createAlg("EdmReader");

    bdte_reco = BDTE.Reco("Reco", Config(path = args.model, optimal_features = SELECTED_FEATURES), mode=string_to_mode(args.mode));
    task.addAlg(bdte_reco)

    if(string_to_mode(args.mode) == Mode.INFERENCE):
        task.createAlg("EdmWriter");

    task.show()
    task.run()
