# Detector Geometry Construction

Detector geometry construction is an important part in detector simulation. In [simulation framework part](framework.html#detector-geometry-management), we already know the detector element interface. In this part, we will learn detail about each detector element, including how PMT is placed into this detector element.

## World in Geant4
In Geant4, we need to create a default world, then place everything in this world.

* **Location**: `offline/Simulation/DetSimV2/DetSimOptions`
* **Implementation**: `LSExpDetectorConstruction`

The hierarchy of detector construction in the world:

* Top Rock (`TopRockConstruction`)
    * Experimental Hall (`ExpHallConstruction`)
        * Top Tracker (`TopTrackerConstruction`)
        * Top Chimney (`UpperChimney`)
* Bottom Rock (`BottomRockConstruction`)
    * Water Pool (`WaterPoolConstruction`)
        * PMTs in Water Pool (copy number from 30,000)
        * Central Detector (`DetSim1Construction`)
            * PMTs in Central Detector (copy number, 20 inch: 0, 3 inch: 300,000)
            * Supporting sticks (`StrutAcrylicConstruction` and `FastenerAcrylicConstruction`)
            * Bottom Chimney (`LowerChimney`)
            * Calibration Units

## Central Detector (CD)

* **Location**: `offline/Simulation/DetSimV2/CentralDetector`
* **CD Implementation**: `DetSim1Construction`
* There are several detector elements are placed into CD
    * 20inch PMT (copy number from 0 to 17611)
    * 3inch PMT (copy number from 300,000)
    * supporting sticks

The positions are loaded by `HexagonPosBall` (in `offline/Simulation/DetSimV2/SimUtil`). Their input data in:

* 20 inch: `data/Detector/Geometry/PMTPos_CD_LPMT.csv`
* 3 inch: `data/Detector/Geometry/PMTPos_CD_SPMT.csv`
* supporting sticks: `offline/Simulation/DetSimV2/DetSimOptions/data/Strut_Acrylic.csv`

The file format is simple, first column is a number at specific z, second and third column are theta (deg) and phi (deg) of radial position. 

You could count the total number easily:

    $ grep . data/Detector/Geometry/PMTPos_CD_LPMT.csv | wc -l
    17612
    $ grep . data/Detector/Geometry/PMTPos_CD_SPMT.csv | wc -l
    25600
    $ grep . offline/Simulation/DetSimV2/DetSimOptions/data/Strut_Acrylic.csv | wc -l
    480

You can also count the number at each theta:

    $ grep . offline/Simulation/DetSimV2/DetSimOptions/data/Strut_Acrylic.csv | awk '{print $2}' | uniq -c
     24 12
     24 24
     24 36
     24 48
     24 60
     24 72
     24 84
     24 96
     24 102
     24 108
     24 114
     24 120
     24 126
     24 132
     24 138
     24 144
     24 150
     24 156
     24 162
     24 168

**Please note**, it is better to use geometry service instead. Because in the future, the PMT positions may be not loaded from these csv files.

## Water Pool (WP)

* **Location**: `offline/Simulation/DetSimV2/DetSimOptions`
* **WP Implementation**: `WaterPoolConstruction`

The positions of PMTs are calculated by `CalPositionCylinder` (in `offline/Simulation/DetSimV2/SimUtil`).

![Fig. Visualization of WP](detsim/figs/vis-water-pool.png)

Detail see JUNO-doc-1850 (<http://juno.ihep.ac.cn/cgi-bin/Dev_DocDB/ShowDocument?docid=1850>).

## Top Tracker (TT)
* **Location**: `offline/Simulation/DetSimV2/TopTracker`
* **WP Implementation**: `TopTrackerConstruction`

## Chimney
* **Location**: `offline/Simulation/DetSimV2/Chimney`
* **Chimney Implementation**: `UpperChimney` and `LowerChimney`.

Detail see JUNO-doc-1363 (<http://juno.ihep.ac.cn/cgi-bin/Dev_DocDB/ShowDocument?docid=1363>).

## Calibration Units
* **Location**: `offline/Simulation/DetSimV2/CalibUnit`
* **Implementation** could be found in this directory.

## JUNO Prototype detector
* **Location**: `offline/Simulation/DetSimV2/CentralDetector`
* **Implementation**: `PrototypeConstruction`.

The positions of PMTs are calculated by `Prototype::PMT20inchPos`, `Prototype::PMT8inchPos` and `Prototype::PMT8inchPos_BTM` (in `offline/Simulation/DetSimV2/SimUtil/include/PMTinPrototypePos.hh`).

![Fig. Visualization of Prototype Detector](detsim/figs/vis-prototype.png)

The script could be found in `$DETSIMOPTIONSROOT/share/examples/prototype`:

    $ python pyjob_prototype.py --vis

## An example: a dummy detector only contains one PMT
To debug and develop new PMT in simulation, it is better to visualize only one. We create a dummy detector element to do such work.

* **Location**: `offline/Simulation/DetSimV2/CentralDetector`
* **Implementation**: `PrototypeOnePMTConstruction`

The position of PMT is controlled by `OnePMTPlacement`.

![Fig. Visualization of Dummy Detector](detsim/figs/vis-prototype-one.png)

The script could be found in `$DETSIMOPTIONSROOT/share/examples/prototype`:

    $ python pyjob_prototype_onepmt.py --vis

You could change PMT by `--pmt-name`:

    $ python pyjob_prototype_onepmt.py --vis --pmt-name PMTMask

If you want to register a new PMT, you need to modify `ExpDetectorConstruction::setupPrototypeDetectorOnePMT`.

----
