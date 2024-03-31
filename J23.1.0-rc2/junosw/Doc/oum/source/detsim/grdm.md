# Simulation using GRDM (Geant4 Radioactivity Decay Module)

There are two similar ways to invoke GRDM using particle gun in the simulation:
* For one nuclide, specify the pdgcode and the mac file
* For multiple nuclides (in decay chains), specify a file containing the necessary information

Please note that, the corresponding MAC files need to be specified by the option `--mac`.

## Example: one nuclide
```bash
$ python $TUTORIALROOT/share/tut_detsim.py \
         --mac $TUTORIALROOT/share/mac/U238/run-1000822100.mac \
         gun --particles 1000822100

```

Some examples could be also found in:
```
/junofs/production/data-production/Pre-Releases/J20v1r0-Pre2/background/LS/validation.ini
```

In the MAC file, the decay chain is controlled by the grdm command. 
The daughter nuclide will stop to decay with the limitation. 
For example, the decay of the nuclide (1000822100) is controlled using following MAC file:

```
/run/initialize
/grdm/nucleusLimits 210 210 82 82
```

## Example: multiple nuclides in decay chain
```bash
$ python $TUTORIALROOT/share/tut_detsim.py gun --particles Th232.conf
```
In the file `Th232.conf`, you can specify the parent nuclide, ratio, Amin, Amax, Zmin and Zmax:
```bash
$ cat Th232.conf
1000812080 1 208 208 81 81
1000822120 1 212 212 82 82
1000832120 1 212 212 83 83
1000842120 1 212 212 84 84
```

## References

| Category | MAC file                                         | PDGID      |
|----------|--------------------------------------------------|------------|
| U238     | $TUTORIALROOT/share/mac/U238/run-1000822100.mac  | 1000822100 |
|          | $TUTORIALROOT/share/mac/U238/run-1000822140.mac  | 1000822140 |
|          | $TUTORIALROOT/share/mac/U238/run-1000832100.mac  | 1000832100 |
|          | $TUTORIALROOT/share/mac/U238/run-1000832140.mac  | 1000832140 |
|          | $TUTORIALROOT/share/mac/U238/run-1000842100.mac  | 1000842100 |
|          | $TUTORIALROOT/share/mac/U238/run-1000842140.mac  | 1000842140 |
|          | $TUTORIALROOT/share/mac/U238/run-1000842180.mac  | 1000842180 |
|          | $TUTORIALROOT/share/mac/U238/run-1000862220.mac  | 1000862220 |
|          | $TUTORIALROOT/share/mac/U238/run-1000882260.mac  | 1000882260 |
|          | $TUTORIALROOT/share/mac/U238/run-1000902300.mac  | 1000902300 |
|          | $TUTORIALROOT/share/mac/U238/run-1000902340.mac  | 1000902340 |
|          | $TUTORIALROOT/share/mac/U238/run-1000912341.mac  | 1000912341 |
|          | $TUTORIALROOT/share/mac/U238/run-1000922340.mac  | 1000922340 |
|          | $TUTORIALROOT/share/mac/U238/run-1000922380.mac  | 1000922380 |
| Th232    | $TUTORIALROOT/share/mac/Th232/run-1000812080.mac | 1000812080 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000822120.mac | 1000822120 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000832120.mac | 1000832120 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000842120.mac | 1000842120 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000842160.mac | 1000842160 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000862200.mac | 1000862200 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000882240.mac | 1000882240 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000882280.mac | 1000882280 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000892280.mac | 1000892280 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000902280.mac | 1000902280 |
|          | $TUTORIALROOT/share/mac/Th232/run-1000902320.mac | 1000902320 |
| K40      | $TUTORIALROOT/share/mac/K40/run-1000190400.mac   | 1000190400 |
| Ar39     | $TUTORIALROOT/share/mac/Ar39/run-1000180390.mac  | 1000180390 |
| C14      | $TUTORIALROOT/share/mac/C14/run-1000060140.mac   | 1000060140 |
| Kr85     | $TUTORIALROOT/share/mac/Kr85/run-1000360850.mac  | 1000360850 |

