# Materials and optical properties

Materials and optical properties are important in our simulation. In this part, we will learn where are these materials defined and how to plot optical properties.

## Materials in Simulation

* **Location**: `offline/Simulation/DetSimV2/DetSimOptions`
* **Files**
    * `LSExpDetectorConstructionMaterial.icc`
    * `OpticalProperty.icc`

These files are included directly in `LSExpDetectorConstruction.cc`.

To access some materials, please try to use Geant4's API first:

    LS = G4Material::GetMaterial("LS");

Especially when we implement a detector element, we always need materials. Keeping these materials in the same place could avoid duplication. Some users maybe construct the material in GDML files, so checking this material exists or not is important.

For some reasons, we defined several **scale factors** to scale some variables. This should be also taken care of.

## Plot optical properties

It's important to check what's the optical properties is used in our simulation. A tree named `opticalparam` is saved in user data file.

You can find a script to draw these from user data. It's location:

* `offline/Simulation/DetSimV2/OpticalProperty/draw_from_user.C`

It will load `sample_detsim_user.root` and plot:

* Liquid Scintillator
    * Refractive index
    * Emission spectrum
    * Reemission probability
    * Absorption length
    * Rayleigh length
* Acrylic
    * Absorption length
    * Refractive index
* Water
    * Absorption length
    * Refractive index
* Pyrex
    * Refractive index
* Quantum Efficiency of PMT

You can find plots in our basic distribution wiki page (<http://juno.ihep.ac.cn/mediawiki/index.php/Analysis:Basic_Distributions_of_JUNO>).

**Note**, this script assumes fixed arrays.

You can find the corresponding analysis element in:

* `offline/Simulation/DetSimV2/src/OpticalParameterAnaMgr.cc` or `$ANALYSISCODEROOT/src/OpticalParameterAnaMgr.cc`
