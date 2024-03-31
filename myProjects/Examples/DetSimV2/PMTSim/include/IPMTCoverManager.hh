#ifndef IPMTCoverManager_hh
#define IPMTCoverManager_hh

#include "IPMTManager.hh"

class IPMTCoverManager: public IPMTManager {
public:
    virtual G4LogicalVolume* GetLogicalPMT() = 0;
    virtual G4double GetPMTRadius() = 0;
    virtual G4double GetPMTHeight() = 0;
    virtual G4double GetZEquator() = 0;
    virtual G4ThreeVector GetPosInPMT() = 0;

    IPMTCoverManager(IPMTManager* pmt_inner)
        : m_pmt_inner(pmt_inner) {

    }

    ~IPMTCoverManager() {
        if(m_pmt_inner) {
            delete m_pmt_inner;
        }
    }
protected:
    IPMTManager* m_pmt_inner;
    

};

#endif
