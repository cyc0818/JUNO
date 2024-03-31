#!/usr/bin/env python
# -*- coding:utf-8 -*-
# author: lintao

import Sniper
import argparse

def get_parser():
    parser = argparse.ArgumentParser(description='MCC15A PMTRec (convert SimEvent to CalibEvent).',
                        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("--evtmax", type=int, default=10, help='events to be processed')
    parser.add_argument("--seed", type=int, default=42, help='seed')
    parser.add_argument("--restore-seed-status", default=None, 
                                         help='restore the random engine, both '
                                         'a list of integer or a file contains '
                                         'the list of integer is supported. '
                                         'such as:'
                                         '   --restore-seed-status "1,2,3..."')
    parser.add_argument("--sim-output", default="sample_simsplit.root",
                                        help='output the split SimEvent.')
    parser.add_argument("--calib-output", default="sample_calib.root",
                                        help='output the CalibEvent.')
    parser.add_argument("--input", default="sample_detsim.root",
                                        help='input the SimEvent')
    # == Split Alg ==
    parser.add_argument("--split-gap-time", default=1000, 
                                        type=float,
                                        help="split gap time (ns)")
    # == PMTRec ==
    parser.add_argument("--qe-smear", dest="smearqe", action="store_true",
                                      help="Enable QE smear")
    parser.add_argument("--no-qe-smear", dest="smearqe", action="store_false",
                                      help="Disable QE smear")
    parser.set_defaults(smearqe=True)
    parser.add_argument("--smear-file", default=None,
                                      help="The root file to do QE smear.")

    return parser
