# Analyze user data
You should already know how to run detector simulation. In this part, we will explain the meaning of each variable in user data. You could also read analysis code for more information.

## `evt` tree
In this tree, every entry represents one detector simulation event. 

This detector simulation event is a little different from electronics/trigger event. For example, there is an IBD event (a positron and a neutron) in detector simulation, but there will be two trigger events. The relationship could be complicated when event is split.

* `evtID`: event ID. For each simulation job, it starts from 0.
* `nPhotons`: length of hits in this event. 
    * Note, for one hit, it could be merged by several PEs in the same time window.
* `totalPE`: total PE numbers in this event.
    * If `PEs` are not merged, `nPhotons` is equal to `totalPE`.
* `energy`: optical photon's energy.
* `hitTime`: hit time.
* `pmtID`: each PE's pmt ID
* `PETrackID`: the primary track ID of this optical photon.
* `isCerenkov`: is the original optical photon a Cerenkov light or not.
* `isReemission`: is photon re-emitted during propagation.
* `isOriginalOP`: is photon the original photon without any reemission. 
* `OriginalOPTime`: the original photon's generation time.
* `edep`: the total deposited energy in this event.
* `edepX`, `edepY` and `edepZ`: deposited position.
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
* `edep`: deposited energy of each particle and its secondaries.
* `Qedep`: visible energy of each particle and its secondaries.
