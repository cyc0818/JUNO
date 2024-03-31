# Analyze user data
You should already know how to run detector simulation. In this part, we will explain the meaning of each variable in user data. You could also read analysis code for more information.

## `evt` tree
In this tree, every entry represents one detector simulation event. 

This detector simulation event is a little different from electronics/trigger event. For example, there is an IBD event (a positron and a neutron) in detector simulation, but there will be two trigger events. The relationship could be complicated when event is split.

* `evtID`: event ID. For each simulation job, it starts from 0.
* `edep`: the total deposited energy in this event.
* `edepX`, `edepY` and `edepZ`: deposited position.
* `nPhotons`: length of hits in this event. 
    * Note, for one hit, it could be merged by several PEs in the same time window.
* `totalPE`: total PE numbers in this event.
    * If `PEs` are not merged, `nPhotons` is equal to `totalPE`.
* `LpmtPE`: PE number of large PMT (NNVT and Hamamastu)
* `SpmtPE`: PE number of small PMT
* `NNVTPE`: PE number of NNVT
* `HamaPE`: PE number of Hama
* `CDPE`  : PE number of central detector
* `WPPE`  : PE number of water pool

The above data is in event level. You should add option `--anamgr-normal-hit` in your simulation script if you want to know about hit level information.
```python
python $TUTORIALROOT/share/tut_detsim.py  --anamgr-normal-hit gun
```  
The below is hit level information
* `energy`: optical photon's energy.
* `hitTime`: hit time.
* `pmtID`: each PE's pmt ID
* `PETrackID`: the primary track ID of this optical photon.
* `isCerenkov`: is the original optical photon a Cerenkov light or not.
* `isReemission`: is photon re-emitted during propagation.
* `isOriginalOP`: is photon the original photon without any reemission. 
* `OriginalOPTime`: the original photon's generation time.
* `nPMTs`: fired PMT number.
* `nPE_byPMT`: number of PEs at each PMT.
* `PMTID_byPMT`: the PMT ID.
* `LocalPosX`, `LocalPosY`, `LocalPosZ`: position of photon hit the PMT surface.
* `LocalDirX`, `LocalDirY`, `LocalDirZ`: direction of photon hit the PMT surface.

## `geninfo` tree

* `evtID`: event ID. For each simulation job, it starts from 0.
* `nInitParticles`: number of primary particles.
* `InitPDGID`: PDG code for each particle.
* `InitTRKID`: Track ID in Geant4.
* `InitX`, `InitY`,`InitZ`: initial position.
* `InitPX`, `InitPY`,`InitPZ`: initial direction.
* `InitMass`: initial particle's mass.
* `InitTime`: initial time.
* `ExitX`, `ExitY`, `ExitZ`: position when particle is died or stopped tracking.
* `ExitPX`, `ExitPY`, `ExitPZ`: momentum when particle is died or stopped tracking.
* `ExitT`: stop time.
* `TrackLength`: track length.

## `prmtrkdep` tree
* `evtID`: event ID. For each simulation job, it starts from 0.
* `edep`: deposited energy of each primary particle and its secondaries.
* `edepX`, `edepY`,`edepZ`: X, Y, Z position of deposited energy of each primary particle and its secondaries.
* `Qedep`: quenching energy of each primary particle and its secondaries.
* `QedepX`, `QedepY`,`QedepZ`: X, Y, Z position of quenching energy of each primary particle and its secondaries.
	By default, the energies of secondaries are accumulated to the primary tracks.If the track is also traced by the simtracksvc, then we don't accumulate the energies to the primary tracks. In addition to the primary track we traced, we also traced the secondaries of two physical processes: neutron capture and decay. These two processes name in Geant4 are `nCapture` and `Decay`.
* `edepNotInLS`: deposited energy which are not in LS.

## `nCapture` tree
This tree saves the information of the neutron capture process including the neutron track and its secondaries gamma and deuteron.
* `evtID`: event ID. For each simulation job, it starts from 0.
* `NeutronTrkid`: Neutron track ID
* `NeutronKine`: Initial kinetic energy of neutron 
* `NeutronCaptureT`: Neutron capture time (global time).
* `NCStartX`,`NCStartY`,`NCStartZ`: position where the neutron starts.
* `NCStopX`,`NCStopY`,`NCStopZ`: position where the neutron stops.
* `NCTrackLength`: track length of neutron track.
* `trkid`: parent track ID of these secondaries (aka the neutron track ID).
* `pdgid`: PDG of secondary particles (aka gamma and deuteron).
* `kine` : initial kinetic energy of secondary particles
* `px`,`py`,`pz`:initial momentum of secondary particles
* `x`,`y`,`z`,`t`: initial position and time of secondary particles 


## `michael` tree
However, the name of this tree may be misleading. it should be `michel` tree.
This tree saves the information of Michel electron. If you want to know more about the details of Michel electron , please browse website: `https://hsm.stackexchange.com/questions/2157/how-did-the-term-michel-electron-come-about`. 
* `evtID`: event ID. For each simulation job, it starts from 0.
* `pdgid`: PDG of Michel electron.
* `kine` : initial kinetic energy of Michel electron
* `px`,`py`,`pz`:initial momentum of secondary particles
* `x`,`y`,`z`,`t`: initial position and time of secondary particles 


## `radio` tree
This tree saves the radioactive decay information.
let's understand these varible we saved in this tree through a example as follow:

```bash
root [3] radio->Scan()
***********************************************************************************************************************
*    Row   * Instance * evtID.evt *  radioidx *     trkid *  parentid *     pdgid *      name *      time *        Ek *
***********************************************************************************************************************
*        0 *        0 *         0 *           *           *           *           *           *           *           *
*        1 *        0 *         1 *           *           *           *           *           *           *           *
*        2 *        0 *         2 *         0 *   9462704 *   9462690 * 1.000e+09 *       C11 * 9.729e+11 *         0 *
*        2 *        1 *         2 *         0 *         0 *   9462704 * 1.000e+09 *       B11 * 9.729e+11 * 4.474e-05 *
*        2 *        2 *         2 *         0 *         0 *   9462704 *        12 *      nu_e * 9.729e+11 * 0.9048269 *
*        2 *        3 *         2 *         0 *         0 *   9462704 *       -11 *        e+ * 9.729e+11 * 0.0555424 *
*        3 *        0 *         3 *         0 *    261123 *    260922 * 1.000e+09 *       N13 * 5.278e+11 *         0 *
*        3 *        1 *         3 *         0 *         0 *    261123 * 1.000e+09 *       C13 * 5.278e+11 * 2.725e-05 *
*        3 *        2 *         3 *         0 *         0 *    261123 *        12 *      nu_e * 5.278e+11 * 0.6669805 *
*        3 *        3 *         3 *         0 *         0 *    261123 *       -11 *        e+ * 5.278e+11 * 0.5314663 *
*        4 *        0 *         4 *           *           *           *           *           *           *           *
*        5 *        0 *         5 *           *           *           *           *           *           *           *
*        6 *        0 *         6 *           *           *           *           *           *           *           *
*        7 *        0 *         7 *         0 *  13299155 *  13299128 * 1.000e+09 *      Al28 * 3.927e+11 *         0 *
*        7 *        1 *         7 *         0 *         0 *  13299155 * 1.000e+09 * Si28[1779 * 3.927e+11 * 8.625e-05 *
*        7 *        2 *         7 *         0 *         0 *  13299155 *       -12 * anti_nu_e * 3.927e+11 * 1.7381457 *
*        7 *        3 *         7 *         0 *         0 *  13299155 *        11 *        e- * 3.927e+11 * 1.1250020 *
*        7 *        4 *         7 *         1 *  13299156 *  13299155 * 1.000e+09 * Si28[1779 * 3.927e+11 *         0 *
*        7 *        5 *         7 *         1 *         0 *  13299156 * 1.000e+09 *      Si28 * 3.927e+11 * 6.073e-05 *
*        7 *        6 *         7 *         1 *         0 *  13299156 *        22 *     gamma * 3.927e+11 * 1.7789692 *
```
`evtID` means event ID and it starts from 0. From the above table, you can find there is no information when `evtID = 0,1,4,5,6`, which means that there is no radiactive decay in these events. When `evtID = 2`, there are 4 rows and the particles name are `C11, B11, nu_e, e+`. These 4 rows represent a radioactive decay process `C11 --> B11 + nu_e + e+`. Also, you can see that only `C11` has trkid (aka track ID ) value in these 4 rows, which implicity means that `C11` is parent nucleus, and others are daughter nucleus.  

So the `radioidx` is a tag. Particles which have same `radioidx` means they are from one radiodecay process in this event. `trkid` is track ID of the particle (note that only the track ID of parent nucleus is saved the others are set to be 0). `parentid` means the parent ID of this particle. `time` and `Ek` are the time point and kinetic energy of parent and daughter nucleus  when radioactive decay happend. 

Therefore we can find the radioactive processes `C11 --> B11 + nu_e + e+` in `eventID = 2` ; `N13 --> C13 + nu_e + e+` in `eventID = 3`; `Al28 --> Si28[1779] + anti_nu_e + e-` and `Si28[1779] --> Si28 + gamma` in `eventID = 7`.


## `opticalparam` tree
This tree saves optical parameters information we used. 

## How to disable or enable these trees.
Although we can list all the options for each tree we mentioned above, it is important for users to understand options we used from the code. For example, we can try to understand the option `--anamgr-genevt`.
Firstly, you can browse the code in `junosw/Examples/Tutorial/python/Tutorial/JUNODetSimModule.py`.
```python

        # == genevt ==
        grp_anamgr.add_argument("--anamgr-genevt", action="store_true", dest="anamgr_genevt", help=mh("TBD"))
        grp_anamgr.add_argument("--no-anamgr-genevt", action="store_false", dest="anamgr_genevt", help=mh("TBD"))
        grp_anamgr.set_defaults(anamgr_genevt=True)

```
the above code will register the option and the default value is true which means it is enabled as default. If you want to disable the tree you should use option `--no-anamgr-genevt` in simulation scripts. And then

```python
       # == genevt anamgr ==
        if args.anamgr_genevt:
            detsimfactory.property("AnaMgrList").append("GenEvtInfoAnaMgr")

```
As you can see, the above code means we use the `GenEvtInfoAnaMgr` tool.
Therefore, we can search for this tool in the C++ code in directory `juno/Simulation/DetSimV2/AnalysisCode/src`
Then you will find this tool is corresponded with `GenEvtInfoAnaMgr.cc`. Ok, You can browse the code to know about which tree will be saved.
```C++
    m_evt_tree = svc->bookTree(*m_par, "SIMEVT/geninfo", "Generator Info");

    m_evt_tree->Branch("evtID", &m_eventID, "evtID/I");
    m_evt_tree->Branch("nInitParticles", &m_init_nparticles, "nInitParticles/I");
```
Thus, `geninfo` tree will be saved and you know the `--anamgr-genevt` control the tree `geninfo` in user data from the above code.

Therefore, you can know many options of analysis manager if you understand the above methods.


