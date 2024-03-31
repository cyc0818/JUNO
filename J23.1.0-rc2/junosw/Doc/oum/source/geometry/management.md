# Geometry Services
## Geometry/PMTParamSvc

### Gets functions
| functions  | Description | 
| -------- | --------  | 
| ```bool isCD(int pmtid)``` | CD PMT or not|
| ```bool isWP(int pmtid)``` | WP PMT or not |
| ```bool is20inch(int pmtid)``` | 20-inch PMT or not|
| ```bool is3inch(int pmtid)``` | 3-inch PMT or not|
| ```bool isNormalNNVT(int pmtid)``` | Normal NNVT (Normal MCP) PMT or not |
| ```bool isHamamatsu(int pmtid)``` | Hamamatsu (Dynode) PMT or not|
| ```bool isHighQENNVT(int pmtid)``` | High QE NNVT (High QE MCP) PMT or not |
| ```bool isHZC(int pmtid)``` | HZC (3-inch) PMT or not |
| ```float getPMTX(int pmtid)``` | Position |
| ```float getPMTY(int pmtid)``` | Position |
| ```float getPMTZ(int pmtid)``` | Position |
| ```float getPMTTheta(int pmtid)``` | Rotaion angle along Y axis ($\pi+\theta$) |
| ```float getPMTPhi(int pmtid)``` | Rotaion angle along Z axis |
| ```float getPMTPsi(int pmtid)``` | Rotaion angle along Z axis |
| ```float getPMTmeanR_CD_LPMT()``` | Mean radius from the center of CD for LPMT|
| ```float getPMTmeanR_CD_SPMT()``` | Mean radius from the center of CD for SPMT |
| ```int get_NTotal_CD_LPMT()``` | The number of LPMT in CD |
| ```int get_NTotal_CD_SPMT()``` | The number of SPMT in CD |
| ```int get_NTotal_WP_LPMT()``` | The number of LPMT in WP |
|``` string PMTCategoryList()```|  enum PMT_CATEGORY list as string|
|``` int getPMTCategory(int pmtID)```| Return enum PMT_CATEGORY|

### Usage

```C++
#include "Geometry/IPMTParamSvc.h"

SniperPtr<IPMTParamSvc> pmtParamSvc(*getParent(), "PMTParamSvc");
if (pmtParamSvc.invalid())
    {
        LogError << "Can't Locate  PMTParamSvc." << std::endl;
            return false;
    }
    
// examples
int num_CD_LPMT = pmtParamSvc->get_NTotal_CD_LPMT();
float pmt_x = pmtParamSvc->getPMTX(0);                   // use sim id of PMT
```

## Simulation/SimSvc/PMTSimParamSvc

### Gets functions


| functions  | Description | 
| -------- | --------  | 
| ```double get_gain(int pmtId)``` |   LPMT/SPMT   |
| ```double get_sigmaGain(int pmtId)```| LPMT/SPMT |
| ```double get_efficiency(int pmtId)``` | SPMT|
| ```double get_pde(int pmtId)``` | LPMT |
| ```double get_QE(int pmtId)``` | SPMT |
| ```double get_dcr(int pmtId)``` | LPMT/SPMT |
| ```double get_afterPulseProb(int pmtId)``` | LPMT/SPMT |
| ```double get_prePulseProb(int pmtId)``` | LPMT/SPMT |
| ```double get_tts(int pmtId)``` | LPMT/SPMT |
| ```double get_timeOffset(int pmtId)``` | LPMT/SPMT |
| ```TGraph* get_MCPTT()``` | LPMT |
| ```TGraph* get_MCPAmp()``` | LPMT |
| ```int dynodeAP1_amp()``` | LPMT |
| ```double dynodeAP1_time()``` | LPMT |
| ```int dynodeAP2_amp()``` | LPMT |
| ```double mcpAP2_time()``` | LPMT |
| ```int mcpAP1_amp()``` | LPMT |
| ```double mcpAP1_time()``` | LPMT |
| ```int mcpAP2_amp()``` | LPMT |
| ```double mcpAP2_time()``` | LPMT |
| ```int mcpAP3_amp()``` | LPMT |
| ```double mcpAP3_time()``` | LPMT |


### Usage
```C++
#include "IPMTSimParamSvc/IPMTSimParamSvc.h"

SniperPtr<IPMTSimParamSvc> simpmtsvc(*getRoot(), "PMTSimParamSvc");
if (simpmtsvc.invalid())
    {
        LogError << "can't find service PMTSimParamSvc" << std::endl;
        assert(0);
    }
else 
    {
        LogInfo << "Retrieve PMTSimParamSvc Successfully. " << std::endl;
        IPMTSimParamSvc* m_simpmtsvc = simpmtsvc.data();
    }
// examples
double pmt_pde = m_simpmtsvc->get_pde(0);   // sim id
double pmt_QE = m_simpmtsvc->get_pde(300000); // sim id

```

# Geometry Data files
## Checkout the data
```bash
cd $WORKTOP
svn co https://juno.ihep.ac.cn/svn/offline-data/trunk/ data
```
## Large PMTs in CD
### Info
- Numbers of PMTs: 17612
- Sim ID: 0 ~ 17611
    
### Position of PMTs
- Data file
```bash
$WORKTOP/data/Detector/Geometry/PMTPos_CD_LPMT.csv
```

- Data format

| PMT ID | x | y | z | $\theta$ | $\phi$ |
| -------- | -------- | -------- | -------- | -------- | -------- |
| 0     | -930.2976     | -111.8724     | 19365.0 | 2.7702  | 186.8571  |
| 1 | -492.5657 | -797.0872 | 19365.0 | 2.7702 | 238.2857|
| ... | ... | ... |... | ... | ...|

- Description
**Position in CD** : $x,y,z$ 
**Roation along Y** : $\pi$ (id >= 0 && id <= 20); $0$ (id >= 17591 && id <= 17611); $\pi+ \theta$ (else)
**Roation along Z** : $\phi$ (id > 20 && id< 17591)

### Types of PMTs
- Data file
```bash
$WORKTOP/data/data/Detector/Geometry/PMTType_CD_LPMT.csv
```

- Data format

| PMT ID | Type |
| -------- | -------- |
| 0   | HighQENNVT   | 
| 1   | Hamamatsu |
| ... | ... |
| 13  | NNVT|
| ... | ... |

- Description

**Types of Large PMT in CD** : **HighQENNVT, Hamamatsu, NNVT**

### Electronics parameters
- Data file
```bash
$WORKTOP/data/Simulation/SimSvc/PMTParam_CD_LPMT.root
```

- Data format

| Branch | leaflist |Description |
| -------- | -------- | -------- |
| pmtID | pmtID/I   | Simulation ID|
| SN    | SN/C | Series Number|
| Gain  | Gain/D |
| Resolution | Resolution/D |
| PDE | PDE/D |
| DCR | DCR/D |
| TTS | TTS/D |
| TTS_SS | TTS_SS/D |
| HV | HV/D |
| Amplitude | Amplitude/D |
| PvsV | PvsV/D |
| SvsN | SvsN/D |
| RiseTime | RiseTime/D |
| FallTime | FallTime/D |
| timeOffset | timeOffset/D |
| FWHM | FWHM/D |
| prePulseProb | prePulseProb/D |
| afterPulseProb | afterPulseProb/D |


- Description

Elecsim Parameters of Large PMT in CD.
It has same Branch names as its in the old file, but pmtPosX, pmtPosY, pmtPosZ are removed.


## Small PMTs in CD
### Info
- Numbers of PMTs: 25600
- Sim ID: 300000 ~ 325599
    
### Position of PMTs
- Data file
```bash
$WORKTOP/data/Detector/Geometry/PMTPos_CD_SPMT.csv
```

- Data format

| PMT ID | x | y | z | $\theta$ | $\phi$ |
| -------- | -------- | -------- | -------- | -------- | -------- |
|300000| -3734.2473| 1835.0656| 18932.1786| 12.3950| 153.8298|
|300001| -3470.8248| 1705.6158| 18994.3072| 11.5082| 153.8298|
| ... | ... | ... |... | ... | ...|

- Description

**Position in CD** : $x,y,z$ 
**Roation along Y** : $\pi+ \theta$ 
**Roation along Z** : $\phi$
**$x,y,z$ are calculated by equation**:

$
x = r_{\text{ball}} \cdot \sin(\theta) \cdot \cos(\phi) \\
y = r_{\text{ball}} \cdot \sin(\theta) \cdot \sin(\phi) \\
z = r_{\text{ball}} \cdot \cos(\theta)
$

which $r_{\text{ball}} = 19384 \;\text{mm}$ for small PMTs in CD.

### Types of PMTs
- Data file
```bash
$WORKTOP/data/Detector/Geometry/PMTType_CD_SPMT.csv
```

- Data format

| PMT ID | Type |
| -------- | -------- |
|300000 | HZC|
|300001 | HZC|
| ... | ... |

- Description
There is only one type for small PMTs, which is HZC.


### Electronics parameters
- Data file
```bash
$WORKTOP/data/Simulation/SimSvc/PMTParam_CD_SPMT.root
```

- Data format

| Branch | leaflist |Description |
| -------- | -------- | -------- |
| pmtID | pmtID/I   | Simulation ID|
| QE    | QE/D | Quantum efficiency|
| efficiency  | efficiency/D |
| gain | gain/D |
| sigmaGain | sigmaGain/D |
| afterPulseProb | afterPulseProb/D |
| prePulseProb | prePulseProb/D |
| darkRate | darkRate/D |
| timeSpread | timeSpread/D |
| timeOffset | timeOffset/D |


- Description
Elecsim Parameters of Small PMT in CD.
It has same Branch names as its in the old file, but pmtPosX, pmtPosY, pmtPosZ are removed.
The new branch is QE, which the data is achieved from 

```bash
/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre0/data/Simulation/ElecSim/pmtdata.txt
```


## Large PMTs in WP
### Info
- Numbers of PMTs: 2400
- Sim ID: 30000 ~ 32399
### Position of PMTs
- Data file
```bash
$WORKTOP/data/Detector/Geometry/PMTPos_WP_LPMT.csv
```

- Data format

| PMT ID | x | y | z | $\theta$ | $\phi$ |
| -------- | -------- | -------- | -------- | -------- | -------- |
|30000| -1205.3| -879.478| 20502| 4.16242| 216.117|
|30001| -576.011| -735.794| 20502| 2.60962| 231.945|
| ... | ... | ... |... | ... | ...|

- Description 

The data is from Haoqi Lu. It is updating.
Position in CD :
Roation along Y : 
Roation along Z :