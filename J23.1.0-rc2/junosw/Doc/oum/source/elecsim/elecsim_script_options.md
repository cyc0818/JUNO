# How to write a electronics simulation script?

You can you the following command to run elecsim:

    python $TUTORIALROOT/share/tut_det2elec.py --input det_sample.root

The `det_sample.root` is the input file, a detsim EDM file. This command just use the default configuration of elecsim. You can use the following option to write your own elecsim script.


## The useful options in electronics simulation

### common options

* `--run-number`: run id of the electronics simulation, default:0 
* `--start-event-number`: start event id of the electronics simulation, default:0
* `--evtmax`: the event number be simulated, `--evtmax -1` for simulate all the event, default:-1
* `--loglevel`: log level for elecsim,`Test`,`Debug`,`Info`,`Warn`,`Error` and `Fatal` can be used.`Test` output the most information and `Fatal` output the least information, default:`Info`
* `--output`: output file name (Event Data Model).
* `--user-output`: output user data file name.
* `--seed`: seed for the random number used in electronics simulation. default:5
* `--start`: start time of electronics simulation,timeStamp of event in elecsim is start from this time. format:`yyyy-mm-ddTHH:MM:SS`, default:`1970-01-01 00:00:00`
* `--end`: end time of electronics simualtion, if the timeStamp of next loaded detsim event beyond the end time, the elecsim will end. format:`yyyy-mm-ddTHH:MM:SS`, default:`2014-09-01 12:05:00`
* `--rate`:event rate for tag, unit:Hz example: `IBD:1`, default:1
* `--startidx`: ElecSim load DetSim event from this eventid in input file, example: `IBD:1`, default:0
* `--loop`: Whether circularly readin for the TAG, 0 for disable loop, and 1 for enable loop, example `IBD:0`, default:0
* `--splitted-input`: whether to support splitted detsim event as elecsim input, example: `Muon:1`. Default is non-splitted input.
* `--TotalLpmt`: CD LPMT number used in elecsim,a useless option, please ingore it
* `--enableLPMT`/`--disableLPMT`: enableLPMT/disableLPMT in the electronics simulation default:`enableLPMT`
* `--enableSPMT`/`--enableSPMT`: enableSPMT/disableSPMT in the electronics simulation default:`disableLPMT`
* `--enableTT`/`--disableTT`: enable/disable Top-Tracker elecrtronics simulation, default:`enableTT`
* `--enableWP`/`--disableWP`: enable/disable Water Pool electronics simulation, default:`disableWP`
* `--enableSNMode`/`--disableSNMode`: enable/disable super-nova electronics simulation,if you want to simulate super-nova event, you shoud add `--enableSNMode` in your script default:`disableSNMode`
* `--enableUserRootFile`/`--disableUserRootFile`: two useless option, please ingore it
* `--evtBufferLength`: time length for pulse buffer in electronics simulation, default:5000
* `--nHitsThreshold`: the threshold for cd pre-trigger, default:200
* `--enableOEC`/`--disenableOEC`: whether enable elecsim invoked by OEC. default:`disenableOEC`

### options about PMT response

* `--darkRateScale`: a scale factor for CD LPMT dark pulse count(DCR),default:1.0
* `--darkRateScale_SPMT`: a scale factor for CD SPMT dark pulse count(DCR),default:1.0
* `--darkRateScale_WP`: a scale factor for WP LPMT dark pulse count(DCR),default:1.0
* `--enableAfterPulse`/`--disableAfterPulse`: enable/disable the generation of WP CD LPMT after pulse, default:`enableAfterPulse`
* `--enableMergeLPMTPulse`/`--disableMergeLPMTPulse`: whether to merge the CD WP LPMT pulses within 1ns, default:`disableMergeLPMTPulse`
* `--enableDarkPulse`/`--disableDarkPulse`: enable/disable the generation of CD LPMT dark pulse, default:`enableDarkPulse`
* `--enableEfficiency`/`--disableEfficiency`: two useless option,please ingore it.
* `--enablePmtTTS`/`--disablePmtTTS`: enable/disable the TTS(transient time spread) of CD LPMT, default:`enablePmtTTS`
* `--enableDynodeTTS2theta`/`--disableDynodeTTS2theta`: enable/disable Hamamatsu PMT TTS change with the photoelectron incident angle, default:`enableDynodeTTS2theta`
* `--enableMCPTTS2theta`/`--disableMCPTTS2theta`: enable/disable MCP PMT TTS change with the photoelectron incident angle, default:`enableDynodeMCP2theta`
* `--enableDynodeTT2theta`/`--disableDynodeTT2theta`: enable/disable Hamamatsu PMT TT(ransient time) change with the photoelectron incident angle, default:`enableDynodeTT2theta`
* `--enableMCPTT2theta`/`--disableMCPTT2theta`: enable/disable MCP PMT TT(ransient time) change with the photoelectron incident angle, default:`enableMCPTT2theta`
* `--enableWPDarkPulse`/`--disableWPDarkPulse`: enable/disable the generation of Water Pool LPMT dark pulse, default:`disableWPDarkPulse`
* `--enableMCPLargeSignal`/`--disableMCPLargeSignal`: whether to simulate the large signal of MCP PMT, default:`enableMCPLargeSignal`
* `--enableChargeSmearing`/`--disableChargeSmearing`: enableChargeSmearing/disableChargeSmearing the charge spectrum, default:`enableChargeSmearing`

### option about trigger simulation

* `--LpmtTrigger_alg`: the option to select CD LPMT trigger mode, `MC-based`,`real-logic` and `MM-trigger` can be used. default:`MC-based`
* `--WPLpmtTrigger_alg`: the option to select WP trigger mode,`WP-logic` can be used. default:`WP-logic`
* `--enableForceTrigger`/`--disableForceTrigger`: whether to use force trigger mode, default:`disableForceTrigger`
* `--enableTriggerByGroup`/`--disableTriggerByGroup`: two useless options, please ingore it.
* `--Trigger_FiredPmtNum`: trigger threshold for MC-based trigger(nPMT),default:200
* `--Trigger_window`: trigger window for MC-based trigger(ns), default:80
* `--Trigger_slip_window`: length of an electronics clock cycle(ns) for MC-based trigger, (slipping length of each trigger step). default:16
* `--StdTrigger_FiredPmtNum`: trigger threshold for standard majority trigger(nPMT),default:300
* `--StdTrigger_window`: trigger window for standard majority trigger(ns),default:300
* `--StdTrigger_slip_window`: length of an electronics clock cycle(ns) for standard majority trigger, (slipping length of each trigger step). default:16 
* `--VFLTrigger_FiredPmtNum`: trigger threshold for VFL trigger(nPMT). default:100
* `--VFLTrigger_window`: trigger window for standard majority trigger(ns),default:48
* `--VFLTrigger_slip_window`: length of an electronics clock cycle(ns) for VFL trigger, (slipping length of each trigger step). default:16
* `--Trigger_Mode`: trigger mode for real-logic trigger, `onlyStd`,`onlyVFL` and `OR-ed` can be used, default:`OR-ed`
* `--Trigger_FiredPmtNum_WP`: trigger threshold for single region in water pool(nPMT),default:17
* `--Trigger_FiredPmtNum_WP_low`: trigger low threshold for adjacent region in water pool,default:13
* `--Trigger_window_WP`: trigger window for water pool trigger(ns),default:240
* `--Trigger_slip_window_WP`: length of an electronics clock cycle(ns) for water pool trigger, (slipping length of each trigger step). default:16
* `--enableTriggerSPMTbyLPMT`/`--disableTriggerSPMTbyLPMT`: whether trigger SPMT by LPMT. default:`enableTriggerSPMTbyLPMT`
`--enableTimeOffsetCorrection`/`--disableTimeOffsetCorrection`: whether to do the calibration time offset correction during VFL trigger,default:`disableTimeOffsetCorrection`

### options about electronics response and readout

* `--enableOvershoot`/`--disableOvershoot`: whether to consider the overshoot of waveform, default:`enableOvershoot`
* `--enableSaturation`/`--disableSaturation`: whether to consider the saturation of waveform amplitudes, default:`enableSaturation`
* `--enableNoise`/`--disableNoise`: whether add electronics white noise. default:`enableNoise`
* `--enableFADC`/`--disableFADC`: whether to use FADC to digitize the waveform. default:`enableFADC`
* `--enableCDLpmtTQ`/`--disableCDLpmtTQ`: wherther to do CD LPMT FPGA simulation and output the TQ information. default:`enableCDLpmtTQ`
* `--preTimeTolerance`/`--postTimeTolerance`: readout window: [trigger time - preTimeTolerance,trigger time + postTimeTolerance]. default:[trigger time - 300,trigger time + 700]
* `--FadcOffset`: Offset of waveform baseline (percent of full range), default:0.25

### option for laser trigger(for calibration)

* `--enableLaserTrigger`/`--disableLaserTrigger`: whether to use laser trigger mode, default:`disableLaserTrigger`
* `--enableMultiLaserLoad`/`--disableMultiLaserLoad`:  , default:`disableMultiLaserLoad`
* `--nLaserEventLoad`: number of laser events loaded into one timing window. default:1
* `--enableGen`/`--disableGen`: whether to store the generator information, default:`enableGen`

### option about MC-Truth storing

* `--savetruths`: whether to save TAG electronics simulation truths, example `IBD:1`, default:1
* `--enableSaveDarkPulseTruths`/`--disableSaveDarkPulseTruths`: whether to save the pulse-level dark pulse truth, default:`enableSaveDarkPulseTruths`
* `--enableSaveTruths`/`--disableSaveTruths`: whether to save all the truth, default:`enableSaveTruths`
* `--enableSplitTrackRange`/`--disableSplitTrackRange`: whether to store track-level truth just whithin a time range. default:`disableSplitTrackRange`
* `--enableStoreElecTruthEDM`/`--disableStoreElecTruthEDM`: whether to store the ElecTruthEDM of CD LPMT. default:`enableStoreElecTruthEDM`

## Write an event-mixing script

Use the above options, we can write an simple event-mixing elecsim script, for example:

    python $TUTORIALROOT/share/tut_det2elec.py  --input IBD:ibd_detsim.root --rate IBD:2 --input Muon:muon_detsim.root --rate Muon:28 --splitted-input Muon:1

This script mix the IBD and Muon event in the elecsim. The rate for IBD is 2 Hz and rate for Muon is 28 Hz. As the output of muon is splitted, the option `--splitted-input Muon:1` is used to let elecsim know the whether load data using merging input service or not. More types of samples can be mixed use the `--input` and `--rate` options, you can mix in more event as you need.
