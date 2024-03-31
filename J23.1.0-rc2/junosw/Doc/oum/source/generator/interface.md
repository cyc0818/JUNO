# Generator Interface
In JUNOSW, `GenTool` is used to integrate the external physics generators. `HepMC` is used to represent the final states of the physics generators.  

## Atmospheric neutrino generator interface
We are developing the interfaces of three generators in order to use them directly in JUNO Offline so that we can generate events as the input of detector simulation event by event instead of pre-producing and sampling which is we used to do. 

Atmospheric neutrinos detected by JUNO offer a valuable signal for studying neutrino mass ordering and CP phase. However, they also represent a significant background in rare event searches, such as for DSNB and proton decay.

To fully understand atmospheric neutrino interactions at GeV energies in JUNO, it is essential to account for the complex GeV neutrino interaction cross section. To this end, we have incorporated popular GeV neutrino event generators such as GENIE, GiBUU, and NuWro into JUNO's offline software framework. This allows us to generate neutrino interaction events, including initial and final states as well as Fermi motion, and develop atmospheric neutrino generator-detsim interfaces.


### GenGENIE
Based on GENIE version 3.02.00, it's available since J22.2.0-rc1. It uses [HepMC](https://rivet.hepforge.org/code/hepmc.bak/) to store the events in EDM file. One event corresponds to a GenEvent which contains a GenVertex signal_process_vertex.

As an interface, we don't modify the core algorithm of GENIE and try to keep the options we need as same as the origin application. Please see [GENIE](generator.html#GENIE) part of Generator section for more details of these options.
```bash
    python ${GENGENIEROOT}/share/run.py –n --seed –p -e –f –t --tune --cross-sections
```
Corresponding to `gevgen_atmo`, we can run `runatmo.py` to the same things. What's more, we add some new options to achieve some convenient functions needed in JUNO atmospheric neutrino study.
```bash
    python ${GENGENIEROOT}/share/runatmo.py –n --seed --ev-min --ev-max –f -t/--tar-tag --tune --cross-sections --event-generator-list --enable-isotropic --user-flux-frac --enable-deex
```
* --ev-min: the minimal energy
* --ev-max: the maximal energy
* –f: 3D flux (FLUKA/BGLRS/HAKKM) or 1D flux histogram
* --tar-tag: 0:LS, 1:Water, 2:Rock
* --enable-isotropic: rotate the neutrino direction to a uniform distribution when using 1D flux 
* --user-flux-frac: specify the fraction between 1D flux used
* --enable-deex: do the de-excitation of the residual nuclear
Since we design the output event format as HepMC, thi interface `runatmo.py` can also be directly used in offline's detector simulation by a suboption `gevgen_atmo`.
```bash
    python ${TUTORIALROOT}/share/tut_detsim.py ... gevgen_atmo –n --seed --ev-min --ev-max –f -t/--tar-tag --tune --cross-sections --event-generator-list --isotropic --user-flux-frac --deex
```
### GenGiBUU
waiting for qiyu's supplyment
### GenNuWro
waiting for qiyu's supplyment
