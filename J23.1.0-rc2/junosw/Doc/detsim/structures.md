# Simulation software structures
JUNO simulation consists of several parts:

* user interface
* event data model
* physics generators and its interface
* detector simulation
* electronics and digitization simulation
* external libraries

In this doc, we focus on physics generators and detector simulation part. For electronics and digitization simulation, you could read other document.

We will also show you where are the external libraries.

## User Interface
If you already try quick start of detector simulation, you could be familiar with our script `tut_detsim.py`. It is a unified interface for both users and developers.

* **Location**: `$TUTORIALROOT/share` or `offline/Examples/Tutorial/share`.

Script `tut_detsim.py` loads and configure several service and algorithms:

* common services, such as ROOT IO, Buffer Service, Random Service, `RootWriter`.
* Algorithm `GenTools`. It could combine different tools together.
* Algorithm `DetSimAlg` and several service and tools.

Physics generators are integrated with `tut_detsim.py` by using different sub-command. For example, `hepevt` could read `HepEvt` format file. However, `hepevt` could also invoke specified executable by specifying `--exe` name.

The position and time of particles are set by additional tools. So if a new type of generators is added, it could be integrated easily.

By following almost same options, user could learn to use another generator quickly.

## Event data model
* **Location** of `GenEvent`: `$GENEVENTV2ROOT` or `offline/DataModel/GenEventV2`.
* **Location** of `SimEvent`: `$SIMEVENTV2ROOT` or `offline/DataModel/SimEventV2`.

Note, for `GenEvent`, we actually use `HepMC 2`. Due to limitation of `HepMC 2`, `GenEvent` is not serialize into ROOT file.

You could find more information about `SimEvent` at [here](../datamodel/simevent.html).


## Physics generators and interface
Most of physics generator are written by physicists for their study. However, physics generator interface are developed by offline persons to integrate physics generators and detector simulation. The interface allows different format input and converts them to a unified data format.

### Physics generators

* **Location** of Most of standalone physics generators: `offline/Generator`.
* Then it contains several packages:
    * `InverseBeta` or `$INVERSEBETAROOT`.
    * `Muon` or `$MUONROOT`.
    * `NuSolGen` or `$NUSOLGENROOT`
    * `RadioActivity`. In this directory, there are several packages:
        * `AmBe`
        * `AmC`
        * `Cf252`
        * `Co60`
        * `Cs137`
        * `Ge68`
        * `K40`
        * `PuC`
    * `Supernova` or `$SUPERNOVAROOT`.

As mentioned before, most of physics generators save their results in `HepEvt` mode. But there are also some of them are in their own format. So we need an additional `GenTool` to load them again and convert them.

### `GenTool`: physics generator interface

* **Location**: `offline/Simulation/GenTools` or `$GENTOOLS`.

The design of `GenTool` is quite simple. For every event, we could add or modify vertex and particles in this event, by using different tools.

![Fig. Data flow of generator interface](detsim/figs/generator-interface-dataflow.png)

In this figure, particle gun or `HepEvt` parser or `GenDecay` could generate particles with four momenta. Then position and time of this particle are updated by other tools.

These tools could be combined together, so it is flexible and reusable.

### `GenDecay`: radioactivity generator
* **Location**: `offline/Simulation/GenDecay` or `$GENDECAYROOT`.

Package `GenDecay` is originally developed by Brett Viren. We migrated it to JUNO software. It could use nuclear decay data as input and calculate the decay chain automatically.

It uses an external library `libmore` to parse ENSDF data.

## Detector simulation
* **Location**: `offline/Simulation/DetSimV2`.

JUNO detector simulation software is based on Geant4. For flexibility and modular design, a detector simulation framework is introduced to integrate underlying framework `SNiPER` and Geant4. Then the detector simulation software are separated into different packages. Developers could modify code in one package without affecting other packages. Finally, these modules are integrated together.

* `DetSimOptions`: integrate all.
* Simulation framework
    * `DetSimAlg`
    * `G4Svc`
* Detector components
    * `CentralDetector`
    * `TopTracker`
    * `Chimney`
    * `CalibUnit`
    * Some also in `DetSimOptions`, such as Water Pool.
* User actions
    * `AnalysisCode`
* Physics processes
    * `PhysiSim`: customized physics processes.
* Sensitive detectors
    * `PMTSim`: PMT constructions and optical model.
* Utilities
    * `GenSim`: generator interface related.
    * `SimUtil`: PMT positions, user track information (`NormalTrackInfo`) and so on.

## External Libraries
### Geant4
* **Location**: `ExternalLibs/Geant4/9.4.p04`
* **Source code**: `ExternalLibs/Build/geant4.9.4.p04`

### CLHEP
* **Location**: `ExternalLibs/CLHEP/2.1.0.1`
* **Source code**: `ExternalLibs/Build/clhep-2.1.0.1`

### Xerces-C
* **Location**: `ExternalLibs/Xercesc/3.1.1`
* **Source code**: `ExternalLibs/Build/xerces-c-3.1.1`

### HepMC
* **Location**: `ExternalLibs/HepMC/2.06.09`
* **Source code**: `ExternalLibs/Build/HepMC-2.06.09`

### libmore
* **Location**: `ExternalLibs/libmore/0.8.3`
* **Source code**: `ExternalLibs/Build/more-0.8.3`
