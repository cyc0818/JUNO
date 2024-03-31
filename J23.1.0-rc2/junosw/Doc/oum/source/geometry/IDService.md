# Identifier (New Identifier for CD)
## junosw/Detector/Identifier/CdID

### Work functions
| functions  | Description | 
| -------- | --------  | 
| ```bool is20inch(const Identifier& id)``` | injudge whether the PMT is 20inch |
| ```bool is3inch(const Identifier& id)``` | injudge whether the PMT is 3inch |
| ```bool isNorth(const Identifier& id)``` | injudge whether the PMT is in north |
| ```bool isSouth(const Identifier& id)``` | injudge whether the PMT is in south |
| ```int positionNo (const Identifier& id)``` | return the pisition number of PMT |
| ```int upOrDown (const Identifier& id)``` | injudge whether the small PMT is in up or down |
| ```int pmtType (const Identifier& id)``` |  return the PMT type  |




### Usage

```C++
#include "Identifier/CdID.h"

CdID::largeOrSmall(id) 

```


# IDServices (Identifier Service )
## junosw/Detector/Identifier/IDService

### Gets functions
| functions  | Description | 
| -------- | --------  | 
| ```Identifier copyNo2Id(int CopyNo)``` | from copy number to get identifier |
| ```int id2CopyNo(Identifier id)``` | from identifier to get copy number |


### Usage

```C++
#include "Identifier/IDService.h"

IDService* idServ = IDService::getIdServ();
idServ -> init();
idServ -> copyNo2Id(copyNo);

```