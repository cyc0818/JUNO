# Introduction to JUNO Event Data Model (EDM)

## Organization of EDM

The entry of an event is `JM::EvtNavigator`. From this navigator, users could access header objects via the EDM path. With a header object, the corresponding event objects could be accessed. This design allows the lazy loading of event data. For an example, some users may want to get the waveforms information, but some may not. With this design, the waveforms information are only loaded on demand. 

For each header object, it will belong to a unique EDM path. The default EDM path could be found in `offline/DataModel/EDMUtil/src/JunoEDMDefinitions.cc`. Users are allowed to customize these paths in the design. For example, users want to compare two different reconstruction algorithms, but these algorithms are configured to be run in the same job. In order to store the two different results, users can choose different EDM paths. 

A list of the EDM classes could be found in following page:
* https://juno.ihep.ac.cn/~offline/Doxygen/edm/html/classes.html

The real data and simulated data share the same EDM at electronics readout, calibrated event and reconstructed event. 

| Stage                  | Components         | Header object               | Event object             | Contained types       | default EDM Path     |
|------------------------|--------------------|-----------------------------|--------------------------|-----------------------|----------------------|
| Physics Generator      | Track              | `JM::GenHeader`             | `JM::GenEvt`             | `HepMC::GenEvent`     | `/Event/Gen`         |
| Detector Simulation    | Track, CD, WP, TT  | `JM::SimHeader`             | `JM::SimEvt`             | `JM::SimTrack`        | `/Event/Sim`         |
|                        |                    |                             |                          | `JM::SimPMTHit`       |                      |
|                        |                    |                             |                          | `JM::SimTTHit`        |                      |
| Elec Truth (elecsim)   | CD-LPMT (pulse)    | `JM::CdLpmtElecTruthHeader` | `JM::CdLpmtElecTruthEvt` | `JM::LpmtElecTruth`   | `/Event/CdLpmtTruth` |
|                        | CD-SPMT (pulse)    | `JM::CdSpmtElecTruthHeader` | `JM::CdSpmtElecTruthEvt` | `JM::SpmtElecTruth`   | `/Event/CdSpmtTruth` |
|                        | WP (pulse)         | `JM::WpElecTruthHeader`     | `JM::WpElecTruthEvt`     | `JM::WpElecTruth`     | `/Event/WpTruth`     |
|                        | TT                 | `JM::TtElecTruthHeader`     | `JM::TtElecTruthEvt`     | `JM::TtElecTruth`     | `/Event/TtTruth`     |
|                        | Track              | `JM::TrackElecTruthHeader`  | `JM::TrackElecTruthEvt`  | `JM::TrackElecTruth`  | `/Event/TrackTruth`  |
| Trigger                | CD                 | `JM::CdTriggerHeader`       | `JM::CdTriggerEvt`       |                       | `/Event/CdTrigger`   |
|                        | WP                 | `JM::WpTriggerHeader`       | `JM::WpTriggerEvt`       |                       | `/Event/WpTrigger`   |
|                        | TT                 | `JM::TtTriggerHeader`       | `JM::TtTriggerEvt`       |                       | `/Event/TtTrigger`   |
| Electronics Readout    | CD-LPMT (waveform) | `JM::CdWaveformHeader`      | `JM::CdWaveformEvt`      | `JM::ElecWaveform`    | `/Event/CdWaveform`  |
|                        | CD-LPMT (t/q)      | `JM::CdLpmtElecHeader`      | `JM::CdLpmtElecEvt`      | `JM::ElecChannel`     | `/Event/CdLpmtElec`  |
|                        | CD-SPMT (t/q)      | `JM::CdSpmtElecHeader`      | `JM::CdSpmtElecEvt`      |                       | `/Event/CdSpmtElec`  |
|                        | WP (t/q)           | `JM::WpElecHeader`          | `JM::WpElecEvt`          | `JM::ElecChannel`     | `/Event/WpElec`      |
|                        | WP (waveform)      | `JM::WpWaveformHeader`      | `JM::WpWaveformEvt`      | `JM::ElecWaveform`    | `/Event/WpWaveform`  |
|                        | TT                 | `JM::TtElecHeader`          | `JM::TtElecEvt`          | `JM::ElecChannel`     | `/Event/TtElec`      |
| Calibration            | CD-LPMT (hit)      | `JM::CdLpmtCalibHeader`     | `JM::CdLpmtCalibEvt`     | `JM::CalibPmtChannel` | `/Event/CdLpmtCalib` |
|                        | CD-SPMT (hit)      | `JM::CdSpmtCalibHeader`     | `JM::CdSpmtCalibEvt`     | `JM::CalibPmtChannel` | `/Event/CdSpmtCalib` |
|                        | WP                 | `JM::WpCalibHeader`         | `JM::WpCalibEvt`         | `JM::CalibPmtChannel` | `/Event/WpCalib`     |
|                        | TT                 | `JM::TtCalibHeader`         | `JM::TtCalibEvt`         | `JM::CalibTtChannel`  | `/Event/TtCalib`     |
| Reconstruction         | CD (Vertex)        | `JM::CdVertexRecHeader`     | `JM::CdVertexRecEvt`     | `JM::RecVertex`       | `/Event/CdVertexRec` |
|                        | CD (Track)         | `JM::CdTrackRecHeader`      | `JM::CdTrackRecEvt`      | `JM::RecTrack`        | `/Event/CdTrackRec`  |
|                        | WP                 | `JM::WpRecHeader`           | `JM::WpRecEvt`           | `JM::RecTrack`        | `/Event/WpRec`       |
|                        | TT                 | `JM::TtRecHeader`           | `JM::TtRecEvt`           |                       | `/Event/TtRec`       |

## Access the EDM objects from EvtNavigator

A template function `JM::getHeaderObject` is provided to access the header object from an event navigator. Users need to specify the type of the header object. The default EDM path will be used if users don't specify any paths.  

Here is an example:

```c++
// header to be included
#include "EvtNavigator/EvtNavHelper.h"

...

    auto elec_header = JM::getHeaderObject<JM::CdWaveformHeader>(evtnav);
    auto elec_truth_header = JM::getHeaderObject<JM::CdLpmtElecTruthHeader>(evtnav);
    auto elec_truth_header_track = JM::getHeaderObject<JM::TrackElecTruthHeader>(evtnav);
    auto trig_header = JM::getHeaderObject<JM::CdTriggerHeader>(evtnav); 

...

```

## Add the EDM objects into EvtNavigator

A template function `JM::addHeaderObject` is provided to add the header object into an event navigator. 

Here is an example:

```c++
    auto gen_hdr = new JM::GenHeader();
    auto sim_hdr = new JM::SimHeader();

    auto elec_hdr = new JM::CdWaveformHeader();
    auto elec_hdr_spmt = new JM::CdSpmtElecHeader();

    auto trig_hdr = new JM::CdTriggerHeader();

    auto elec_truth_hdr = new JM::CdLpmtElecTruthHeader;
    auto elec_truth_hdr_spmt = new JM::CdSpmtElecTruthHeader;
    auto elec_truth_trk = new JM::TrackElecTruthHeader;

    JM::addHeaderObject(evtnav, gen_hdr);
    JM::addHeaderObject(evtnav, sim_hdr);
    JM::addHeaderObject(evtnav, elec_hdr);
    JM::addHeaderObject(evtnav, elec_hdr_spmt);
    JM::addHeaderObject(evtnav, trig_hdr);
    JM::addHeaderObject(evtnav, elec_truth_hdr);
    JM::addHeaderObject(evtnav, elec_truth_hdr_spmt);
    JM::addHeaderObject(evtnav, elec_truth_trk);


```


## Control the output streams in Python
By default, the python scripts already define which output streams will be saved in the output EDM file. If users want to enable or disable some output streams, they can use the option `--output-stream`. This option could be used several times in the same command, to control the output stream by setting on or off. 

```bash
$ python $TUTORIALROOT/share/tut_elec2rec.py --output-stream /Event/CdLpmtElec:off 
$ python $TUTORIALROOT/share/tut_elec2rec.py --output-stream /Event/CdLpmtElec:off --output-stream /Event/CdLpmtCalib:off
```

