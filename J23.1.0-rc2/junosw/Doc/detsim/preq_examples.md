# A quick guide to get started with JUNO offline software

In this guide, we will learn some basic commands used in our software. For detail of each part, you need to read every part's document or code.

## Environment Setup

The JUNO software is installed under a site-specific directory. For example, if you are using SLC 6:

* For IHEP, the directory is `/afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6`
* For CNAF, the directory is `/opt/exp_software/juno/JUNO-ALL-SLC6`.

For example, you want to use a stable version `J17v1r1` at IHEP:

    $ source /afs/ihep.ac.cn/soft/juno/JUNO-ALL-SLC6/Release/J17v1r1/setup.sh
    Setup Official Offline Software

If you are using a virtual machine, you can setup the `trunk` version of offline as following:

    $ source ~/juno-dev/setup.sh 
    Setup Official Offline Software


## Detector Simulation

Run detector simulation using particle gun:

    $ python $TUTORIALROOT/share/tut_detsim.py --evtmax 10 gun

Please check file `sample_detsim.root` is created under your current directory.

## Electronics Simulation

Run electronics simulation:

    $ python $TUTORIALROOT/share/tut_det2elec.py --evtmax 10

Please check file `sample_elecsim.root` is created under your current directory.

## Waveform Reconstruction

Run waveform reconstruction:

    $ python $TUTORIALROOT/share/tut_elec2calib.py --evtmax 10

Please check file `sample_calib.root` is created under your current directory.

## Vertex/Energy Reconstruction

    $ python $TUTORIALROOT/share/tut_calib2rec.py --evtmax 10

Please check file `sample_rec.root` is created under your current directory.

## Event Display

Start event display, but please wait due to too many PMTs:

    $ serena.exe
    Vis creating...
    VisClient initialize...
    InitJVis...
    initStyle
    initStatus
    initGeom
      init CdGeom
    Info in <TGeoNavigator::BuildCache>: --- Maximum geometry depth set to 100
    m_geom: 0x25e3430
    nPmt 54311 nPmt20inch 17739 nPmt3inch 36572
    centerDet printPmt begin 0
    centerDEt printPmt end 0
    initCdInfo begins
    initCdInfo end
    centerDet printPmt begin 0
    centerDEt printPmt end 0
    initEvtMgr
      init JVisEvtMgr 
    name sample_detsim.root
    Open m_simFile sample_detsim.root
    Open m_simEventTree SimEvent entries: 10
    name sample_calib.root
    Open m_calibFile sample_calib.root
    Open m_calibEventTree CalibEvent entries: 10
    name sample_rec.root
    Open m_recFile sample_rec.root
    Open m_recEventTree RecHeader entries: 10
    hasSim   1
    hasCalib 1
    hasRec   1
    initialize JVisEvtMgr successfully 
    initHisto
    initEve
    initEveGeom
    initEvePmt
    Digit Size : 54311
    initEvePmt MainWin name fEveBrowser3
    initEveRec

After initialization, you will see following:

![Fig. Event Display after initialization](detsim/figs/serena-init.png)

You could use your mouse to control view of detector.

Then, click in event control, will load an event. You could also view by time or charge:

![Fig. Event Display of one event](detsim/figs/serena-one-event.png)

You could also use 2D view, by clicking **2D Projection**:

![Fig. Event Display of one event](detsim/figs/serena-one-event-2D.png)
