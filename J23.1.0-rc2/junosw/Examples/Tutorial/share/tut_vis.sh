#!/bin/sh
# a script to run the chain job of simulation-calibration-reconstruction-eventdisplay
# author: Zhengyun You

EVT=3

python tut_detsim.py --gdml --no-pmt3inch --evtmax $EVT gun --volume pTarget
#python tut_detsim.py --gdml --evtmax $EVT gun --volume pTarget
python tut_det2calib.py --evtmax $EVT
python tut_calib2rec.py --gdml --evtmax $EVT
python tut_elec2rec.py --evtmax $EVT
serena.exe
#serena.exe --geom=sample_detsim.root --sim=sample_detsim.root --calib=sample_calib.root --rec=sample_rec.root [--simus=sample_detsim_user_op.root]

#  export JUNO_GEOMETRY_PATH=/public/data/wuchx/juno_offline/Identifier/wuchx-J23-Identifier-mergy/data/Detector/Geometry


#The following part will write out photon path in detsim_user file for visualization in event display (the log file is huge)

#python tut_detsim.py --loglevel Info --anamgr-photon-tracking --gdml --no-pmt3inch --evtmax $EVT gun --volume pTarget >& detsim.log
#python tut_det2calib.py --evtmax $EVT
#python tut_calib2rec.py --evtmax $EVT
#ln -sf sample_detsim_user.root sample_detsim_user_op.root
#serena.exe

