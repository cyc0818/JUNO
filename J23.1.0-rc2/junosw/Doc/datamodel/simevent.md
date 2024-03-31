# The data model for detector simulation 
## Code directory
* `$SIMEVENTV2ROOT`, the top directory for the data model. It includes:
    * Event, header files
    * src, source files
    * cmt, the CMT package information
    * tests, test scripts

## design
There are two major parts for the data model, one is `SimHeader` and another is `SimEvent`. `SimHeader` is used for simple event selection, so that the whole event data is loaded lazily and it could avoid the heavy ROOTIO. `SimEvent` represents the whole event data, which includes:

1. `SimTrack`, represent a primary track in simulation. 
2. `SimPMTHit`, represent a hit of PMT in Central Detector and Water Pool.
3. `SimTTHit`, represent a hit in Top Tracker.

## `SimTrack`
The `init` status means when geant4 start tracking of this particle. The `exit` means when geant4 stop tracking of the particle. When the particle is out of world in geant4 or the energy is not enough for tracking, it will be marked as 'died'.

Following is the detail information:

* `pdg_id`, the PDG MC numbering
* `track_id`, track id in geant4
* `init_mass`, the mass of particle
* `init_px`, `init_py` and `init_pz`, the initial momentum
* `init_x`, `init_y` and `init_z`, the initial position
* `init_t`, the initial time
* `exit_px`, `exit_py` and `exit_pz`, the momentum when stop
* `exit_x`, `exit_y` and `exit_z`, the position when stop
* `exit_t`, the stop time
* `track_length`, the particle track length from `init` to `exit` status.
* `edep`, the deposit energy
* `edep_x`, `edep_y` and `edep_z`, the position considering deposit energy.
    * edep_pos = sum(edep[i]*step_pos[i])/sum(edep[i])
* `Qedep`, the visible energy, which is proportional to generated photons.
* `Qedep_x`, `Qedep_y` and `Qedep_z`, the position considering visible energy.
* `edep_notinLS`, the energy not deposit in LS.

## `SimPMTHit`
* `pmtid`, PMT ID in detector simulation. Please note, it is not the same ID as Geometry Service Designed.
* `npe`, number of collected photon-electrons. By default, one hit only has one PE. When the hit merge is enabled, npe will greater than 1.
* `hittime`, the hit time
* `timewindow`, the merged time window.

## `SimTTHit`

