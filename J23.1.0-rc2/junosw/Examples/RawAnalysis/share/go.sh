#!/bin/bash

#-----------------------------------------------------------------
#   Introduction
#-----------------------------------------------------------------
#   raw2rtraw.py:
#       Description: Get Raw_Events from GCU or DAQ input test file and output. Only support single input file now.
#       Important options:
#           required:   --type: input file type: GCU or DAQ
#           optional:   --output-elec: convert Raw_Events to offline Elec_Events and output to rtraw file
#                       --output-root: output Raw_Events to root file
#               use neither and there will be no output file, only to check the raw file.
#
#   gcu_evt_build.py:
#       Description: Generate an offline elec rtraw file from multiple GCU input test files.
#
#-----------------------------------------------------------------
#   Examples
#-----------------------------------------------------------------
#   process a single input test file:
#       exec python raw2rtraw.py --input xx --type xx --output-elec xx --output-root xx --evtmax x
#
#       DAQ example:
exec python raw2rtraw.py --input /afs/ihep.ac.cn/users/z/zhaoth/work/bak/data/0415/DS.1.JUNODAQ.test.ds-1.20230415163323.003.dat --type DAQ --output-root singleDAQ --evtmax 10
#
#       GCU example:
#exec python raw2rtraw.py --input /junofs/users/xiexc82/daq_data_2023-3-21/1_0_03-21-0249_1008_24_- --type GCU --output-root singleGCU --evtmax -1
#
#   process a data.list of GCU test files:
#exec python gcu_evt_build.py --input data.list --output-root events --evtmax -1



