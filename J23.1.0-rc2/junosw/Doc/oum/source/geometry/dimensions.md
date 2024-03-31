# Dimensions of geometries

## PMT

| Name                         | Values  | source code                                                   |
| ---------------------------- | ------- | ------------------------------------------------------------- |
| HamamatsuPMT                 |         | `Simulation/DetSimV2/PMTSim/src/HamamatsuR12860PMTManager.cc` |
| HamamatsuPMT.R               | 254.*mm |                                                               |
| HamamatsuPMT.H               | 640.*mm |                                                               |
| HamamatsuPMT.top             | 190.*mm |                                                               |
| HamamatsuPMT.bottom          | 450.*mm | HamamatsuPMT.H - HamamatsuPMT.top                             |
|                              |         |                                                               |
| HamamatsuMask                |         | `Simulation/DetSimV2/PMTSim/src/HamamatsuMaskManager.cc`      |
| HamamatsuMask.thickness      | 8.*mm   |                                                               |
| HamamatsuMask.thicknessBottom| 8.*mm   |                                                               |
| HamamatsuMask.gap            | 2.*mm   |                                                               |
| HamamatsuMask.gapBottom      | 5.*mm   |                                                               |
| HamamatsuMask.innerR         | 256.*mm | HamamatsuPMT.R + HamamatsuMask.gap                            |
| HamamatsuMask.outerR         | 264.*mm | HamamatsuMask.innerR + HamamatsuMask.thickness                |
| HamamatsuMask.innerTop       | 192.*mm | HamamatsuPMT.top + HamamatsuMask.gap                          |
| HamamatsuMask.outerTop       | 200.*mm | HamamatsuMask.innerTop + HamamatsuMask.gap                    |
| HamamatsuMask.innerBottom    | 455.*mm | HamamatsuPMT.bottom + HamamatsuMask.gapBottom                 |
| HamamatsuMask.outerBottom    | 465.*mm | HamamatsuMask.innerBottom + Hamamatsu.gapBottom               |
| HamamatsuMask.head.below     | 40.*mm  | below the equator (Acrylic)                                   |
| HamamatsuMask.tail.R1        | 265.*mm |                                                               |
| HamamatsuMask.tail.R2        | 149.*mm | 134.+15.                                                      |
| HamamatsuMask.tail.H         | 350.*mm | 320.*mm + 30.*mm                                              |
|                              |         |                                                               |
| NNVTMCPPMT                   |         | `Simulation/DetSimV2/PMTSim/src/NNVTMCPPMTManager.cc`         |
|                              |         |                                                               |
| NNVTMaskManager              |         | `Simulation/DetSimV2/PMTSim/src/NNVTMaskManager.cc`           |
|                              |         |                                                               |


