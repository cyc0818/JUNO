# Materials and optical properties

Materials and optical properties are important in our simulation. In this part, we will learn where are these materials defined and how to plot optical properties. And also we use MCParamSvc to manage all the Optical parameters and we will introduce this Svc.

## Materials in Simulation

* **Location**: `junosw/Simulation/DetSimV2/DetSimOptions`
* **Files**
    * `LSExpDetectorConstructionMaterial.icc`

These files are included directly in `LSExpDetectorConstruction.cc`.

The data of materials and optical properties are in `data/Simulation/DetSim/Material`
You can browse the data from `https://juno.ihep.ac.cn/trac/browser/offline-data/trunk/Simulation/DetSim/Material`

The follow code shows how to access this data in `LSExpDetectorConstructionMaterial.icc`
```C++
ESR = new G4Material("ESR", 1.0*g/cm3, 2);
ESR->AddElement(H, 2.*1.00794/(12.0107+2.*1.00794));
ESR->AddElement(C, 12.0107/(12.0107+2.*1.00794));

G4MaterialPropertiesTable* ESRMPT = new G4MaterialPropertiesTable();

G4cout << "Setup ESR Material Tables from MCParamsSvc" << G4endl;
helper_mpt(ESRMPT, "ABSLENGTH", mcgt.data(), "Material.ESR.ABSLENGTH");  
```
We use the helper function `helper_mpt` to get the data. There are 4 agruments in function `helper_pmt`. The first is a G4MaterialPropertiesTable, the second is the one property you defined , the third argument is an instance of MCParamSvc which will used to access data and the fourth agrument represent the relative path to the data `Material/ESR/ABSLENGTH`.


To access some materials, please try to use Geant4's API first:

    LS = G4Material::GetMaterial("LS");

Especially when we implement a detector element, we always need materials. Keeping these materials in the same place could avoid duplication. Some users maybe construct the material in GDML files, so checking this material exists or not is important.

For some reasons, we defined several **scale factors** to scale some variables. This should be also taken care of.

## Plot optical properties

It's important to check what's the optical properties is used in our simulation. A tree named `opticalparam` is saved in user data file.

You can find a script to draw these from user data. It's location:

* `junosw/Simulation/DetSimV2/OpticalProperty/draw_from_user.C`

It will load `sample_detsim_user.root` and plot:

* Liquid Scintillator
    * Refractive index
    * Fast component emmission spectrum
    * Slow component emmission spectrum
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

You can find plots in our basic distribution wiki page (<https://juno.ihep.ac.cn/mediawiki/index.php/Analysis:Basic_Distributions_of_JUNO>).

**Note**, this script assumes fixed arrays.

You can find the corresponding analysis element in:

* `junosw/Simulation/DetSimV2/src/OpticalParameterAnaMgr.cc` or `$ANALYSISCODEROOT/src/OpticalParameterAnaMgr.cc`


## Replace optical parameters
In this part we will introduce how to replace the optical parameters. We provide 3 ways to specify the optical parameters.
### Replace all optical parameters through the argument `--MaterialDataDir`:

    $ python $TUTORIALROOT/share/tut_detsim.py --MaterialDataDir /home/ihep/2020-3-17/trunk/data/Simulation/DetSim/ --evtmax 10 gun

This means that you will use all the optical parameters from directory `/home/ihep/2020-3-17/trunk/data/Simulation/DetSim/`.

### Replace one file of optical parameters through the argument `--replace-param` 
for example:

    $ python $TUTORIALROOT/share/tut_detsim.py --replace-param Material.LS.FASTCOMPONENT:FASTCOMPONENT --evtmax 10 gun

`Material.LS.FASTCOMPONENT` means a relative path `Material/LS/FASTCOMPONENT` and we want to replace the file of FASTCOMPONENT of LS 
After the colon is the specified file name which is in current directory. Also you can browse the website to understand where the optical parameter is from : `https://juno.ihep.ac.cn/trac/browser/offline-data/trunk/Simulation/DetSim/Material/LS/FASTCOMPONENT`

### Replace single value through the agument `--replace-param`

    $ python $TUTORIALROOT/share/tut_detsim.py --replace-param Material.LS.ConstantProperty.ScintillationYield:1/keV --evtmax 2 gun >& log.txt
 
`Material.LS.ConstantProperty.ScintillationYield` means that there is a property in `Material/LS/ConstantProperty`. The below is the `ConstantProperty` file.
```bash
% cat ConstantProperty 
#some constant property of LS

#SCINTILLATIONYIELD
ScintillationYield   11522/MeV
ResolutionScale      1.0

#birks law
BirksConstant1  6.5e-3*g/cm2/MeV
BirksConstant2  1.5e-6*(g/cm2/MeV)*(g/cm2/MeV)
```
In this file we use `1/keV` to replace the value `11522/MeV` in file.

